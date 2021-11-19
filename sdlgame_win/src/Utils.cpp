#include <cmath>
#include "Utils.h"

float Utils::distance(const Vector2D& p1, const Vector2D& p2) {
	auto diff = p2 - p1;
	auto x = diff.x;
	auto y = diff.y;
	return sqrt(x * x + y * y);
}

Vector2D Utils::directionBetween(const Vector2D& p1, const Vector2D& p2)
{
	return (p2 - p1) / distance(p1, p2);
}

void Utils::setRenderDrawColor(const RGBVals color, uint8_t opacity, SDL_Renderer* renderer)
{
	// TODO add error handling.
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, opacity);
}

void Utils::drawRect(SDL_Rect* rect, const std::optional<RGBVals>& color, uint8_t opacity, SDL_Renderer* renderer)
{
	if (color) {
		setRenderDrawColor(*color, opacity, renderer);
	}
	SDL_RenderDrawRect(renderer, rect);
}
