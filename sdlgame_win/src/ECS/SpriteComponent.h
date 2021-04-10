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

	SpriteComponent(const TransformComponent& transformC, std::string texId, int srcH, int srcW, bool isAnimated, uint32_t RGBVals[] = nullptr)
		: transform(transformC) {
		srcRect.h = srcH;
		srcRect.w = srcW;
		animated = isAnimated;
		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);
		animations.emplace("idle", idle);
		animations.emplace("walk", walk);
		play("idle");
		setTex(texId, RGBVals);
	}

	void setTex(std::string texId, uint32_t RGBVals[] = nullptr) {
		texture = Game::assetManager.getTexture(texId);
	}

	void update() override;
	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, spriteFlip);
	}
	void play(const char* animName);
private:
	const TransformComponent& transform;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};
	int xOffset = 0;
	int yOffset = 0;
	bool animated = false;
	int frames = 0;
	int speed = 100;
};