#include "PLayerKeyboardController.h"
#include "Components.h"
#include "../Globals.h"

void PlayerKeyboardController::update() {
	const auto & kbh = Globals::get().keyboardHandler;
	// RELEASED
	if (kbh.isReleased(SDLK_w)) {
		transform.velocity.y = 0;
		sprite.play("idle");
	}
	if (kbh.isReleased(SDLK_a)) {
		transform.velocity.x = 0;
		sprite.play("idle");
	}
	if (kbh.isReleased(SDLK_s)) {
		transform.velocity.y = 0;
		sprite.play("idle");
	}
	if (kbh.isReleased(SDLK_d)) {
		transform.velocity.x = 0;
		sprite.play("idle");
	}
	if (kbh.isReleased(SDLK_LSHIFT) && shiftActive) {
		shiftActive = false;
		transform.speed /= shiftSpeedMult;
	}
	// PRESSED
	if (kbh.isPressed(SDLK_w)) {
	transform.velocity.y = -1;
		sprite.play("walk");
	}
	if (kbh.isPressed(SDLK_a)) {
		transform.velocity.x = -1;
		sprite.setSpriteFlip(SDL_FLIP_HORIZONTAL);
		sprite.play("walk");
	}
	if (kbh.isPressed(SDLK_s)) {
		transform.velocity.y = 1;
		sprite.play("walk");
	}
	if (kbh.isPressed(SDLK_d)) {
		sprite.setSpriteFlip(SDL_FLIP_NONE);
		transform.velocity.x = 1;
		sprite.play("walk");
	}
	if (kbh.isPressed(SDLK_LSHIFT) && !shiftActive) {
		shiftActive = true;
		transform.speed *= shiftSpeedMult;
	}
}