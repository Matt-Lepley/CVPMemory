#include "Game.h"

int main(int argc, char* argv[]) {

	Game game;

	game.init();
	game.loop();
	game.clean();

	return 0;
}