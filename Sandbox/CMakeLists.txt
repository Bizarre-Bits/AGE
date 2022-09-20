CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)

add_executable(Sandbox src/main.cpp)
target_link_libraries(Sandbox PRIVATE age_core)

