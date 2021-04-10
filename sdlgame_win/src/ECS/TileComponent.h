#pragma once

#include <iostream>
#include <SDL.h>
#include "ECS.h"
#include "../TextureManager.h"
#include "../Vector2D.h"

class TileComponent : public Component {
public:
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};
	Vector2D position{ 0, 0 };

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id) {
		texture = Game::assets->getTexture(id);

		// Keep track of where they actually are, not where they are being drawn.
		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = tsize * tscale;
	}

	TileComponent() {}

	~TileComponent() {
		SDL_DestroyTexture(texture);
	}

	void update() override {
		// Dest = initial position - camera pos.
		destRect.x = static_cast<int>(position.x) - Game::camera.x;
		destRect.y = static_cast<int>(position.y) - Game::camera.y;
	}

	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};