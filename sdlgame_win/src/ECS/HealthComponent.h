#pragma once
#include <algorithm>
#include "ECS.h"

class HealthComponent : public Component {
public:
	HealthComponent(unsigned int maxHealth) : maxHealth(maxHealth) {
		setFullHealth();
	}
	void setFullHealth() {
		health = maxHealth;
	}
	unsigned int healthSub(unsigned int delta) {
		if (delta > health) {
			health = 0;
		}
		else {
			health -= delta;
		}
		return health;
	}
	unsigned int healthInc(unsigned int delta) {
		health = std::min(maxHealth, health + delta);
		return health;
	}
	unsigned int getHealth() {
		return health;
	}
	void update() override;
private:
	unsigned int maxHealth = 0;
	unsigned int health = 0;
};
