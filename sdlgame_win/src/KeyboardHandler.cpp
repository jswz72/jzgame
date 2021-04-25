#include "KeyboardHandler.h"
#include "SDL.h"
#include "Game.h"

void KeyboardHandler::handleKeyboardEvent(SDL_KeyboardEvent event) {
	SDL_Keycode keycode = event.keysym.sym;
	if (event.type == SDL_KEYUP) {
		keyState[keycode] = RELEASED;
	}
	else if (event.type == SDL_KEYDOWN) {
		// TODO better way to handle this.
		if (keycode == SDLK_ESCAPE && keyState[keycode] == RELEASED) {
			Game::isPaused = !Game::isPaused;
		}
		keyState[keycode] = PRESSED;
	}
}

bool KeyboardHandler::isPressed(SDL_Keycode keycode) const {
	if (keyState.find(keycode) != keyState.end()) {
		return keyState.at(keycode);
	}
	return false;
}

bool KeyboardHandler::isReleased(SDL_Keycode keycode) const {
	if (keyState.find(keycode) != keyState.end()) {
		return !keyState.at(keycode);
	}
	return false;
}