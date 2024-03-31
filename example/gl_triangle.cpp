#include <chrono>
#include <iostream>
#include <vector>

#include "global_alias.h"
#include "gl_utils.hpp"

int main() {
  std::vector<vec2> vertices = {
      vec2(-0.5f, -0.5f), // Vertex 1
      vec2(0.5f, -0.5f),  // Vertex 2
      vec2(0.0f, 0.5f)    // Vertex 3
  };

  std::vector<vec3> colors = {
      vec3(1.0f, 0.0f, 0.0f), // Color for Vertex 1 (Red)
      vec3(0.0f, 1.0f, 0.0f), // Color for Vertex 2 (Green)
      vec3(0.0f, 0.0f, 1.0f)  // Color for Vertex 3 (Blue)
  };
  std::vector<vec3i> indices = {{0, 1, 2}};

  // initialize
  auto t0{std::chrono::high_resolution_clock::now()};
  size_t frame_count{0};

  wvxy::GlUtils gl_utils{800, 800};
  auto window = gl_utils.window;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_utils.Draw(vertices, colors, indices);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // frame time
    auto t1 = std::chrono::high_resolution_clock ::now();
    std::chrono::duration<double> frame_time = t1 - t0;
    t0 = t1;
    frame_count++;

    if (frame_count % 100 == 0) {
      std::cout << "Frame Time = " << frame_time.count() << '\t' << "\t";
      std::cout << "FPS" << 1.f / frame_time.count() << '\n';
    }
  }

  return 0;
}