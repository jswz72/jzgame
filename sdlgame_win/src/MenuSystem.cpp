#include "MenuSystem.h"

#include <cassert>
#include "kiss_sdl.h"
#include "Globals.h"

MenuSystem::MenuSystem(int windowWidth, int windowHeight, SDL_Window* window, SDL_Renderer* renderer)
	: window(window), renderer(renderer), windowWidth(windowWidth), windowHeight(windowHeight){
	auto kissRenderer = kiss_init("", &arr, windowWidth, windowHeight, window, renderer);
	assert(kissRenderer);
	initPauseMenu();
	initDebugMenu();
}

void MenuSystem::initPauseMenu() {
	const auto padding = 20;
	kiss_window_new(&pauseWindow, nullptr, 1, padding, padding, windowWidth - (padding * 2),
		windowHeight - (padding * 2));
	pauseWindow.visible = true;
	const auto debugX = pauseWindow.rect.w / 2;
	const auto debugY = pauseWindow.rect.h / 2;
	kiss_button_new(&debugButton, &pauseWindow, "Debug", debugX, debugY);
	auto exitX = debugX;
	auto exitY = pauseWindow.rect.h * 3/4;
	kiss_button_new(&exitButton, &pauseWindow, "Exit", exitX, exitY);
}

void MenuSystem::initDebugMenu() {
	const auto padding = 20;
	kiss_window_new(&debugWindow, nullptr, 1, padding, padding, windowWidth - (padding * 2),
		windowHeight - (padding * 2));
	debugWindow.visible = true;
	const auto labelX = debugWindow.rect.w / 2;
	const auto labelY = debugWindow.rect.h / 2;
	kiss_label_new(&debugLabel, &debugWindow, "Debug:", labelX, labelY);
	auto selectButtonX = labelX + debugLabel.rect.w;
	auto selectButtonY = labelY;
	kiss_selectbutton_new(&debugCheckbox, &debugWindow, selectButtonX, selectButtonY);
	debugCheckbox.selected = Globals::get().debug;
	auto backX = labelX;
	auto backY = debugWindow.rect.h * 3/4;
	kiss_button_new(&debugBackButton, &debugWindow, "Debug", backX, backY);
}

void MenuSystem::handlePauseMenuEvents(SDL_Event* event) {
	kiss_window_event(&pauseWindow, event, &kissDraw);
	if (kiss_button_event(&debugButton, event, &kissDraw)) {
		screenStack.push(MenuScreen::DebugMenu);
	}
	if (kiss_button_event(&exitButton, event, &kissDraw)) {
		Globals::get().isRunning = false;
	}
}

void MenuSystem::handleDebugMenuEvents(SDL_Event* event) {
	kiss_window_event(&debugWindow, event, &kissDraw);
	if (kiss_selectbutton_event(&debugCheckbox, event, &kissDraw)) {
		Globals::get().debug = debugCheckbox.selected;
	}
	if (kiss_button_event(&debugBackButton, event, &kissDraw)) {
		screenStack.pop();
	}
}

void MenuSystem::handleEvents(SDL_Event* event) {
	if (Globals::get().isPaused) {
		if (!lastPauseState) {
			screenStack.push(MenuScreen::PauseMenu);
		}
		lastPauseState = true;
		if (screenStack.top() == MenuScreen::PauseMenu) {
			handlePauseMenuEvents(event);
		} else if (screenStack.top() == MenuScreen::DebugMenu) {
			handleDebugMenuEvents(event);
		}
	} else {
		while (!screenStack.empty()) {
			screenStack.pop();
		}
		lastPauseState = false;
	}
}

void MenuSystem::drawPauseWindow() {
	auto& renderer = Globals::get().renderer;
	kiss_window_draw(&pauseWindow, renderer);
	kiss_button_draw(&debugButton, renderer);
	kiss_button_draw(&exitButton, renderer);
}

void MenuSystem::drawDebugWindow() {
	auto& renderer = Globals::get().renderer;
	kiss_window_draw(&debugWindow, renderer);
	kiss_label_draw(&debugLabel, renderer);
	kiss_selectbutton_draw(&debugCheckbox, renderer);
	kiss_button_draw(&debugBackButton, renderer);
}

void MenuSystem::draw() {
	if (screenStack.empty()) {
		return;
	}
	if (screenStack.top() == MenuScreen::PauseMenu) {
		drawPauseWindow();
	}
	else if (screenStack.top() == MenuScreen::DebugMenu) {
		drawDebugWindow();
	}
}
