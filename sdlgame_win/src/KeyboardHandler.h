#pragma once
#include <SDL.h>
#include <unordered_map>

class KeyboardHandler {
public:
	KeyboardHandler() {}
	~KeyboardHandler() {}

	void handleKeyboardEvent(SDL_Event event);
	bool isPressed(SDL_Keycode);
	bool isReleased(SDL_Keycode);
private:
	enum KEYSTATE {
		RELEASED,
		PRESSED,
	};
	std::unordered_map<SDL_Keycode, KEYSTATE> keyState;
};
