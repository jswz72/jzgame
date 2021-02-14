#include "Collision.h"
#include "ECS/ColliderComponent.h"
#include <iostream>

bool Collision::AABB(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x + a.w >= b.x && b.x + b.w >= a.x &&
        a.y + a.h >= b.y && b.y + b.h >= a.y);
}


bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
    if (AABB(colA.collider, colB.collider)) {
        // DEBUG:
        // std::cout << colA.tag << " hit " << colB.tag << std::endl;
        return true;
    }
    return false;
}