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

#ifndef CSSOM_PROPERTY_VALUE_H_
#define CSSOM_PROPERTY_VALUE_H_

#include "base/memory/ref_counted.h"
#include "base/optional.h"
#include "cobalt/base/polymorphic_equatable.h"

namespace cobalt {
namespace cssom {

class PropertyValueVisitor;

// A base type for all values of CSS properties.
// All derived classes must be immutable.
class PropertyValue : public base::RefCountedThreadSafe<PropertyValue>,
                      public base::PolymorphicEquatable {
 public:
  virtual void Accept(PropertyValueVisitor* visitor) = 0;

  // TODO(***REMOVED***): |ToString| should be pure virtual function and supported for
  // each value type.
  virtual base::optional<std::string> ToString() {
    NOTIMPLEMENTED() << "Should be supported for each value type.";
    return base::nullopt;
  }

 protected:
  virtual ~PropertyValue() {}

  friend class base::RefCountedThreadSafe<PropertyValue>;
};

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_PROPERTY_VALUE_H_
