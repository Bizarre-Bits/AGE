CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

file(GLOB_RECURSE _SRCS src/*.cpp)
file(GLOB_RECURSE _HDRS src/*.h)
file(GLOB_RECURSE _INCLDS include/*.h)

file(GLOB _IMGUI_SRCS vendor/imgui/*.cpp)
file(GLOB _IMGUI_BE vendor/imgui/backends/imgui_impl_opengl3*)

add_library(Core SHARED ${_SRCS} ${_HDRS} ${_INCLDS} ${_IMGUI_SRCS} ${_IMGUI_BE})
target_include_directories(Core
        PUBLIC
        src
        include
        vendor/GLFW/include
        vendor/spdlog/include
        vendor/imgui/
        )

set(BUILD_SHARED_LIBS ON)

add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/GLFW)

set(CMAKE_POSITION_INDEPENDENT_CODE 1)
set(SPDLOG_ENABLE_PCH ON)

add_subdirectory(vendor/spdlog)


target_link_libraries(Core PUBLIC spdlog glad glfw ${GLFW_LIBRARIES})

target_precompile_headers(Core PUBLIC include/agepch.h)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    message(STATUS "debug build mode")
    target_compile_definitions(Core PUBLIC DEBUG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL Release)
    message(STATUS "release build mode")
    target_compile_definitions(Core PUBLIC NDEBUG RELEASE)
else ()
    message(STATUS "undefined build mode")
    target_compile_definitions(Core PUBLIC NDEBUG)
endif ()

if (WIN32)
    message(STATUS ">>>> Windows")
    target_compile_definitions(Core PUBLIC AGE_PLATFORM_WINDOWS)
endif ()

if (UNIX AND NOT APPLE)
    message(STATUS ">>>> LINUX")
    target_compile_definitions(Core PUBLIC AGE_PLATFORM_LINUX)
endif ()