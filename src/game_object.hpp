#pragma once

#include "global_alias.h"

struct GameObject {
 public:
  GameObject(std::vector<vec2> vertices, std::vector<vec3> colors,
             std::vector<vec3i> indices = {})
      : vertices{vertices}, colors{colors}, indices{indices} {}

  std::vector<vec2> vertices;
  std::vector<vec3> colors;
  std::vector<vec3i> indices;

  void Draw();
};