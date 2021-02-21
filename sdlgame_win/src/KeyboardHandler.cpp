#include "KeyboardHandler.h"

void KeyboardHandler::handleKeyboardEvent(SDL_KeyboardEvent event) {
	SDL_Keycode keycode = event.keysym.sym;
	if (event.type == SDL_KEYUP) {
		keyState[keycode] = RELEASED;
	} else if (event.type == SDL_KEYDOWN) {
		keyState[keycode] = PRESSED;
	}
}

bool KeyboardHandler::isPressed(SDL_Keycode keycode) {
	if (keyState.find(keycode) != keyState.end()) {
		return keyState[keycode];
	}
	return false;
}

bool KeyboardHandler::isReleased(SDL_Keycode keycode) {
	if (keyState.find(keycode) != keyState.end()) {
		return !keyState[keycode];
	}
	return false;
}
