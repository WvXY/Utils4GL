#include <chrono>
#include <iostream>
#include <vector>

#include "global_alias.h"
#include "gl_utils.hpp"

int main() {
  std::vector<vec2> vertices = {{0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}};
  std::vector<vec3> colors = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
  std::vector<vec3i> indices = {{0, 1, 2}};

  // initialize
  auto t0{std::chrono::high_resolution_clock::now()};
  size_t frame_count{0};

  wvxy::GlUtils gl_utils{600, 800};

  while (!glfwWindowShouldClose(gl_utils.window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_utils.Draw(vertices, colors, indices);

    glfwSwapBuffers(gl_utils.window);
    glfwPollEvents();

    // frame time
    auto t1 = std::chrono::high_resolution_clock ::now();
    std::chrono::duration<double> frame_time = t1 - t0;
    t0 = t1;
    frame_count++;

    if (frame_count % 100 == 0) {
      std::cout << "Frame Time = " << frame_time << '\t' << "\t";
      std::cout << "FPS" << 1.f / frame_time.count() << '\n';
    }
  }

  return 0;
}