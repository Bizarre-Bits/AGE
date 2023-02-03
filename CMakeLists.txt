CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(Sandbox)
add_subdirectory(AGEditor)

set(FINAL_EXE_BINARY_DIR AGEditor)

add_subdirectory(AGE)
