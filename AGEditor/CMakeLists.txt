CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE _EDITOR_SRCS src/*.cpp src/*.h)

add_executable(AGEditor ${_EDITOR_SRCS})
include_directories(AGEditor src)
target_link_libraries(AGEditor PUBLIC age_core)

file(GLOB_RECURSE _AGEDITOR_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
message(STATUS ${_AGEDITOR_ASSETS})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/assets)

foreach (assetSrc ${_AGEDITOR_ASSETS})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/assets/client_assets/ "" assetPath ${assetSrc})
    message(STATUS "Client Assets: Add custom command for ${assetPath}")
    list(APPEND assetsOutputList ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/${assetPath})
    target_sources(AGEditor PUBLIC ${assetSrc})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/${assetPath}
            COMMAND ${CMAKE_COMMAND} -E copy ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/${assetPath}
            DEPENDS ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets
            COMMENT "Client Assets: Copy ${assetPath}")
endforeach ()

add_custom_target(client_assets ALL DEPENDS ${assetsOutputList})

add_dependencies(AGEditor client_assets)
