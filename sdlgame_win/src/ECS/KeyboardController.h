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
			std::cout << "W RELEASED" << std::endl;
			transform->velocity.y = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_a)) {
			std::cout << "A RELEASED" << std::endl;
			transform->velocity.x = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_s)) {
			std::cout << "S RELEASED" << std::endl;
			transform->velocity.y = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_d)) {
			std::cout << "D RELEASED" << std::endl;
			transform->velocity.x = 0;
			sprite->play("idle");
		}
		if (Game::keyboardHandler.isReleased(SDLK_ESCAPE)) {
			Game::isRunning = false;
		}
		if (Game::keyboardHandler.isPressed(SDLK_w)) {
			std::cout << "W PRESSED" << std::endl;
			transform->velocity.y = -1;
			sprite->play("walk");
		}
		if (Game::keyboardHandler.isPressed(SDLK_a)) {
			std::cout << "A PRESSED" << std::endl;
			transform->velocity.x = -1;
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			sprite->play("walk");
		}
		if (Game::keyboardHandler.isPressed(SDLK_s)) {
			std::cout << "S PRESSED" << std::endl;
			transform->velocity.y = 1;
			sprite->play("walk");
		}
		if (Game::keyboardHandler.isPressed(SDLK_d)) {
			std::cout << "D PRESSED" << std::endl;
			sprite->spriteFlip = SDL_FLIP_NONE;
			transform->velocity.x = 1;
			sprite->play("walk");
		}
	}
};