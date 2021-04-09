#include "SpriteComponent.h"

void SpriteComponent::update() {
	if (animated) {
		srcRect.x = xOffset + srcRect.w * static_cast<int>(
			(SDL_GetTicks() / speed) % frames);
	}
	srcRect.y = animIndex * srcRect.h;
	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->getWidth();
	destRect.h = transform->getHeight();
}
	

void SpriteComponent::play(const char* animName) {
	const auto anim = animations[animName];
	frames = anim.frames;
	animIndex = anim.index;
	speed = anim.speed;
}


