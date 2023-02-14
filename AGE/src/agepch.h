//
// Created by alex on 27.08.22.
//

#pragma once

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

#include "Age/Debug/Assert.h"
#include "Age/Core/Log.h"
#include "Age/Debug/Instrumentor.h"

#ifdef AGE_PLATFORM_WINDOWS
  #include <Windows.h>
#endif