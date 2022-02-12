#include "Collision.h"
#include "Components/ColliderComponent.h"
#include <iostream>

bool oneWayAABB(const Rect& a, const Rect& b) {
	return (a.x + a.w > b.x && b.x + b.w > a.x &&
		a.y + a.h > b.y && b.y + b.h > a.y);

}

bool Collision::AABB(const Rect& a, const Rect& b) {
	return oneWayAABB(a, b) || oneWayAABB(b, a);
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	return false;
}