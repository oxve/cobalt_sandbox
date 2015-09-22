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

// A mutually exclusive lock that can be used to coordinate with other threads.

#ifndef STARBOARD_MUTEX_H_
#define STARBOARD_MUTEX_H_

#include "starboard/export.h"
#include "starboard/types.h"

#ifdef __cplusplus
extern "C" {
#endif

// The private representation of a mutex for synchronizing access to a resource
// between threads. A mutex is assumed to put the waiting thread to sleep,
// rather than spinning.
typedef struct SbMutexPrivate SbMutexPrivate;

// A handle to a mutex.
typedef SbMutexPrivate *SbMutex;

// Enumeration of possible results from acquiring a mutex.
typedef enum SbMutexResult {
  // The mutex was acquired successfully.
  kSbMutexAcquired,

  // The mutex was not acquired because it was held by someone else.
  kSbMutexBusy,

  // The mutex has already been destroyed.
  kSbMutexDestroyed,
} SbMutexResult;

// Well-defined value for an invalid mutex handle.
const SbMutex kSbMutexInvalid = (SbMutex)kSbInvalidPointer;

// Returns whether the given mutex handle is valid.
SB_C_INLINE bool SbMutexIsValid(SbMutex mutex) {
  return mutex != kSbMutexInvalid;
}

// Returns whether the given result is a success.
SB_C_INLINE bool SbMutexIsSuccess(SbMutexResult result) {
  return result == kSbMutexAcquired;
}

// Creates a new mutex, returning the handle to the newly created mutex, or
// kSbMutexInvalid if unable to create a new mutex.
SB_EXPORT SbMutex SbMutexCreate();

// Destroys a mutex, returning whether the destruction was successful. The mutex
// specified by |mutex| is invalidated.
SB_EXPORT bool SbMutexDestroy(SbMutex mutex);

// Acquires |mutex|, blocking indefinitely, returning the acquisition result.
// SbMutexes are not reentrant, so a recursive acquisition will block forever.
SB_EXPORT SbMutexResult SbMutexAcquire(SbMutex mutex);

// Acquires |mutex|, without blocking, returning the acquisition result.
// SbMutexes are not reentrant, so a recursive acquisition will always fail.
SB_EXPORT SbMutexResult SbMutexAcquireTry(SbMutex mutex);

// Releases |mutex| held by the current thread, returning whether the release
// was successful. Releases should always be successful if the mutex is held by
// the current thread.
SB_EXPORT bool SbMutexRelease(SbMutex handle);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // STARBOARD_MUTEX_H_
