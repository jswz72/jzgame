#pragma once

#include <cmath>
#include <iostream>
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component {
public:
    Entity* source = nullptr;

    ProjectileComponent(int rng, Vector2D vel, Entity* src) :
        range(rng), velocity(vel), source(src) { }

    Vector2D getPos();

    void update() override;
    void init() override;

private:
	struct InitPos {
        Vector2D pos{ 0, 0 };
        bool initialized = false;
    };
	TransformComponent* transform = nullptr;
    ColliderComponent* collider = nullptr;
    int range = 0;
    int distance = 0;
    Vector2D velocity{ 0, 0 };
    InitPos initialPos;
};