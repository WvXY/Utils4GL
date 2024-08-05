#include "gl_utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace wvxy {

GlUtils::GlUtils(int screen_width, int screen_height, std::string window_name)
    : SCR_WIDTH(screen_width),
      SCR_HEIGHT(screen_height),
      windowName(window_name) {
  init();
}

GlUtils::~GlUtils() {
  glfwDestroyWindow(window);
  // glDeleteVertexArrays(1, &VAO);
  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);
  // glDeleteBuffers(1, &CBO);
  glfwTerminate();
}

void GlUtils::init() {
  initGLFW();
  initGLAD();
  initViewport();
  glfwSwapInterval(0);  // vsync : 0 off, 1 on
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

void GlUtils::framebufferSizeCallback(GLFWwindow* window, int width,
                                      int height) {
  glViewport(0, 0, width, height);
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
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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

unsigned int GlUtils::loadTexture(const std::string& path) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int width, height, nrChannels;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  return texture;
}

// TODO separate into basicShader and textureShader
GLuint GlUtils::createBuffer(std::vector<vec3>& vertices,
                             std::vector<vec3>& colors,
                             std::vector<vec3i>& indices,
                             std::vector<vec2>& texCoords) {
  GLuint VBO, VAO, CBO;
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
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec3i),
                 indices.data(), GL_STATIC_DRAW);
  }

  if (texCoords.size() != 0) {
    GLuint TBO;
    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vec2),
                 texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(2);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return VAO;
}

void GlUtils::draw(std::vector<vec3> vertices, std::vector<vec3> colors,
                   std::vector<vec3i> indices, std::vector<vec2> texCoords) {
  basicShader.use();

  auto VAO = createBuffer(vertices, colors, indices, texCoords);
  glBindVertexArray(VAO);

  if (indices.size() == 0)
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
  else
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
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
