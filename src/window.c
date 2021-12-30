#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "logger.h"
#include "util.h"

#include "window.h"

#define TITLE "bmod"
#define WINDOWED_WIDTH 640
#define WINDOWED_HEIGHT 480



/* static functions */

static void cb_framebuffer_size(GLFWwindow *handle, int newWidth, int newHeight)
{
	struct Window *win = glfwGetWindowUserPointer(handle);
	win->width = newWidth;
	win->height = newHeight;
	glViewport(0, 0, newWidth, newHeight);
}

// mouse callbacks
static void cb_cursor_pos(GLFWwindow *handle, double xpos, double ypos)
{
	struct Window *win = glfwGetWindowUserPointer(handle);
	win->mouse.xdelta = xpos - win->mouse.xpos;
	win->mouse.ydelta = ypos - win->mouse.ypos;
	win->mouse.xpos = xpos;
	win->mouse.ypos = ypos;
}
static void cb_scroll(GLFWwindow *handle, double xoffset, double yoffset)
{
	struct Window *win = glfwGetWindowUserPointer(handle);
	win->mouse.xscroll = xoffset;
	win->mouse.yscroll = yoffset;
}
static void cb_mouse_button(GLFWwindow *handle, int button, int action, int mods)
{
	struct Window *win = glfwGetWindowUserPointer(handle);
	switch (action) {
		case GLFW_PRESS:
			win->mouse.deltas[button] = BUTTON_PRESSED;
			win->mouse.buttons[button] = true;
			break;
		case GLFW_RELEASE:
			win->mouse.deltas[button] = BUTTON_RELEASED;
			win->mouse.buttons[button] = false;
			break;
	}
}

// keyboard callback
static void cb_key(GLFWwindow *handle, int key, int scancode, int action, int mods)
{
	struct Window *win = glfwGetWindowUserPointer(handle);
	switch (action) {
		case GLFW_PRESS:
			win->keyboard.deltas[key] = BUTTON_PRESSED;
			win->keyboard.keys[key] = true;
			break;
		case GLFW_RELEASE:
			win->keyboard.deltas[key] = BUTTON_RELEASED;
			win->keyboard.keys[key] = false;
			break;
	}

}

// controller callback
static void cb_joystick(int jid, int event)
{
	GLFWwindow *handle = glfwGetCurrentContext(); // this is evil. why must i do this?
	struct Window *win = glfwGetWindowUserPointer(handle);
	switch (event) {
		case GLFW_CONNECTED:
			if (glfwJoystickIsGamepad(jid)) {
				win->gamepad.jid = jid;
				win->gamepad.name = glfwGetGamepadName(jid);
				win->gamepad.active = true;
				log_log("Controller connected: %s\n", win->gamepad.name);
			}
			break;
		case GLFW_DISCONNECTED:
			if (jid == win->gamepad.jid) {
				win->gamepad.active = false;
				// all of win->gamepad is now invalid, apart from .active
				log_log("Controller disconnected\n");
			}
			break;
	}
}

static void cb_error(int code, const char *description)
{
	log_error("GLFW error %d: %s\n", code, description);
}

static void goFullscreen(struct Window *win)
{
	const GLFWvidmode *mode = glfwGetVideoMode(win->mon);
	glfwSetWindowMonitor(win->handle, win->mon, 0, 0, mode->width, mode->height, mode->refreshRate);
	win->isFullscreen = true;
}

static void goWindowed(struct Window *win)
{
	const GLFWvidmode *mode = glfwGetVideoMode(win->mon);
	int xpos = (mode->width / 2) - (WINDOWED_WIDTH / 2);
	int ypos = (mode->height / 2) - (WINDOWED_HEIGHT / 2);
	glfwSetWindowMonitor(win->handle, NULL, xpos, ypos, WINDOWED_WIDTH, WINDOWED_HEIGHT, mode->refreshRate);
	win->isFullscreen = false;
}

/* extern functions */

void window_toggle_fullscreen(struct Window *win)
{
	win->isFullscreen = !win->isFullscreen;
	if (win->isFullscreen) goFullscreen(win);
	else goWindowed(win);
}

