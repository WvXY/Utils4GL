cmake_minimum_required(VERSION 3.20)
project(WvXYEngineGL)

set(CMAKE_CXX_STANDARD 23)

find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
        ${SRC_FILES}
        ${EXTERNAL_LIB}
        main.cpp
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

