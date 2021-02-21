#pragma once
#include <cstdint>
#include <limits>
#include <SDL.h>
#include <unordered_map>
#include "Vector2D.h"

struct MouseButtonData {
	enum ButtonState {
		RELEASED,
		PRESSED,
	};
	ButtonState state = RELEASED;
	Vector2D pos;
};

class MouseButtonHandler {
public:
	MouseButtonHandler();
	~MouseButtonHandler() {}

	void handleMouseButtonEvent(SDL_MouseButtonEvent event);
	MouseButtonData* mouseButtonData(uint8_t button);
private:
	MouseButtonData* buttonState[std::numeric_limits<uint8_t>::max()];
};