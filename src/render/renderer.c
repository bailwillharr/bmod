#include <glad/glad.h>

#include "../logger.h"
#include "../game.h"
#include "shader.h"
#include "camera.h"

#include "renderer.h"

// extern constants
const char *SHADER_NAMES[] = {
	"texture",
	"test"
};

static void uniform_to_all_shaders(GLuint *shaders, const char *u, mat4 mat)
{
	for (int i = 0; i < SHADER_COUNT; i++) {
		glUseProgram(shaders[i]);
		glUniformMatrix4fv(glGetUniformLocation(shaders[i], u), 1, GL_FALSE, (float *)mat);
	}
}

int renderer_create(struct Renderer *ren, float aspect, float fov)
{

	// load shaders
	for (int i = 0; i < SHADER_COUNT; i++) {
		ren->shaders[i] = shader_create(SHADER_NAMES[i]);
		log_log("shader '%s' loaded\n", SHADER_NAMES[i]);
	}

	camera_init(&ren->cam, aspect, fov, (vec3){ 0.0, 0.0, 0.0 }, (vec2){ 0.0, 0.0 });
	uniform_to_all_shaders(ren->shaders, "p", ren->cam.p);

	glEnable(GL_DEPTH_TEST);

	return 0;
}

void renderer_prepare(struct Renderer *ren)
{
	camera_update(&ren->cam);
	// update shaders
	uniform_to_all_shaders(ren->shaders, "v", ren->cam.v);

	glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_destroy(struct Renderer *ren)
{
	for (int i = 0; i < SHADER_COUNT; i++) {
		shader_destroy(ren->shaders[i]);
	}
}


void renderer_on_window_resize(struct Renderer *ren, int width, int height)
{
	// re-initialise camera with new aspect ratio
	camera_init(&ren->cam, (float)width / (float)height, ren->cam.fov, ren->cam.pos, ren->cam.rot);
	uniform_to_all_shaders(ren->shaders, "p", ren->cam.p);
}