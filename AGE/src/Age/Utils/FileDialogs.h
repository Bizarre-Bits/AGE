//
// Created by alex on 2/22/23.
//

#pragma once

#include "Age/Core/Core.h"

namespace AGE {
  struct FileDialogs {
    static age_string_t OpenFile(const char* filter);
    static age_string_t SaveFile(const char* filter);
  };

}// namespace AGE
