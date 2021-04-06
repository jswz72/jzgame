#pragma once
#include "SDL.h"
#include "kiss_sdl.h"

class MenuSystem {
public:
	MenuSystem(int windowWidth, int windowHeight, SDL_Window* window, SDL_Renderer* renderer);
	void handleEvents(SDL_Event* event);
	void draw();
private:
	void initPauseMenu(int windowWidth, int windowHeight);
	void handlePauseEvents(SDL_Event* event);
	void drawPauseWindow();

	SDL_Window* window;
	SDL_Renderer* renderer;
	kiss_window pauseWindow;
	kiss_selectbutton debugCheckbox;
	kiss_label debugLabel;
	kiss_button exitButton;
	kiss_array arr;
	int d = 1;
};