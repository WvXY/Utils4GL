cmake_minimum_required(VERSION 3.6)
project(Utils)

set(CMAKE_CXX_STANDARD 23)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC
        ${SRC_DIR}/*.cpp
        ${SRC_DIR}/*.hpp
        ${SRC_DIR}/*.h
)

#include(cmake_src/copy_shaders.cmake)
set(CMAKE_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/cmake_src)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/glad)
add_subdirectory(example)
