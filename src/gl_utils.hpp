#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "global_alias.h"
#include "shader.hpp"
#include "camera.hpp"

#include <glm/ext/matrix_transform.hpp>

namespace wvxy {

class GlUtils {
 public:
  GlUtils(int screen_width,
          int screen_height,
          std::string windowName = "OpenGL Window");
  ~GlUtils();
  GlUtils(const GlUtils&) = delete;
  GlUtils& operator=(const GlUtils&) = delete;

  int SCR_WIDTH = 800;
  int SCR_HEIGHT = 600;
  GLFWwindow* window;

  Shader basicShader{"../../shaders/basic_shader.vert",
                     "../../shaders/basic_shader.frag"};

  Camera camera{};

  unsigned int loadTexture(const std::string& path);

  void draw();
  void createBuffer(std::vector<vec3> vertices,
                    std::vector<vec3> colors,
                    std::vector<vec3i> indices = {},
                    std::vector<vec2> texCoords = {});

  void releaseBuffers() const;  // need to release buffers before new creation,
                                // otherwise it will cause memory leak

  void setTitle(std::string newTitle);
  void addInfoToTitle(std::string extraInfo);

  void setWireframeMode(bool wireframeMode);

  void processInput(GLFWwindow* window);

  void virtual run();

  float deltaTime;

 private:
  void init();
  GLFWwindow* initGLFW();
  void initGLAD();
  void initViewport();

  std::string windowName;
  GLuint VAO, VBO, EBO, CBO, TBO;

  // camera
  bool firstMouse{true};
  float lastX, lastY;

  static void framebufferSizeCallback(GLFWwindow* window,
                                      int width,
                                      int height);
  static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow* window,
                             double xoffset,
                             double yoffset);

  void handleMouseMovement(double xpos, double ypos);
  void handleScroll(double xoffset, double yoffset);

  size_t indicesSize = 0;
  size_t verticesSize = 0;
};

}  // namespace wvxy
