CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
cmake_policy(SET CMP0076 NEW)

project(AGE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 23)

add_library(age_core STATIC)

file(GLOB_RECURSE _AGE_CORE_SRCS src/agepch.cpp src/agepch.h src/Age/*.cpp src/Age/*.h)

target_sources(age_core PRIVATE ${_AGE_CORE_SRCS})
target_include_directories(age_core PUBLIC src include)

target_precompile_headers(age_core PUBLIC src/agepch.h)

option(AGE_INCLUDE_OPENGL "If ON, OpenGL-specific implementations will be added to the project" ON)
option(AGE_INCLUDE_VULKAN "If ON, Vulkan-specific implementations will be added to the project" ON)
option(AGE_INCLUDE_DIRECTX "If ON, DirectX-specific implementations will be added to the project" OFF)

set(AGE_RENDER_API OPENGL CACHE STRING "Render platform to be used. (OPENGL)")

add_subdirectory(vendor/glm)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/entt)
add_subdirectory(vendor/yaml-cpp)
target_link_libraries(age_core PUBLIC spdlog glm EnTT::EnTT yaml-cpp)

message(STATUS "Vulkan SDK: $ENV{VULKAN_SDK}")

find_package(Vulkan REQUIRED)
target_link_directories(age_core PUBLIC "$ENV{VULKAN_SDK}/lib")
target_link_libraries(age_core PUBLIC shaderc_combined spirv-cross-core spirv-cross-glsl)
target_include_directories(age_core PUBLIC ${Vulkan_INCLUDE_DIRS})

##################################################
#                Figure out OS                   #
##################################################

if (WIN32)
    target_compile_definitions(age_core PUBLIC AGE_OS_WINDOWS)
    message(STATUS "Windows platform")
    set(AGE_OS "Windows")
    set(AGE_PLATFORM "Desktop")
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(age_core PUBLIC AGE_OS_LINUX)
    message(STATUS "Linux platform")
    set(AGE_OS "Linux")
    set(AGE_PLATFORM "Desktop")
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
#           Add platform specific files          #
##################################################

if (AGE_PLATFORM STREQUAL "Desktop")
    message(STATUS "Adding desktop specific code")
    file(GLOB _DESKTOP_CODE_SRCS src/Platform/Desktop/*.cpp)
    list(APPEND _PLATFORM_CODE_SRCS ${_DESKTOP_CODE_SRCS})
endif ()

if (AGE_OS STREQUAL "Linux")
    message(STATUS "Adding Linux specific code")
    file(GLOB _LINUX_CODE_SRCS src/Platform/Linux/*.cpp)
    list(APPEND _PLATFORM_CODE_SRCS ${_LINUX_CODE_SRCS})
elseif (AGE_OS STREQUAL "Windows")
    message(STATUS "Adding Windows specific code")
    file(GLOB _WINDOWS_CODE_SRCS src/Platform/Windows/*.cpp)
    list(APPEND _PLATFORM_CODE_SRCS ${_WINDOWS_CODE_SRCS})
endif ()

target_sources(age_core PUBLIC ${_PLATFORM_CODE_SRCS})

# Adding third party sources into the separate libraries for precompiled header to work
add_library(age_imgui STATIC)
file(GLOB _IMGUI_SRCS vendor/imgui/*.cpp vendor/imgui/*.h)
target_sources(age_imgui PUBLIC ${_IMGUI_SRCS})
target_include_directories(age_imgui PUBLIC vendor/imgui)

add_library(age_stbimage STATIC vendor/stb_image/stb_image.cpp vendor/stb_image/stb_image.h)
target_include_directories(age_stbimage PUBLIC vendor/stb_image)

add_library(age_imguizmo STATIC vendor/ImGuizmo/ImGuizmo.h vendor/ImGuizmo/ImGuizmo.cpp)
target_link_libraries(age_imguizmo PUBLIC age_imgui)
target_include_directories(age_imguizmo PUBLIC vendor/ImGuizmo)

target_link_libraries(age_core PUBLIC age_imgui age_stbimage age_imguizmo)

##################################################
#           Configure Render Platforms           #
##################################################

target_include_directories(age_imgui PUBLIC vendor/imgui)

if (AGE_INCLUDE_OPENGL OR AGE_INCLUDE_VULKAN)
    add_subdirectory(vendor/GLFW)
    target_link_libraries(age_core PUBLIC glfw ${GLFW_LIBRARIES})
    target_link_libraries(age_imgui PUBLIC glfw ${GLFW_LIBRARIES})

    set(_IMGUI_BE_SRCS vendor/imgui/backends/imgui_impl_glfw.cpp vendor/imgui/backends/imgui_impl_glfw.h)
endif ()

if (AGE_INCLUDE_OPENGL)
    message(STATUS "Including OpenGl")

    add_library(age_core_glad STATIC)

    list(APPEND _IMGUI_BE_SRCS
            vendor/imgui/backends/imgui_impl_opengl3.cpp
            vendor/imgui/backends/imgui_impl_opengl3.h
            vendor/imgui/backends/imgui_impl_opengl3_loader.h
            )

    file(GLOB _AGE_OPENGL_PLATFORM_SRCS src/RenderAPI/OpenGL/*)
    file(GLOB _AGE_OPENGL_PLATFORM_C_SRCS vendor/GLAD/src/glad.c)

    target_sources(age_core PRIVATE ${_AGE_OPENGL_PLATFORM_SRCS})
    target_sources(age_core_glad PRIVATE ${_AGE_OPENGL_PLATFORM_C_SRCS})

    target_include_directories(age_core PUBLIC vendor/GLFW/include vendor/imgui)
    target_include_directories(age_core_glad PUBLIC vendor/GLAD/include)

    target_compile_definitions(age_core PUBLIC AGE_INCLUDE_OPENGL)

    target_link_libraries(age_core PUBLIC age_core_glad)
endif ()

if (AGE_INCLUDE_VULKAN)
    message(STATUS "Including Vulkan")

    target_compile_definitions(age_core PUBLIC AGE_INCLUDE_VULKAN)
    list(APPEND _IMGUI_BE_SRCS vendor/imgui/backends/imgui_impl_vulkan.cpp vendor/imgui/backends/imgui_impl_vulkan.h)

    file(GLOB _AGE_VULKAN_PLATFORM_SRCS src/RenderAPI/Vulkan/*.cpp src/RenderAPI/Vulkan/*.h)

    target_sources(age_core PUBLIC ${_AGE_VULKAN_PLATFORM_SRCS})

    target_link_libraries(age_imgui PUBLIC vulkan)
    target_link_libraries(age_core PUBLIC vulkan)
endif ()

target_compile_definitions(age_core PUBLIC AGE_RENDER_API_${AGE_RENDER_API})
message(STATUS ${_IMGUI_BE_SRCS})
target_sources(age_imgui PUBLIC ${_IMGUI_BE_SRCS})


##################################################
#               Import Assets                    #
##################################################

if (NOT FINAL_EXE_BINARY_DIR OR FINAL_EXE_BIN_DIR STREQUAL "")
    message(FATAL_ERROR "There is no target \"${FINAL_EXE_BINARY_DIR}\" (FINAL_EXE_BINARY_DIR)")
else ()
    message(STATUS "Final exe binary dir is \"${FINAL_EXE_BINARY_DIR}\"")
endif ()

file(GLOB_RECURSE _AGECORE_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/core_assets/*)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/core_assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/core_assets
        DEPENDS ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/client_assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/client_assets
        DEPENDS ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets)

foreach (assetSrc ${_AGECORE_ASSETS})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/assets/core_assets/ "" assetPath ${assetSrc})
    message(STATUS "AGE_CORE: Add custom command for ${assetPath}")
    list(APPEND assetsOutputList ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/core_assets/${assetPath})
    target_sources(age_core PUBLIC ${assetSrc})
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/core_assets/${assetPath}
            COMMAND ${CMAKE_COMMAND} -E copy ${assetSrc} ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/core_assets/${assetPath}
            DEPENDS ${assetSrc} ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/core_assets
            COMMENT "Copy ${assetPath}")
endforeach ()

add_custom_target(core_assets ALL DEPENDS ${assetsOutputList})
add_custom_target(client_assets_dir ALL DEPENDS ${CMAKE_BINARY_DIR}/${FINAL_EXE_BINARY_DIR}/assets/client_assets)

add_dependencies(age_core core_assets client_assets_dir)
