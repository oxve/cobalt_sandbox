// Copyright 2017 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cobalt/h5vcc/h5vcc_accessibility.h"

#include "base/message_loop.h"
#include "cobalt/base/accessibility_settings_changed_event.h"
#include "starboard/accessibility.h"
#include "starboard/memory.h"

namespace cobalt {
namespace h5vcc {

H5vccAccessibility::H5vccAccessibility(base::EventDispatcher* event_dispatcher)
    : event_dispatcher_(event_dispatcher) {
  message_loop_proxy_ = base::MessageLoopProxy::current();
  event_dispatcher_->AddEventCallback(
      base::AccessibilitySettingsChangedEvent::TypeId(),
      base::Bind(&H5vccAccessibility::OnApplicationEvent,
                 base::Unretained(this)));
}

bool H5vccAccessibility::high_contrast_text() const {
#if SB_API_VERSION >= 4
  SbAccessibilityDisplaySettings settings;
  SbMemorySet(&settings, 0, sizeof(settings));

  if (!SbAccessibilityGetDisplaySettings(&settings)) {
    return false;
  }

  return settings.is_high_contrast_text_enabled;
#else   // SB_API_VERSION >= 4
  return  false;
#endif  // SB_API_VERSION >= 4
}

bool H5vccAccessibility::text_to_speech() const {
#if SB_API_VERSION >= 4
  SbAccessibilityTextToSpeechSettings settings;
  SbMemorySet(&settings, 0, sizeof(settings));

  if (!SbAccessibilityGetTextToSpeechSettings(&settings)) {
    return false;
  }

  return settings.has_text_to_speech_setting &&
      settings.is_text_to_speech_enabled;
#else   // SB_API_VERSION >= 4
  return  false;
#endif  // SB_API_VERSION >= 4
}

void H5vccAccessibility::AddHighContrastTextListener(
    const H5vccAccessibilityCallbackHolder& holder) {
  DCHECK_EQ(base::MessageLoopProxy::current(), message_loop_proxy_);
  high_contrast_text_listener_.reset(
      new H5vccAccessibilityCallbackReference(this, holder));
}

void H5vccAccessibility::OnApplicationEvent(const base::Event* event) {
  UNREFERENCED_PARAMETER(event);
  // This method should be called from the application event thread.
  DCHECK_NE(base::MessageLoopProxy::current(), message_loop_proxy_);
  message_loop_proxy_->PostTask(
      FROM_HERE, base::Bind(&H5vccAccessibility::InternalOnApplicationEvent,
                            base::Unretained(this)));
}

void H5vccAccessibility::InternalOnApplicationEvent() {
  DCHECK_EQ(base::MessageLoopProxy::current(), message_loop_proxy_);
  if (high_contrast_text_listener_) {
    high_contrast_text_listener_->value().Run();
  }
}

}  // namespace h5vcc
}  // namespace cobalt
