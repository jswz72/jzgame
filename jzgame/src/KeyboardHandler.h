#pragma once
#include <SDL.h>
#include <unordered_map>

class KeyboardHandler {
public:
	void handleKeyboardEvent(SDL_KeyboardEvent event);
	bool isPressed(SDL_Keycode keycode) const;
	bool isReleased(SDL_Keycode keycode) const;
private:
	enum KeyState {
		RELEASED,
		PRESSED,
	};
	std::unordered_map<SDL_Keycode, KeyState> keyState;
};
