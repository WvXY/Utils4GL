#include "gl_utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace wvxy {

GlUtils::GlUtils(int screen_width, int screen_height, std::string window_name)
    : SCR_WIDTH(screen_width),
      SCR_HEIGHT(screen_height),
      windowName(window_name)
{
  init();
}

GlUtils::~GlUtils() {
  glfwDestroyWindow(window);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &CBO);
  glfwTerminate();
}

void GlUtils::init() {
  initGLFW();
  initGLAD();
  initViewport();
  glfwSwapInterval(0);  // vsync : 0 off, 1 on
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void GlUtils::setTitle(std::string newTitle) {
  windowName = newTitle;
  glfwSetWindowTitle(window, windowName.c_str());
}

void GlUtils::addInfoToTitle(std::string extraInfo) {
  glfwSetWindowTitle(window, (windowName + " | " + extraInfo).c_str());
}

void GlUtils::setWireframeMode(bool wireframeMode) {
  if (!wireframeMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else  // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

GLFWwindow* GlUtils::initGLFW() {
  if (!glfwInit()) {
    std::cout << stderr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // opengl 4.X
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // opengl x.6
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName.c_str(), nullptr,
                            nullptr);
  if (window == nullptr) {
    std::cout << stderr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  return window;
}

void GlUtils::initGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << stderr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GlUtils::initViewport() { glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); }

// another approach to create buffer
// void GlUtils::CreateBuffer(std::vector<vec2>& vertices,
//                           std::vector<vec3>& colors) {
//  glGenVertexArrays(1, &VAO);
//  glGenBuffers(1, &VBO);
//  glGenBuffers(1, &CBO);
//  glBindVertexArray(VAO);
//
//  glBindBuffer(GL_ARRAY_BUFFER, VBO);
//  glBufferData(GL_ARRAY_BUFFER,
//               vertices.size() * sizeof(vec2) + colors.size() * sizeof(vec3),
//               nullptr, GL_STATIC_DRAW);
//  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec2),
//                  vertices.data());
//  glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2),
//                  colors.size() * sizeof(vec3), colors.data());
//
//  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
//                        (void*)(vertices.size() * sizeof(vec2)));
//  glEnableVertexAttribArray(1);
//
//  glBindBuffer(GL_ARRAY_BUFFER, 0);
//  glBindVertexArray(0);
//}

void GlUtils::createBuffer(std::vector<vec3>& vertices,
                           std::vector<vec3>& colors,
                           std::vector<vec3i>& indices) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &CBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), colors.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
  glEnableVertexAttribArray(1);

  if (indices.size() != 0) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec3i),
                 indices.data(), GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GlUtils::draw(std::vector<vec3> vertices, std::vector<vec3> colors,
                   std::vector<vec3i> indices) {
  basicShader.use();
  createBuffer(vertices, colors, indices);

  glBindVertexArray(VAO);  // Bind the Vertex Array Object

  if (indices.size() == 0)
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
  else
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);  // Unbind the VAO (not strictly necessary)
}

void GlUtils::run() {
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

}  // namespace wvxy
