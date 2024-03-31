#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "gl_utils.hpp"
#include "global_alias.h"

int main() {
  std::vector<vec2> vertices = {
      {0.0f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}, {0.2f, 0.4f}};

  std::vector<vec3> colors = {{1.0f, 1.0f, 0.0f},
                              {0.0f, 1.0f, 1.0f},
                              {0.0f, 0.0f, 1.0f},
                              {1.0f, 0.0f, 1.0f}};

  std::vector<vec3i> indices = {{0, 1, 2}, {0, 1, 3}};

  // initialize
  auto t0{std::chrono::high_resolution_clock::now()};
  size_t frame_count{0};

//    wvxy::GlUtils gl_utils{800, 800};
  auto gl_utils = std::make_unique<wvxy::GlUtils>(800, 800, "Triangle");

  auto* window = gl_utils->window;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.4f, 0.6f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_utils->Draw(vertices, colors, indices);

    glfwPollEvents();
    glfwSwapBuffers(window);

    // frame time
    auto t1 = std::chrono::high_resolution_clock ::now();
    std::chrono::duration<double> frame_time = t1 - t0;
    t0 = t1;
    frame_count++;

    if (frame_count % 100 == 0) {
      std::cout << "Frame Time: " << frame_time.count() << '\t' << "\t";
      std::cout << "FPS: " << 1.f / frame_time.count() << '\n';
    }
  }

  return 0;
}