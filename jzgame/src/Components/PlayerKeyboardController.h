#pragma once

#include "Components.h"
#include "../EntityComponent.h"

class PlayerKeyboardController : public Component {
public:
	PlayerKeyboardController(TransformComponent& transformC, SpriteComponent& spriteC)
		: transform(transformC), sprite(spriteC) {}
	void update() override;
private:
	TransformComponent& transform;
	SpriteComponent& sprite;
	bool shiftActive = false;
	int shiftSpeedMult = 2;
};