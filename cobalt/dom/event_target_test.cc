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

#include "cobalt/dom/event_target.h"

#include "cobalt/dom/testing/mock_event_listener.h"
#include "cobalt/script/script_object_handle_visitor.h"
#include "testing/gtest/include/gtest/gtest.h"

using ::testing::_;
using ::testing::AllOf;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::InvokeWithoutArgs;
using ::testing::Pointee;
using ::testing::Property;

namespace cobalt {
namespace dom {

namespace {

// Use NiceMock as we don't care about EqualTo or MarkJSObjectAsNotCollectable
// calls on the listener in most cases.
typedef ::testing::NiceMock<testing::MockEventListener> NiceMockEventListener;

class TestScriptObjectHandleVisitor : public script::ScriptObjectHandleVisitor {
 private:
  void Visit(script::ScriptObjectHandle* handle) OVERRIDE{};
};

void ExpectHandleEventCall(const scoped_refptr<NiceMockEventListener>& listener,
                           const scoped_refptr<Event>& event,
                           const scoped_refptr<EventTarget>& target) {
  // Note that we must pass the raw pointer to avoid reference counting issue.
  EXPECT_CALL(
      *listener,
      HandleEvent(AllOf(
          Eq(event.get()), Pointee(Property(&Event::target, Eq(target.get()))),
          Pointee(Property(&Event::current_target, Eq(target.get()))),
          Pointee(Property(&Event::event_phase, Eq(Event::kAtTarget))),
          Pointee(Property(&Event::IsBeingDispatched, Eq(true))))))
      .RetiresOnSaturation();
}

void ExpectHandleEventCall(const scoped_refptr<NiceMockEventListener>& listener,
                           const scoped_refptr<Event>& event,
                           const scoped_refptr<EventTarget>& target,
                           void (Event::*function)(void)) {
  // Note that we must pass the raw pointer to avoid reference counting issue.
  EXPECT_CALL(
      *listener,
      HandleEvent(AllOf(
          Eq(event.get()), Pointee(Property(&Event::target, Eq(target.get()))),
          Pointee(Property(&Event::current_target, Eq(target.get()))),
          Pointee(Property(&Event::event_phase, Eq(Event::kAtTarget))),
          Pointee(Property(&Event::IsBeingDispatched, Eq(true))))))
      .WillOnce(InvokeWithoutArgs(event.get(), function))
      .RetiresOnSaturation();
}

void ExpectNoHandleEventCall(
    const scoped_refptr<NiceMockEventListener>& listener) {
  EXPECT_CALL(*listener, HandleEvent(_)).Times(0);
}

}  // namespace

TEST(EventTargetTest, SingleEventListenerFired) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event = new Event("fired");
  scoped_refptr<NiceMockEventListener> event_listener =
      new NiceMockEventListener;

  ExpectHandleEventCall(event_listener, event, event_target);
  event_target->AddEventListener("fired", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event));
}

TEST(EventTargetTest, SingleEventListenerNotFired) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event = new Event("fired");
  scoped_refptr<NiceMockEventListener> event_listener =
      new NiceMockEventListener;

  ExpectNoHandleEventCall(event_listener);
  event_target->AddEventListener("notfired", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event));
}

// Test if multiple event listeners of different event types can be added and
// fired properly.
TEST(EventTargetTest, MultipleEventListeners) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event = new Event("fired");
  scoped_refptr<NiceMockEventListener> event_listener_fired_1 =
      new NiceMockEventListener;
  scoped_refptr<NiceMockEventListener> event_listener_fired_2 =
      new NiceMockEventListener;
  scoped_refptr<NiceMockEventListener> event_listener_not_fired =
      new NiceMockEventListener;

  InSequence in_sequence;
  ExpectHandleEventCall(event_listener_fired_1, event, event_target);
  ExpectHandleEventCall(event_listener_fired_2, event, event_target);
  ExpectNoHandleEventCall(event_listener_not_fired);

  event_target->AddEventListener("fired", event_listener_fired_1, false);
  event_target->AddEventListener("notfired", event_listener_not_fired, false);
  event_target->AddEventListener("fired", event_listener_fired_2, true);

  EXPECT_TRUE(event_target->DispatchEvent(event));
}

// Test if event listener can be added and later removed.
TEST(EventTargetTest, AddRemoveEventListener) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event = new Event("fired");
  scoped_refptr<NiceMockEventListener> event_listener =
      new NiceMockEventListener;

  ExpectHandleEventCall(event_listener, event, event_target);
  event_target->AddEventListener("fired", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event));

  ExpectNoHandleEventCall(event_listener);
  event_target->RemoveEventListener("fired", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event));

  ExpectHandleEventCall(event_listener, event, event_target);
  event_target->AddEventListener("fired", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event));
}

