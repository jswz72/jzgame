#pragma once
#include <algorithm>
#include "ECS.h"
#include "TransformComponent.h"

class HealthComponent : public Component {
public:
	HealthComponent(unsigned int maxHealth, TransformComponent *transform)
			: maxHealth(maxHealth), transform(transform) {
		setFullHealth();
	}
	HealthComponent(unsigned int maxHealth) : HealthComponent(maxHealth, nullptr) {}
	void setFullHealth() {
		health = maxHealth;
	}
	unsigned int healthSub(unsigned int delta);
	unsigned int healthInc(unsigned int delta);
	unsigned int getHealth() {
		return health;
	}
	void update() override;
	void draw() override;
private:
	unsigned int maxHealth = 0;
	unsigned int health = 0;
	TransformComponent* transform = nullptr;
	int yOffset = 6;
	int healthBarHeight = 9;
	int healthBarWidth = 70;
};
