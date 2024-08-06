#version 460

layout(location = 0) in vec3 fragColor;

out vec4 outColor;

uniform vec3 lightColor;

void main() {
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 res = ambient * fragColor;
  outColor = vec4(res, 1.0);
}
