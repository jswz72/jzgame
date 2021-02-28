#include "SpriteComponent.h"

void SpriteComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
	srcRect.w = transform->width;
	srcRect.h = transform->height;
}

void SpriteComponent::update() {
	if (animated) {
		srcRect.x = xOffset + srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
	}
	srcRect.y = animIndex * transform->height;
	destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	destRect.w = transform->width * transform->scale;
	destRect.h = transform->height * transform->scale;
}
	

void SpriteComponent::play(const char* animName) {
	const auto anim = animations[animName];
	frames = anim.frames;
	animIndex = anim.index;
	speed = anim.speed;
}


