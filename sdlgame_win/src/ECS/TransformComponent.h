#pragma once
#include <SDL.h>
#include <iostream>
#include "ECS.h"
#include "../Vector2D.h"
#include "../Game.h"

class TransformComponent : public Component {
public:

	Vector2D position;
	Vector2D velocity;
	float speed = 1;

	int height = 32;
	int width = 32;
	int scale = 1;

	TransformComponent() {
		position.zero();
	}
	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}
	TransformComponent(int sc) : scale(sc) {
		// Roughly middle of screen.
		position.x = 400;
		position.y = 320;
	}
	TransformComponent(float x, float y, int h, int w, int sc) : height(h), width(w), scale(sc) {
		position.x = x;
		position.y = y;
	}
	TransformComponent(float x, float y, int h, int w, int sc, float sp) : height(h), width(w), scale(sc), speed(sp){
		position.x = x;
		position.y = y;
	}
	TransformComponent(Vector2D pos, int h, int w, int sc) :
		position(pos), height(h), width(w), scale(sc) {}
	TransformComponent(Vector2D pos, int sc) : position(pos), scale(sc) {}
	TransformComponent(Vector2D pos, int sc, int spd) : position(pos), scale(sc), speed(spd) {}
	TransformComponent(float x, float y, int h, int w) :
		TransformComponent(x, y, h, w, 1) {}

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