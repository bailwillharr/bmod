#ifndef GAME_H
#define GAME_H

#include "window.h"
#include "render/renderer.h"
#include "input.h"

struct Game {
	struct Window win;
	struct Renderer ren;
	struct Input input;
};

int game_create(struct Game * g);
int game_loop(struct Game * g);
void game_destroy(struct Game * g);

#endif
