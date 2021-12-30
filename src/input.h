#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include "types.h"

enum InputButtons {

	INPUT_BUTTON_FULLSCREEN = 0,
	INPUT_BUTTON_QUIT,

	INPUT_BUTTON_JUMP,
	INPUT_BUTTON_CROUCH,

	INPUT_BUTTON_COUNT
};

struct Input { // keyboard/controller agnostic input states

	bool buttons[INPUT_BUTTON_COUNT];
	enum ButtonDelta deltas[INPUT_BUTTON_COUNT];

	// mouse / right anolog stick
	float lookAxisDX, lookAxisDY;

	// WASD / left anolog stick
	float moveDX, moveDY;

};

#endif
