#include "game.h"

struct Mapping {
	enum InputButtons out;
	int in;
};

void game_input_handleKeyboard(struct Game * g)
{

	const struct Mapping map[] = {
		{INPUT_BUTTON_FULLSCREEN, GLFW_KEY_F11},
		{INPUT_BUTTON_QUIT, GLFW_KEY_ESCAPE},
		{INPUT_BUTTON_JUMP, GLFW_KEY_SPACE},
		{INPUT_BUTTON_CROUCH, GLFW_KEY_LEFT_SHIFT},
	};

	for (int i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
		g->input.buttons[map[i].out] = g->win.keyboard.keys[map[i].in];
		g->input.deltas[map[i].out] = g->win.keyboard.deltas[map[i].in];
	}

	g->input.lookAxisDX = g->win.mouse.xdelta;
	g->input.lookAxisDY = g->win.mouse.ydelta;
	g->input.moveDX = 0.0f;
	g->input.moveDX += g->win.keyboard.keys[GLFW_KEY_D] ? 0.1f : 0.0f;
	g->input.moveDX -= g->win.keyboard.keys[GLFW_KEY_A] ? 0.1f : 0.0f;
	g->input.moveDY = 0.0f;
	g->input.moveDY += g->win.keyboard.keys[GLFW_KEY_S] ? 0.1f : 0.0f;
	g->input.moveDY -= g->win.keyboard.keys[GLFW_KEY_W] ? 0.1f : 0.0f;
}

void game_input_handleGamepad(struct Game * g)
{

	const struct Mapping map[] = {
		{INPUT_BUTTON_FULLSCREEN, GLFW_GAMEPAD_BUTTON_START},
		{INPUT_BUTTON_QUIT, GLFW_GAMEPAD_BUTTON_BACK},
		{INPUT_BUTTON_JUMP, GLFW_GAMEPAD_BUTTON_A},
		{INPUT_BUTTON_CROUCH, GLFW_GAMEPAD_BUTTON_B},
	};

	for (int i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
		g->input.buttons[map[i].out] = g->win.gamepad.state.buttons[map[i].in];
		g->input.deltas[map[i].out] = g->win.gamepad.deltas[map[i].in];
	}

	const float SENS = 5.0f;
	const float MOVE_SPEED = 0.2f;

	g->input.lookAxisDX = g->win.gamepad.state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] * SENS;
	g->input.lookAxisDY = g->win.gamepad.state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] * SENS;
	g->input.moveDX = g->win.gamepad.state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] * MOVE_SPEED;
	g->input.moveDY = g->win.gamepad.state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] * MOVE_SPEED;

}
