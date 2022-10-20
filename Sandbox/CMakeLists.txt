CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 23)

add_executable(Sandbox src/main.cpp src/Sandbox.cpp src/Sandbox.h src/Sandbox2DLayer.cpp src/Sandbox2DLayer.h src/SandboxLayer.cpp src/SandboxLayer.h)
target_link_libraries(Sandbox PUBLIC age_core)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/assets)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

