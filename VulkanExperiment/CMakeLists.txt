CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(VulkanExperiment)
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE _VULKANEXPERIMENT_SRCS src/*.cpp src/*.h)

add_executable(VulkanExperiment ${_VULKANEXPERIMENT_SRCS})
include_directories(VulkanExperiment src)
target_link_libraries(VulkanExperiment PUBLIC age_core)

file(GLOB_RECURSE _VULKANEXPERIMENT_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/client_assets/VulkanExperiment/*)
message(STATUS ${_VULKANEXPERIMENT_ASSETS})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/VulkanExperiment
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/VulkanExperiment
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets)

foreach (assetSrc ${_VULKANEXPERIMENT_ASSETS})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR}/assets/client_assets/VulkanExperiment "" assetPath ${assetSrc})
    message(STATUS "AGE_CLIENT: Add custom command for ${assetPath}")
    list(APPEND assetsOutputList ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/VulkanExperiment/${assetPath})
    target_sources(VulkanExperiment PUBLIC ${assetSrc})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/VulkanExperiment/${assetPath}
            COMMAND ${CMAKE_COMMAND} -E copy ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/VulkanExperiment/${assetPath}
            DEPENDS ${assetSrc} ${CMAKE_CURRENT_BINARY_DIR}/assets/client_assets/VulkanExperiment
            COMMENT "Client Assets: Copy ${assetPath}")
endforeach ()

add_custom_target(VulkanExperiment_client_assets ALL DEPENDS ${assetsOutputList})

add_dependencies(VulkanExperiment VulkanExperiment_client_assets)
add_dependencies(VulkanExperiment core_assets)
