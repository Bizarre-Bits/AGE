CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
project(AGE)
set(CMAKE_CXX_STANDARD 23)

# Project Subdirectories
add_subdirectory(AGEditor)
add_subdirectory(Sandbox)

set(FINAL_EXE_BINARY_DIR Sandbox)
add_subdirectory(AGE)
