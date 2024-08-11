#include "gl_utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace wvxy {

namespace GlUtils {

ExampleApp::ExampleApp(int screen_width, int screen_height, std::string window_name)
    : SCR_WIDTH(screen_width),
      SCR_HEIGHT(screen_height),
      windowName(window_name) {
  init();
}

ExampleApp::~ExampleApp() {
  glfwDestroyWindow(window);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &CBO);
  glDeleteBuffers(1, &TBO);
  glfwTerminate();
}

void ExampleApp::init() {
  initGLFW();
  initGLAD();
  initViewport();
  glfwSwapInterval(0);  // vsync : 0 off, 1 on
  glEnable(GL_DEPTH_TEST);
}

void ExampleApp::setTitle(std::string newTitle) {
  windowName = newTitle;
  glfwSetWindowTitle(window, windowName.c_str());
}

void ExampleApp::addInfoToTitle(std::string extraInfo) {
  glfwSetWindowTitle(window, (windowName + " | " + extraInfo).c_str());
}

void ExampleApp::setWireframeMode(bool wireframeMode) {
  if (!wireframeMode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else  // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void ExampleApp::framebufferSizeCallback(GLFWwindow* window,
                                      int width,
                                      int height) {
  glViewport(0, 0, width, height);
}

void ExampleApp::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  ExampleApp* instance = static_cast<ExampleApp*>(glfwGetWindowUserPointer(window));
  if (instance) {
    instance->handleMouseMovement(xpos, ypos);
  }
}

void ExampleApp::handleMouseMovement(double xpos, double ypos) {
  float xposf = static_cast<float>(xpos);
  float yposf = static_cast<float>(ypos);

  if (firstMouse) {
    lastX = xposf;
    lastY = yposf;
    firstMouse = false;
  }

  float xoffset = xposf - lastX;
  float yoffset = lastY - yposf;
  lastX = xposf;
  lastY = yposf;
  camera.processMouseMovement(xoffset, yoffset);
}

void ExampleApp::scrollCallback(GLFWwindow* window,
                             double xoffset,
                             double yoffset) {
  ExampleApp* instance = static_cast<ExampleApp*>(glfwGetWindowUserPointer(window));
  if (instance) {
    // instance->handleScroll(xoffset, yoffset);
    instance->camera.processMouseScroll(static_cast<float>(yoffset));
  }
}

GLFWwindow* ExampleApp::initGLFW() {
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
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  return window;
}

void ExampleApp::initGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << stderr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void ExampleApp::initViewport() {
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

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

unsigned int ExampleApp::loadTexture(const std::string& path) {
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
void ExampleApp::createBuffer(std::vector<vec3> vertices,
                             std::vector<vec3> colors,
                             std::vector<vec3i> indices,
                             std::vector<vec2> texCoords,
                             std::vector<vec3> normals
                             ) {
  verticesSize += vertices.size();
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &CBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
  glEnableVertexAttribArray(0);

  if (colors.size() == 0) {
    colors = std::vector<vec3>(vertices.size(), {1.0f, 1.0f, 1.0f});
  }
  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), colors.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
  glEnableVertexAttribArray(1);

  if (indices.size() != 0) {
    indicesSize = indices.size();
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec3i),
                 indices.data(), GL_STATIC_DRAW);
  }

  if (texCoords.size() != 0) {
    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(vec2),
                 texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(2);
  }

  if (normals.size() != 0) {
    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glEnableVertexAttribArray(3);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void ExampleApp::createBuffer(GameObject& go) {
  createBuffer(go.vertices, go.colors, go.indices, go.texCoords, go.normals);
}

void ExampleApp::draw() {
  basicShader.use();

  glBindVertexArray(VAO);

  if (indicesSize == 0)
    glDrawArrays(GL_TRIANGLES, 0, verticesSize);
  else
    glDrawElements(GL_TRIANGLES, indicesSize * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);
}

void ExampleApp::releaseBuffers() const {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &CBO);
  glDeleteBuffers(1, &TBO);
}

void ExampleApp::run() {
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void ExampleApp::processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboard(Camera::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(Camera::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(Camera::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(Camera::RIGHT, deltaTime);
}

}

}  // namespace wvxy
