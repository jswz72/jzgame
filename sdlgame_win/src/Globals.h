#pragma once
#include <vector>
#include <SDL.h>
#include "AssetManager.h"
#include "ECS/ECS.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"

class ColliderComponent;

class Globals {
public:
	bool isRunning = false;
	bool isPaused = false;
	bool debug = true;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera{ 0,0,0,0 };
	AssetManager assetManager;
	EntityManager entityManager;
	float timeDelta = 0;
	KeyboardHandler keyboardHandler;
	MouseButtonHandler mouseButtonHandler;

	template<typename T>
	T cameraRelative(T posItem) {
		posItem.x -= camera.x;
		posItem.y -= camera.y;
		return posItem;
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