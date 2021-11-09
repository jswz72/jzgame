#include "ColliderComponent.h"
#include "../Utils.h"
#include "../Globals.h"

void ColliderComponent::initColliderComponent(SDL_Rect collider) {
	auto assetPath = std::filesystem::current_path() / "assets";
	dstRect = Globals::get().cameraRelative(collider);

	Globals::get().colliders.push_back(this);
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

ColliderComponent::ColliderComponent(SDL_Rect rect) {
	collider = rect;
	initColliderComponent(collider);
}

void ColliderComponent::update() {
	if (transform) {
		collider = getCollider(transform);
	}

	dstRect = collider;
	dstRect.x -= Globals::get().camera.x;
	dstRect.y -= Globals::get().camera.y;
}

void ColliderComponent::draw() {
	if (!Globals::get().debug) {
		return;
	}
	const auto color = (entity->getTag() == "player") ?
		RGBVals::turquoise() : RGBVals::white();
	Utils::drawRect(&dstRect, color);
}