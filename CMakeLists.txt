cmake_minimum_required(VERSION 3.20)
project(Utils4C++)

set(CMAKE_CXX_STANDARD 23)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# https://github.com/tinyobjloader/tinyobjloader/blob/release/CMakeLists.txt
set(tinyobjloader-Source ${EXT_DIR}/tiny_obj_loader.h)

file(GLOB SRC_FILES
        ${SRC_DIR}/*.cpp
        ${SRC_DIR}/*.hpp
        ${SRC_DIR}/*.h
)

file(GLOB EXTERNAL_LIB
        ${EXT_DIR}/*.h
)

include(cmake/copy_shaders.cmake)
set(CMAKE_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(${EXT_DIR}/glad)
add_subdirectory(src)
add_subdirectory(archive)
