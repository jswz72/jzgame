#include "KeyboardHandler.h"

void KeyboardHandler::handleKeyboardEvent(SDL_Event event) {
	SDL_Keycode keycode = event.key.keysym.sym;
	if (event.type == SDL_KEYUP) {
		keyState[event.key.keysym.sym] = RELEASED;
	} else if (event.type == SDL_KEYDOWN) {
		keyState[event.key.keysym.sym] = PRESSED;
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
