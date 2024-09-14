find_package(Eigen3 CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

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
