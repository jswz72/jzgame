#include "ColliderComponent.h"
#include "../Utils.h"
#include "../Globals.h"

void ColliderComponent::populateDstRect() {
	dstRect = static_cast<SDL_Rect>(Globals::get().cameraRelative<Rect>(collider));
}

void ColliderComponent::initColliderComponent(Rect collider) {
	populateDstRect();
	Globals::get().colliders.push_back(this);
}

ColliderComponent::ColliderComponent(TransformComponent* transformC)
		: transform(transformC) {
	const auto position = transform->getPosition();
	collider = { position.x, position.y, transform->getWidth(), transform->getHeight() };
	initColliderComponent(collider);
}

ColliderComponent::ColliderComponent(float xpos, float ypos, float size) {
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

	populateDstRect();
}

void ColliderComponent::draw() {
	if (!Globals::get().debug) {
		return;
	}
	const auto color = entity->hasGroup(GroupLabel::Players) ?
		RGBVals::turquoise() : RGBVals::white();
	Utils::drawRect(&dstRect, color);
}