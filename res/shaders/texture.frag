#version 330

in vec3 fColor;

out vec4 frag_color;

void main()
{
	frag_color = vec4(fColor, 1.0);
}
