#include "ColliderComponent.h"

void ColliderComponent::init() {
	auto assetPath = std::filesystem::current_path() / "assets";
	tex = TextureManager::loadTexture(assetPath / "ColTex.png");
	if (transform) {
		auto position = transform->getPosition();
		collider = { static_cast<int>(position.x), static_cast<int>(position.y),
			transform->getWidth(), transform->getHeight() };
	}
	srcRect = { 0, 0, 32, 32 };
	dstRect = collider;
	dstRect.x -= Game::camera.x;
	dstRect.y -= Game::camera.y;

	Game::colliders.push_back(this);
}

void ColliderComponent::update() {
	if (transform) {
		auto position = transform->getPosition();
		collider = { static_cast<int>(position.x), static_cast<int>(position.y),
			transform->getWidth(), transform->getHeight() };
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
	if (tag == "player") {
		SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
	} else if (tag == "terrain") {
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
	}
	SDL_RenderDrawRect(Game::renderer, &dstRect );
}

