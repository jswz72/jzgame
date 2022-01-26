#include "PlayerMouseController.h"

#include "../Globals.h"
// TODO get rid of Game import, move createProjectile out of game?
#include "../Game.h"
#include "../Utils.h"
#include "../Vector2D.h"
#include "Components.h"

void PlayerMouseController::update() {
	auto lmb = Globals::get().mouseButtonHandler.mouseButtonData(SDL_BUTTON_LEFT);
	if (lmb.state && (SDL_GetTicks() - lastFire >= fireInterval)) {
		auto collider = &entity->getComponent<ColliderComponent>().collider;
		Vector2D colliderPos{ collider->x, collider->y };
		Vector2D mousePos{ Globals::get().camera.x + lmb.pos.x, Globals::get().camera.y + lmb.pos.y };
		Vector2D direction = Utils::directionBetween(colliderPos, mousePos);
		Game::createProjectile(colliderPos, direction, 1000, 10, "projectile", entity);
		lastFire = SDL_GetTicks();
	}
}