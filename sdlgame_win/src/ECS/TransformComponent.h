#pragma once
#include <SDL.h>
#include <iostream>
#include "ECS.h"
#include "../Vector2D.h"
#include "../Game.h"

const float DEFAULT_SPEED = 1;
const int DEFAULT_SCALE = 1;

class TransformComponent : public Component {
public:
	Vector2D velocity;
	float speed = DEFAULT_SPEED;

	TransformComponent(Vector2D pos, int h, int w, int sc, float sp) :
		position(pos), rawHeight(h), rawWidth(w), scale(sc), speed(sp) {}

	TransformComponent(Vector2D pos, int sc, float spd, float heightSF, float widthSF, float xOff, float yOff)
		: position(pos), scale(sc), speed(spd), heightScaleFactor(heightSF), widthScaleFactor(widthSF), xOffset(xOff), yOffset(yOff) {}

	int getBaseHeight() const {
		return rawHeight * scale;
	}

	int getBaseWidth() const {
		return rawWidth * scale;
	}

	int getHeight() const {
		return static_cast<int>(getBaseHeight() * heightScaleFactor);
	}

	int getWidth() const {
		return static_cast<int>(getBaseWidth() * widthScaleFactor);
	}

	Vector2D getBasePosition() const {
		return position;
	}

	Vector2D getPosition() const {
		return Vector2D{ position.x + static_cast<int>(getBaseWidth() * xOffset),
						 position.y + static_cast<int>(getBaseHeight() * yOffset) };
	}

	Vector2D getCameraRelativePosition() const {
		auto pos = getPosition();
		pos.x -= Game::camera.x;
		pos.y -= Game::camera.y;
		return pos;
	}

	void setBasePosition(Vector2D pos) {
		position = pos;
	}

	void setPosition(Vector2D pos) {
		position = { pos.x - static_cast<int>(getBaseWidth() * xOffset),
					 pos.y - static_cast<int>(getBaseHeight() * yOffset) };
	}

	void init() override {
		velocity.zero();
	}

	void update() override {
		Vector2D normalizedVel = velocity.normalized();
		position.x += normalizedVel.x * speed * Game::timeDelta;
		position.y += normalizedVel.y * speed * Game::timeDelta;
	}

	void draw() override {
		if (!Game::debug) {
			return;
		}
		auto relPos = getCameraRelativePosition();
		SDL_Rect transform_rect{ static_cast<int>(relPos.x), static_cast<int>(relPos.y),
								 getWidth(), getHeight() };
		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Game::renderer, &transform_rect);
	}

private:
	Vector2D position{ 0, 0 };
	int rawHeight = 32;
	int rawWidth = 32;
	int scale = DEFAULT_SCALE;
	float yOffset = 0;
	float xOffset = 0;
	float widthScaleFactor = 1;
	float heightScaleFactor = 1;
};