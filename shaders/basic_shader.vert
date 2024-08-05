#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

uniform mat4 transform;

void main()
{
    fragColor = inColor;
    gl_Position = transform * vec4(inPos, 1.0);
    fragTexCoord = inTexCoord;
}
