// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by {{code_generator}}. DO NOT MODIFY!

#ifndef {{v8_class}}_h
#define {{v8_class}}_h

{% filter conditional(conditional_string) %}
{% for filename in header_includes %}
#include "{{filename}}"
{% endfor %}

namespace blink {

{% if has_event_constructor %}
class Dictionary;
{% endif %}
{% if named_constructor %}
class {{v8_class}}Constructor {
public:
    static v8::Handle<v8::FunctionTemplate> domTemplate(v8::Isolate*);
    static const WrapperTypeInfo wrapperTypeInfo;
};

{% endif %}
class {{v8_class}} {
public:
    {% if has_private_script %}
    class PrivateScript {
    public:
        {% for method in methods if method.is_implemented_in_private_script %}
        static bool {{method.name}}Method({{method.argument_declarations_for_private_script | join(', ')}});
        {% endfor %}
        {% for attribute in attributes if attribute.is_implemented_in_private_script %}
        static bool {{attribute.name}}AttributeGetter(LocalFrame* frame, {{cpp_class}}* holderImpl, {{attribute.cpp_type}}* result);
        {% if not attribute.is_read_only %}
        static bool {{attribute.name}}AttributeSetter(LocalFrame* frame, {{cpp_class}}* holderImpl, {{attribute.argument_cpp_type}} cppValue);
        {% endif %}
        {% endfor %}
    };

