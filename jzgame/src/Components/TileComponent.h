#pragma once

#include <iostream>
#include <SDL.h>
#include "../EntityComponent.h"
#include "../TextureManager.h"
#include "../Vector2D.h"
#include "../Globals.h"

class TileComponent : public Component {
public:
	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string texId, int navValue)
			: tileSize(tsize), navValue(navValue){
		texture = Globals::get().assetManager.getTexture(texId);

		// Keep track of where they actually are, not where they are being drawn.
		position.x = xpos;
		position.y = ypos;

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

	int getNavValue() { return navValue; };

	void update() override {
		const auto relPos = Globals::get().cameraRelative(position);
		destRect.x = relPos.x;
		destRect.y = relPos.y;
	}

	void draw() override {
		TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	Vector2D<> center() const {
		const float halfWidth = static_cast<float>(tileSize * 0.5);
		return Vector2D<>( position.x + halfWidth, position.y + halfWidth );
	}
private:
	// Position tracks where the tile actually is in the game world.
	Vector2D<int> position{ 0, 0 };
	SDL_Texture* texture = nullptr;
	// srcRect is where we will pull from the source texture.
	SDL_Rect srcRect{};
	// destRect is where the tile will be drawn relative to camera.
	SDL_Rect destRect{};
	int navValue;
	int tileSize = 0;
};