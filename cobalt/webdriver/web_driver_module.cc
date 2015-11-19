/*
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cobalt/webdriver/web_driver_module.h"

#include <string>
#include <vector>

#include "base/bind.h"
#include "base/values.h"
#include "cobalt/webdriver/dispatcher.h"
#include "cobalt/webdriver/protocol/capabilities.h"
#include "cobalt/webdriver/protocol/window_id.h"
#include "cobalt/webdriver/server.h"
#include "cobalt/webdriver/session_driver.h"
#include "cobalt/webdriver/util/command_result.h"
#include "cobalt/webdriver/util/dispatch_command_factory.h"
#include "cobalt/webdriver/window_driver.h"

namespace cobalt {
namespace webdriver {
namespace {

// Only one session is supported. This is the session ID for that session.
const char kWebDriverSessionId[] = "session-0";

// Variable names for variable path components.
const char kSessionIdVariable[] = ":sessionId";
const char kWindowHandleVariable[] = ":windowHandle";
const char kElementId[] = ":id";

// Error messages related to session creation.
const char kMaxSessionsCreatedMessage[] =
    "Maximum number of sessions have been created.";
const char kUnsupportedCapabilities[] =
    "An unsupported capability was requested.";
const char kUnknownSessionCreationError[] =
    "An unknown error occurred trying to create a new session.";

// Looks up the SessionDriver instance that is mapped to the
// the sessionId variable in the path.
SessionDriver* LookUpSessionDriverOrReturnInvalidResponse(
    const base::Callback<SessionDriver*(const protocol::SessionId&)>&
        get_session_cb,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    WebDriverDispatcher::CommandResultHandler* result_handler) {
  DCHECK(path_variables);
  protocol::SessionId session_id(
      path_variables->GetVariable(kSessionIdVariable));

  SessionDriver* session_driver = get_session_cb.Run(session_id);
  // If there is no session with this ID, then return an error.
  if (!session_driver) {
    result_handler->SendInvalidRequestResponse(
        WebDriverDispatcher::CommandResultHandler::kInvalidPathVariable,
        session_id.id());
  }
  return session_driver;
}

// There are no path variables to look up in this case. Ignore the path
// variables and return the session's current window.
WindowDriver* LookUpCurrentWindowDriver(
    SessionDriver* session_driver,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    WebDriverDispatcher::CommandResultHandler* result_handler) {
  DCHECK(session_driver);
  // If the current window has been closed, an error should be returned, but
  // we don't support closing windows on Cobalt.
  return session_driver->GetCurrentWindow();
}

// There are no path variables to look up in this case. This function simply
// returns the same SessionDriver that was passed in.
SessionDriver* LookUpSessionDriver(
    SessionDriver* session_driver,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    WebDriverDispatcher::CommandResultHandler* result_handler) {
  DCHECK(session_driver);
  return session_driver;
}

// Looks up the WindowDriver instance that is mapped to the
// the sessionId variable in the path.
WindowDriver* LookUpWindowDriverOrReturnInvalidResponse(
    SessionDriver* session_driver,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    WebDriverDispatcher::CommandResultHandler* result_handler) {
  DCHECK(path_variables);
  DCHECK(session_driver);
  protocol::WindowId window_id(
      path_variables->GetVariable(kWindowHandleVariable));
  WindowDriver* window_driver = session_driver->GetWindow(window_id);
  if (!window_driver) {
    result_handler->SendInvalidRequestResponse(
        WebDriverDispatcher::CommandResultHandler::kInvalidPathVariable,
        window_id.id());
  }
  return window_driver;
}

// Looks up the ElementDriver instance that is mapped to the
// the sessionId variable in the path.
ElementDriver* LookUpElementDriverOrReturnInvalidResponse(
    SessionDriver* session_driver,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    WebDriverDispatcher::CommandResultHandler* result_handler) {
  DCHECK(path_variables);
  DCHECK(session_driver);
  ElementDriver* element_driver = NULL;
  WindowDriver* window_driver =
      LookUpCurrentWindowDriver(session_driver, path_variables, result_handler);
  if (window_driver) {
    protocol::ElementId element_id(path_variables->GetVariable(kElementId));
    element_driver = window_driver->GetElementDriver(element_id);
    if (!element_driver) {
      result_handler->SendInvalidRequestResponse(
          WebDriverDispatcher::CommandResultHandler::kInvalidPathVariable,
          element_id.id());
    }
  }
  return element_driver;
}

}  // namespace

WebDriverModule::WebDriverModule(
    int server_port, const CreateSessionDriverCB& create_session_driver_cb,
    const base::Closure& shutdown_cb)
    : create_session_driver_cb_(create_session_driver_cb),
      shutdown_cb_(shutdown_cb),
      webdriver_dispatcher_(new WebDriverDispatcher()) {
  get_session_driver_ =
      base::Bind(&WebDriverModule::GetSessionDriver, base::Unretained(this));

  typedef util::DispatchCommandFactory<SessionDriver> SessionCommandFactory;
  typedef util::DispatchCommandFactory<WindowDriver> WindowCommandFactory;
  typedef util::DispatchCommandFactory<ElementDriver> ElementCommandFactory;

  scoped_refptr<SessionCommandFactory> session_command_factory(
      new SessionCommandFactory(
          base::Bind(&LookUpSessionDriverOrReturnInvalidResponse,
                     get_session_driver_),
          base::Bind(&LookUpSessionDriver)));

  scoped_refptr<WindowCommandFactory> current_window_command_factory(
      new WindowCommandFactory(
          base::Bind(&LookUpSessionDriverOrReturnInvalidResponse,
                     get_session_driver_),
          base::Bind(&LookUpCurrentWindowDriver)));

  scoped_refptr<WindowCommandFactory> window_command_factory(
      new WindowCommandFactory(
          base::Bind(&LookUpSessionDriverOrReturnInvalidResponse,
                     get_session_driver_),
          base::Bind(&LookUpWindowDriverOrReturnInvalidResponse)));

  scoped_refptr<ElementCommandFactory> element_command_factory(
      new ElementCommandFactory(
          base::Bind(&LookUpSessionDriverOrReturnInvalidResponse,
                     get_session_driver_),
          base::Bind(&LookUpElementDriverOrReturnInvalidResponse)));

  // Server commands.
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet, "/status",
      base::Bind(&WebDriverModule::GetServerStatus, base::Unretained(this)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kPost, "/session",
      base::Bind(&WebDriverModule::CreateSession, base::Unretained(this)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet, "/sessions",
      base::Bind(&WebDriverModule::GetActiveSessions, base::Unretained(this)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet, "/shutdown",
      base::Bind(&WebDriverModule::Shutdown, base::Unretained(this)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kDelete, StringPrintf("/session/%s", kSessionIdVariable),
      base::Bind(&WebDriverModule::DeleteSession, base::Unretained(this)));

  // Session commands.
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet, StringPrintf("/session/%s", kSessionIdVariable),
      session_command_factory->GetCommandHandler(
          base::Bind(&SessionDriver::GetCapabilities)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet,
      StringPrintf("/session/%s/window_handle", kSessionIdVariable),
      session_command_factory->GetCommandHandler(
          base::Bind(&SessionDriver::GetCurrentWindowHandle)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet,
      StringPrintf("/session/%s/window_handles", kSessionIdVariable),
      session_command_factory->GetCommandHandler(
          base::Bind(&SessionDriver::GetWindowHandles)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kPost,
      StringPrintf("/session/%s/url", kSessionIdVariable),
      session_command_factory->GetCommandHandler(
          base::Bind(&SessionDriver::Navigate)));

  // Specified window commands.
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet,
      StringPrintf("/session/%s/window/%s/size", kSessionIdVariable,
                   kWindowHandleVariable),
      window_command_factory->GetCommandHandler(
          base::Bind(&WindowDriver::GetWindowSize)));

  // Current window commands.
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet,
      StringPrintf("/session/%s/url", kSessionIdVariable),
      current_window_command_factory->GetCommandHandler(
          base::Bind(&WindowDriver::GetCurrentUrl)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet,
      StringPrintf("/session/%s/title", kSessionIdVariable),
      current_window_command_factory->GetCommandHandler(
          base::Bind(&WindowDriver::GetTitle)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kPost,
      StringPrintf("/session/%s/element", kSessionIdVariable),
      current_window_command_factory->GetCommandHandler(
          base::Bind(&WindowDriver::FindElement)));
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kPost,
      StringPrintf("/session/%s/elements", kSessionIdVariable),
      current_window_command_factory->GetCommandHandler(
          base::Bind(&WindowDriver::FindElements)));

  // Element commands.
  webdriver_dispatcher_->RegisterCommand(
      WebDriverServer::kGet, StringPrintf("/session/%s/element/%s/name",
                                          kSessionIdVariable, kElementId),
      element_command_factory->GetCommandHandler(
          base::Bind(&ElementDriver::GetTagName)));

  // The WebDriver API implementation will be called on the HTTP server thread.
  thread_checker_.DetachFromThread();

  // Create a new WebDriverServer and pass in the Dispatcher.
  webdriver_server_.reset(new WebDriverServer(
      server_port,
      base::Bind(&WebDriverDispatcher::HandleWebDriverServerRequest,
                 base::Unretained(webdriver_dispatcher_.get()))));
}

WebDriverModule::~WebDriverModule() {}

SessionDriver* WebDriverModule::GetSessionDriver(
    const protocol::SessionId& session_id) {
  DCHECK(thread_checker_.CalledOnValidThread());
  SessionDriver* session_driver = NULL;
  if (session_ && (session_->session_id() == session_id)) {
    return session_.get();
  }
  return NULL;
}


// https://code.google.com/p/selenium/wiki/JsonWireProtocol#/status
void WebDriverModule::GetServerStatus(
    const base::Value* parameters,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    scoped_ptr<WebDriverDispatcher::CommandResultHandler> result_handler) {
  DCHECK(thread_checker_.CalledOnValidThread());
  result_handler->SendResult(base::nullopt, protocol::Response::kSuccess,
                             protocol::ServerStatus::ToValue(status_));
}

// https://code.google.com/p/selenium/wiki/JsonWireProtocol#/sessions
void WebDriverModule::GetActiveSessions(
    const base::Value* parameters,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    scoped_ptr<WebDriverDispatcher::CommandResultHandler> result_handler) {
  DCHECK(thread_checker_.CalledOnValidThread());
  std::vector<protocol::SessionId> sessions;
  if (session_) {
    sessions.push_back(session_->session_id());
  }
  result_handler->SendResult(base::nullopt, protocol::Response::kSuccess,
                             util::internal::ToValue(sessions));
}

// https://code.google.com/p/selenium/wiki/JsonWireProtocol#GET_/session/:sessionId
void WebDriverModule::CreateSession(
    const base::Value* parameters,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    scoped_ptr<WebDriverDispatcher::CommandResultHandler> result_handler) {
  DCHECK(thread_checker_.CalledOnValidThread());

  base::optional<protocol::RequestedCapabilities> requested_capabilities =
      protocol::RequestedCapabilities::FromValue(parameters);
  if (!requested_capabilities) {
    result_handler->SendInvalidRequestResponse(
        WebDriverDispatcher::CommandResultHandler::kInvalidParameters, "");
    return;
  }

  util::CommandResult<protocol::Capabilities> command_result =
      CreateSessionInternal(requested_capabilities.value());

  base::optional<protocol::SessionId> session_id;
  if (command_result.is_success()) {
    session_id = session_->session_id();
  }
  util::internal::ReturnResponse(session_id, command_result,
                                 result_handler.get());
}


// https://code.google.com/p/selenium/wiki/JsonWireProtocol#DELETE_/session/:sessionId
void WebDriverModule::DeleteSession(
    const base::Value* parameters,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    scoped_ptr<WebDriverDispatcher::CommandResultHandler> result_handler) {
  DCHECK(thread_checker_.CalledOnValidThread());

  if (session_) {
    // Extract the sessionId variable from the path
    std::string session_id_variable =
        path_variables->GetVariable(kSessionIdVariable);

    if (session_id_variable == session_->session_id().id()) {
      // If the sessionID matches, the delete the session.
      session_.reset();
    }
  }
  // If the session doesn't exist, then this is a no-op.
  result_handler->SendResult(base::nullopt, protocol::Response::kSuccess,
                             scoped_ptr<base::Value>());
}

void WebDriverModule::Shutdown(
    const base::Value* parameters,
    const WebDriverDispatcher::PathVariableMap* path_variables,
    scoped_ptr<WebDriverDispatcher::CommandResultHandler> result_handler) {
  DCHECK(thread_checker_.CalledOnValidThread());

  // It's expected that the application will terminate, so it's okay to
  // leave the request hanging.
  shutdown_cb_.Run();
}

util::CommandResult<protocol::Capabilities>
WebDriverModule::CreateSessionInternal(
    const protocol::RequestedCapabilities& requested_capabilities) {
  typedef util::CommandResult<protocol::Capabilities> CommandResult;

  if (session_) {
    // A session has already been created. We can only create one.
    return CommandResult(protocol::Response::kSessionNotCreatedException,
                         kMaxSessionsCreatedMessage);
  }

  // We will only ever create sessions with one set of capabilities. So ignore
  // the desired capabilities (for now).
  if (requested_capabilities.required() &&
      !requested_capabilities.required()->AreCapabilitiesSupported()) {
    return CommandResult(protocol::Response::kSessionNotCreatedException,
                         kUnsupportedCapabilities);
  }

  session_ =
      create_session_driver_cb_.Run(protocol::SessionId(kWebDriverSessionId));
  if (!session_) {
    // Some failure to create the new session.
    return CommandResult(protocol::Response::kUnknownError,
                         kUnknownSessionCreationError);
  }

  return session_->GetCapabilities();
}
}  // namespace webdriver
}  // namespace cobalt
