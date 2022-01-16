#version 330

vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

in vec3 Norm;
in vec2 uv;

out vec4 frag_color;

uniform sampler2D tex;

void main() {
	vec3 dir = vec3(-0.123, 0.4, 0.56);
    vec3 lighting = max(dot( normalize(Norm), normalize(dir)), 0.0) * vec3(1.0, 1.0, 1.0);
    frag_color = vec4(lighting, 1.0) * texture(tex, uv);
}