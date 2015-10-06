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

#include "cobalt/dom/ui_event_with_key_state.h"

namespace cobalt {
namespace dom {

bool UIEventWithKeyState::GetModifierState(const std::string& keyArg) const {
  // Standard names of modifier keys defined here:
  // http://www.w3.org/TR/DOM-Level-3-Events-key/#keys-modifier
  if (keyArg == "Alt") {
    return alt_key();
  } else if (keyArg == "Control") {
    return ctrl_key();
  } else if (keyArg == "Meta") {
    return meta_key();
  } else if (keyArg == "Shift") {
    return shift_key();
  }

  return false;
}

}  // namespace dom
}  // namespace cobalt
