CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(Sandbox)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE _SANDBOX_SRCS src/*.cpp src/*.h)

add_executable(Sandbox ${_SANDBOX_SRCS})
include_directories(Sandbox src)
target_link_libraries(Sandbox PUBLIC age_core)

file(GLOB_RECURSE _SANDBOX_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
message(STATUS ${_SANDBOX_ASSETS})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/assets)

foreach (assetSrc ${_SANDBOX_ASSETS})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/assets/client_assets/ "" assetPath ${assetSrc})
    message(STATUS "AGE_CLIENT: Add custom command for ${assetPath}")
    list(APPEND assetsOutputList ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/${assetPath})
    target_sources(Sandbox PUBLIC ${assetSrc})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/${assetPath}
            COMMAND ${CMAKE_COMMAND} -E copy ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/${assetPath}
            DEPENDS ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets
            COMMENT "Client Assets: Copy ${assetPath}")
endforeach ()

add_custom_target(Sandbox_client_assets ALL DEPENDS ${assetsOutputList})

add_dependencies(Sandbox Sandbox_client_assets)
add_dependencies(Sandbox core_assets)
