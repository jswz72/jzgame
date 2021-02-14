#pragma once
#include <iostream>
#include <map>
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "Animation.h"


class SpriteComponent : public Component {
public:
	int animIndex = 0;
	std::map<const char*, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() {
		srcRect.x = srcRect.y = 0;
	}
	SpriteComponent(std::string texId, int x, int y, uint32_t RGBVals[] = nullptr) {
		srcRect.x = x;
		xOffset = x;
		srcRect.y = y;
		yOffset = y;
		setTex(texId, RGBVals);
	}

	SpriteComponent(std::string texId, bool isAnimated, uint32_t RGBVals[] = nullptr) {
		animated = isAnimated;
		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);
		animations.emplace("idle", idle);
		animations.emplace("walk", walk);
		play("idle");

		setTex(texId, RGBVals);
	}

	SpriteComponent(std::string texId) {
		srcRect.x = srcRect.y = 0;
		setTex(texId);
	}

	~SpriteComponent() {
		// SDL_DestroyTexture(texture);
	}

	void setTex(std::string texId, uint32_t RGBVals[] = nullptr) {
		// if (texture)
		// 	SDL_DestroyTexture(texture);
		texture = Game::assets->getTexture(texId);
		// texture = TextureManager::loadTexture(filepath, RGBVals);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}
	void update() override {
		if (animated) {
			srcRect.x = xOffset + srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, spriteFlip);
	}
	void play(const char* animName) {
		const auto anim = animations[animName];
		frames = anim.frames;
		animIndex = anim.index;
		speed = anim.speed;
	}

private:
	TransformComponent* transform;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	int xOffset = 0;
	int yOffset = 0;

	bool animated = false;
	int frames = 0;
	int speed = 100;
};