#version 460

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 fragColor;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    fragColor = vec4(inColor, 1.0);
}
