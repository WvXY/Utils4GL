#pragma once

#include "global_alias.h"

namespace wvxy {

struct GameObject {
 public:
  GameObject(std::vector<vec3> vertices, std::vector<vec3> colors,
             std::vector<vec3i> indices)
      : vertices{vertices}, colors{colors}, indices{indices} {
    hasIndices = true;
  }

  GameObject(std::vector<vec3> vertices, std::vector<vec3> colors)
      : vertices{vertices}, colors{colors} {}

  std::vector<vec3> vertices;
  std::vector<vec3> colors;
  std::vector<vec3i> indices;

  bool hasIndices = false;

  void draw();
};

}
