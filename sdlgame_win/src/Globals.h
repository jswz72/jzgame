#pragma once
#include <vector>
#include <SDL.h>
#include "AssetManager.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"

class ColliderComponent;

class Globals {
public:
	bool isRunning = false;
	bool isPaused = false;
	bool debug = false;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	AssetManager assetManager = AssetManager();
	std::vector<ColliderComponent*> colliders;
	float timeDelta;
	KeyboardHandler keyboardHandler;
	MouseButtonHandler mouseButtonHandler;

	Vector2D cameraRelative(const Vector2D& position) {
		return Vector2D{ position.x - camera.x, position.y - camera.y };
	}
	SDL_Rect cameraRelative(const SDL_Rect& rect) {
		const auto newPos = cameraRelative(Vector2D{ rect.x,rect.y });
		return SDL_Rect{ static_cast<int>(newPos.x), static_cast<int>(newPos.y), rect.w, rect.h };
	}
	static Globals& get() {
		static Globals globals;
		return globals;
	}
	Globals(const Globals&) = delete;
	Globals& operator=(const Globals&) = delete;
private:
	Globals() {};
	~Globals() {};
};