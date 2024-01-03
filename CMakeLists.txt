# CMake setup
cmake_minimum_required(VERSION 3.27)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project details
project(engine)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Binaries
add_executable(engine src/main.cpp)

# Local libraries
add_subdirectory(lib/glad/)
target_link_libraries(engine PRIVATE glad)
