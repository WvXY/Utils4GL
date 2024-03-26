cmake_minimum_required(VERSION 3.20)
project(OpenGLDemo)

set(CMAKE_CXX_STANDARD 23)

#find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
        ${SRC}
        gl_triangle.cpp
)

target_link_libraries(${PROJECT_NAME}
#        Eigen3::Eigen
        OpenGL::GL
        glfw
        glad
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# Move shader folder
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/shaders
#        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders"
#        COMMAND ${CMAKE_COMMAND} -E echo "[PostBuild] Moved shaders to: ${CMAKE_BINARY_DIR}/shaders"
#)

include(${CMAKE_SCRIPTS}/copy_shaders.cmake)

copy_shaders()