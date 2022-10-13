CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)

add_executable(Sandbox src/Sandbox.cpp src/Sandbox.h)
target_link_libraries(Sandbox PUBLIC age_core)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/assets)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

