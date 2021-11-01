#include "ColliderComponent.h"

void ColliderComponent::initColliderComponent(SDL_Rect collider) {
	auto assetPath = std::filesystem::current_path() / "assets";
	tex = TextureManager::loadTexture(assetPath / "ColTex.png");
	srcRect = { 0, 0, 32, 32 };
	dstRect = collider;
	dstRect.x -= Game::camera.x;
	dstRect.y -= Game::camera.y;

	Game::colliders.push_back(this);
}

ColliderComponent::ColliderComponent(TransformComponent* transformC)
		: transform(transformC) {
	auto position = transform->getPosition();
	collider = { static_cast<int>(position.x), static_cast<int>(position.y),
		transform->getWidth(), transform->getHeight() };
	initColliderComponent(collider);
}

ColliderComponent::ColliderComponent(int xpos, int ypos, int size) {
	collider.x = xpos;
	collider.y = ypos;
	collider.h = collider.w = size;
	initColliderComponent(collider);
}

void ColliderComponent::update() {
	if (transform) {
		collider = getCollider(transform);
	}

	dstRect = collider;
	dstRect.x -= Game::camera.x;
	dstRect.y -= Game::camera.y;
}

void ColliderComponent::draw() {
	if (!Game::debug) {
		return;
	}
	TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
	if (entity->getTag() == "player") {
		SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
	}
	else if (entity->getTag() == "terrain") {
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
	}
	SDL_RenderDrawRect(Game::renderer, &dstRect);
}