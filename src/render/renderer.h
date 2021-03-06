#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include "camera.h"
#include <glad/glad.h>

enum Shaders {
	SHADER_TEXTURE = 0,
	SHADER_TEST,
	SHADER_COUNT
};

struct Renderer {
	struct Camera cam;
	GLuint shaders[SHADER_COUNT];
};

extern const char * SHADER_NAMES[];

int renderer_create(struct Renderer * ren, float aspect, float fov);
void renderer_prepare(struct Renderer * ren);
void renderer_destroy(struct Renderer * ren);

void renderer_on_window_resize(struct Renderer * ren, int width, int height);

#endif
