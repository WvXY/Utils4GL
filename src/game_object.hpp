#pragma once

#include <utility>

#include "global_alias.h"

namespace wvxy {

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

  // a temporary solution for the current project
  void fromVerticeNormal(std::vector<vec3> data) {
        for (int i = 0; i < data.size(); i += 2) {
          vertices.push_back(data[i]);
          normals.push_back(data[i + 1]);
        }
  }

  void draw();
};

}  // namespace wvxy
