CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE _SRCS src/*.cpp)
file(GLOB_RECURSE _HDRS include/*.h)

add_library(Core SHARED ${_SRCS} ${_HDRS})
target_include_directories(Core PUBLIC include)
