#pragma once
#include "SDL.h"
#include <stack>
#include "kiss_sdl.h"

class MenuSystem {
public:
	MenuSystem(int windowWidth, int windowHeight, SDL_Window* window, SDL_Renderer* renderer);
	void handleEvents(SDL_Event* event);
	void draw();
private:
	void initPauseMenu();
	void initDebugMenu();
	void handlePauseMenuEvents(SDL_Event* event);
	void handleDebugMenuEvents(SDL_Event* event);
	void drawPauseWindow();
	void drawDebugWindow();

	int windowWidth;
	int windowHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;

	kiss_window pauseWindow;
	kiss_button debugButton;
	kiss_button resumeButton;
	kiss_button exitButton;

	kiss_window debugWindow;
	kiss_label debugTransformLabel;
	kiss_selectbutton debugTransformCheckbox;
	kiss_label debugColliderLabel;
	kiss_selectbutton debugColliderCheckbox;
	kiss_label debugQuadTreeLabel;
	kiss_selectbutton debugQuadTreeCheckbox;
	kiss_label debugPathfindingLabel;
	kiss_selectbutton debugPathfindingCheckbox;
	kiss_label debugNavMapLabel;
	kiss_selectbutton debugNavMapCheckbox;
	kiss_label debugLabelsLabel;
	kiss_selectbutton debugLabelsCheckbox;
	kiss_label debugInvincibilityLabel;
	kiss_selectbutton debugInvincibilityCheckbox;

	kiss_button debugBackButton;

	kiss_array arr;
	int kissDraw = 1;
	bool lastPauseState = false;
	enum class MenuScreen {
		PauseMenu,
		DebugMenu
	};
	std::stack<MenuScreen> screenStack;
};