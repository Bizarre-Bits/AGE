//
// Created by a on 24.08.22.
//

#ifndef AGE_CORE_H
#define AGE_CORE_H

#include <string>

#ifdef DEBUG
  #if defined AGE_PLATFORM_WINDOWS
    #define AGE_DEBUG_BREAK() __debugbreak()
  #elif defined AGE_PLATFORM_LINUX
    #include <csignal>
    #define AGE_DEBUG_BREAK() raise(SIGTRAP)
  #else
    #error "Platform does not support debug breaks yet"
  #endif
  #define AGE_ENABLE_ASSERTS
#else
  #define AGE_DEBUG_BREAK()
#endif

#define AGE_EXPAND_MACRO(x) x
#define AGE_STRINGIFY(x) #x

#define AGE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

//A macro for bitfields
#define BIT(x) (1 << x)

using age_string_t = std::string;


// DLL export/import macros definitions
#if defined AGE_PLATFORM_WINDOWS
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport)// Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport)// Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__((visibility("default")))
    #define DLL_LOCAL __attribute__((visibility("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#endif//AGE_CORE_H
