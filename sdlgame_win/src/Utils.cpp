#include <cmath>
#include "Utils.h"

Vector2D Utils::directionBetween(const Vector2D& p1, const Vector2D& p2)
{
	auto vectDistance = p2 - p1;
	auto x = vectDistance.x;
	auto y = vectDistance.y;
	auto vectLength = sqrt(x * x + y * y);
	// TODO doesn't seem to be returning correct result sometimes.
	return vectDistance / vectLength;
}