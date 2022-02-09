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

//MenuSystem::~MenuSystem() {
//	delete debugWindow;
//	delete pauseWindow;
//}

void MenuSystem::initPauseMenu() {
	const auto padding = 20;
	//pauseWindow = new kiss_window{};
	kiss_window_new(&pauseWindow, nullptr, 1, padding, padding, windowWidth - (padding * 2),
		windowHeight - (padding * 2));
	pauseWindow.visible = true;
	const auto xPos = pauseWindow.rect.w / 2 - 20;
	const auto debugY = pauseWindow.rect.h / 2;
	kiss_button_new(&debugButton, &pauseWindow, "Debug", xPos, debugY);
	auto resumeY = pauseWindow.rect.h * 3/4;
	kiss_button_new(&resumeButton, &pauseWindow, "Resume", xPos, resumeY); 
	kiss_button_new(&exitButton, &pauseWindow, "Exit", xPos, resumeY + 25);
}

void MenuSystem::initDebugMenu() {
	const auto padding = 20;

	//debugWindow = new kiss_window{};
	kiss_window_new(&debugWindow, nullptr, 1, padding, padding, windowWidth - (padding * 2),
		windowHeight - (padding * 2));
	
	debugWindow.visible = true;
	Vector2D<int> windowCenter = { debugWindow.rect.w / 2, debugWindow.rect.h / 2 };
	const auto labelX = windowCenter.x - 100;
	auto yPos = windowCenter.y - 100;
	auto offset = 25;
	const auto selectButtonX = windowCenter.x + 100;

	kiss_label_new(&debugTransformLabel, &debugWindow, "Transform:", labelX, yPos);
	kiss_selectbutton_new(&debugTransformCheckbox, &debugWindow, selectButtonX, yPos);
	debugTransformCheckbox.selected = Globals::get().debug.transform;

	yPos += offset;
	kiss_label_new(&debugColliderLabel, &debugWindow, "Collider:", labelX, yPos);
	kiss_selectbutton_new(&debugColliderCheckbox, &debugWindow, selectButtonX, yPos);
	debugColliderCheckbox.selected = Globals::get().debug.collider;

	yPos += offset;
	kiss_label_new(&debugQuadTreeLabel, &debugWindow, "QuadTree:", labelX, yPos);
	kiss_selectbutton_new(&debugQuadTreeCheckbox, &debugWindow, selectButtonX, yPos);
	debugQuadTreeCheckbox.selected = Globals::get().debug.quadtree;

	yPos += offset;
	kiss_label_new(&debugPathfindingLabel, &debugWindow, "Pathfinding:", labelX, yPos);
	kiss_selectbutton_new(&debugPathfindingCheckbox, &debugWindow, selectButtonX, yPos);
	debugPathfindingCheckbox.selected = Globals::get().debug.pathfinding;

	yPos += offset;
	kiss_label_new(&debugNavMapLabel, &debugWindow, "NavMap:", labelX, yPos);
	kiss_selectbutton_new(&debugNavMapCheckbox, &debugWindow, selectButtonX, yPos);
	debugNavMapCheckbox.selected = Globals::get().debug.navmap;

	yPos += offset;
	kiss_label_new(&debugLabelsLabel, &debugWindow, "Labels:", labelX, yPos);
	kiss_selectbutton_new(&debugLabelsCheckbox, &debugWindow, selectButtonX, yPos);
	debugLabelsCheckbox.selected = Globals::get().debug.labels;

	auto backY = debugWindow.rect.h * 3/4;
	kiss_button_new(&debugBackButton, &debugWindow, "Back", windowCenter.x, backY); 
}

void MenuSystem::handlePauseMenuEvents(SDL_Event* event) {
	kiss_window_event(&pauseWindow, event, &kissDraw);
	if (kiss_button_event(&debugButton, event, &kissDraw)) {
		screenStack.push(MenuScreen::DebugMenu);
		debugWindow.visible = true;
	}
	if (kiss_button_event(&resumeButton, event, &kissDraw)) {
		screenStack.pop();
		pauseWindow.visible = false;
		Globals::get().isPaused = false;
	}
	if (kiss_button_event(&exitButton, event, &kissDraw)) {
		Globals::get().isRunning = false;
	}
}

void MenuSystem::handleDebugMenuEvents(SDL_Event* event) {
	kiss_window_event(&debugWindow, event, &kissDraw);
	if (kiss_selectbutton_event(&debugTransformCheckbox, event, &kissDraw)) {
		Globals::get().debug.transform = debugTransformCheckbox.selected;
	}
	if (kiss_selectbutton_event(&debugColliderCheckbox, event, &kissDraw)) {
		Globals::get().debug.collider = debugColliderCheckbox.selected;
	}
	if (kiss_selectbutton_event(&debugQuadTreeCheckbox, event, &kissDraw)) {
		Globals::get().debug.quadtree = debugQuadTreeCheckbox.selected;
	}
	if (kiss_selectbutton_event(&debugPathfindingCheckbox, event, &kissDraw)) {
		Globals::get().debug.pathfinding = debugPathfindingCheckbox.selected;
	}
	if (kiss_selectbutton_event(&debugNavMapCheckbox, event, &kissDraw)) {
		Globals::get().debug.navmap = debugNavMapCheckbox.selected;
	}
	if (kiss_selectbutton_event(&debugLabelsCheckbox, event, &kissDraw)) {
		Globals::get().debug.labels = debugLabelsCheckbox.selected;
	}
	if (kiss_button_event(&debugBackButton, event, &kissDraw)) {
		screenStack.pop();
		debugWindow.visible = false;
	}
}

void MenuSystem::handleEvents(SDL_Event* event) {
	if (Globals::get().isPaused) {
		if (!lastPauseState) {
			screenStack.push(MenuScreen::PauseMenu);
			pauseWindow.visible = true;
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
			debugWindow.visible = false;
			pauseWindow.visible = false;
		}
		lastPauseState = false;
	}
}

void MenuSystem::drawPauseWindow() {
	if (!pauseWindow.visible) {
		return;
	}
	auto& renderer = Globals::get().renderer;
	kiss_window_draw(&pauseWindow, renderer);
	kiss_button_draw(&debugButton, renderer);
	kiss_button_draw(&resumeButton, renderer);
	kiss_button_draw(&exitButton, renderer);
}

void MenuSystem::drawDebugWindow() {
	if (!debugWindow.visible) {
		return;
	}
	auto& renderer = Globals::get().renderer;
	kiss_window_draw(&debugWindow, renderer);

	kiss_label_draw(&debugTransformLabel, renderer);
	kiss_selectbutton_draw(&debugTransformCheckbox, renderer);
	kiss_label_draw(&debugColliderLabel, renderer);
	kiss_selectbutton_draw(&debugColliderCheckbox, renderer);
	kiss_label_draw(&debugQuadTreeLabel, renderer);
	kiss_selectbutton_draw(&debugQuadTreeCheckbox, renderer);
	kiss_label_draw(&debugPathfindingLabel, renderer);
	kiss_selectbutton_draw(&debugPathfindingCheckbox, renderer);
	kiss_label_draw(&debugNavMapLabel, renderer);
	kiss_selectbutton_draw(&debugNavMapCheckbox, renderer);
	kiss_label_draw(&debugLabelsLabel, renderer);
	kiss_selectbutton_draw(&debugLabelsCheckbox, renderer);


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
	//kissDraw = 0;
}
