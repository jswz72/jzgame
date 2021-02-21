#pragma once

#include "../Game.h"
#include "ECS.h"
#include "TransformComponent.h"

class MouseController : public Component {
public:
	void update() override {
		auto lmb = Game::mouseButtonHandler.mouseButtonData(SDL_BUTTON_LEFT);
		if (lmb->state) {
			std::cout << "LMB Pressed at " << (Game::camera.x + lmb->pos.x) << "," << (Game::camera.y + lmb->pos.y) << std::endl;
		}
	}
private:
};
