#include "gl_utils.hpp"

int main() {
  GlUtils glUtils;
  GLFWwindow *window = glUtils.InitGLFW();
  glUtils.InitGLAD();
  glUtils.InitViewport();
  unsigned int shaderProgram = glUtils.CompileLinkShader(glUtils.vertexShaderSource, glUtils.fragmentShaderSource);
  glUtils.InitVAO();
  glUtils.InitVBO();
  glUtils.InitEBO();
  glUtils.Draw();
  return 0;
}