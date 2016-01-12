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

#ifndef GLIMP_GLES_DRAW_STATE_H_
#define GLIMP_GLES_DRAW_STATE_H_

#include <algorithm>
#include <utility>
#include <vector>

#include "glimp/egl/surface.h"
#include "glimp/gles/buffer.h"
#include "glimp/gles/program.h"
#include "glimp/gles/sampler.h"
#include "glimp/gles/vertex_attribute.h"
#include "glimp/nb/ref_counted.h"

namespace glimp {
namespace gles {

// Types passed in as parameters to draw calls (like DrawArrays()) to
// describe the set of only enabled vertex attributes.
typedef std::vector<std::pair<unsigned int, VertexAttribute*> >
    EnabledVertexAttributeList;

// Similar to EnabledVertexAttributeList, but lists only samplers with
// textures bound to them.
typedef std::vector<std::pair<unsigned int, Sampler*> > EnabledSamplerList;

struct ViewportState {
  ViewportState() : x(-1), y(-1), width(-1), height(-1) {}
  ViewportState(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}

  int x;
  int y;
  int width;
  int height;
};

struct ScissorState {
  ScissorState() : x(-1), y(-1), width(-1), height(-1) {}
  ScissorState(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}

  int x;
  int y;
  int width;
  int height;
};

struct ClearColor {
  // Setup initial values as defined in the specification.
  //   https://www.khronos.org/opengles/sdk/docs/man/xhtml/glClearColor.xml
  ClearColor() : red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f) {}
  ClearColor(float red, float green, float blue, float alpha)
      : red(std::min(1.0f, std::max(1.0f, red))),
        green(std::min(1.0f, std::max(1.0f, green))),
        blue(std::min(1.0f, std::max(1.0f, blue))),
        alpha(std::min(1.0f, std::max(1.0f, alpha))) {}

  // Clear color properties setup by calls to glClearColor().
  //   https://www.khronos.org/opengles/sdk/docs/man/xhtml/glClearColor.xml
  float red;
  float green;
  float blue;
  float alpha;
};

// Represents the state set by glColorMask().
//   https://www.khronos.org/opengles/sdk/docs/man/xhtml/glColorMask.xml
struct ColorMask {
  // Setup initial values as defined in the specification.
  //   https://www.khronos.org/opengles/sdk/docs/man/xhtml/glColorMask.xml
  ColorMask() : red(true), green(true), blue(true), alpha(true) {}
  ColorMask(bool red, bool green, bool blue, bool alpha)
      : red(red), green(green), blue(blue), alpha(alpha) {}

  bool red;
  bool green;
  bool blue;
  bool alpha;
};

// The DrawState structure aggregates all GL state relevant to draw (or clear)
// commands.  It will be modified as GL ES commands are issued, but it will
// only be propagated to the platform-specific implementations when draw (or
// clear) calls are made.  A dirty flag is kept
struct DrawState {
  DrawState() : draw_surface(NULL) {}
  // The color the next color buffer clear will clear to.
  ClearColor clear_color;

  // Identifies which channels a draw (or clear) call is permitted to modify.
  ColorMask color_mask;

  // The current surface that draw (or clear) commands will target.
  egl::Surface* draw_surface;

  // The list of all active samplers that are available to the next draw call.
  EnabledSamplerList samplers;

  // The list of vertex attribute binding information for the next draw call.
  EnabledVertexAttributeList vertex_attributes;

  // The scissor rectangle.  Draw calls should not modify pixels outside of
  // this.
  ScissorState scissor;

  // The viewport defines how normalized device coordinates should be
  // transformed to screen pixel coordinates.
  ViewportState viewport;

  // The currently bound array buffer, set by calling
  // glBindBuffer(GL_ARRAY_BUFFER).
  nb::scoped_refptr<Buffer> array_buffer;

  // The currently bound element array buffer, set by calling
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER).
  nb::scoped_refptr<Buffer> element_array_buffer;

  // The currently in-use Program object, set by a call to glUseProgram().
  nb::scoped_refptr<Program> used_program;
};

// The dirty flags structure tracks which draw state members have been modified
// since the last draw call was made.  This can be leveraged by implementations
// to avoid re-configurating draw state that has not changed.  Implementations
// should manually set these flags to false after they have been processed.
struct DrawStateDirtyFlags {
  DrawStateDirtyFlags() { MarkAll(); }

  void MarkAll() {
    clear_color_dirty = true;
    color_mask_dirty = true;
    draw_surface_dirty = true;
    samplers_dirty = true;
    vertex_attributes_dirty = true;
    scissor_dirty = true;
    viewport_dirty = true;
    array_buffer_dirty = true;
    element_array_buffer_dirty = true;
    used_program_dirty = true;
  }

  bool clear_color_dirty;
  bool color_mask_dirty;
  bool draw_surface_dirty;
  bool samplers_dirty;
  bool vertex_attributes_dirty;
  bool scissor_dirty;
  bool viewport_dirty;
  bool array_buffer_dirty;
  bool element_array_buffer_dirty;
  bool used_program_dirty;
};

}  // namespace gles
}  // namespace glimp

#endif  // GLIMP_GLES_DRAW_STATE_H_
