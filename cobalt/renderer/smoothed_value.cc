/*
 * Copyright 2016 Google Inc. All Rights Reserved.
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

#include "cobalt/renderer/smoothed_value.h"

namespace cobalt {
namespace renderer {

SmoothedValue::SmoothedValue(base::TimeDelta time_to_converge)
      : time_to_converge_(time_to_converge), previous_derivative_(0) {
  DCHECK(base::TimeDelta() < time_to_converge_);
}

void SmoothedValue::SetTarget(float target) {
  if (previous_target_) {
    previous_derivative_ = GetCurrentDerivative();
  }
  previous_target_ = target_;
  target_ = target;
  target_set_time_ = base::TimeTicks::HighResNow();
}

void SmoothedValue::SnapToTarget() {
  previous_target_ = *target_;
  previous_derivative_ = 0;
}

float SmoothedValue::GetCurrentValue() const {
  if (!previous_target_) {
    // If only one target has ever been set, simply return it.
    return *target_;
  }

  // Compute the current value based off of a cubic bezier curve.
  float t = SmoothedValue::t();
  float one_minus_t = 1 - t;
  float P0 = SmoothedValue::P0();
  float P1 = SmoothedValue::P1();
  float P2 = SmoothedValue::P2();
  float P3 = SmoothedValue::P3();

  return one_minus_t * one_minus_t * one_minus_t * P0 +
         3 * one_minus_t * one_minus_t * t * P1 + 3 * one_minus_t * t * t * P2 +
         t * t * t * P3;
}

float SmoothedValue::t() const {
  DCHECK(target_) << "SetTarget() must have been called previously.";

  base::TimeDelta time_diff = base::TimeTicks::HighResNow() - target_set_time_;

  float t = static_cast<float>(time_diff.InMillisecondsF() /
                               time_to_converge_.InMillisecondsF());

  DCHECK_LE(0, t);

  return t > 1.0f ? 1.0f : t;
}

float SmoothedValue::P1() const {
  // See comments in header for why P1() is calculated this way.
  return *previous_target_ + previous_derivative_ / 3.0f;
}

float SmoothedValue::P2() const {
  // See comments in header for why P2() is calculated this way.
  return P3();
}

float SmoothedValue::GetCurrentDerivative() const {
  if (!previous_target_) {
    // If only one target has ever been set, simply return it.
    return 0;
  }

  float t = SmoothedValue::t();
  float one_minus_t = 1 - t;
  float P0 = SmoothedValue::P0();
  float P1 = SmoothedValue::P1();
  float P2 = SmoothedValue::P2();
  float P3 = SmoothedValue::P3();

  return 3 * one_minus_t * one_minus_t * (P1 - P0) +
         6 * one_minus_t * t * (P2 - P1) + 3 * t * t * (P3 - P2);
}

}  // namespace renderer
}  // namespace cobalt
