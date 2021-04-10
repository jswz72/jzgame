#include "PlayerMouseController.h"

#include "../Game.h"
#include "../Utils.h"
#include "../Vector2D.h"
#include "Components.h"

void PlayerMouseController::update() {
	auto lmb = Game::mouseButtonHandler.mouseButtonData(SDL_BUTTON_LEFT);
	if (lmb->state && (SDL_GetTicks() - lastFire >= fireInterval)) {
		auto collider = &entity->getComponent<ColliderComponent>().collider;
		Vector2D colliderPos{ static_cast<float>(collider->x), static_cast<float>(collider->y) };
		Vector2D mousePos{ Game::camera.x + lmb->pos.x, Game::camera.y + lmb->pos.y };
		Vector2D direction = Utils::directionBetween(colliderPos, mousePos);
		Game::createProjectile(colliderPos, direction, 1000, 10, "projectile", entity);
		lastFire = SDL_GetTicks();
	}
}