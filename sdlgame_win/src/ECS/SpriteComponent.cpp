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
	destRect.x = cameraRelPos.x;
	destRect.y = cameraRelPos.y;
	destRect.w = transform.getRawWidth();
	destRect.h = transform.getRawHeight();
}

void SpriteComponent::play(const char* animName) {
	const auto anim = animations[animName];
	frames = anim.frames;
	animIndex = anim.index;
	animSpeed = anim.speed;
}