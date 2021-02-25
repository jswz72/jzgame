#pragma once

#include <iostream>
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component {
public:
    Entity* source;

    ProjectileComponent(int rng, int spd, Vector2D vel, Entity* src) : range(rng),
        speed(spd), velocity(vel), source(src) { }
    ~ProjectileComponent() {};

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
    }

    void update() override {
        distance += speed * Game::timeDelta;
        if (distance > range) {
            std::cout << "Out of range" << std::endl;
            entity->destroy();
        }
        // This means that when it goes out of view it gets destroyed?
        // Probably don't want that.
        //else if (transform->position.x > Game::camera.x + Game::camera.w ||
        //    transform->position.x < Game::camera.x ||
        //    transform->position.y > Game::camera.y + Game::camera.h ||
        //    transform->position.y < Game::camera.y) {
        //    // Outside of game view...
        //    std::cout << "Out of bounds" << std::endl;
        //    entity->destroy();
        //}
    }
private:
    TransformComponent* transform;
    int range = 0;
    int speed = 0;
    int distance = 0;
    Vector2D velocity;
};