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

#include "cobalt/webdriver/element_driver.h"

#include "cobalt/webdriver/util/call_on_message_loop.h"

namespace cobalt {
namespace webdriver {
namespace {
std::string GetTagName(dom::Element* element) {
  DCHECK(element);
  return element->tag_name();
}
}  // namespace

ElementDriver::ElementDriver(
    const protocol::ElementId& element_id,
    const base::WeakPtr<dom::Element>& element,
    const scoped_refptr<base::MessageLoopProxy>& message_loop)
    : element_id_(element_id),
      element_(element),
      element_message_loop_(message_loop) {}

util::CommandResult<std::string> ElementDriver::GetTagName() {
  return util::CallWeakOnMessageLoopAndReturnResult(
      element_message_loop_,
      base::Bind(&ElementDriver::GetWeak, base::Unretained(this)),
      base::Bind(&::cobalt::webdriver::GetTagName),
      protocol::Response::kStaleElementReference);
}

}  // namespace webdriver
}  // namespace cobalt
