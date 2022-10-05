CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)

add_executable(Sandbox src/Sandbox.cpp src/Sandbox.h)
target_link_libraries(Sandbox PUBLIC age_core)

