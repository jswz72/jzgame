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
#include "../Globals.h"

class SpriteComponent : public Component {
public:

	SpriteComponent(const TransformComponent& transformC, std::string texId,
			int srcH=DEFAULT_RAW_HEIGHT, int srcW=DEFAULT_RAW_WIDTH,
			bool isAnimated=false) : transform(transformC) {
		srcRect.h = srcH;
		srcRect.w = srcW;
		animated = isAnimated;
		// TODO this is not universal.
		Animation idle = Animation(0, 3, 100);
		Animation walk = Animation(1, 8, 100);
		animations.emplace("idle", idle);
		animations.emplace("walk", walk);
		play("idle");
		texture = Globals::get().assetManager.getTexture(texId);
	}

	void setSpriteFlip(SDL_RendererFlip sf) {
		spriteFlip = sf;
	}

	// Set the given animName as the "playing" animation, to be acted on during update.
	void play(const char* animName);

	void update() override;
	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, spriteFlip);
	}
private:
	int animIndex = 0;
	std::map<const char*, Animation> animations;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	const TransformComponent& transform;
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};
	bool animated = false;
	int frames = 0;
	int animSpeed = 100;
};