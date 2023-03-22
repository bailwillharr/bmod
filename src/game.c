#include <stdio.h>
#include <string.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "game.h"
#include "logger.h"
#include "util.h"
#include "window.h"
#include "render/renderer.h"
#include "game_input.h"
#include "game_camera.h"

struct MeshVertex {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

struct MeshFileHeader {
	unsigned int        vertex_count;
	unsigned int        index_count;
	int                 material;
	// Vertices begin immediately after this header.
	// Indices begin immediately after that
} header;

struct MeshVertex *gun_vertices;
unsigned int *gun_indices;
unsigned int gun_texture;
unsigned int vao, vbo, ebo;

static struct GameObject *createObjects()
{
	struct GameObject *root = object_create_sibling(NULL, "root");
	return root;
}

int game_create(struct Game * g)
{
	ASSERT(window_create(&g->win))
	ASSERT(renderer_create(&g->ren, (float)g->win.width / (float)g->win.height, 90))
	
	memset(g->input.buttons, false, sizeof(bool) * INPUT_BUTTON_COUNT);

	g->obj_root = createObjects();

	// initiliase world objects

	//object_print_hier(g->obj_root);

	FILE *fp = fopen("meshes/gun_0.bmod_mesh", "rb");

	fread(&header, sizeof(struct MeshFileHeader), 1, fp);
	
	log_log("vertex_count = %d\nindex_count = %d\n", header.vertex_count, header.index_count);

	gun_vertices = calloc(sizeof(struct MeshVertex),header.vertex_count);
	gun_indices = calloc(sizeof(unsigned int),header.index_count);

	fread(gun_indices, sizeof(unsigned int) * header.index_count, 1, fp);
	fread(gun_vertices, sizeof(float) * 8 * header.vertex_count, 1, fp);

	fclose(fp);

	log_log("sizeof(struct MeshVertex): %lu\n", sizeof(struct MeshVertex));
	log_log("offsetof(struct MeshVertex, u): %lu\n", offsetof(struct MeshVertex, u));

	fp = fopen("textures/gun.glraw", "rb");
	if (!fp) {
		log_error("unable to open tex\n");
		return -1;
	}

	fseek(fp, 0x02, SEEK_SET);
	uint64_t tex_data_offset;
	fread(&tex_data_offset, sizeof(uint64_t), 1, fp);

	glGenTextures(1, &gun_texture);
	glBindTexture(GL_TEXTURE_2D, gun_texture);
	fseek(fp, 0L, SEEK_END);
	uint64_t end = ftell(fp);

	uint8_t *texbuf = malloc(end); // allocated a little too much here but it works
	fseek(fp, tex_data_offset, SEEK_SET);
	fread(texbuf, 1, end, fp);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4096, 4096, 0, GL_RGB, GL_UNSIGNED_BYTE, texbuf);

	free(texbuf);
	fclose(fp);

	glGenerateMipmap(GL_TEXTURE_2D);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct MeshVertex) * header.vertex_count, &gun_vertices[0], GL_STATIC_DRAW);
	free(gun_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * header.index_count, &gun_indices[0], GL_STATIC_DRAW);
	free(gun_indices);
	glEnableVertexAttribArray(glGetAttribLocation(g->ren.shaders[SHADER_TEST], "aPosition"));
	glEnableVertexAttribArray(glGetAttribLocation(g->ren.shaders[SHADER_TEST], "aNormal"));
	glEnableVertexAttribArray(glGetAttribLocation(g->ren.shaders[SHADER_TEST], "aUV"));
	glVertexAttribPointer(glGetAttribLocation(g->ren.shaders[SHADER_TEST], "aPosition"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)offsetof(struct MeshVertex, x));
	glVertexAttribPointer(glGetAttribLocation(g->ren.shaders[SHADER_TEST], "aNormal"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)offsetof(struct MeshVertex, nx));
	glVertexAttribPointer(glGetAttribLocation(g->ren.shaders[SHADER_TEST], "aUV"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)offsetof(struct MeshVertex, u));
	glBindVertexArray(0);

	return 0;
}

int game_loop(struct Game * g)
{

	double lastFrame = glfwGetTime();
	double lastSecond = lastFrame;
	int frames = 0;

	g->ren.cam.pos[0] += 13.0f;
	g->ren.cam.pos[1] += 12.0f;
	g->ren.cam.pos[2] += 1.0f;
	g->ren.cam.rot[0] -= 3.14159f / 8.0f;
	g->ren.cam.rot[1] += 3.14159f / 2.0f;

	while (!glfwWindowShouldClose(g->win.handle)) {

		// logic

		double now =  glfwGetTime();
		double frameDelta = now - lastFrame;
		lastFrame = now;
		if (now - lastSecond > 1.0) {
			// runs every second
			log_log("FPS: %d\n", frames);
			frames = 0;
			lastSecond = now;
		}

		if (g->win.gamepad.active == true) {
			// use gamepad
			game_input_handleGamepad(g);
		} else {
			// use keyboard
			game_input_handleKeyboard(g);
		}

		if (g->input.deltas[INPUT_BUTTON_FULLSCREEN] == BUTTON_PRESSED) {
			window_toggle_fullscreen(&g->win);
		}
		if (g->input.deltas[INPUT_BUTTON_QUIT] == BUTTON_PRESSED) {
			glfwSetWindowShouldClose(g->win.handle, GLFW_TRUE);
		}
		if (g->win.keyboard.deltas[GLFW_KEY_O] == BUTTON_PRESSED) {
			glfwSetInputMode(g->win.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		if (g->win.keyboard.deltas[GLFW_KEY_I] == BUTTON_PRESSED) {
			glfwSetInputMode(g->win.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (g->win.keyboard.deltas[GLFW_KEY_V] == BUTTON_PRESSED) {
			g->win.swapInterval = !g->win.swapInterval;
			glfwSwapInterval(g->win.swapInterval);
		}

		if (g->win.resizedFlag == true) {
			renderer_on_window_resize(&g->ren, g->win.width, g->win.height);
			g->win.resizedFlag = false;
		}
		game_camera_update(g, frameDelta);

		// render
		frames++;
		renderer_prepare(&g->ren);
		glBindVertexArray(vao);
		glUseProgram(g->ren.shaders[SHADER_TEST]);
		glDrawElements(GL_TRIANGLES, header.index_count, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(g->win.handle);

		// events
		ASSERT(window_prepare_input(&g->win))
		glfwPollEvents();

	}

	return 0;
}

void game_destroy(struct Game * g)
{
	renderer_destroy(&g->ren);
	window_destroy(&g->win);
}
