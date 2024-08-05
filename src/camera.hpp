#pragma once

#include "global_alias.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace wvxy {

class Camera {
 public:
  Camera() = default;
  Camera(vec3 position, vec3 target, vec3 up)
      : position(position), target(target), up(up) {}
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  vec3 position{0.0f, 0.0f, 3.0f};
  vec3 target{0.0f, 0.0f, 0.0f};
  vec3 up{0.0f, 1.0f, 0.0f};

  vec3 direction() { return glm::normalize(target - position); }
  vec3 cameraRight() { return -glm::normalize(glm::cross(up, direction())); }
  vec3 cameraUp() { return glm::cross(direction(), cameraRight()); }

  int projectionType = 0;  // 0: perspective, 1: ortho

  mat4 view() { return glm::lookAt(position, target, up); }

  mat4 projection() {
    if (projectionType == 0)
      return perspective();
    else
      return ortho();
  }

 private:
  mat4 perspective() {
    return glm::perspective(
        glm::radians(60.f),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
        0.1f, 100.0f);
  }
  mat4 ortho() {
    return glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, 0.001f, 100.0f);
  }

  unsigned int screenWidth = 800;
  unsigned int screenHeight = 800;
};

}  // namespace wvxy