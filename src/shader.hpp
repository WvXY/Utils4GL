#pragma once

#include <glad/glad.h>  // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "global_alias.h"

namespace wvxy {
class Shader {
 public:
  Shader(std::string vertexPath, std::string fragmentPath);
  ~Shader();

  void use();
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat4(const std::string& name, mat4 value) const;

  unsigned int getID() { return ID; }

  // void createProgram() {
  //   CreateProgram(vertexCode, fragmentCode);
  // }

 private:
  unsigned int ID;
  bool isCompiled = false;  // Temp solution
  std::string vertexCode;
  std::string fragmentCode;

  std::string loadShaderSource(std::string path);
  void compileShader(std::string& source, GLuint& target, GLenum type);
  void createProgram(std::string& vertexCode, std::string& fragmentCode);
};

}  // namespace wvxy