#include "ColliderComponent.h"
#include "../Utils.h"

void ColliderComponent::initColliderComponent(SDL_Rect collider) {
	auto assetPath = std::filesystem::current_path() / "assets";
	dstRect = Game::cameraRelative(collider);

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
	const auto color = (entity->getTag() == "player") ?
		RGBVals::turquoise() : RGBVals::white();
	Utils::drawRect(&dstRect, color);
}