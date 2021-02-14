#pragma once
#include <SDL.h>

class ColliderComponent;

class Collision {
public:
	// Axis-aligned bounding-box
	static bool AABB(const SDL_Rect& a, const SDL_Rect& b);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};