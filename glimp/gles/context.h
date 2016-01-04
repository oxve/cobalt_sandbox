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

#ifndef GLIMP_GLES_CONTEXT_H_
#define GLIMP_GLES_CONTEXT_H_

#include <GLES3/gl3.h>

#include "glimp/egl/surface.h"
#include "glimp/gles/context_impl.h"
#include "glimp/nb/scoped_ptr.h"
#include "starboard/thread.h"

namespace glimp {
namespace gles {

class Context {
 public:
  Context(nb::scoped_ptr<ContextImpl> context_impl, Context* share_context);

  // Returns current thread's current context, or NULL if nothing is current.
  static Context* GetTLSCurrentContext();

  // Sets the current thread's current context.  None of the parameters can
  // be NULL (use ReleaseTLSCurrentContext() if you wish to release the current
  // context).
  static bool SetTLSCurrentContext(Context* context,
                                   egl::Surface* draw,
                                   egl::Surface* read);

  // Releases the current thread's current context.
  static void ReleaseTLSCurrentContext();

  // Returns the thread that currently holds this Context, or kSbThreadInvalid
  // if no thread currently holds the context.
  SbThread current_thread() const { return current_thread_; }

 private:
  void MakeCurrent(egl::Surface* draw, egl::Surface* read);
  void ReleaseContext();

  // A reference to the platform-specific implementation aspects of the context.
  nb::scoped_ptr<ContextImpl> impl_;

  // The thread that currently holds this context as its current context.
  SbThread current_thread_;

  // Has this context ever been made current before?
  bool has_been_current_;
};

}  // namespace gles
}  // namespace glimp

#endif  // GLIMP_GLES_CONTEXT_H_
