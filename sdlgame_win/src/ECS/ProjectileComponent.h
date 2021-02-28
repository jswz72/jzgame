#pragma once

#include <cmath>
#include <iostream>
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component {
public:
    Entity* source;

    ProjectileComponent(int rng, Vector2D vel, Entity* src) : range(rng),
        velocity(vel), source(src) { }
    ~ProjectileComponent() {};

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        collider = &entity->getComponent<ColliderComponent>();
        transform->velocity = velocity;
        initialPos = { static_cast<float>(collider->collider.x),
            static_cast<float>(collider->collider.y) };
    }

    void update() override {
        Vector2D curColliderPos{ static_cast<float>(collider->collider.x),
            static_cast<float>(collider->collider.y) };
        auto posDiff = curColliderPos - initialPos;
        auto x = posDiff.x;
        auto y = posDiff.y;
        distance = sqrt(x * x + y * y);
        if (distance > range) {
            std::cout << "Out of range" << std::endl;
            entity->destroy();
        }
    }
private:
    TransformComponent* transform;
    ColliderComponent* collider;
    int range = 0;
    int distance = 0;
    Vector2D velocity;
    Vector2D initialPos;
};