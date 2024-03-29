#include "HealthComponent.h"
#include "../Globals.h"
#include "../RGBvals.h"
#include "../Utils.h"

unsigned int HealthComponent::healthSub(unsigned int delta) {
	if (delta > health) {
		health = 0;
	}
	else {
		health -= delta;
	}
	return health;
}

unsigned int HealthComponent::healthInc(unsigned int delta) {
	health = std::min(maxHealth, health + delta);
	return health;
}

void HealthComponent::update()
{
	if (health == 0) {
		entity->destroy();
	}
}

void HealthComponent::draw() {
	if (!transform) {
		return;
	}
	auto pos = Globals::get().cameraRelative(transform->getPosition());
	if (pos.x < 0 || pos.y < 0) {
		return;
	}
	auto x = static_cast<int>(pos.x + (transform->getWidth() / 2) - (healthBarWidth / 2));
	auto y = static_cast<int>(pos.y - yOffset - healthBarHeight);
	SDL_Rect maxHealthRect = { x, y, healthBarWidth, healthBarHeight };

	Utils::setRenderDrawColor(RGBVals::gray());
	SDL_RenderFillRect(Globals::get().renderer, &maxHealthRect);
	float curHealthPct = static_cast<float>(health) / static_cast<float>(maxHealth);
	SDL_Rect curHealthRect = { x, y, static_cast<int>(healthBarWidth * curHealthPct), healthBarHeight };
	Utils::setRenderDrawColor(RGBVals::red());
	SDL_RenderFillRect(Globals::get().renderer, &curHealthRect);
}
