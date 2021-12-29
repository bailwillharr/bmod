#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "../glad/glad.h"

enum Shaders {
	SHADER_TEXTURE = 0,
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

#endif
