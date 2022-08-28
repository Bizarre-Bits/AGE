//
// Created by alex on 27.08.22.
//

#ifndef AGE_AGEPCH_H
#define AGE_AGEPCH_H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <cstdint>

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef AGE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#endif //AGE_AGEPCH_H
