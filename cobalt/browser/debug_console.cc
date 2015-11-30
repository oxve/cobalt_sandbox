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

#include "cobalt/browser/debug_console.h"

#include "base/command_line.h"
#include "base/file_util.h"
#include "base/path_service.h"
#include "cobalt/base/cobalt_paths.h"
#include "cobalt/base/source_location.h"
#include "cobalt/browser/switches.h"
#include "cobalt/dom/event_names.h"

namespace cobalt {
namespace browser {

namespace {
// Files for the debug console web page are bundled with the executable.
const char kInitialDebugConsoleUrl[] =
    "file:///cobalt/browser/debug_console/debug_console.html";

const char kDebugConsoleOffString[] = "off";
const char kDebugConsoleOnString[] = "on";
const char kDebugConsoleHudString[] = "hud";

// Convert from a debug console visibility setting string to an integer
// value specified by a constant defined in debug::DebugHub.
base::optional<int> DebugConsoleModeStringToInt(
    const std::string& mode_string) {
  // Static casting is necessary in order to get around what appears to be a
  // compiler error on Linux when implicitly constructing a base::optional<int>
  // from a static const int.
  if (mode_string == kDebugConsoleOffString) {
    return static_cast<int>(debug::DebugHub::kDebugConsoleOff);
  } else if (mode_string == kDebugConsoleHudString) {
    return static_cast<int>(debug::DebugHub::kDebugConsoleHud);
  } else if (mode_string == kDebugConsoleOnString) {
    return static_cast<int>(debug::DebugHub::kDebugConsoleOn);
  } else {
    DLOG(WARNING) << "Debug console mode \"" << mode_string
                  << "\" not recognized.";
    return base::nullopt;
  }
}

// Convert from mode to string.
std::string DebugConsoleModeIntToString(int mode) {
  switch (mode) {
    case debug::DebugHub::kDebugConsoleHud:
      return kDebugConsoleHudString;
    case debug::DebugHub::kDebugConsoleOn:
      return kDebugConsoleOnString;
    case debug::DebugHub::kDebugConsoleOff:
      return kDebugConsoleOffString;
    default:
      NOTREACHED();
      return kDebugConsoleOffString;
  }
}

// Returns the debug console mode as specified by the command line.
// If unspecified by the command line, base::nullopt is returned.
base::optional<int> GetDebugConsoleModeFromCommandLine() {
#if defined(ENABLE_COMMAND_LINE_SWITCHES)
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kDebugConsoleMode)) {
    const std::string debug_console_mode_string =
        command_line->GetSwitchValueASCII(switches::kDebugConsoleMode);
    return DebugConsoleModeStringToInt(debug_console_mode_string);
  }
#endif  // ENABLE_COMMAND_LINE_SWITCHES

  return base::nullopt;
}

// Returns the path of the temporary file used to store debug console visibility
// mode preferences.
FilePath GetDebugConsoleModeStoragePath() {
  FilePath dir;
  CHECK(PathService::Get(cobalt::paths::DIR_COBALT_DEBUG_OUT, &dir));
  return dir.Append("last_debug_console_mode.txt");
}

// Saves the specified visibility mode preferences to disk so that they can
// be restored in another session.
void SaveModeToPreferences(int mode) {
  std::string mode_string = DebugConsoleModeIntToString(mode);
  file_util::WriteFile(GetDebugConsoleModeStoragePath(), mode_string.c_str(),
                       static_cast<int>(mode_string.size()));
}

// Load debug console visibility mode preferences from disk.
base::optional<int> LoadModeFromPreferences() {
  std::string saved_contents;
  if (file_util::ReadFileToString(GetDebugConsoleModeStoragePath(),
                                  &saved_contents)) {
    return DebugConsoleModeStringToInt(saved_contents);
  }
  return base::nullopt;
}

// Returns the debug console's initial visibility mode.
int GetInitialMode() {
  // First check to see if the mode is explicitly set from the command line.
  base::optional<int> mode_from_command_line =
      GetDebugConsoleModeFromCommandLine();
  if (mode_from_command_line) {
    return *mode_from_command_line;
  }

  // Now check to see if mode preferences have been saved to disk.
  base::optional<int> mode_from_preferences = LoadModeFromPreferences();
  if (mode_from_preferences) {
    return *mode_from_preferences;
  }

  // If all else fails, default the debug console to off.
  return debug::DebugHub::kDebugConsoleOff;
}

// A function to create a DebugHub object, to be injected into WebModule.
scoped_refptr<script::Wrappable> CreateDebugHub(
    const debug::DebugHub::GetHudModeCallback& get_hud_mode_function,
    const debug::DebugHub::ExecuteJavascriptCallback&
        execute_javascript_callback,
    const debug::Debugger::CreateDebugServerCallback&
        create_debug_server_callback) {
  return new debug::DebugHub(get_hud_mode_function, execute_javascript_callback,
                             create_debug_server_callback);
}

}  // namespace

DebugConsole::DebugConsole(
    const WebModule::OnRenderTreeProducedCallback&
        render_tree_produced_callback,
    const base::Callback<void(const std::string&)>& error_callback,
    media::MediaModule* media_module, network::NetworkModule* network_module,
    const math::Size& window_dimensions,
    render_tree::ResourceProvider* resource_provider, float layout_refresh_rate,
    const debug::DebugHub::ExecuteJavascriptCallback&
        execute_javascript_callback,
    const debug::Debugger::CreateDebugServerCallback&
        create_debug_server_callback) {
  mode_ = GetInitialMode();

  WebModule::Options web_module_options;
  web_module_options.name = "DebugConsoleWebModule";

  // Attach a DebugHub object to the "debugHub" Window attribute for this
  // web module so that JavaScript within this WebModule has access to DebugHub
  // functionality.
  web_module_options.injected_window_attributes["debugHub"] =
      base::Bind(&CreateDebugHub,
                 base::Bind(&DebugConsole::GetMode, base::Unretained(this)),
                 execute_javascript_callback, create_debug_server_callback);
  web_module_.reset(new WebModule(
      GURL(kInitialDebugConsoleUrl), render_tree_produced_callback,
      error_callback, media_module, network_module, window_dimensions,
      resource_provider, layout_refresh_rate, web_module_options));
}

DebugConsole::~DebugConsole() {}

bool DebugConsole::FilterKeyEvent(
    const scoped_refptr<dom::KeyboardEvent>& event) {
  // Assume here the full debug console is visible - pass all events to its
  // web module, and return false to indicate the event has been consumed.
  web_module_->InjectEvent(event);
  return false;
}

void DebugConsole::SetMode(int mode) {
  int mode_to_save;
  {
    base::AutoLock lock(mode_mutex_);
    mode_ = mode;
    mode_to_save = mode_;
  }
  SaveModeToPreferences(mode_to_save);
}

void DebugConsole::CycleMode() {
  int mode_to_save;
  {
    base::AutoLock lock(mode_mutex_);
    mode_ = (mode_ + 1) % debug::DebugHub::kDebugConsoleNumModes;
    mode_to_save = mode_;
  }
  SaveModeToPreferences(mode_to_save);
}

int DebugConsole::GetMode() {
  base::AutoLock lock(mode_mutex_);
  return mode_;
}

}  // namespace browser
}  // namespace cobalt

#endif  // ENABLE_DEBUG_CONSOLE
