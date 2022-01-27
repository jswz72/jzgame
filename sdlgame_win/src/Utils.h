#pragma once
#include <cmath>
#include <optional>
#include "RGBVals.h"
#include "Vector2D.h"
#include "Globals.h"

class Utils {
public:
	template <typename T>
	static T distance(const Vector2D<T>& p1, const Vector2D<T>& p2) {
		auto diff = p2 - p1;
		auto x = diff.x;
		auto y = diff.y;
		return sqrt(x * x + y * y);
	}
	static Vector2D<> directionBetween(const Vector2D<>& p1, const Vector2D<>& p2);
	static void setRenderDrawColor(const RGBVals color,
		uint8_t opacity = SDL_ALPHA_OPAQUE, SDL_Renderer* renderer = Globals::get().renderer);
	static void drawRect(SDL_Rect* rect,
		const std::optional<RGBVals>& color = std::nullopt,
		uint8_t opacity = SDL_ALPHA_OPAQUE, SDL_Renderer* renderer = Globals::get().renderer);
};