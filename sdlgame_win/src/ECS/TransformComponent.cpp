#include "TransformComponent.h"

#include "../Globals.h"

void TransformComponent::draw() {
	if (!Globals::get().debug) {
		return;
	}
	auto relPos = Globals::get().cameraRelative(getPosition());
	SDL_Rect transform_rect{ static_cast<int>(relPos.x), static_cast<int>(relPos.y),
							 static_cast<int>(getWidth()),
							 static_cast<int>(getHeight()) };
	Utils::drawRect(&transform_rect, RGBVals::red());
}

void TransformComponent::update() {
	rawPosition = getNewRawPos();
}


Vector2D<> TransformComponent::getNewPosition() const {
	return applyOffsets(getNewRawPos());
}

float TransformComponent::getNewRawXPos() const {
	auto xPos = rawPosition.x;
	Vector2D<> normalizedVel = velocity.normalized();
	return xPos + normalizedVel.x * speed * Globals::get().timeDelta;
}

float TransformComponent::getNewRawYPos() const {
	auto yPos = rawPosition.y;
	Vector2D<> normalizedVel = velocity.normalized();
	return yPos + normalizedVel.y * speed * Globals::get().timeDelta;
}

Vector2D<> TransformComponent::getNewRawPos() const {
	return Vector2D<>{ getNewRawXPos(), getNewRawYPos() };
}

