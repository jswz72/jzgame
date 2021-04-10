#include "ProjectileComponent.h"

Vector2D ProjectileComponent::getPos() {
	collider = &entity->getComponent<ColliderComponent>();
	return { static_cast<float>(collider->collider.x), static_cast<float>(collider->collider.y) };
}

void ProjectileComponent::update() {
	if (!initialPos.initialized) {
		assert(entity->hasComponent<ColliderComponent>());
		initialPos.pos = getPos();
		initialPos.initialized = true;
		return;
	}
	transform.velocity = velocity;
	// TODO why collider pos and not transform?
	Vector2D curColliderPos{ static_cast<float>(collider->collider.x),
		static_cast<float>(collider->collider.y) };
	auto posDiff = curColliderPos - initialPos.pos;
	auto x = posDiff.x;
	auto y = posDiff.y;
	distance = static_cast<int>(sqrt(x * x + y * y));
	if (distance > range) {
		std::cout << "Out of range" << std::endl;
		entity->destroy();
	}
}

void ProjectileComponent::init() {
	transform.velocity = velocity;
	if (collider) {
		initialPos.pos = getPos();
		initialPos.initialized = true;
	}
}
