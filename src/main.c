#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>

#include "logger.h"
#include "util.h"
#include "game.h"

const char *LOG_NAME = ".bmod.log";

const char *INSTALL_RESOURCE_DIR = "/usr/local/share/bmod";
const char *DEBUG_RESOURCE_DIR = "res";

int main(int argc, char *argv[])
{

	// intialise logger
	char log_path[FILENAME_MAX] = { 0 };
	snprintf(log_path, FILENAME_MAX-1, "%s/%s", getenv("HOME"), LOG_NAME);
	log_init(log_path);

	// check if res/ exists
	struct stat sb;
	if( stat(DEBUG_RESOURCE_DIR, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		// use res/
		chdir(DEBUG_RESOURCE_DIR);
	} else {
		// use /usr/local/share/bmod
		chdir(INSTALL_RESOURCE_DIR);
	}

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
