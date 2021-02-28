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
	Vector2D position{ 0, 0 };
	Vector2D velocity;
	float speed = DEFAULT_SPEED;

	int height = 32;
	int width = 32;
	int scale = DEFAULT_SCALE;

	TransformComponent(float x, float y, int h, int w, int sc, float sp) :
			height(h), width(w), scale(sc), speed(sp) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(Vector2D pos, int h, int w, int sc, float sp) :
		position(pos), height(h), width(w), scale(sc), speed(sp) {}

	TransformComponent(float x, float y, int h, int w, int sc) :
		TransformComponent(x, y, h, w, sc, DEFAULT_SPEED) {}

	TransformComponent(Vector2D pos, int h, int w, int sc) :
		position(pos), height(h), width(w), scale(sc) {}

	TransformComponent(float x, float y, int h, int w) :
		TransformComponent(x, y, h, w, DEFAULT_SCALE, DEFAULT_SPEED) {}

	TransformComponent(Vector2D pos, int sc, float spd) : position(pos), scale(sc), speed(spd) {}

	TransformComponent(Vector2D pos, int sc) : TransformComponent(pos, sc, DEFAULT_SPEED) {}

	TransformComponent(float x, float y) :
		TransformComponent(Vector2D{ x, y }, DEFAULT_SCALE, DEFAULT_SPEED) {}

	TransformComponent() {}

	void init() override {
		velocity.zero();
	}

	void update() override {
		position.x += velocity.x * speed * Game::timeDelta;
		position.y += velocity.y * speed * Game::timeDelta;
	}

	/*void draw() override {
		SDL_Rect transform_rect{ static_cast<int>(position.x), static_cast<int>(position.y),
		                         height, width };
		SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Game::renderer, &transform_rect);
	}*/
};