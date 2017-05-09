// Copyright 2017 Google Inc. All Rights Reserved.
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

#ifndef STARBOARD_WIN32_DIRECTORY_INTERNAL_H_
#define STARBOARD_WIN32_DIRECTORY_INTERNAL_H_

#include "starboard/directory.h"

#include <algorithm>
#include <deque>
#include <string>

#include "starboard/log.h"
#include "starboard/memory.h"
#include "starboard/shared/internal_only.h"
#include "starboard/shared/win32/file_internal.h"

class SbDirectoryPrivate {
 public:
  explicit SbDirectoryPrivate(HANDLE handle) : directory_handle(handle) {}

  bool HasValidHandle() const {
    return starboard::shared::win32::IsValidHandle(directory_handle);
  }

  HANDLE directory_handle;
  std::deque<std::string> next_directory_entries;

  // SbDirectoryPrivate is neither copyable nor movable.
  SbDirectoryPrivate(const SbDirectoryPrivate&) = delete;
  SbDirectoryPrivate& operator=(const SbDirectoryPrivate&) = delete;
};

namespace starboard {
namespace shared {
namespace win32 {

inline bool HasValidHandle(SbDirectory directory) {
  if (!SbDirectoryIsValid(directory)) {
    return false;
  }

  return directory->HasValidHandle();
}

// This function strips trailing file separators from a directory name.
// For example if the directory name was "C:\\Temp\\\\\\", it would
// strip them, so that the directory name is now to be "C:\\temp".
inline void TrimExtraFileSeparators(std::wstring* dirname_pointer) {
  SB_DCHECK(dirname_pointer);
  std::wstring& dirname = *dirname_pointer;
  auto new_end =
      std::find_if_not(dirname.rbegin(), dirname.rend(), [](wchar_t c) {
        return c == SB_FILE_SEP_CHAR || c == SB_FILE_ALT_SEP_CHAR;
      });
  dirname.erase(new_end.base(), dirname.end());
}

inline bool IsAbsolutePath(const std::wstring& path) {
  wchar_t full_path[SB_FILE_MAX_PATH];
  DWORD full_path_size =
      GetFullPathNameW(path.c_str(), SB_ARRAY_SIZE(full_path), full_path, NULL);
  if (full_path_size == 0) {
    return false;
  }

  return CompareStringEx(LOCALE_NAME_USER_DEFAULT, NORM_IGNORECASE,
                         path.c_str(), path.size(), full_path, full_path_size,
                         NULL, NULL, 0) == CSTR_EQUAL;
}

}  // namespace win32
}  // namespace shared
}  // namespace starboard

#endif  // STARBOARD_WIN32_DIRECTORY_INTERNAL_H_
