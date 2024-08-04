cmake_minimum_required(VERSION 3.20)
project(WvXYEngineGL)

set(CMAKE_CXX_STANDARD 23)

find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${tinyobjloader-Source}
        gl_triangle.cpp
        ../src/shader.cpp
        ../src/shader.hpp
)

target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        Eigen3::Eigen
        glfw
        glad
)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${SRC_DIR}
        PRIVATE ${EXT_DIR}
)

include(${CMAKE_SCRIPTS}/copy_shaders.cmake)

copy_shaders()

####################################
add_executable(OpenGL-Legacy gl_legacy_demo.cpp)
target_include_directories(OpenGL-Legacy
        PRIVATE ${SRC_DIR}
        PRIVATE ${EXT_DIR}
)
target_link_libraries(OpenGL-Legacy
        OpenGL::GL
        glfw
        glad
)

####################################
add_executable(OpenGL-Modern gl_modern_demo.cpp)
target_include_directories(OpenGL-Modern
        PRIVATE ${SRC_DIR}
        PRIVATE ${EXT_DIR}
)
target_link_libraries(OpenGL-Modern
        OpenGL::GL
        glfw
        glad
)
