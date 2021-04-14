#include "HealthComponent.h"

void HealthComponent::update()
{
	if (health == 0) {
		entity->destroy();
	}
}
