#pragma once

#include "../Game.h"
#include "Components.h"
#include "ECS.h"

class KeyboardController : public Component {
public:
	void init() override;
	void update() override;
private:
	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;
};