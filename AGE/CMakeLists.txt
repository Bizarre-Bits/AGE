CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(age_core STATIC)

target_include_directories(age_core INTERFACE include PUBLIC src vendor/imgui vendor/glm/)

file(GLOB_RECURSE _AGE_CORE_SRCS src/agepch.h src/agepch.cpp src/Age/*.cpp src/Age/*.h)
list(REMOVE_ITEM _AGE_CORE_SRCS ${REMOVE_CMAKE})

file(GLOB _IMGUI_SRCS vendor/imgui/*.cpp vendor/imgui/*.h)

target_sources(age_core PRIVATE ${_AGE_CORE_SRCS} PUBLIC ${_IMGUI_SRCS})

option(AGE_INCLUDE_OPENGL "If ON, OpenGL-specific implementations will be added to the project" ON)
option(AGE_INCLUDE_VULCAN "If ON, Vulcan-specific implementations will be added to the project" OFF)
option(AGE_INCLUDE_DIRECTX "If ON, DirectX-specific implementations will be added to the project" OFF)

set(AGE_RENDER_PLATFORM OPENGL CACHE STRING "Render platform to be used. (OPENGL)")

add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)

target_link_libraries(age_core PUBLIC spdlog glm)

##################################################
#                Figure out OS                   #
##################################################
if (WIN32)
    target_compile_definitions(age_core PUBLIC AGE_OS_WINDOWS)
    message(STATUS "Windows platform")
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(age_core PUBLIC AGE_OS_LINUX)
    message(STATUS "Linux platform")
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(age_core PUBLIC DEBUG)
    message(STATUS "Debug configuration")
else ()
    target_compile_definitions(age_core PUBLIC NDEBUG)
    message(STATUS "Non-debug configuration")
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "Release configuration")
endif ()

##################################################
#           Configure Render Platforms           #
##################################################

# find vulcan
# find directX

if (AGE_INCLUDE_OPENGL)
    message(STATUS "Including OpenGl")
    add_subdirectory(vendor/GLFW)
    add_subdirectory(vendor/GLAD)

    target_link_libraries(age_core PUBLIC glad glfw ${GLFW_LIBRARIES})

    file(GLOB _IMGUI_BE vendor/imgui/backends/imgui_impl_opengl3* vendor/imgui/backends/imgui_impl_glfw.*)
    file(GLOB _AGE_OPENGL_PLATFORM_SRCS src/Platform/OpenGL/*)

    target_sources(age_core PUBLIC ${_IMGUI_BE} ${_AGE_OPENGL_PLATFORM_SRCS})

    target_include_directories(age_core PUBLIC vendor/GLFW/include vendor/GLAD/include vendor/imgui)

    target_compile_definitions(age_core PUBLIC AGE_INCLUDE_OPENGL)
endif ()

target_compile_definitions(age_core PUBLIC AGE_RENDER_PLATFORM_${AGE_RENDER_PLATFORM})

target_precompile_headers(age_core PRIVATE src/agepch.h)


