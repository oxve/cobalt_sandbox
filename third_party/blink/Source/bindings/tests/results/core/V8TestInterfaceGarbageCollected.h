// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#ifndef V8TestInterfaceGarbageCollected_h
#define V8TestInterfaceGarbageCollected_h

#include "bindings/core/v8/ScriptWrappable.h"
#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8DOMWrapper.h"
#include "bindings/core/v8/V8EventTarget.h"
#include "bindings/core/v8/WrapperTypeInfo.h"
#include "bindings/tests/idls/core/TestInterfaceGarbageCollected.h"
#include "platform/heap/Handle.h"

namespace blink {

class V8TestInterfaceGarbageCollected {
public:
    static bool hasInstance(v8::Handle<v8::Value>, v8::Isolate*);
    static v8::Handle<v8::Object> findInstanceInPrototypeChain(v8::Handle<v8::Value>, v8::Isolate*);
    static v8::Handle<v8::FunctionTemplate> domTemplate(v8::Isolate*);
    static TestInterfaceGarbageCollected* toImpl(v8::Handle<v8::Object> object)
    {
        return blink::toScriptWrappableBase(object)->toImpl<TestInterfaceGarbageCollected>();
    }
    static TestInterfaceGarbageCollected* toImplWithTypeCheck(v8::Isolate*, v8::Handle<v8::Value>);
    static const WrapperTypeInfo wrapperTypeInfo;
    static void refObject(ScriptWrappableBase*);
    static void derefObject(ScriptWrappableBase*);
    static void trace(Visitor* visitor, ScriptWrappableBase* scriptWrappableBase)
    {
        visitor->trace(scriptWrappableBase->toImpl<TestInterfaceGarbageCollected>());
    }
    static EventTarget* toEventTarget(v8::Handle<v8::Object>);
    static void constructorCallback(const v8::FunctionCallbackInfo<v8::Value>&);
    static const int eventListenerCacheIndex = v8DefaultWrapperInternalFieldCount + 0;
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + 1;
    static inline ScriptWrappableBase* toScriptWrappableBase(TestInterfaceGarbageCollected* impl)
    {
        return impl->toScriptWrappableBase();
    }
    static void installConditionallyEnabledProperties(v8::Handle<v8::Object>, v8::Isolate*) { }
    static void installConditionallyEnabledMethods(v8::Handle<v8::Object>, v8::Isolate*) { }
};

} // namespace blink

#endif // V8TestInterfaceGarbageCollected_h
