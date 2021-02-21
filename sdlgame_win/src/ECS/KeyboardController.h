#pragma once

#include "../Game.h"
#include "ECS.h"
#include "TransformComponent.h"

class KeyboardController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override {
		if (Game::keyboardHandler.isReleased(SDLK_w)) {
			transform->velocity.y = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_a)) {
			transform->velocity.x = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_s)) {
			transform->velocity.y = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_d)) {
			transform->velocity.x = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_ESCAPE)) {
			Game::isRunning = false;
		}
		if (Game::keyboardHandler.isPressed(SDLK_w)) {
			transform->velocity.y = -1;
			sprite->play("walk");
		}
		if (Game::keyboardHandler.isPressed(SDLK_a)) {
			transform->velocity.x = -1;
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			sprite->play("walk");
		}
		if (Game::keyboardHandler.isPressed(SDLK_s)) {
			transform->velocity.y = 1;
			sprite->play("walk");
		}
		if (Game::keyboardHandler.isPressed(SDLK_d)) {
			sprite->spriteFlip = SDL_FLIP_NONE;
			transform->velocity.x = 1;
			sprite->play("walk");
		}
	}
};