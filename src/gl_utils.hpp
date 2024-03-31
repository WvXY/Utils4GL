#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include "global_alias.h"

namespace wvxy {

class GlUtils {
 public:
  GlUtils(int screen_width, int screen_height);
  ~GlUtils();
  GlUtils(const GlUtils&) = delete;
  GlUtils& operator=(const GlUtils&) = delete;

  int SCR_WIDTH = 800;
  int SCR_HEIGHT = 600;
  GLFWwindow* window;

  void Draw(std::vector<vec2>& vertices, std::vector<vec3>& colors,
            std::vector<vec3i>& indices);

  void virtual Run();

 private:
  GLuint VBO;
  GLuint VAO;
  GLuint EBO;
  GLuint CBO;

  std::string vertexShaderSource;
  std::string fragmentShaderSource;
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint program;

  void Init();
  GLFWwindow* InitGLFW();
  void InitGLAD();
  void InitViewport();

  std::string ReadShaderSource(const std::string path);
  void CompileShader(std::string& source, GLuint& target, GLenum type);
  void CreateProgram(GLuint& program, GLuint& vertexShader,
                     GLuint& fragmentShader);

  void CreateBuffer(std::vector<vec2>& vertices, std::vector<vec3>& colors,
                    std::vector<vec3i>& indices);
};
}  // namespace wvxy
