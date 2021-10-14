#include "Collision.h"
#include "ECS/ColliderComponent.h"
#include <iostream>

bool oneWayAABB(const SDL_Rect& a, const SDL_Rect& b) {
	return (a.x + a.w > b.x && b.x + b.w > a.x &&
		a.y + a.h > b.y && b.y + b.h > a.y);

}

bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b) {
	return oneWayAABB(a, b) || oneWayAABB(b, a);
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	return false;
}