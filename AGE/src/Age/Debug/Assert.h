//
// Created by alex on 28.08.22.
//

#pragma once

#include "Age/Core/Core.h"
#include "Age/Core/Log.h"

#include <filesystem>

#ifdef AGE_ENABLE_ASSERTS
  #define AGE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
    {                                                     \
      if (!(check)) {                                     \
        AGE##type##ERROR(msg, __VA_ARGS__);               \
        AGE_DEBUG_BREAK();                                \
      }                                                   \
    }
  #define AGE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) AGE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
  #define AGE_INTERNAL_ASSERT_NO_MSG(type, check) AGE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", AGE_STRINGIFY(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

  #define AGE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
  #define AGE_INTERNAL_ASSERT_GET_MACRO(...) AGE_EXPAND_MACRO(AGE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, AGE_INTERNAL_ASSERT_WITH_MSG, AGE_INTERNAL_ASSERT_NO_MSG))

  #define AGE_ASSERT(...) AGE_EXPAND_MACRO(AGE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
  #define AGE_CORE_ASSERT(...) AGE_EXPAND_MACRO(AGE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

  #define AGE_CORE_ASSERT_FORMATTED(...) AGE_EXPAND_MACRO(AGE_INTERNAL_ASSERT_IMPL(_CORE_, __VA_ARGS__))
  #define AGE_ASSERT_FORMATTED(...) AGE_EXPAND_MACRO(AGE_INTERNAL_ASSERT_IMPL(_, __VA_ARGS__))

#else
  #define AGE_ASSERT(...)
  #define AGE_CORE_ASSERT(...)

  #define AGE_CORE_ASSERT_FORMATTED(...)
  #define AGE_ASSERT_FORMATTED(...)
#endif
