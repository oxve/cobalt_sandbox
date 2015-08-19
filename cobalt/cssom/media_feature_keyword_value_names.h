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

#ifndef CSSOM_MEDIA_FEATURE_KEYWORD_VALUE_NAMES_H_
#define CSSOM_MEDIA_FEATURE_KEYWORD_VALUE_NAMES_H_

namespace cobalt {
namespace cssom {

// Lower-case names of CSS media feature values that are not of type length,
// ratio, integer, or resolution.
//   http://www.w3.org/TR/css3-mediaqueries/#media1
extern const char* const kInterlaceMediaFeatureKeywordValueName;
extern const char* const kLandscapeMediaFeatureKeywordValueName;
extern const char* const kPortraitMediaFeatureKeywordValueName;
extern const char* const kProgressiveMediaFeatureKeywordValueName;

}  // namespace cssom
}  // namespace cobalt

#endif  // CSSOM_MEDIA_FEATURE_KEYWORD_VALUE_NAMES_H_
