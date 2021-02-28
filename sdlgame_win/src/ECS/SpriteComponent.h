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

	SpriteComponent() {
		srcRect.x = srcRect.y = 0;
	}

	void setTex(std::string texId, uint32_t RGBVals[] = nullptr) {
		texture = Game::assets->getTexture(texId);
	}

	void init() override;
	void update() override;
	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, spriteFlip);
	}
	void play(const char* animName);
private:
	TransformComponent* transform = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};
	int xOffset = 0;
	int yOffset = 0;
	bool animated = false;
	int frames = 0;
	int speed = 100;
};