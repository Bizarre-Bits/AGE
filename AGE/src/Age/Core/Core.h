//
// Created by a on 24.08.22.
//

#pragma once

#include <string>
#include <memory>

#ifdef DEBUG
  #if AGE_OS_WINDOWS
    #define AGE_DEBUG_BREAK() __debugbreak()
  #elif AGE_OS_LINUX
    #include <csignal>
    #define AGE_DEBUG_BREAK() raise(SIGTRAP)
  #else
    #error "Platform does not support debug breaks yet"
  #endif

  #define AGE_ENABLE_ASSERTS 1
  #define AGE_PROFILE 1
#else
  #define AGE_DEBUG_BREAK()
  #define AGE_ENABLE_ASSERTS 0
  #define AGE_PROFILE 0
#endif

#define AGE_EXPAND_MACRO(x) x
#define AGE_STRINGIFY(x) #x

#define AGE_BIND_EVENT_FN(x) [this](auto&& ...args) -> decltype(auto) {return this->x(std::forward<decltype(args)>(args)...); }

//A macro for bitfields
#define BIT(x) (1 << (x))

using age_string_t = std::string;

namespace AGE {
  template<typename T>
  using Ref = std::shared_ptr<T>;

  template<typename T, typename ... Args>
  constexpr Ref<T> MakeRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template<typename T>
  using Scope = std::unique_ptr<T>;

  template<typename T, typename ... Args>
  constexpr Scope<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
}
