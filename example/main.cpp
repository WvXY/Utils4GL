#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "gl_utils.hpp"
#include "global_alias.h"
#include "game_object.hpp"
#include "camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
  std::vector<vec3> v0 = {{0.0f, 0.5f, 1.f},
                          {0.5f, -0.5f, 3.f},
                          {-0.5f, -0.5f, 2.f},
                          {0.2f, 0.4f, 0.f}};
  std::vector<vec3> c0 = {{1.0f, 1.0f, 1.0f},
                          {0.0f, 1.0f, 1.0f},
                          {0.0f, 0.0f, 1.0f},
                          {1.0f, 0.0f, 1.0f}};
  std::vector<vec3i> i0 = {{0, 1, 2}, {0, 1, 3}};
  std::vector<vec2> tc0 = {
      {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}, {0.5f, 0.5f}};
  auto go = std::make_unique<wvxy::GameObject>(v0, c0, i0);

  std::vector<vec3> v1 = {{0.0f, 0.8f, 1.f},
                          {0.5f, -0.4f, 2.f},
                          {0.5f, 0.5f, 1.f},
                          {0.2f, 0.4f, 0.f}};
  std::vector<vec3> c1 = {{1.0f, 0.0f, 0.0f},
                          {0.0f, 1.0f, 0.0f},
                          {0.0f, 0.0f, 1.0f},
                          {1.0f, 0.0f, 1.0f}};
  std::vector<vec3i> i1 = {{0, 1, 2}};

  std::vector<vec3> cubePositions = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  // initialize
  auto time_start{std::chrono::high_resolution_clock::now()};
  auto t0 = time_start;
  size_t frame_count{0};

  wvxy::GlUtils glApp{800, 800, "Triangle"};
  auto* window = glApp.window;
  auto& camera = glApp.camera;
  auto& shader = glApp.basicShader;
  shader.use();

  // TODO add functions to manage all the texture stuff
  auto tex0 = glApp.loadTexture("../../assets/wall.jpg");
  glUniform1i(glGetUniformLocation(shader.getID(), "tex0"), 0);

  /*------------------------Loop-------------------------*/
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.6f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glApp.processInput(window);

    // texture
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, tex0);

    shader.setMat4("projection", camera.projection());
    glApp.basicShader.setMat4("view", camera.view());

    glApp.createBuffer(v0, c0, i0, tc0);
    for (unsigned int i = 0; i < 100; i++) {

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      glApp.basicShader.setMat4("model", model);

      glApp.draw();
    }
    glApp.releaseBuffers();

    glApp.createBuffer(v1, c1, i1);
    for (unsigned int i = 0; i < 20; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      glApp.basicShader.setMat4("model", model);

      glApp.draw();
    }
    glApp.releaseBuffers();

    glfwPollEvents();
    glfwSwapBuffers(window);

    // frame time
    frame_count++;
    auto t1 = std::chrono::high_resolution_clock ::now();
    glApp.deltaTime = std::chrono::duration<float>(t1 - t0).count();
    t0 = t1;
    glApp.addInfoToTitle("FPS: " + std::to_string(1.f / glApp.deltaTime));
  }

  return 0;
}