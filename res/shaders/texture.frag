#version 330

in vec3 fColor;
in vec3 normal;
in vec2 uv;

out vec4 frag_color;

void main()
{
	frag_color = vec4(fColor, 1.0);
}