// Test if one event listener can be used by multiple events.
TEST(EventTargetTest, EventListenerReuse) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event_1 = new Event("fired_1");
  scoped_refptr<Event> event_2 = new Event("fired_2");
  scoped_refptr<NiceMockEventListener> event_listener =
      new NiceMockEventListener;

  ExpectHandleEventCall(event_listener, event_1, event_target);
  ExpectHandleEventCall(event_listener, event_2, event_target);
  event_target->AddEventListener("fired_1", event_listener, false);
  event_target->AddEventListener("fired_2", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event_1));
  EXPECT_TRUE(event_target->DispatchEvent(event_2));

  ExpectHandleEventCall(event_listener, event_1, event_target);
  event_target->RemoveEventListener("fired_2", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event_1));
  EXPECT_TRUE(event_target->DispatchEvent(event_2));

  ExpectHandleEventCall(event_listener, event_1, event_target);
  // The capture flag is not the same so the event will not be removed.
  event_target->RemoveEventListener("fired_1", event_listener, true);
  EXPECT_TRUE(event_target->DispatchEvent(event_1));
  EXPECT_TRUE(event_target->DispatchEvent(event_2));

  ExpectNoHandleEventCall(event_listener);
  event_target->RemoveEventListener("fired_1", event_listener, false);
  EXPECT_TRUE(event_target->DispatchEvent(event_1));
  EXPECT_TRUE(event_target->DispatchEvent(event_2));
}

TEST(EventTargetTest, StopPropagation) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event = new Event("fired");
  scoped_refptr<NiceMockEventListener> event_listener_fired_1 =
      new NiceMockEventListener;
  scoped_refptr<NiceMockEventListener> event_listener_fired_2 =
      new NiceMockEventListener;

  InSequence in_sequence;
  ExpectHandleEventCall(event_listener_fired_1, event, event_target,
                        &Event::StopPropagation);
  ExpectHandleEventCall(event_listener_fired_2, event, event_target);

  event_target->AddEventListener("fired", event_listener_fired_1, false);
  event_target->AddEventListener("fired", event_listener_fired_2, true);

  EXPECT_TRUE(event_target->DispatchEvent(event));
}

TEST(EventTargetTest, StopImmediatePropagation) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<Event> event = new Event("fired");
  scoped_refptr<NiceMockEventListener> event_listener_fired_1 =
      new NiceMockEventListener;
  scoped_refptr<NiceMockEventListener> event_listener_fired_2 =
      new NiceMockEventListener;

  ExpectHandleEventCall(event_listener_fired_1, event, event_target,
                        &Event::StopImmediatePropagation);
  ExpectNoHandleEventCall(event_listener_fired_2);

  event_target->AddEventListener("fired", event_listener_fired_1, false);
  event_target->AddEventListener("fired", event_listener_fired_2, true);

  EXPECT_TRUE(event_target->DispatchEvent(event));
}

TEST(EventTargetTest, PreventDefault) {
  scoped_refptr<Event> event;
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<NiceMockEventListener> event_listener_fired =
      new NiceMockEventListener;

  event_target->AddEventListener("fired", event_listener_fired, false);
  event = new Event("fired", Event::kNotBubbles, Event::kNotCancelable);
  ExpectHandleEventCall(event_listener_fired, event, event_target,
                        &Event::PreventDefault);
  EXPECT_TRUE(event_target->DispatchEvent(event));

  event = new Event("fired", Event::kNotBubbles, Event::kCancelable);
  ExpectHandleEventCall(event_listener_fired, event, event_target,
                        &Event::PreventDefault);
  EXPECT_FALSE(event_target->DispatchEvent(event));
}

// Test MarkJSObjectAsNotCollectable on added event listener is called when
// it is called on the EventTarget.
TEST(EventTargetTest, MarkJSObjectAsNotCollectable) {
  scoped_refptr<EventTarget> event_target = new EventTarget;
  scoped_refptr<NiceMockEventListener> event_listener =
      new NiceMockEventListener;
  TestScriptObjectHandleVisitor script_object_handle_visitor;

  EXPECT_CALL(*event_listener,
              MarkJSObjectAsNotCollectable(&script_object_handle_visitor))
      .Times(::testing::AtLeast(1));
  event_target->AddEventListener("fired", event_listener, false);
  event_target->MarkJSObjectAsNotCollectable(&script_object_handle_visitor);

  EXPECT_CALL(*event_listener, MarkJSObjectAsNotCollectable(
                                   &script_object_handle_visitor)).Times(0);
  event_target->RemoveEventListener("fired", event_listener, false);
  event_target->MarkJSObjectAsNotCollectable(&script_object_handle_visitor);
}

}  // namespace dom
}  // namespace cobalt
