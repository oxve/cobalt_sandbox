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

// Should be included by internal headers - will assert that the header is only
// included in starboard implementation files, and not publicly.

#ifndef STARBOARD_SHARED_INTERNAL_ONLY_H_
#define STARBOARD_SHARED_INTERNAL_ONLY_H_

#if !defined(STARBOARD_IMPLEMENTATION)
#  error "This file cannot be included outside of a starboard implementation."
#endif

#endif  // STARBOARD_SHARED_INTERNAL_ONLY_H_
