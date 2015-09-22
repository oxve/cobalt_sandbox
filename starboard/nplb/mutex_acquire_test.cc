// Copyright 2015 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "starboard/configuration.h"
#include "starboard/mutex.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

TEST(SbMutexAcquireTest, AcquiresUncontended) {
  SbMutex mutex = SbMutexCreate();
  EXPECT_TRUE(SbMutexIsValid(mutex));

  SbMutexResult result = SbMutexAcquire(mutex);
  EXPECT_EQ(result, kSbMutexAcquired);
  EXPECT_TRUE(SbMutexIsSuccess(result));

  bool release_result = SbMutexRelease(mutex);
  EXPECT_TRUE(release_result);

  bool destroy_result = SbMutexDestroy(mutex);
  EXPECT_TRUE(destroy_result);
}

TEST(SbMutexAcquireTest, DoesNotAcquireInvalid) {
  SbMutex mutex = kSbMutexInvalid;
  SbMutexResult result = SbMutexAcquire(mutex);
  EXPECT_EQ(result, kSbMutexDestroyed);
  EXPECT_FALSE(SbMutexIsSuccess(result));
}

// TODO(***REMOVED***): Add tests for acquiring Mutices contended by other threads.

}  // namespace
