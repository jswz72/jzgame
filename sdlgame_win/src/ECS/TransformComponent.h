#pragma once
#include <SDL.h>
#include <iostream>
#include "ECS.h"
#include "../Rect.h"
#include "../Utils.h"
#include "../Vector2D.h"

const float DEFAULT_SPEED = 1;
const float DEFAULT_SCALE = 1;
const float DEFAULT_RAW_HEIGHT = 32;
const float DEFAULT_RAW_WIDTH = 32;

class TransformComponent : public Component {
public:
	Vector2D velocity;
	float speed = DEFAULT_SPEED;

	TransformComponent(Vector2D pos, float h, float w, float sc, float sp) :
		rawPosition(pos), rawHeight(h), rawWidth(w), scale(sc), speed(sp) {}

	// xOffset, yOffset, heightScaleFactor, widthScaleFactor are used to modify the
	// transform rawPosition. rawPosition is used for sprites, while the modified
	// form is used for all other computations including collisions.  This can be
	// used to draw a smaller hitbox on a sprite.
	TransformComponent(Vector2D pos, float sc, float spd, float heightSF, float widthSF,
		float xOff, float yOff, float rawHeight=DEFAULT_RAW_HEIGHT, float rawWidth=DEFAULT_RAW_WIDTH) :
		rawPosition(pos), scale(sc), speed(spd), heightScaleFactor(heightSF),
		widthScaleFactor(widthSF), xOffset(xOff), yOffset(yOff), rawHeight(rawHeight),
		rawWidth(rawWidth) {}

	void setRawPosition(const Vector2D pos) {
		rawPosition = pos;
	}

	void setPosition(Vector2D pos) {
		rawPosition = { pos.x - (getRawWidth() * xOffset),
						pos.y - (getRawHeight() * yOffset) };
	}

	Vector2D getPosition() const {
		return applyOffsets(rawPosition);
	}

	Rect getRect() const {
		const auto pos = getPosition();
		return Rect{ pos.x, pos.y, getWidth(), getHeight() };
	}

	Vector2D getCenter() const {
		const auto &rect = getRect();
		return Vector2D{rect.x + (rect.w / 2), rect.y + (rect.h / 2)};
	}

	float getHeight() const {
		return getRawHeight() * heightScaleFactor;
	}

	float getWidth() const {
		return getRawWidth() * widthScaleFactor;
	}

	float getRawHeight() const {
		return rawHeight * scale;
	}

	float getRawWidth() const {
		return rawWidth * scale;
	}

	Vector2D getRawPosition() const {
		return rawPosition;
	}

	// Get the expected position for next update.
	Vector2D getNewPosition() const;

	void init() override {
		velocity.zero();
	}
	void update() override;
	void draw() override;

private:
	Vector2D rawPosition{ 0, 0 };
	float rawHeight = 32;
	float rawWidth = 32;
	float scale = DEFAULT_SCALE;
	float yOffset = 0;
	float xOffset = 0;
	float widthScaleFactor = 1;
	float heightScaleFactor = 1;

	Vector2D applyOffsets(Vector2D pos) const {
		return Vector2D{ pos.x + (getRawWidth() * xOffset),
						 pos.y + (getRawHeight() * yOffset) };
	}

	Vector2D getNewRawPos() const; 
	float getNewRawXPos() const; 
	float getNewRawYPos() const;
};