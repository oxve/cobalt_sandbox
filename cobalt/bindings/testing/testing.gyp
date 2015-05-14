# Copyright 2014 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'variables': {

    # Base directory into which generated sources and intermediate files should
    # be generated.
    'bindings_output_dir': '<(SHARED_INTERMEDIATE_DIR)/bindings/testing',

    # Bindings for the interfaces in this list will be generated, and there must
    # be an implementation declared in a header that lives in the same
    # directory of each IDL.
    'source_idl_files': [
        'AnonymousGetterSetterInterface.idl',
        'ArbitraryInterface.idl',
        'BaseInterface.idl',
        'BooleanTypeTestInterface.idl',
        'CallbackFunctionInterface.idl',
        'ConstantsInterface.idl',
        'ConstructorInterface.idl',
        'ConstructorWithArgumentsInterface.idl',
        'DerivedGetterSetterInterface.idl',
        'DerivedInterface.idl',
        'DOMStringTestInterface.idl',
        'InheritanceTestInterface.idl',
        'GetterSetterInterface.idl',
        'GlobalInterfaceParent.idl',
        'NoConstructorInterface.idl',
        'NoInterfaceObjectInterface.idl',
        'NullableTypesTestInterface.idl',
        'NumericTypesTestInterface.idl',
        'OperationsTestInterface.idl',
        'TargetInterface.idl',
        'Window.idl',
    ],

    # Partial interfaces and the right-side of "implements"
    # Code will not get generated for these interfaces; they are used to add
    # functionality to other interfaces.
    'dependency_idl_files': [
        'ImplementedInterface.idl',
        'PartialInterface.idl',
    ],

    # Specify component for generated window IDL.
    'window_component': 'testing',
  },

  'includes': [
    # Defines a `generated_sources` variable which is a list of all generated
    # source files.
    '../javascriptcore_bindings.gypi',
  ],

  'targets': [
    {
      'target_name': 'bindings',
      'type': 'static_library',
      'include_dirs': [
        '<(DEPTH)/third_party/WebKit/Source/JavaScriptCore',
        '<(DEPTH)/third_party/WebKit/Source/WTF',
      ],
      'sources': [
        '<@(generated_sources)',
        'constants_interface.cc',
      ],
      'dependencies': [
        '<(DEPTH)/third_party/WebKit/Source/JavaScriptCore/JavaScriptCore.gyp/JavaScriptCore.gyp:javascriptcore',
        '<(DEPTH)/testing/gmock.gyp:gmock',
        # generated_bindings target is defined in javascriptcore_bindings.gypi
        'generated_bindings',
      ],
    },

    {
      'target_name': 'bindings_test',
      'type': '<(gtest_target_type)',
      'sources': [
        'boolean_type_bindings_test.cc',
        'constants_bindings_test.cc',
        'constructor_bindings_test.cc',
        'callback_function_test.cc',
        'dependent_interface_test.cc',
        'dom_string_bindings_test.cc',
        'getter_setter_test.cc',
        'global_interface_bindings_test.cc',
        'inheritance_bindings_test.cc',
        'interface_object_test.cc',
        'nullable_types_bindings_test.cc',
        'numeric_type_bindings_test.cc',
        'object_bindings_test.cc',
        'operations_bindings_test.cc',
        'optional_arguments_bindings_test.cc',
        'variadic_arguments_bindings_test.cc',
      ],
      'dependencies': [
        'bindings',
        '<(DEPTH)/base/base.gyp:run_all_unittests',
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/script/javascriptcore.gyp:engine',
        '<(DEPTH)/testing/gmock.gyp:gmock',
        '<(DEPTH)/testing/gtest.gyp:gtest',
      ],
    },

    {
      'target_name': 'bindings_test_deploy',
      'type': 'none',
      'dependencies': [
        'bindings_test',
      ],
      'variables': {
        'executable_name': 'bindings_test',
      },
      'includes': [ '../../build/deploy.gypi' ],
    },

    {
      'target_name': 'bindings_sandbox',
      'type': '<(final_executable_type)',
      'sources': [
        'bindings_sandbox_main.cc',
      ],
      'dependencies': [
        'bindings',
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/script/javascriptcore.gyp:engine',
      ]
    },

    {
      'target_name': 'bindings_sandbox_deploy',
      'type': 'none',
      'dependencies': [
        'bindings_sandbox',
      ],
      'variables': {
        'executable_name': 'bindings_sandbox',
      },
      'includes': [ '../../build/deploy.gypi' ],
    },
  ],
}
