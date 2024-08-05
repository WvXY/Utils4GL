#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "gl_utils.hpp"
#include "global_alias.h"
#include "game_object.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
  std::vector<vec3> v0 = {{0.0f, 0.5f, 0.f},
                          {0.5f, -0.5f, 0.f},
                          {-0.5f, -0.5f, 0.f},
                          {0.2f, 0.4f, 0.f}};
  std::vector<vec3> c0 = {{1.0f, 1.0f, 0.0f},
                          {0.0f, 1.0f, 1.0f},
                          {0.0f, 0.0f, 1.0f},
                          {1.0f, 0.0f, 1.0f}};
  std::vector<vec3i> i0 = {{0, 1, 2}, {0, 1, 3}};
  std::vector<vec2> tc0 = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}, {0.5f, 0.5f}};
  auto go = std::make_unique<wvxy::GameObject>(v0, c0, i0);

  std::vector<vec3> v1 = {{0.0f, 0.8f, 0.f},
                          {0.5f, -0.4f, 0.f},
                          {0.5f, 0.5f, 0.f},
                          {0.2f, 0.4f, 0.f}};
  std::vector<vec3> c1 = {{1.0f, 0.0f, 0.0f},
                          {0.0f, 1.0f, 0.0f},
                          {0.0f, 0.0f, 1.0f},
                          {1.0f, 0.0f, 1.0f}};
  std::vector<vec3i> i1 = {{0, 1, 2}};

  // initialize
  auto time_start{std::chrono::high_resolution_clock::now()};
  auto t0 = time_start;
  size_t frame_count{0};

  wvxy::GlUtils glApp{800, 800, "Triangle"};
  auto* window = glApp.window;

  // TODO add functions to manage all the texture stuff
  auto tex0 = glApp.loadTexture("../../assets/wall.jpg");
  glUniform1i(glGetUniformLocation(glApp.basicShader.getID(), "tex0"), 0);

  /*------------------------Loop-------------------------*/
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.4f, 0.3f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // transform
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int transformLoc =
        glGetUniformLocation(glApp.basicShader.getID(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                       glm::value_ptr(trans));

    // texture
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, tex0);

    // draw
    glApp.draw(v0, c0, i0, tc0);
    glApp.draw(v1, c1, i1);

    glfwPollEvents();
    glfwSwapBuffers(window);

    // frame time
    frame_count++;

    if (frame_count % 100 == 0) {
      auto t1 = std::chrono::high_resolution_clock ::now();
      std::chrono::duration<double> frame_time = t1 - t0;
      t0 = t1;
      glApp.addInfoToTitle("FPS: " +
                           std::to_string(100.f / frame_time.count()));
    }
  }

  return 0;
}