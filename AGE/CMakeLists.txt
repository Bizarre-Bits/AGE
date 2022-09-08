CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)


# Core lib files
file(GLOB_RECURSE _SRCS src/Age/*.cpp)
file(GLOB_RECURSE _HDRS src/Age/*.h)
file(GLOB_RECURSE _INCLDS include/*.h)


# TODO: define it outside of cmake lists
set(AGE_RENDER_PLATFORM OPENGL)

set(AGE_NOT_FOUND_RENDER_PLATFORMS "")

# TODO Some render platform-specific code. Should be added package location and fallback to OpenGL.
set(AGE_VULCAN_AVAILABLE OFF)
set(AGE_DIRECTX_AVAILABLE OFF)

# OpenGL is always available as we linking it from the repo
set(AGE_OPENGL_AVAILABLE ON)


# Fatal error if there is no available render platform
set(AGE_RENDER_PLATFORM_CHECK "AGE_${AGE_RENDER_PLATFORM}_AVAILABLE")

if (NOT ${${AGE_RENDER_PLATFORM_CHECK}})
    message(SEND_ERROR
            "No render platform is available! You tried to compile AGE with ${AGE_RENDER_PLATFORM}, but it seems it cannot be resolved\n"
            "Packages we could not find (some of them might be essential for the platform you've chosen): ${AGE_NOT_FOUND_RENDER_PLATFORMS}"
            )
    message(STATUS "${AGE_RENDER_PLATFORM} could not be resolved, falling back to the OPENGL")
    set(AGE_RENDER_PLATFORM OPENGL)
endif ()

if (AGE_RENDER_PLATFORM STREQUAL OPENGL)
    add_subdirectory(vendor/GLAD)
    add_subdirectory(vendor/GLFW)

    set(AGE_RENDER_PLATFORM_LIBS glfw glad ${GLFW_LIBRARIES})
    set(AGE_RENDER_PLATFORM_INCLUDES vendor/GLFW/include)

    file(GLOB _IMGUI_BE vendor/imgui/backends/imgui_impl_opengl3* vendor/imgui/backends/imgui_impl_glfw.*)
    file(GLOB _RENDER_PLATFORM_SRCS src/Platform/OpenGL/*.cpp)
    file(GLOB _RENDER_PLATFORM_HEADERS src/Platform/OpenGL/*.h)
endif ()


# ImGui sources
file(GLOB _IMGUI_SRCS vendor/imgui/*.cpp vendor/imgui/*.h)

add_library(Core STATIC ${_SRCS} ${_RENDER_PLATFORM_SRCS} ${_RENDER_PLATFORM_HEADERS} src/agepch.cpp src/agepch.h ${_HDRS} ${_INCLDS} ${_IMGUI_SRCS} ${_IMGUI_BE})
target_include_directories(Core
        PUBLIC
        include
        src
        vendor/spdlog/include
        vendor/imgui/
        ${AGE_RENDER_PLATFORM_INCLUDES}
        )

set(CMAKE_POSITION_INDEPENDENT_CODE 1)
set(SPDLOG_ENABLE_PCH ON)
add_subdirectory(vendor/spdlog)

add_subdirectory(vendor/glm)

target_link_libraries(Core PUBLIC spdlog ${AGE_RENDER_PLATFORM_LIBS} glm)

target_precompile_headers(Core PUBLIC src/agepch.h)

if (AGE_RENDER_PLATFORM STREQUAL OPENGL)
    target_compile_definitions(Core PUBLIC AGE_RENDER_PLATFORM_OPENGL)
endif ()

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