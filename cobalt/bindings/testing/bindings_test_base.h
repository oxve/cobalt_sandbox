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

#ifndef BINDINGS_TESTING_BINDINGS_TEST_BASE_H_
#define BINDINGS_TESTING_BINDINGS_TEST_BASE_H_

#include "cobalt/base/polymorphic_downcast.h"
#include "cobalt/script/javascript_engine.h"
#include "cobalt/script/global_object_proxy.h"
#include "cobalt/script/source_code.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace cobalt {
namespace bindings {
namespace testing {

template <class MockT>
class BindingsTestBase : public ::testing::Test {
 protected:
  BindingsTestBase()
      : engine_(script::JavaScriptEngine::CreateEngine()),
        global_object_proxy_(engine_->CreateGlobalObject()),
        // Use StrictMock so TESTING will fail if unexpected method is called.
        test_mock_(new ::testing::StrictMock<MockT>()) {
  }

  bool EvaluateScript(const std::string& script, std::string* out_result) {
    scoped_refptr<script::SourceCode> source =
        script::SourceCode::CreateSourceCode(script);
    return global_object_proxy_->EvaluateScript(source, out_result);
  }
  MockT& test_mock() { return *test_mock_.get(); }

 protected:
  const scoped_ptr<script::JavaScriptEngine> engine_;
  const scoped_refptr<script::GlobalObjectProxy> global_object_proxy_;
  const scoped_refptr<MockT> test_mock_;
};

// Use this fixture to create a new MockT object with a BaseClass wrapper, and
// bind the wrapper to the javascript variable "test".
template <class MockT, class BaseClass = MockT>
class InterfaceBindingsTest : public BindingsTestBase<MockT> {
 public:
  InterfaceBindingsTest() {
    global_object_proxy_->Bind("test",
                               make_scoped_refptr<BaseClass>((test_mock_)));
  }
};

// Use this fixture to create a new MockT object with bindings defined for the
// BaseClass wrapper. Bindings for properties on the BaseClass object will be
// added to the global object, and the MockT object will be set as the global
// interface's implementation.
template <class MockT, class BaseClass = MockT>
class GlobalBindingsTestBase : public BindingsTestBase<MockT> {
 public:
  GlobalBindingsTestBase() {
    global_object_proxy_->SetGlobalInterface(
        make_scoped_refptr<BaseClass>(test_mock_));
  }
};

}  // namespace testing
}  // namespace bindings
}  // namespace cobalt

#endif  // BINDINGS_TESTING_BINDINGS_TEST_BASE_H_
