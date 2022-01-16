#version 330

out vec4 frag_color;

void main()
{
	frag_color = vec4(mod(gl_FragCoord.x, 10.0) / 10.0, mod(gl_FragCoord.y, 10) / 10.0, 0.0, 0.5);
}