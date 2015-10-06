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

#include "cobalt/dom/keyboard_event.h"

#include "cobalt/dom/event_names.h"
#include "cobalt/dom/keyboard_code.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace cobalt {
namespace dom {

TEST(KeyboardEventTest, ShouldHaveBubblesAndCancelableSet) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_TRUE(keyboard_event->bubbles());
  EXPECT_TRUE(keyboard_event->cancelable());
}

TEST(KeyboardEventTest, CanGetKeyLocation) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_EQ(keyboard_event->key_location(),
            KeyboardEvent::kDomKeyLocationStandard);

  scoped_refptr<KeyboardEvent> keyboard_event_l = new KeyboardEvent(
      EventNames::GetInstance()->keydown(), KeyboardEvent::kDomKeyLocationLeft,
      KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_EQ(keyboard_event_l->key_location(),
            KeyboardEvent::kDomKeyLocationLeft);

  scoped_refptr<KeyboardEvent> keyboard_event_r = new KeyboardEvent(
      EventNames::GetInstance()->keydown(), KeyboardEvent::kDomKeyLocationRight,
      KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_EQ(keyboard_event_r->key_location(),
            KeyboardEvent::kDomKeyLocationRight);
}

TEST(KeyboardEventTest, CanGetKeyIdentifier) {
  scoped_refptr<KeyboardEvent> keyboard_event_a =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, kA, 0, false);
  EXPECT_EQ(keyboard_event_a->key_identifier(), "a");

  scoped_refptr<KeyboardEvent> keyboard_event_ca =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kCtrlKey, kA, 0, false);
  EXPECT_EQ(keyboard_event_ca->key_identifier(), "a");

  scoped_refptr<KeyboardEvent> keyboard_event_sa =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kShiftKey, kA, 0, false);
  EXPECT_EQ(keyboard_event_sa->key_identifier(), "A");

  scoped_refptr<KeyboardEvent> keyboard_event_1 =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, k1, 0, false);
  EXPECT_EQ(keyboard_event_1->key_identifier(), "1");

  scoped_refptr<KeyboardEvent> keyboard_event_s1 =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kShiftKey, k1, 0, false);
  EXPECT_EQ(keyboard_event_s1->key_identifier(), "!");

  scoped_refptr<KeyboardEvent> keyboard_event_left =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, kLeft, 0, false);
  EXPECT_EQ(keyboard_event_left->key_identifier(), "ArrowLeft");

  scoped_refptr<KeyboardEvent> keyboard_event_sleft =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kShiftKey, kLeft, 0, false);
  EXPECT_EQ(keyboard_event_sleft->key_identifier(), "ArrowLeft");
}

TEST(KeyboardEventTest, CanGetAltKey) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_FALSE(keyboard_event->alt_key());

  scoped_refptr<KeyboardEvent> keyboard_event_a =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kAltKey, 0, 0, false);
  EXPECT_TRUE(keyboard_event_a->alt_key());
}

TEST(KeyboardEventTest, CanGetCtrlKey) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_FALSE(keyboard_event->ctrl_key());

  scoped_refptr<KeyboardEvent> keyboard_event_c =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kCtrlKey, 0, 0, false);
  EXPECT_TRUE(keyboard_event_c->ctrl_key());
}

TEST(KeyboardEventTest, CanGetMetaKey) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_FALSE(keyboard_event->meta_key());

  scoped_refptr<KeyboardEvent> keyboard_event_m =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kMetaKey, 0, 0, false);
  EXPECT_TRUE(keyboard_event_m->meta_key());
}

TEST(KeyboardEventTest, CanGetShiftKey) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_FALSE(keyboard_event->shift_key());

  scoped_refptr<KeyboardEvent> keyboard_event_s =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kShiftKey, 0, 0, false);
  EXPECT_TRUE(keyboard_event_s->shift_key());
}

TEST(KeyboardEventTest, CanGetModifierState) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_FALSE(keyboard_event->GetModifierState("Alt"));
  EXPECT_FALSE(keyboard_event->GetModifierState("Control"));
  EXPECT_FALSE(keyboard_event->GetModifierState("Meta"));
  EXPECT_FALSE(keyboard_event->GetModifierState("Shift"));

  scoped_refptr<KeyboardEvent> keyboard_event_m =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kAltKey | KeyboardEvent::kCtrlKey |
                            KeyboardEvent::kMetaKey | KeyboardEvent::kShiftKey,
                        0, 0, false);
  EXPECT_TRUE(keyboard_event_m->GetModifierState("Alt"));
  EXPECT_TRUE(keyboard_event_m->GetModifierState("Control"));
  EXPECT_TRUE(keyboard_event_m->GetModifierState("Meta"));
  EXPECT_TRUE(keyboard_event_m->GetModifierState("Shift"));
}

TEST(KeyboardEventTest, CanGetRepeat) {
  scoped_refptr<KeyboardEvent> keyboard_event =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kNoModifier, 0, 0, false);
  EXPECT_FALSE(keyboard_event->repeat());

  scoped_refptr<KeyboardEvent> keyboard_event_r =
      new KeyboardEvent(EventNames::GetInstance()->keydown(),
                        KeyboardEvent::kDomKeyLocationStandard,
                        KeyboardEvent::kShiftKey, 0, 0, true);
  EXPECT_TRUE(keyboard_event_r->repeat());
}
}  // namespace dom
}  // namespace cobalt
