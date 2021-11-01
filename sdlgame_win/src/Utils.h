#pragma once
#include <optional>
#include "Game.h"
#include "RGBVals.h"
#include "Vector2D.h"

class Utils {
public:
	static Vector2D directionBetween(const Vector2D& p1, const Vector2D& p2);
	static void setRenderDrawColor(const RGBVals color,
		uint8_t opacity = SDL_ALPHA_OPAQUE, SDL_Renderer* renderer = Game::renderer);
	static void drawRect(SDL_Rect* rect,
		const std::optional<RGBVals>& color = std::nullopt,
		uint8_t opacity = SDL_ALPHA_OPAQUE, SDL_Renderer* renderer = Game::renderer);
};