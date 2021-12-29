#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "util.h"
#include "game.h"

const char *LOG_PATH = "log.txt";

int main(int argc, char *argv[])
{

	log_init(LOG_PATH);

	struct Game g;

	if (game_create(&g) != 0)
	{
		log_log("Error in 'game_create'\n");
		log_destroy();
		return EXIT_FAILURE;
	}

	if (game_loop(&g) != 0) {
		log_log("Error in 'game_loop'\n");
		log_destroy();
		return EXIT_FAILURE;
	}

	game_destroy(&g);

	log_destroy();
	return EXIT_SUCCESS;

}
