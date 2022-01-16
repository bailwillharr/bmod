#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "types.h"

struct Keyboard {
	bool keys[GLFW_KEY_LAST+1];
	enum ButtonDelta deltas[GLFW_KEY_LAST+1];
};

struct Mouse {
	bool buttons[GLFW_MOUSE_BUTTON_LAST+1];
	enum ButtonDelta deltas[GLFW_MOUSE_BUTTON_LAST+1];
	double xpos, ypos;
	double xdelta, ydelta;
	double xscroll, yscroll;
};

struct Gamepad {
	bool active;
	int jid;
	const char *name;
	GLFWgamepadstate state;
	enum ButtonDelta deltas[GLFW_GAMEPAD_BUTTON_LAST+1];
};

struct Window {
	
	GLFWwindow *handle;
	// game width/height
	int width, height;
	GLFWmonitor *mon;
	bool isFullscreen;
	int swapInterval;

	bool resizedFlag; // set to true if window was just resized

	struct Keyboard keyboard;
	struct Mouse mouse;
	struct Gamepad gamepad;

};

int window_create(struct Window *win);
void window_destroy(struct Window *win);

int window_prepare_input(struct Window *win);
void window_toggle_fullscreen(struct Window *win);

#endif
