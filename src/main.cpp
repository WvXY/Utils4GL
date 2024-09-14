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

using namespace wvxy;

int main() {
  std::vector<vec3> vertices = {// Front face
                                {-0.5f, -0.5f, -0.5f},
                                {0.5f, -0.5f, -0.5f},
                                {0.5f, 0.5f, -0.5f},
                                {0.5f, 0.5f, -0.5f},
                                {-0.5f, 0.5f, -0.5f},
                                {-0.5f, -0.5f, -0.5f},

                                // Back face
                                {-0.5f, -0.5f, 0.5f},
                                {0.5f, -0.5f, 0.5f},
                                {0.5f, 0.5f, 0.5f},
                                {0.5f, 0.5f, 0.5f},
                                {-0.5f, 0.5f, 0.5f},
                                {-0.5f, -0.5f, 0.5f},

                                // Left face
                                {-0.5f, 0.5f, 0.5f},
                                {-0.5f, 0.5f, -0.5f},
                                {-0.5f, -0.5f, -0.5f},
                                {-0.5f, -0.5f, -0.5f},
                                {-0.5f, -0.5f, 0.5f},
                                {-0.5f, 0.5f, 0.5f},

                                // Right face
                                {0.5f, 0.5f, 0.5f},
                                {0.5f, 0.5f, -0.5f},
                                {0.5f, -0.5f, -0.5f},
                                {0.5f, -0.5f, -0.5f},
                                {0.5f, -0.5f, 0.5f},
                                {0.5f, 0.5f, 0.5f},

                                // Bottom face
                                {-0.5f, -0.5f, -0.5f},
                                {0.5f, -0.5f, -0.5f},
                                {0.5f, -0.5f, 0.5f},
                                {0.5f, -0.5f, 0.5f},
                                {-0.5f, -0.5f, 0.5f},
                                {-0.5f, -0.5f, -0.5f},

                                // Top face
                                {-0.5f, 0.5f, -0.5f},
                                {0.5f, 0.5f, -0.5f},
                                {0.5f, 0.5f, 0.5f},
                                {0.5f, 0.5f, 0.5f},
                                {-0.5f, 0.5f, 0.5f},
                                {-0.5f, 0.5f, -0.5f}};

  std::vector<vec3> normals = {// Front face normals
                               {0.0f, 0.0f, -1.0f},
                               {0.0f, 0.0f, -1.0f},
                               {0.0f, 0.0f, -1.0f},
                               {0.0f, 0.0f, -1.0f},
                               {0.0f, 0.0f, -1.0f},
                               {0.0f, 0.0f, -1.0f},

                               // Back face normals
                               {0.0f, 0.0f, 1.0f},
                               {0.0f, 0.0f, 1.0f},
                               {0.0f, 0.0f, 1.0f},
                               {0.0f, 0.0f, 1.0f},
                               {0.0f, 0.0f, 1.0f},
                               {0.0f, 0.0f, 1.0f},

                               // Left face normals
                               {-1.0f, 0.0f, 0.0f},
                               {-1.0f, 0.0f, 0.0f},
                               {-1.0f, 0.0f, 0.0f},
                               {-1.0f, 0.0f, 0.0f},
                               {-1.0f, 0.0f, 0.0f},
                               {-1.0f, 0.0f, 0.0f},

                               // Right face normals
                               {1.0f, 0.0f, 0.0f},
                               {1.0f, 0.0f, 0.0f},
                               {1.0f, 0.0f, 0.0f},
                               {1.0f, 0.0f, 0.0f},
                               {1.0f, 0.0f, 0.0f},
                               {1.0f, 0.0f, 0.0f},

                               // Bottom face normals
                               {0.0f, -1.0f, 0.0f},
                               {0.0f, -1.0f, 0.0f},
                               {0.0f, -1.0f, 0.0f},
                               {0.0f, -1.0f, 0.0f},
                               {0.0f, -1.0f, 0.0f},
                               {0.0f, -1.0f, 0.0f},

                               // Top face normals
                               {0.0f, 1.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f},
                               {0.0f, 1.0f, 0.0f}};

  auto go1 = std::make_unique<GlUtils::GameObject>();
  go1->setVertices(vertices);
  go1->setNormals(normals);

  std::vector<vec3> groundVertices = {// Triangle 1
                                      {-100.0f, -1.0f, -100.0f},
                                      {100.0f, -1.0f, -100.0f},
                                      {100.0f, -1.0f, 100.0f},

                                      // Triangle 2
                                      {100.0f, -1.0f, 100.0f},
                                      {-100.0f, -1.0f, 100.0f},
                                      {-100.0f, -1.0f, -100.0f}};

  std::vector<vec3> groundNormals = {// Normals for both triangles (since it's
                                     // flat, they all have the same normal)
                                     {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                     {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
                                     {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};

  auto groundObject = std::make_unique<GlUtils::GameObject>();
  groundObject->setVertices(groundVertices);
  groundObject->setNormals(groundNormals);

  // TODO: separate into an APP class
  // initialize
  auto time_start{std::chrono::high_resolution_clock::now()};
  auto t0 = time_start;
  size_t frame_count{0};

  GlUtils::ExampleApp glApp{800, 800, "GLDEMO"};
  // glApp.setWireframeMode(true);

  auto* window = glApp.window;
  auto& camera = glApp.camera;
  auto& shader = glApp.basicShader;
  shader.use();

  // TODO add functions to manage all the texture stuff
  auto tex0 = glApp.loadTexture("../../assets/wall.jpg");
  glUniform1i(glGetUniformLocation(shader.getID(), "tex0"), 0);

  /*------------------------Loop-------------------------*/
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.3f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glApp.processInput(window);
    vec3 lightPos = {sin(glfwGetTime()), 2.f, cos(glfwGetTime())};

    // // texture
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, tex0);

    shader.setMat4("projection", camera.projection());
    shader.setMat4("view", camera.view());
    shader.setVec3("lightColor", {1.0f, 1.0f, 1.0f});
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.position);

    glApp.createBuffer(*go1);
    shader.setMat4("model", go1->model);
    glApp.draw();
    glApp.releaseBuffers();

    glApp.createBuffer(*groundObject);
    shader.setMat4("model", groundObject->model);
    glApp.draw();
    glApp.releaseBuffers();

    glfwSwapBuffers(window);
    glfwPollEvents();

    // frame time
    frame_count++;
    auto t1 = std::chrono::high_resolution_clock ::now();
    glApp.deltaTime = std::chrono::duration<float>(t1 - t0).count();
    t0 = t1;
    glApp.addInfoToTitle("FPS: " + std::to_string(1.f / glApp.deltaTime));
  }

  return 0;
}