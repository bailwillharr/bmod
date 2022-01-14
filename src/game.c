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

int game_create(struct Game * g)
{
	ASSERT(window_create(&g->win))
	ASSERT(renderer_create(&g->ren, (float)g->win.width / (float)g->win.height, 90))
	
	memset(g->input.buttons, false, sizeof(bool) * INPUT_BUTTON_COUNT);

	// initiliase world objects


	//object_print_hier(g->obj_root);

	return 0;
}

int game_loop(struct Game * g)
{

	double lastFrame = glfwGetTime();
	double lastSecond = lastFrame;
	int frames = 0;

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

		game_camera_update(g, frameDelta);

		// render
		frames++;
		renderer_prepare(&g->ren);

		float vertices[] = {
			2.5,	-1.0,	-5.0,	1.0, 0.0, 0.0,
			3.0,	-1.0,	-5.0,	0.0, 1.0, 0.0,
			2.5,	-0.5,	-5.0,	0.0, 0.0, 1.0,

			2.5,	-0.5,	-5.0,	0.0, 0.0, 1.0,
			3.0,	-1.0,	-5.0,	0.0, 1.0, 0.0,
			3.0,	-0.5,	-5.0,	1.0, 1.0, 1.0
		};

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), (void *)vertices, GL_STATIC_DRAW);

		GLuint attr_pos = glGetAttribLocation(g->ren.shaders[SHADER_TEXTURE], "aPosition");
		GLuint attr_col = glGetAttribLocation(g->ren.shaders[SHADER_TEXTURE], "aColor");

		glEnableVertexAttribArray(attr_pos);
		glVertexAttribPointer(attr_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(0 * sizeof(float)));
		glEnableVertexAttribArray(attr_col);
		glVertexAttribPointer(attr_col, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glUseProgram(g->ren.shaders[SHADER_TEXTURE]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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
