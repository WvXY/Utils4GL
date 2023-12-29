#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gl_utils.hpp"


void framebuffer_size_callback(
        GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

GlUtils::GlUtils(float verts[], unsigned int indices[]) :
        size_verts(sizeof(verts)), size_indices(sizeof(indices))
{
  InitGLFW();
  InitGLAD();
  InitViewport();
  auto vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
  auto fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  auto shaderProgram = CreateProgram(vertexShader, fragmentShader);
  CreateBuffer(*verts, *indices, sizeof(verts), sizeof(indices), VAO);
  render_loop(window, shaderProgram, *verts, *indices, sizeof(verts), sizeof(indices));
}


GLFWwindow *GlUtils::InitGLFW()
{
  if (!glfwInit()) {
    std::cout << stderr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // opengl 3.X
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // opengl x.3
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << stderr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

void GlUtils::InitGLAD()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << stderr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GlUtils::InitViewport()
{
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

unsigned int GlUtils::CompileShader(std::string &source, int type)
{
  unsigned int shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cout << stderr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return shader;
}

unsigned int GlUtils::CreateProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
  unsigned int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << stderr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

void GlUtils::CreateBuffer(
        float &verts,
        unsigned int &indices,
        size_t size_verts,
        size_t size_indices,
        unsigned int &VAO)
{
  unsigned int VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size_verts, &verts, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_indices, &indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void GlUtils::render_loop(GLFWwindow *window, unsigned int &shaderProgram,
                          float &verts, unsigned int &indices,
                          size_t size_verts, size_t size_indices )
{
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    unsigned int VAO;
    CreateBuffer(verts, indices, sizeof(verts), sizeof(indices), VAO);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
