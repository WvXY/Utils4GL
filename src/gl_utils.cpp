#include "gl_utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace wvxy {

GlUtils::GlUtils(int screen_width, int screen_height)
    : SCR_WIDTH(screen_width), SCR_HEIGHT(screen_height) {
  Init();
}

GlUtils::~GlUtils() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &CBO);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(program);
  glfwTerminate();
}

void GlUtils::Init() {
  InitGLFW();
  InitGLAD();
  InitViewport();
  glfwSwapInterval(0);  // vsync : 0 off, 1 on

  std::filesystem::path p = std::filesystem::current_path().parent_path();
  fragmentShaderSource =
      ReadShaderSource(p.string() + "/shaders/simple_shader.frag");
  vertexShaderSource =
      ReadShaderSource(p.string() + "/shaders/simple_shader.vert");
  CompileShader(vertexShaderSource, vertexShader, GL_VERTEX_SHADER);
  CompileShader(fragmentShaderSource, fragmentShader, GL_FRAGMENT_SHADER);
  CreateProgram(program, vertexShader, fragmentShader);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow* GlUtils::InitGLFW() {
  if (!glfwInit()) {
    std::cout << stderr << "Failed to initialize GLFW" << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // opengl 4.X
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // opengl x.6
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window", NULL, NULL);
  if (window == NULL) {
    std::cout << stderr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  return window;
}

void GlUtils::InitGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << stderr << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GlUtils::InitViewport() { glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); }

// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
std::string GlUtils::ReadShaderSource(const std::string path) {
  std::ifstream file{path};

  if (!file.is_open()) {
    std::cout << "Error: File do not exist or could not open" << std::endl;
    throw std::runtime_error("Could not open file: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return buffer.str();
}

void GlUtils::CompileShader(std::string& source, GLuint& target, GLenum type) {
  target = glCreateShader(type);
  auto* src = source.c_str();
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

void GlUtils::CreateProgram(GLuint& program, GLuint& vertexShader, GLuint& fragmentShader) {
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << stderr << "ERROR::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  //  glDeleteShader(vertexShader);
  //  glDeleteShader(fragmentShader);
}

void GlUtils::CreateBuffer(std::vector<vec2>& vertices,
                           std::vector<vec3>& colors,
                           std::vector<vec3i>& indices) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &CBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(vec2) + colors.size() * sizeof(vec3),
               NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vec2),
                  vertices.data());
  glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2),
                  colors.size() * sizeof(vec3), colors.data());

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3),
                        (void*)(vertices.size() * sizeof(vec2)));
  glEnableVertexAttribArray(1);
  //  glEnableVertexAttribArray(0);

  //  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  //  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), colors.data(),
  //               GL_STATIC_DRAW);
  //  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
  //  glEnableVertexAttribArray(1);

  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec3i),
  //           indices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GlUtils::Draw(std::vector<vec2>& vertices, std::vector<vec3>& colors,
                   std::vector<vec3i>& indices) {
  CreateBuffer(vertices, colors, indices);
  glUseProgram(program);   // Use the shader program
  glBindVertexArray(VAO);  // Bind the Vertex Array Object
  glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the triangle
  glBindVertexArray(0);  // Unbind the VAO (not strictly necessary)
}

void GlUtils::Run() {
  while (!glfwWindowShouldClose(window)) {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

}  // namespace wvxy
