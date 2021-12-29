#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

uniform mat4 p;
uniform mat4 v;

out vec3 fColor;

void main()
{
	gl_Position = p * v * vec4(aPosition, 1.0);
	fColor = aColor;
}