    {% endif %}
    static bool hasInstance(v8::Handle<v8::Value>, v8::Isolate*);
    {% if is_array_buffer_or_view %}
    static {{cpp_class}}* toImpl(v8::Handle<v8::Object> object);
    {% else %}
    static v8::Handle<v8::Object> findInstanceInPrototypeChain(v8::Handle<v8::Value>, v8::Isolate*);
    static v8::Handle<v8::FunctionTemplate> domTemplate(v8::Isolate*);
    static {{cpp_class}}* toImpl(v8::Handle<v8::Object> object)
    {
        return blink::toScriptWrappableBase(object)->toImpl<{{cpp_class}}>();
    }
    {% endif %}
    static {{cpp_class}}* toImplWithTypeCheck(v8::Isolate*, v8::Handle<v8::Value>);
    {% if has_partial_interface %}
    static WrapperTypeInfo wrapperTypeInfo;
    {% else %}
    static const WrapperTypeInfo wrapperTypeInfo;
    {% endif %}
    static void refObject(ScriptWrappableBase*);
    static void derefObject(ScriptWrappableBase*);
    static void trace(Visitor* visitor, ScriptWrappableBase* scriptWrappableBase)
    {
        {% if gc_type == 'GarbageCollectedObject' %}
        visitor->trace(scriptWrappableBase->toImpl<{{cpp_class}}>());
        {% elif gc_type == 'WillBeGarbageCollectedObject' %}
#if ENABLE(OILPAN)
        visitor->trace(scriptWrappableBase->toImpl<{{cpp_class}}>());
#endif
        {% endif %}
    }
    {% if has_visit_dom_wrapper %}
    static void visitDOMWrapper(v8::Isolate*, ScriptWrappableBase*, const v8::Persistent<v8::Object>&);
    {% endif %}
    {% if is_active_dom_object %}
    static ActiveDOMObject* toActiveDOMObject(v8::Handle<v8::Object>);
    {% endif %}
    {% if is_event_target %}
    static EventTarget* toEventTarget(v8::Handle<v8::Object>);
    {% endif %}
    {% if interface_name == 'Window' %}
    static v8::Handle<v8::ObjectTemplate> getShadowObjectTemplate(v8::Isolate*);
    {% endif %}
    {% for method in methods %}
    {% if method.is_custom %}
    {% filter conditional(method.conditional_string) %}
    static void {{method.name}}MethodCustom(const v8::FunctionCallbackInfo<v8::Value>&);
    {% endfilter %}
    {% endif %}
    {% endfor %}
    {% if constructors or has_custom_constructor or has_event_constructor %}
    static void constructorCallback(const v8::FunctionCallbackInfo<v8::Value>&);
    {% endif %}
    {% if has_custom_constructor %}
    static void constructorCustom(const v8::FunctionCallbackInfo<v8::Value>&);
    {% endif %}
    {% for attribute in attributes %}
    {% if attribute.has_custom_getter %}{# FIXME: and not attribute.implemented_by #}
    {% filter conditional(attribute.conditional_string) %}
    static void {{attribute.name}}AttributeGetterCustom(const v8::PropertyCallbackInfo<v8::Value>&);
    {% endfilter %}
    {% endif %}
    {% if attribute.has_custom_setter %}{# FIXME: and not attribute.implemented_by #}
    {% filter conditional(attribute.conditional_string) %}
    static void {{attribute.name}}AttributeSetterCustom(v8::Local<v8::Value>, const v8::PropertyCallbackInfo<void>&);
    {% endfilter %}
    {% endif %}
    {% endfor %}
    {# Custom special operations #}
    {% if indexed_property_getter and indexed_property_getter.is_custom %}
    static void indexedPropertyGetterCustom(uint32_t, const v8::PropertyCallbackInfo<v8::Value>&);
    {% endif %}
    {% if indexed_property_setter and indexed_property_setter.is_custom %}
    static void indexedPropertySetterCustom(uint32_t, v8::Local<v8::Value>, const v8::PropertyCallbackInfo<v8::Value>&);
    {% endif %}
    {% if indexed_property_deleter and indexed_property_deleter.is_custom %}
    static void indexedPropertyDeleterCustom(uint32_t, const v8::PropertyCallbackInfo<v8::Boolean>&);
    {% endif %}
    {% if named_property_getter and named_property_getter.is_custom %}
    static void namedPropertyGetterCustom(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>&);
    {% endif %}
    {% if named_property_setter and named_property_setter.is_custom %}
    static void namedPropertySetterCustom(v8::Local<v8::String>, v8::Local<v8::Value>, const v8::PropertyCallbackInfo<v8::Value>&);
    {% endif %}
    {% if named_property_getter and
          named_property_getter.is_custom_property_query %}
    static void namedPropertyQueryCustom(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Integer>&);
    {% endif %}
    {% if named_property_deleter and named_property_deleter.is_custom %}
    static void namedPropertyDeleterCustom(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Boolean>&);
    {% endif %}
    {% if named_property_getter and
          named_property_getter.is_custom_property_enumerator %}
    static void namedPropertyEnumeratorCustom(const v8::PropertyCallbackInfo<v8::Array>&);
    {% endif %}
    {# END custom special operations #}
    {% if has_custom_legacy_call_as_function %}
    static void legacyCallCustom(const v8::FunctionCallbackInfo<v8::Value>&);
    {% endif %}
    {# Custom internal fields #}
    {% set custom_internal_field_counter = 0 %}
    {% if is_event_target and not is_node %}
    {# Event listeners on DOM nodes are explicitly supported in the GC controller. #}
    static const int eventListenerCacheIndex = v8DefaultWrapperInternalFieldCount + {{custom_internal_field_counter}};
    {% set custom_internal_field_counter = custom_internal_field_counter + 1 %}
    {% endif %}
    {# persistentHandleIndex must be the last field, if it is present.
       Detailed explanation: https://codereview.chromium.org/139173012
       FIXME: Remove this internal field, and share one field for either:
       * a persistent handle (if the object is in oilpan) or
       * a C++ pointer to the DOM object (if the object is not in oilpan) #}
    static const int internalFieldCount = v8DefaultWrapperInternalFieldCount + {{custom_internal_field_counter}};
    {# End custom internal fields #}
    static inline ScriptWrappableBase* toScriptWrappableBase({{cpp_class}}* impl)
    {
        return impl->toScriptWrappableBase();
    }
    {% if interface_name == 'Window' %}
    static bool namedSecurityCheckCustom(v8::Local<v8::Object> host, v8::Local<v8::Value> key, v8::AccessType, v8::Local<v8::Value> data);
    static bool indexedSecurityCheckCustom(v8::Local<v8::Object> host, uint32_t index, v8::AccessType, v8::Local<v8::Value> data);
    {% endif %}
    static void installConditionallyEnabledProperties(v8::Handle<v8::Object>, v8::Isolate*){% if has_conditional_attributes %};
    {% else %} { }
    {% endif %}
    static void installConditionallyEnabledMethods(v8::Handle<v8::Object>, v8::Isolate*){% if conditionally_enabled_methods %};
    {% else %} { }
    {% endif %}
    {% if has_partial_interface %}
    static void updateWrapperTypeInfo(InstallTemplateFunction, InstallConditionallyEnabledMethodsFunction);
    static void install{{v8_class}}Template(v8::Handle<v8::FunctionTemplate>, v8::Isolate*);
    {% for method in methods if method.overloads and method.overloads.has_partial_overloads %}
    static void register{{method.name | blink_capitalize}}MethodForPartialInterface(void (*)(const v8::FunctionCallbackInfo<v8::Value>&));
    {% endfor %}
    {% endif %}
    {% if not has_custom_to_v8 and not is_script_wrappable %}

private:
    friend v8::Handle<v8::Object> wrap({{cpp_class}}*, v8::Handle<v8::Object> creationContext, v8::Isolate*);
    static v8::Handle<v8::Object> createWrapper({{pass_cpp_type}}, v8::Handle<v8::Object> creationContext, v8::Isolate*);
    {% endif %}
    {% if has_partial_interface %}

private:
    static InstallTemplateFunction install{{v8_class}}TemplateFunction;
    {% endif %}
};

{% if has_custom_to_v8 %}
v8::Handle<v8::Value> toV8({{cpp_class}}*, v8::Handle<v8::Object> creationContext, v8::Isolate*);

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& callbackInfo, {{cpp_class}}* impl)
{
    v8SetReturnValue(callbackInfo, toV8(impl, callbackInfo.Holder(), callbackInfo.GetIsolate()));
}

template<typename CallbackInfo>
inline void v8SetReturnValueForMainWorld(const CallbackInfo& callbackInfo, {{cpp_class}}* impl)
{
     v8SetReturnValue(callbackInfo, toV8(impl, callbackInfo.Holder(), callbackInfo.GetIsolate()));
}

template<typename CallbackInfo>
inline void v8SetReturnValueFast(const CallbackInfo& callbackInfo, {{cpp_class}}* impl, const ScriptWrappable*)
{
     v8SetReturnValue(callbackInfo, toV8(impl, callbackInfo.Holder(), callbackInfo.GetIsolate()));
}

{% elif not is_script_wrappable %}
v8::Handle<v8::Object> wrap({{cpp_class}}* impl, v8::Handle<v8::Object> creationContext, v8::Isolate*);

inline v8::Handle<v8::Value> toV8({{cpp_class}}* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    if (UNLIKELY(!impl))
        return v8::Null(isolate);
    v8::Handle<v8::Value> wrapper = DOMDataStore::getWrapperNonTemplate(impl, isolate);
    if (!wrapper.IsEmpty())
        return wrapper;

    return wrap(impl, creationContext, isolate);
}

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& callbackInfo, {{cpp_class}}* impl)
{
    if (UNLIKELY(!impl)) {
        v8SetReturnValueNull(callbackInfo);
        return;
    }
    if (DOMDataStore::setReturnValueNonTemplate(callbackInfo.GetReturnValue(), impl))
        return;
    v8::Handle<v8::Object> wrapper = wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
    v8SetReturnValue(callbackInfo, wrapper);
}

template<typename CallbackInfo>
inline void v8SetReturnValueForMainWorld(const CallbackInfo& callbackInfo, {{cpp_class}}* impl)
{
    ASSERT(DOMWrapperWorld::current(callbackInfo.GetIsolate()).isMainWorld());
    if (UNLIKELY(!impl)) {
        v8SetReturnValueNull(callbackInfo);
        return;
    }
    if (DOMDataStore::setReturnValueFromWrapperForMainWorld<{{v8_class}}>(callbackInfo.GetReturnValue(), impl))
        return;
    v8::Handle<v8::Value> wrapper = wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
    v8SetReturnValue(callbackInfo, wrapper);
}

template<typename CallbackInfo, class Wrappable>
inline void v8SetReturnValueFast(const CallbackInfo& callbackInfo, {{cpp_class}}* impl, Wrappable* wrappable)
{
    if (UNLIKELY(!impl)) {
        v8SetReturnValueNull(callbackInfo);
        return;
    }
    if (DOMDataStore::setReturnValueFromWrapperFast<{{v8_class}}>(callbackInfo.GetReturnValue(), impl, callbackInfo.Holder(), wrappable))
        return;
    v8::Handle<v8::Object> wrapper = wrap(impl, callbackInfo.Holder(), callbackInfo.GetIsolate());
    v8SetReturnValue(callbackInfo, wrapper);
}

{% endif %}{# has_custom_to_v8 #}
{% if has_event_constructor %}
bool initialize{{cpp_class}}({{cpp_class}}Init&, const Dictionary&, ExceptionState&, const v8::FunctionCallbackInfo<v8::Value>& info, const String& = "");

{% endif %}
} // namespace blink
{% endfilter %}

#endif // {{v8_class}}_h
