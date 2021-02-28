#include "ColliderComponent.h"

int ColliderComponent::getWidthFromTransform() {
	assert(transform);
	auto colW = transform->width * transform->scale;
	if (widthScaleFactor)
		colW *= widthScaleFactor;
	return colW;
}

int ColliderComponent::getHeightFromTransform() {
	assert(transform);
	auto colH = transform->height * transform->scale;
	if (heightScaleFactor)
		colH *= heightScaleFactor;
	return colH;
}

void ColliderComponent::init() {
	if (entity->hasComponent<TransformComponent>()) {
		transform = &entity->getComponent<TransformComponent>();
		collider.w = getWidthFromTransform();
		collider.h = getHeightFromTransform();
		collider.x = static_cast<int>(transform->position.x) + xOffset;
		collider.y = static_cast<int>(transform->position.y) + yOffset;

	}
	auto assetPath = std::filesystem::current_path() / "assets";
	tex = TextureManager::loadTexture(assetPath / "ColTex.png");
	srcRect = { 0, 0, 32, 32 };
	int colW = (collider.w) ? collider.w : getWidthFromTransform();
	int colH = (collider.h) ? collider.h : getHeightFromTransform();
	dstRect = { collider.x, collider.y, colW, colH };

	Game::colliders.push_back(this);
}

void ColliderComponent::update() {
	if (transform) {
		collider.w = getWidthFromTransform();
		collider.h = getHeightFromTransform();
		collider.x = static_cast<int>(transform->position.x) + xOffset;
		collider.y = static_cast<int>(transform->position.y) + yOffset;
	}

	dstRect.x = collider.x - Game::camera.x;
	dstRect.y = collider.y - Game::camera.y;
}


void ColliderComponent::draw() {
    // This should be turned off during the actual gameplay.
	TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
	// Turn this on if colliders aren't acting as expected.
	/*if (tag == "player") {
		SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
	} else if (tag == "terrain") {
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
	}
	SDL_RenderDrawRect(Game::renderer, &collider );*/
}

