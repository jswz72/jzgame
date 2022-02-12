#pragma once
#include <SDL.h>

class Rect {
public:
	Rect() {}
	Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	Rect(const SDL_Rect& sdlRect) {
		x = static_cast<float>(sdlRect.x);
		y = static_cast<float>(sdlRect.y);
		w = static_cast<float>(sdlRect.w);
		h = static_cast<float>(sdlRect.h);
	}
	float x = 0;
	float y = 0;
	float w = 0;
	float h = 0;

	explicit operator SDL_Rect () const {
		return SDL_Rect{ static_cast<int>(x),
						 static_cast<int>(y),
						 static_cast<int>(w),
						 static_cast<int>(h) };
	}
};