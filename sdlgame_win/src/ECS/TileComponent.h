#pragma once

#include <iostream>
#include <SDL.h>
#include "ECS.h"
#include "../TextureManager.h"
#include "../Vector2D.h"
#include "../Globals.h"

class TileComponent : public Component {
public:
	Vector2D<> position{ 0, 0 };
	int tileSize = 0;

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string texId)
			: tileSize(tsize) {
		texture = Globals::get().assetManager.getTexture(texId);

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
		// TODO should be doing this?
		SDL_DestroyTexture(texture);
	}

	void update() override {
		// Dest = initial position - camera pos.
		destRect.x = static_cast<int>(position.x) - Globals::get().camera.x;
		destRect.y = static_cast<int>(position.y) - Globals::get().camera.y;
	}

	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	Vector2D<> center() const {
		const auto halfWidth = static_cast<float>(tileSize * 0.5);
		return Vector2D<>( position.x + halfWidth, position.y + halfWidth );
	}
private:
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};
};