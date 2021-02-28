#pragma once

#include "../Game.h"
#include "../Utils.h"
#include "../Vector2D.h"
#include "ECS.h"
#include "ColliderComponent.h"

class MouseController : public Component {
public:
	void update() override {
		auto lmb = Game::mouseButtonHandler.mouseButtonData(SDL_BUTTON_LEFT);
		if (lmb->state) {
			auto collider = &entity->getComponent<ColliderComponent>().collider;
			Vector2D colliderPos{ static_cast<float>(collider->x), static_cast<float>(collider->y) };
			Vector2D mousePos{ Game::camera.x + lmb->pos.x, Game::camera.y + lmb->pos.y };
			Vector2D direction = Utils::directionBetween(colliderPos, mousePos);
			Game::assets->createProjectile(colliderPos, direction, 1000, 1, "projectile", entity);
			//std::cout << "LMB Pressed at " << (Game::camera.x + lmb->pos.x) << "," << (Game::camera.y + lmb->pos.y) << std::endl;
		}
	}
private:
};
