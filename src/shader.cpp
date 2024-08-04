#include "shader.hpp"

#include <utility>

namespace wvxy {

Shader::Shader(std::string vertexPath, std::string fragmentPath)
    : vertexCode{ReadShaderSource(std::move(vertexPath))},
      fragmentCode{ReadShaderSource(std::move(fragmentPath))} {
  // it seems it cannot be created before glut init.
  // CreateProgram(vertexCode, fragmentCode);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() {
  if (!isCompiled) {
    CreateProgram(vertexCode, fragmentCode);
    isCompiled = true;
  }
  glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
std::string Shader::ReadShaderSource(const std::string path) {
  std::ifstream file{path};

  if (!file.is_open()) {
    std::cerr << "Error: File do not exist or could not open" << std::endl;
    throw std::runtime_error("Could not open file: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  return buffer.str();
}

void Shader::CompileShader(std::string& source, GLuint& target, GLenum type) {
  target = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(target, 1, &src, nullptr);
  glCompileShader(target);

  int success;
  char infoLog[512];
  glGetShaderiv(target, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(target, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    glDeleteShader(target);
    exit(EXIT_FAILURE);
  }
}

void Shader::CreateProgram(std::string& vertexCode, std::string& fragmentCode) {
  GLuint vertexShader, fragmentShader;

  CompileShader(vertexCode, vertexShader, GL_VERTEX_SHADER);
  CompileShader(fragmentCode, fragmentShader, GL_FRAGMENT_SHADER);

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  int success;
  char infoLog[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, nullptr, infoLog);
    std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

}  // namespace wvxy