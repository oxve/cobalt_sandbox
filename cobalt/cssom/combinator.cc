/*
 * Copyright 2014 Google Inc. All Rights Reserved.
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

#include "cobalt/cssom/combinator.h"

#include "cobalt/cssom/compound_selector.h"

namespace cobalt {
namespace cssom {

Combinator::Combinator() {}

Combinator::~Combinator() {}

CompoundSelector* Combinator::selector() { return compound_selector_.get(); }

void Combinator::set_selector(scoped_ptr<CompoundSelector> compound_selector) {
  compound_selector_ = compound_selector.Pass();
}

}  // namespace cssom
}  // namespace cobalt
