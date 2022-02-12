#pragma once
#include <SDL.h>
#include "Rect.h"

class ColliderComponent;

class Collision {
public:
	// Axis-aligned bounding-box
	static bool AABB(const Rect& a, const Rect& b);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};