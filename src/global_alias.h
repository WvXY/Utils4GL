#ifndef UTILS_GLOBAL_ALIAS_H
#define UTILS_GLOBAL_ALIAS_H

#ifdef USE_EIGEN
#include <Eigen/Dense>
typedef Eigen::Vector2f vec2;
typedef Eigen::Vector3f vec3;
typedef Eigen::Vector4f vec4;
typedef Eigen::Matrix2f mat2;
typedef Eigen::Matrix3f mat3;
typedef Eigen::Matrix4f mat4;
typedef Eigen::Vector3i ivec3;  // same definition in glsl
typedef Eigen::Vector3i vec3i;

#else  // default to glm
#include <glm/glm.hpp>
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::ivec3 ivec3;
typedef glm::ivec3 vec3i;
#endif

#endif  // UTILS_GLOBAL_ALIAS_H
