// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RECT_F_H_
#define UI_GFX_RECT_F_H_

#include <string>

#include "ui/gfx/point_f.h"
#include "ui/gfx/rect_base.h"
#include "ui/gfx/size_f.h"

namespace gfx {

class InsetsF;

// A floating version of gfx::Rect.
class UI_EXPORT RectF : public RectBase<RectF, PointF, SizeF, InsetsF, float> {
 public:
  RectF();
  RectF(float width, float height);
  RectF(float x, float y, float width, float height);
  explicit RectF(const gfx::SizeF& size);
  RectF(const gfx::PointF& origin, const gfx::SizeF& size);

  ~RectF();

  /// Scales the rectangle by |scale|.
  RectF Scale(float scale) const WARN_UNUSED_RESULT {
    return Scale(scale, scale);
  }

  RectF Scale(float x_scale, float y_scale) const WARN_UNUSED_RESULT {
    SizeF newSize = size().Scale(x_scale, y_scale);
    newSize.ClampToNonNegative();
    return RectF(origin().Scale(x_scale, y_scale), newSize);
  }

  std::string ToString() const;
};

#if !defined(COMPILER_MSVC)
extern template class RectBase<RectF, PointF, SizeF, InsetsF, float>;
#endif

}  // namespace gfx

#endif  // UI_GFX_RECT_F_H_
