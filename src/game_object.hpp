#pragma once

#include <utility>

#include "global_alias.h"

namespace wvxy {
namespace GlUtils {
struct GameObject {
 public:
  GameObject() = default;
  GameObject(std::vector<vec3> vertices,
             std::vector<vec3> colors,
             std::vector<vec3i> indices)
      : vertices{std::move(vertices)},
        colors{std::move(colors)},
        indices{std::move(indices)} {}

  GameObject(std::vector<vec3> vertices, std::vector<vec3> colors)
      : vertices{std::move(vertices)}, colors{colors} {}

  ~GameObject() = default;

  std::vector<vec3> vertices;
  std::vector<vec3> colors;
  std::vector<vec3i> indices;
  std::vector<vec2> texCoords;
  std::vector<vec3> normals;

  mat4 model = mat4(1.0f);

  bool hasIndices() const { return !indices.empty(); }

  void setVertices(std::vector<vec3> vertices) {
    this->vertices = std::move(vertices);
  }
  void setColors(std::vector<vec3> colors) { this->colors = std::move(colors); }
  void setIndices(std::vector<vec3i> indices) {
    this->indices = std::move(indices);
  }
  void setTexCoords(std::vector<vec2> texCoords) {
    this->texCoords = std::move(texCoords);
  }
  void setNormals(std::vector<vec3> normals) {
    this->normals = std::move(normals);
  }

  void setOffset(vec3 offset) {
    model[3][0] = offset.x;
    model[3][1] = offset.y;
    model[3][2] = offset.z;
  }

  void draw();
};

}  // namespace GlUtils

}  // namespace wvxy
