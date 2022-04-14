cmake_minimum_required(VERSION 3.0)
project(SERVER)

set(CXX_FLAGS
 -g
 -Werror
 -std=c++17
)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src/base)