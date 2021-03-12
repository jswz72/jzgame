#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "Game.h"

const bool FULLSCREEN = false;

int main(int argc, const char* argv[]) {
	const int FPS = 60;
	// Max time between frames
	const int frameDelay = 1000 / FPS;
	unsigned int frameStart;
	unsigned int frameTime;
	unsigned int fpsLastTime = SDL_GetTicks();
	unsigned int fpsFrames = 0;

	const int windowWidth = 800;
	const int windowHeight = 640;
	Game::setCameraSize(windowWidth, windowHeight);
	Game* game = new Game(windowWidth, windowHeight);
	game->init("TestGame", FULLSCREEN);

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
		fpsFrames++;
		if (SDL_GetTicks() - fpsLastTime > 1000) {
			fpsLastTime = SDL_GetTicks();
			game->setFpsString(fpsFrames);
			fpsFrames = 0;
		}
	}
	game->clean();
	return 0;
}