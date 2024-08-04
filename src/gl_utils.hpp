#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "global_alias.h"
#include "shader.hpp"

namespace wvxy {

class GlUtils {
 public:
  GlUtils(int screen_width, int screen_height,
          std::string windowName = "OpenGL Window");
  ~GlUtils();
  GlUtils(const GlUtils&) = delete;
  GlUtils& operator=(const GlUtils&) = delete;

  int SCR_WIDTH = 800;
  int SCR_HEIGHT = 600;
  GLFWwindow* window;

  Shader basicShader{"../shaders/basic_shader.vert", "../shaders/basic_shader.frag"};


  void draw(std::vector<vec3> vertices, std::vector<vec3> colors,
            std::vector<vec3i> indices = {});

  void setTitle(std::string newTitle);
  void addInfoToTitle(std::string extraInfo);

  void setWireframeMode(bool wireframeMode);

  void virtual run();

 private:
  void init();
  GLFWwindow* initGLFW();
  void initGLAD();
  void initViewport();

  std::string windowName;

  // TODO: refactor to a class for shaders
  GLuint VBO, VAO, EBO, CBO;

  void createBuffer(std::vector<vec3>& vertices, std::vector<vec3>& colors,
                    std::vector<vec3i>& indices);
  void createBuffer(std::vector<vec3>& vertices, std::vector<vec3>& colors);
};

// class Camera {
//  public:
//   Camera();
//   Camera(vec3 position, vec3 target, vec3 up);
//   Camera(const Camera&) = delete;
//   Camera& operator=(const Camera&) = delete;
//
//   vec3 position;
//   vec3 target;
//   vec3 up;
//
//   void LookAt();
// };

}  // namespace wvxy
