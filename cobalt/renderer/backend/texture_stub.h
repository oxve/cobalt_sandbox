/*
 * Copyright 2014 Google Inc. All Rights Reserved.
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

#ifndef RENDERER_BACKEND_TEXTURE_STUB_H_
#define RENDERER_BACKEND_TEXTURE_STUB_H_

#include "cobalt/renderer/backend/surface_info.h"
#include "cobalt/renderer/backend/texture.h"

namespace cobalt {
namespace renderer {
namespace backend {

// Maintains a scoped array of texture data.
class TextureDataStub : public TextureData {
 public:
  explicit TextureDataStub(const SurfaceInfo& surface_info)
      : surface_info_(surface_info) {}

  const SurfaceInfo& GetSurfaceInfo() const OVERRIDE { return surface_info_; }
  int GetPitchInBytes() const OVERRIDE {
    return surface_info_.width *
           SurfaceInfo::BytesPerPixel(surface_info_.format);
  }
  uint8_t* GetMemory() OVERRIDE { return NULL; }

 private:
  SurfaceInfo surface_info_;
};

// Acts as a texture in the stub graphics system.  It does not store any pixel
// information and cannot redraw itself, but it does store surface information
// and so even the stub graphics system can query for texture metadata.
class TextureStub : public Texture {
 public:
  explicit TextureStub(const SurfaceInfo& surface_info)
      : surface_info_(surface_info) {}

  const SurfaceInfo& GetSurfaceInfo() OVERRIDE { return surface_info_; }

 private:
  SurfaceInfo surface_info_;
};

}  // namespace backend
}  // namespace renderer
}  // namespace cobalt

#endif  // RENDERER_BACKEND_TEXTURE_STUB_H_