int window_create(struct Window *win)
{

	glfwSetErrorCallback(cb_error);

	if (!glfwInit()) {
		log_error("Error initialising GLFW\n");
		return -1;
	}

	// tell logger glfwGetTime() is available
	logger_isTimeAvailable = true;

	// use fixed monitor (for fullscreen mode)
	int monitorCount;
	GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
	for (int i = 0; i < monitorCount; i++) {
		log_log("Monitor %d: %s\n", i, glfwGetMonitorName(monitors[i]));
	}
	win->mon = monitors[0];

	// start off in windowed mode
	win->isFullscreen = false;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);

	// use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	win->handle = glfwCreateWindow(640, 480, TITLE, NULL, NULL);
	if (!win->handle) {
		log_error("Error creating window\n");
		glfwTerminate();
		return -1;
	}

	// go fullscreen now
	goFullscreen(win);

	glfwSetWindowSizeLimits(win->handle, WINDOWED_WIDTH, WINDOWED_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(win->handle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		log_error("Error initialising GLAD\n");
		window_destroy(win);
		return -1;
	}

	log_log("GLFW version: %s\n", glfwGetVersionString());
	log_log("OpenGL version: %s\n", glGetString(GL_VERSION));

	glfwGetFramebufferSize(win->handle, &win->width, &win->height);
	glViewport(0, 0, win->width, win->height);

	// set GLFWwindow user pointer to the Window struct. This helps in callbacks
	glfwSetWindowUserPointer(win->handle, (void *)win);

	// initialise mouse
	glfwGetCursorPos(win->handle, &win->mouse.xpos, &win->mouse.ypos);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(win->handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetInputMode(win->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// controller setup
	const char CUSTOM_MAPPINGS[] = "050000005e040000e002000003090000,Xbox One Wireless Controller,a:b0,b:b1,back:b6,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,leftshoulder:b4,leftstick:b8,lefttrigger:a2,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b9,righttrigger:a5,rightx:a3,righty:a4,start:b7,x:b2,y:b3,platform:Linux,";


	if (glfwUpdateGamepadMappings(CUSTOM_MAPPINGS) == GLFW_FALSE) {
		// it's actually impossible for the function to return GLFW_FALSE
		log_log("Error parsing mapping\n");
	}

	win->gamepad.active = false;
	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {

		if (glfwJoystickPresent(i))
			log_log("joystick %d: %s %s\n", i, glfwGetJoystickName(i), glfwGetJoystickGUID(i));

		if (glfwJoystickIsGamepad(i)) {
			win->gamepad.jid = i;
			win->gamepad.name = glfwGetGamepadName(i);
			win->gamepad.active = true;

			log_log("Connnected controller %d: %s\n", i, win->gamepad.name);

			break;
		}
	}

	ASSERT(window_prepare_input(win))
	memset(win->keyboard.keys, 0, sizeof(bool) * GLFW_KEY_LAST);
	memset(win->mouse.buttons, 0, sizeof(bool) * GLFW_MOUSE_BUTTON_LAST);

	/* EVENT CALLBACKS */
	// screen resolution change
	glfwSetFramebufferSizeCallback(win->handle, cb_framebuffer_size);
	// mouse & keyboard
	glfwSetCursorPosCallback(win->handle, cb_cursor_pos);
	glfwSetMouseButtonCallback(win->handle, cb_mouse_button);
	glfwSetScrollCallback(win->handle, cb_scroll);
	glfwSetKeyCallback(win->handle, cb_key);
	glfwSetJoystickCallback(cb_joystick);

	glfwSwapInterval(0);
	//glfwSwapInterval(1);

	return 0;

}

int window_prepare_input(struct Window *win)
{
	// reset deltas
	win->mouse.xdelta = 0.0;
	win->mouse.ydelta = 0.0;
	win->mouse.xscroll = 0.0;
	win->mouse.yscroll = 0.0;
	memset(win->keyboard.deltas, BUTTON_SAME, sizeof(enum ButtonDelta) * GLFW_KEY_LAST);
	memset(win->mouse.deltas, BUTTON_SAME, sizeof(enum ButtonDelta) * GLFW_MOUSE_BUTTON_LAST);

	// calling a gamepad function to trigger a joystick callback if a controller has been disconnected
	glfwJoystickPresent(GLFW_JOYSTICK_1); // function doesn't actually matter

	// get gamepad state
	if (win->gamepad.active == true) {
		bool oldButtons[GLFW_GAMEPAD_BUTTON_LAST+1];
		memcpy(oldButtons, win->gamepad.state.buttons, sizeof(bool) * (GLFW_GAMEPAD_BUTTON_LAST+1));

		if (glfwGetGamepadState(win->gamepad.jid, &win->gamepad.state) == GLFW_FALSE) {
			log_log("Error getting gamepad state for '%s'\n", win->gamepad.name);
			window_destroy(win);
			return -1;
		}
		// set/clear gamepad deltas
		for (int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
			if (win->gamepad.state.buttons[i] == oldButtons[i])
				win->gamepad.deltas[i] = BUTTON_SAME;
			else if (win->gamepad.state.buttons[i] == true)
				win->gamepad.deltas[i] = BUTTON_PRESSED;
			else if (win->gamepad.state.buttons[i] == false)
				win->gamepad.deltas[i] = BUTTON_SAME;
		}
	}

	return 0;
}

void window_destroy(struct Window *win)
{
	glfwDestroyWindow(win->handle);
	glfwTerminate();

}
