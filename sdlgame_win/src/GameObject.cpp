#include <iostream>
#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y, SDL_Rect* rect,
	uint32_t RGBVals[]) {
	if (rect) {
		srcRect = *rect;
	}
	else {
		srcRect.h = 80;
		srcRect.w = 80;
		srcRect.x = 0;
		srcRect.y = 0;
	}
	objTexture = TextureManager::loadTexture(textureSheet, RGBVals);
	xPos = x;
	yPos = y;
}

void GameObject::update() {
	xPos++;
	yPos++;

	dstRect.x = xPos;
	dstRect.y = yPos;
	dstRect.w = srcRect.w * 2;
	dstRect.h = srcRect.h * 2;
}

void GameObject::setSrcRect(SDL_Rect rect) {
	srcRect = rect;
}

void GameObject::render() {
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &dstRect);
}