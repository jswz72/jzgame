#pragma once

#include "Components.h"
#include "ECS.h"

class PlayerKeyboardController : public Component {
public:
	PlayerKeyboardController(TransformComponent* transformC, SpriteComponent* spriteC)
		: transform(transformC), sprite(spriteC) {}
	void update() override;
private:
	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;
	bool shiftActive = false;
	int shiftSpeedMult = 2;
};