#include "ProjectileComponent.h"

void ProjectileComponent::update() {
	transform.velocity = velocity;
	auto posDiff = transform.getPosition() - initialPos;
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
	initialPos = transform.getPosition();
}
