CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

file(GLOB_RECURSE _SRCS src/*.cpp)
file(GLOB_RECURSE _HDRS src/*.h)
file(GLOB_RECURSE _INCLDS include/*.h)

add_library(Core SHARED ${_SRCS} ${_HDRS} ${_INCLDS})
target_include_directories(Core
        PUBLIC
        src
        include
        vendor/GLFW/include
        vendor/spdlog/include
        )

target_precompile_headers(Core PUBLIC include/agepch.h)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    message("debug build mode")
    target_compile_definitions(Core PUBLIC DEBUG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL Release)
    message("release build mode")
    target_compile_definitions(Core PUBLIC NDEBUG RELEASE)
else ()
    message("undefined build mode")
    target_compile_definitions(Core PUBLIC NDEBUG)
endif ()