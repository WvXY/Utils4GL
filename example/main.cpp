#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "gl_utils.hpp"
#include "global_alias.h"
#include "game_object.hpp"

#include <windows.h>

int main() {
  std::vector<vec3> vertices = {{0.0f, 0.5f, 0.f},
                                {0.5f, -0.5f, 0.f},
                                {-0.5f, -0.5f, 0.f},
                                {0.2f, 0.4f, 0.f}};
  std::vector<vec3> colors = {{1.0f, 1.0f, 0.0f},
                              {0.0f, 1.0f, 1.0f},
                              {0.0f, 0.0f, 1.0f},
                              {1.0f, 0.0f, 1.0f}};
  std::vector<vec3i> indices = {{0, 1, 2}, {0, 1, 3}};
  std::vector<vec2> texCoords = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}, {0.5f, 0.5f}};
  auto go = std::make_unique<wvxy::GameObject>(vertices, colors, indices);

  std::vector<vec3> vertices2 = {{0.0f, 0.8f, 0.f},
                                 {0.5f, -0.4f, 0.f},
                                 {0.5f, 0.5f, 0.f},
                                 {0.2f, 0.4f, 0.f}};
  std::vector<vec3> colors2 = {{1.0f, 0.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f},
                               {0.0f, 0.0f, 1.0f},
                               {1.0f, 0.0f, 1.0f}};
  std::vector<vec3i> indices2 = {{0, 1, 2}};

  // initialize
  auto time_start{std::chrono::high_resolution_clock::now()};
  auto t0 = time_start;
  size_t frame_count{0};

  wvxy::GlUtils app{800, 800, "Triangle"};
  auto* window = app.window;

  // TODO add functions to manage all the texture stuff
  auto tex0 = app.loadTexture("../../assets/wall.jpg");
  glUniform1i(glGetUniformLocation(app.basicShader.getID(), "tex0"), 0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.4f, 0.6f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, tex0);

    app.draw(vertices, colors, indices, texCoords);
    app.draw(vertices2, colors2, indices2);

    glfwPollEvents();
    glfwSwapBuffers(window);

    // frame time
    frame_count++;

    if (frame_count % 100 == 0) {
      auto t1 = std::chrono::high_resolution_clock ::now();
      std::chrono::duration<double> frame_time = t1 - t0;
      t0 = t1;
      app.addInfoToTitle("FPS: " + std::to_string(100.f / frame_time.count()));
    }
  }

  return 0;
}