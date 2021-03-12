#include "ColliderComponent.h"

int ColliderComponent::getWidthFromTransform() {
	assert(transform);
	auto colW = transform->getWidth();
	if (widthScaleFactor)
		colW = static_cast<int>(colW * widthScaleFactor);
	return colW;
}

int ColliderComponent::getHeightFromTransform() {
	assert(transform);
	auto colH = transform->getHeight();
	if (heightScaleFactor)
		colH = static_cast<int>(colH * heightScaleFactor);
	return colH;
}

int ColliderComponent::getXFromTransform() {
	assert(transform);
	auto colX = static_cast<int>(transform->position.x);
	int colW = transform->getWidth();
	if (xOffset)
		colX += static_cast<int>(colW * xOffset);
	return colX;
}

int ColliderComponent::getYFromTransform() {
	assert(transform);
	int colY = static_cast<int>(transform->position.y);
	int colH = transform->getHeight();
	if (yOffset)
		colY += static_cast<int>(colH * yOffset);
	return colY;
}

void ColliderComponent::init() {
	auto assetPath = std::filesystem::current_path() / "assets";
	tex = TextureManager::loadTexture(assetPath / "ColTex.png");
	if (entity->hasComponent<TransformComponent>()) {
		transform = &entity->getComponent<TransformComponent>();
		collider.w = getWidthFromTransform();
		collider.h = getHeightFromTransform();
		collider.x = getXFromTransform();
		collider.y = getYFromTransform();
	}
	srcRect = { 0, 0, 32, 32 };
	dstRect = { collider.x, collider.y, collider.w, collider.h };

	Game::colliders.push_back(this);
}

void ColliderComponent::update() {
	if (transform) {
		collider.w = getWidthFromTransform();
		collider.h = getHeightFromTransform();
		collider.x = getXFromTransform();
		collider.y = getYFromTransform(); 
	}

	dstRect.x = collider.x - Game::camera.x;
	dstRect.y = collider.y - Game::camera.y;
	dstRect.w = collider.w;
	dstRect.h = collider.h;
}


void ColliderComponent::draw() {
    // This should be turned off during the actual gameplay.
	TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
	// Turn this on if colliders aren't acting as expected.
	if (tag == "player") {
		SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
	} else if (tag == "terrain") {
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
	}
	SDL_RenderDrawRect(Game::renderer, &dstRect );
}

