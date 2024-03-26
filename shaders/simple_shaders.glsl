
#version 460
in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos, 1.0);
}


#version 460
out vec4 FragColor;
void main()
{
  FragColor = vec4(0.0f, 1.f, 0.f, 1.0f);
}
