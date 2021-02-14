
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "Game.h"

const bool FULLSCREEN = false;

int main(int argc, const char* argv[]) {
	const int FPS = 60;
	// Max time between frames
	const int frameDelay = 1000 / FPS;
	unsigned int frameStart;
	int frameTime;

	Game* game = new Game();
	game->init("TestGame", 800, 640, FULLSCREEN);

	while (game->running()) {
		// How many ms since start sdl.
		frameStart = SDL_GetTicks();
		game->handleEvents();
		game->update();
		game->render();
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	return 0;
}