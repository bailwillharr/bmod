#version 330

layout (location = 0) in vec3 aPosition;

uniform mat4 p;
uniform mat4 v;

void main()
{
	gl_Position = p * v * vec4(aPosition, 1.0);
}