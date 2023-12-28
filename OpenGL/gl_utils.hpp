#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <gl/GL.h>

class GlUtils {
  private :
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

    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    unsigned int VBO, VAO, EBO;

    GLFWwindow *window;
    GLFWwindow *InitGLFW();


    unsigned int CompileLinkShader(
            const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
      glViewport(0, 0, width, height);
    }

public:
    float vertices[9] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };
    void InitGLAD();
    void InitViewport();

    void InitVAO();
    void InitVBO();
    void InitEBO();
    void Draw();
};
