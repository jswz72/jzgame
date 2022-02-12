#include "Utils.h"

Vector2D<> Utils::directionBetween(const Vector2D<>& p1, const Vector2D<>& p2)
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

bool Utils::containedIn(Vector2D<int> point, SDL_Rect rect)
{
	const auto xMin = rect.x;
	const auto yMin = rect.y;
	const auto xMax = rect.x + rect.w;
	const auto yMax = rect.y + rect.h;
	return point.x >= xMin && point.x <= xMax && point.y >= yMin && point.y <= yMax;
}
