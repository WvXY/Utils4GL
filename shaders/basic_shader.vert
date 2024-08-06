#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragNormal = inNormal;

    fragPos = vec3(model * vec4(inPos, 1.0));
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}
