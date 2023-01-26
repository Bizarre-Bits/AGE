CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 23)

file(GLOB _SANDBOX_SRCS src/*.cpp src/*.h)

add_executable(Sandbox ${_SANDBOX_SRCS})
target_link_libraries(Sandbox PUBLIC age_core)

file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/assets)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

