CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGEditor)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE _AGEDITOR_SRCS src/*.cpp src/*.h)

add_executable(AGEditor ${_AGEDITOR_SRCS})
include_directories(AGEditor src)
target_link_libraries(AGEditor PUBLIC age_core)

file(GLOB_RECURSE _AGEDITOR_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/client_assets/AGEditor/*)
message(STATUS ${_AGEDITOR_ASSETS})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/AGEditor
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/AGEditor
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets)

foreach (assetSrc ${_AGEDITOR_ASSETS})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/assets/client_assets/AGEditor "" assetPath ${assetSrc})
    message(STATUS "AGE_CLIENT: Add custom command for ${assetPath}")
    list(APPEND assetsOutputList ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/AGEditor/${assetPath})
    target_sources(AGEditor PUBLIC ${assetSrc})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/AGEditor/${assetPath}
            COMMAND ${CMAKE_COMMAND} -E copy ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/AGEditor/${assetPath}
            DEPENDS ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/AGEditor
            COMMENT "Client Assets: Copy ${assetPath}")
endforeach ()

add_custom_target(AGEditor_client_assets ALL DEPENDS ${assetsOutputList})

add_dependencies(AGEditor AGEditor_client_assets)
add_dependencies(AGEditor core_assets)
