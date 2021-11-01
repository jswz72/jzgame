#pragma once
#include <cassert>
#include <filesystem>
#include <string>
#include <SDL.h>
#include "ECS.h"
#include "../Game.h"
#include "../TextureManager.h"
#include "TransformComponent.h"

class ColliderComponent : public Component {
public:
	SDL_Rect collider{};

	ColliderComponent(TransformComponent* transformC);

	ColliderComponent(int xpos, int ypos, int size);

	static SDL_Rect getCollider(TransformComponent* transformC) {
		auto position = transformC->getPosition();
		return { static_cast<int>(position.x), static_cast<int>(position.y),
			transformC->getWidth(), transformC->getHeight() };
	}

	void update() override;
	void draw() override;
private:
	TransformComponent* transform = nullptr;
	// So can see collider on map.
	SDL_Texture* tex = nullptr;
	SDL_Rect srcRect, dstRect = { 0,0 };

	void initColliderComponent(SDL_Rect collider);
};
