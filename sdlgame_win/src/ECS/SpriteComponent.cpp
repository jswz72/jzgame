#include "SpriteComponent.h"
#include "../Globals.h"

void SpriteComponent::update() {
	if (animated) {
		const int frameIndex = static_cast<int>(
			(SDL_GetTicks() / animSpeed) % frames);
		srcRect.x = srcRect.w * frameIndex;
	}
	srcRect.y = animIndex * srcRect.h;
	const auto cameraRelPos = Globals::get().cameraRelative(transform.getRawPosition());
	destRect.x = static_cast<int>(cameraRelPos.x);
	destRect.y = static_cast<int>(cameraRelPos.y);
	destRect.w = static_cast<int>(transform.getRawWidth());
	destRect.h = static_cast<int>(transform.getRawHeight());
}

void SpriteComponent::play(const char* animName) {
	const auto anim = animations[animName];
	frames = anim.frames;
	animIndex = anim.index;
	animSpeed = anim.speed;
}