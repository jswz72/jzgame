#pragma once

#include <cmath>
#include <iostream>
#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component {
public:
	Entity* source = nullptr;
	int damage = 0;

	ProjectileComponent(TransformComponent& transformC, int rng, Vector2D<> vel, int damage, Entity* src,
		ColliderComponent* colliderC = nullptr)
		: transform(transformC), range(rng), velocity(vel), source(src), damage(damage), collider(colliderC) { }

	void update() override;
	void init() override;

private:
	Vector2D<> initialPos{ 0, 0 };
	TransformComponent& transform;
	ColliderComponent* collider = nullptr;
	int range = 0;
	int distance = 0;
	Vector2D<> velocity{ 0, 0 };
};