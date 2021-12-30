#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/stat.h>

#include "logger.h"
#include "util.h"
#include "game.h"

const char *LOG_NAME = ".bmod.log";

const char *RES_DIR1 = "res";
const char *RES_DIR2 = "/usr/local/share/bmod";
const char *RES_DIR3 = "/usr/share/bmod";

int main(int argc, char *argv[])
{

	char log_path[FILENAME_MAX] = { 0 };
	snprintf(log_path, FILENAME_MAX-1, "%s/%s", getenv("HOME"), LOG_NAME);
	log_init(log_path);
//	log_init("/root/log");

	// check if res/ exists in current working dir
	struct stat sb;
	if(stat(RES_DIR1, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		// use res/
		// already in correct dir
	} else if (stat(RES_DIR2, &sb) == 0 && S_ISDIR(sb.st_mode)) {
		// use /usr/local/share/bmod
		chdir(RES_DIR2);
	} else {
		// use /usr/share/bmod
		chdir(RES_DIR3);
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
