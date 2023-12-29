#pragma once

#include <GLFW/glfw3.h>
//#include <glad/glad.h>
#include <iostream>
#include <string>


class GlUtils {
public:
    GlUtils(float verts[], unsigned int indices[]);
    ~GlUtils() = default;

    GlUtils(const GlUtils &) = delete;
    GlUtils &operator=(const GlUtils &) = delete;

    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;

    void render_loop(GLFWwindow *window, unsigned int &shaderProgram,
                     float &verts, unsigned int &indices,
                     size_t size_verts, size_t size_indices );

private:
    size_t size_verts;
    size_t size_indices;
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram;

    GLFWwindow *window;
    GLFWwindow *InitGLFW();
    void InitGLAD();
    void InitViewport();
    unsigned int CompileShader(std::string &source, int type);
    unsigned int CreateProgram(unsigned int vertexShader, unsigned int fragmentShader);
    void CreateBuffer(
            float &verts, unsigned int &indices,
            size_t size_verts, size_t size_indices,
            unsigned int &VAO);


    std::string vertexShaderSource =
            "#version 460\n"
            "in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos, 1.0);\n"
            "}\n";

    std::string fragmentShaderSource =
            "#version 460\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "  FragColor = vec4(0.0f, 1.f, 0.f, 1.0f);\n"
            "}\n";
};
