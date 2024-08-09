#pragma once

#include "global_alias.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace wvxy {
namespace GlUtils {
class Camera {
public:
  Camera() = default;
  Camera(vec3 position, vec3 front, vec3 up)
      : position(position), front(front), up(up) {}
  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;

  vec3 position{0.0f, 0.0f, 3.0f};
  vec3 front{0.0f, 0.0f, -1.0f};
  vec3 up{0.0f, 1.0f, 0.0f};

  vec3 right() { return glm::normalize(glm::cross(front, worldUp)); }
  // vec3 up() { return glm::normalize(glm::cross(right(), front)); }

  int projectionType = 0;  // 0: perspective, 1: ortho
  float pitch{0.f};
  float yaw{-90.0f};
  float fov = 45.0f;

  mat4 view() { return glm::lookAt(position, position + front, up); }

  mat4 projection() {
    if (projectionType == 0)
      return perspective();
    else
      return ortho();
  }

  enum KeyboardInput { FORWARD, BACKWARD, LEFT, RIGHT };

  void processKeyboard(KeyboardInput kbdInput, float deltaTime) {
    float velocity = speed * deltaTime;
    if (kbdInput == FORWARD)
      position += front * velocity;
    if (kbdInput == BACKWARD)
      position -= front * velocity;
    if (kbdInput == LEFT)
      position -= right() * velocity;
    if (kbdInput == RIGHT)
      position += right() * velocity;
  }

  void processMouseMovement(float xoffset, float yoffset) {
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    updateCameraVectors();
  }

  void processMouseScroll(float yoffset) {
    if (fov >= 10.0f && fov <= 120.0f)
      fov -= yoffset;
    if (fov <= 10.0f)
      fov = 10.0f;
    if (fov >= 120.0f)
      fov = 120.0f;
  }

private:
  mat4 perspective() {
    return glm::perspective(
        glm::radians(fov),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
        0.1f, 100.0f);
  }
  mat4 ortho() {
    return glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, 0.001f, 100.0f);
  }

  void updateCameraVectors() {
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(front);
    up = glm::normalize(glm::cross(right(), front));
  }

  unsigned int screenWidth = 800;
  unsigned int screenHeight = 800;

  const float sensitivity = 0.3f;
  const float speed = 0.5f;
  const vec3 worldUp{0.0f, 1.0f, 0.0f};
};
}
}  // namespace wvxy