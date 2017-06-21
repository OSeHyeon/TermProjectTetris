#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <sys/types.h>
#include "myio.h"
#include "Block.h"
#include "GameLogic.h"

struct termios initial_settings, new_settings;

int main(int argc, char *argv[]) {
	srand(time(NULL));

	tcgetattr(0, &initial_settings);

	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;
	new_settings.c_cc[VMIN] = 0;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
	GameLogic game = GameLogic();

	system("clear");
	//	sendScore(100);
	while (game.isRun) {
		game.start();
	}

	tcsetattr(0, TCSANOW, &initial_settings);
	printf("\033[0m");
	system("clear");
	return 0;
}
