#include "gl_utils.hpp"

int main() {
  float vertices[] = {
          0.5f,  0.5f, 0.0f,  // top right
          0.5f, -0.5f, 0.0f,  // bottom right
          -0.5f, -0.5f, 0.0f,  // bottom left
          -0.5f,  0.5f, 0.0f   // top left
  };
  unsigned int indices[] = {  // note that we start from 0!
          0, 1, 3,  // first Triangle
          1, 2, 3   // second Triangle
  };

  GlUtils gl_utils(vertices, indices);

  return 0;
}