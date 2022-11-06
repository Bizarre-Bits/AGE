//
// Created by alex on 27.08.22.
//

#ifndef AGE_AGEPCH_H
#define AGE_AGEPCH_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cstdint>

#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "Age/Core/Log.h"
#include "Age/Debug/Instrumentor.h"

#ifdef AGE_PLATFORM_WINDOWS
  #include <Windows.h>
#endif

#endif//AGE_AGEPCH_H
