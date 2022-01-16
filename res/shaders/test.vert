#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 p;
uniform mat4 v;

out vec3 Norm;
out vec2 uv;
  
void main()
{
    gl_Position = p * v * vec4(aPosition, 1.0);
    Norm = aNormal;
    uv = aUV;
}