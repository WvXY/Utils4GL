#include "gl_utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace wvxy {

GlUtils::GlUtils(int screen_width, int screen_height)
    : SCR_WIDTH(screen_width), SCR_HEIGHT(screen_width) {
  Init();
}

GlUtils::~GlUtils() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &CBO);
  glDeleteProgram(shaderProgram);
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
  auto vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
  auto fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  CreateProgram(vertexShader, fragmentShader);
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

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window", nullptr, nullptr);
  if (window == nullptr) {
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

std::string GlUtils::ReadShaderSource(const std::string path) {
  std::ifstream file{path, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    std::cout << "Error: File do not exist or could not open" << std::endl;
    throw std::runtime_error("Could not open file: " + path);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer.data();
}

GLuint GlUtils::CompileShader(std::string& source, int type) {
  GLuint shader = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    glDeleteShader(shader);
    return 0;  // Return 0 to indicate failure
  }
  return shader;
}

GLuint GlUtils::CreateProgram(GLuint vertexShader, GLuint fragmentShader) {
  GLuint program = glCreateProgram();
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

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
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
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2),
               vertices.data(),  // Pass pointer to vertex data
               GL_STATIC_DRAW);

  // Bind and fill color buffer
  glBindBuffer(GL_ARRAY_BUFFER, CBO);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), colors.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(vec3i),
               indices.data(),  // Pass pointer to index data
               GL_STATIC_DRAW);

  // Specify vertex attribute pointers
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GlUtils::BindBuffer() {
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
}

void GlUtils::Draw(std::vector<vec2>& vertices, std::vector<vec3>& colors,
                   std::vector<vec3i>& indices) {
  CreateBuffer(vertices, colors, indices);
  BindBuffer();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
