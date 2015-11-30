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

#if defined(ENABLE_DEBUG_CONSOLE)

#include "cobalt/debug/debug_hub.h"

#include <set>

#include "cobalt/base/console_commands.h"
#include "cobalt/base/console_values.h"
#include "cobalt/base/source_location.h"

namespace cobalt {
namespace debug {

DebugHub::DebugHub(
    const GetHudModeCallback& get_hud_mode_callback,
    const ExecuteJavascriptCallback& execute_javascript_callback,
    const Debugger::CreateDebugServerCallback& create_debug_server_callback)
    : get_hud_mode_callback_(get_hud_mode_callback),
      execute_javascript_callback_(execute_javascript_callback),
      next_log_message_callback_id_(0),
      log_message_callbacks_deleter_(&log_message_callbacks_),
      debugger_(new Debugger(create_debug_server_callback)) {
  // Get log output while still making it available elsewhere.
  const base::LogMessageHandler::OnLogMessageCallback on_log_message_callback =
      base::Bind(&DebugHub::OnLogMessage, base::Unretained(this));
  log_message_handler_callback_id_ =
      base::LogMessageHandler::GetInstance()->AddCallback(
          on_log_message_callback);
}

DebugHub::~DebugHub() {
  base::LogMessageHandler::GetInstance()->RemoveCallback(
      log_message_handler_callback_id_);
}

bool DebugHub::OnLogMessage(int severity, const char* file, int line,
                            size_t message_start, const std::string& str) {
  // Use a lock here and in the other methods, as callbacks may be added by
  // multiple web modules on different threads, and log messages may be
  // be generated on other threads.
  base::AutoLock auto_lock(lock_);

  for (LogMessageCallbacks::const_iterator it = log_message_callbacks_.begin();
       it != log_message_callbacks_.end(); ++it) {
    const LogMessageCallbackInfo* callbackInfo = it->second;
    const scoped_refptr<base::MessageLoopProxy>& message_loop_proxy =
        callbackInfo->message_loop_proxy;

    message_loop_proxy->PostTask(
        FROM_HERE, base::Bind(&DebugHub::LogMessageTo, this, it->first,
                              severity, file, line, message_start, str));
  }
  // Don't suppress the log message.
  return false;
}

void DebugHub::LogMessageTo(int id, int severity, const char* file, int line,
                            size_t message_start, const std::string& str) {
  base::AutoLock auto_lock(lock_);
  LogMessageCallbacks::const_iterator it = log_message_callbacks_.find(id);
  if (it != log_message_callbacks_.end()) {
    DCHECK_EQ(base::MessageLoopProxy::current(),
              it->second->message_loop_proxy);
    it->second->callback.value().Run(severity, file, line, message_start, str);
  }
}

int DebugHub::AddLogMessageCallback(const LogMessageCallbackArg& callback) {
  base::AutoLock auto_lock(lock_);
  const int callback_id = next_log_message_callback_id_++;
  log_message_callbacks_[callback_id] = new LogMessageCallbackInfo(
      this, callback, base::MessageLoopProxy::current());
  return callback_id;
}

void DebugHub::RemoveLogMessageCallback(int callback_id) {
  base::AutoLock auto_lock(lock_);
  LogMessageCallbacks::iterator it = log_message_callbacks_.find(callback_id);
  if (it != log_message_callbacks_.end()) {
    delete it->second;
    log_message_callbacks_.erase(it);
  }
}

// TODO(***REMOVED***) - This function should be modified to return an array of
// strings instead of a single space-separated string, once the bindings
// support return of a string array.
std::string DebugHub::GetConsoleValueNames() const {
  std::string ret = "";
  base::ConsoleValueManager* cvm = base::ConsoleValueManager::GetInstance();
  DCHECK(cvm);

  if (cvm) {
    std::set<std::string> names = cvm->GetOrderedCValNames();
    for (std::set<std::string>::const_iterator it = names.begin();
         it != names.end(); ++it) {
      ret += (*it);
      std::set<std::string>::const_iterator next = it;
      ++next;
      if (next != names.end()) {
        ret += " ";
      }
    }
  }
  return ret;
}

std::string DebugHub::GetConsoleValue(const std::string& name) const {
  std::string ret = "<undefined>";
  base::ConsoleValueManager* cvm = base::ConsoleValueManager::GetInstance();
  DCHECK(cvm);

  if (cvm) {
    base::optional<std::string> result = cvm->GetValueAsPrettyString(name);
    if (result) {
      return *result;
    }
  }
  return ret;
}

int DebugHub::GetDebugConsoleMode() const {
  return get_hud_mode_callback_.Run();
}

std::string DebugHub::ExecuteJavascript(const std::string& javascript) {
  // Assume the command is JavaScript to be exected in the main web module
  // using the callback provided at construction.
  return execute_javascript_callback_.Run(
      javascript, base::SourceLocation("[object DebugHub]", 1, 1));
}

// TODO(***REMOVED***) - This function should be modified to return an array of
// strings instead of a single space-separated string, once the bindings
// support return of a string array.
std::string DebugHub::GetCommandChannels() const {
  std::string result = "";
  base::ConsoleCommandManager* command_mananger =
      base::ConsoleCommandManager::GetInstance();
  DCHECK(command_mananger);

  if (command_mananger) {
    std::set<std::string> channels = command_mananger->GetRegisteredChannels();
    for (std::set<std::string>::const_iterator it = channels.begin();
         it != channels.end(); ++it) {
      result += (*it);
      std::set<std::string>::const_iterator next = it;
      ++next;
      if (next != channels.end()) {
        result += " ";
      }
    }
  }
  return result;
}

std::string DebugHub::GetCommandChannelShortHelp(
    const std::string& channel) const {
  std::string result = "<undefined>";
  base::ConsoleCommandManager* command_mananger =
      base::ConsoleCommandManager::GetInstance();
  DCHECK(command_mananger);
  if (command_mananger) {
    result = command_mananger->GetShortHelp(channel);
  }
  return result;
}

std::string DebugHub::GetCommandChannelLongHelp(
    const std::string& channel) const {
  std::string result = "<undefined>";
  base::ConsoleCommandManager* command_mananger =
      base::ConsoleCommandManager::GetInstance();
  DCHECK(command_mananger);
  if (command_mananger) {
    result = command_mananger->GetLongHelp(channel);
  }
  return result;
}

void DebugHub::SendCommand(const std::string& channel,
                           const std::string& message) {
  base::ConsoleCommandManager* console_command_manager =
      base::ConsoleCommandManager::GetInstance();
  DCHECK(console_command_manager);
  console_command_manager->HandleCommand(channel, message);
}

}  // namespace debug
}  // namespace cobalt

#endif  // ENABLE_DEBUG_CONSOLE
