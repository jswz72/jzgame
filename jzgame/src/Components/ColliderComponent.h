#pragma once
#include <cassert>
#include <filesystem>
#include <string>
#include <SDL.h>
#include "../EntityComponent.h"
#include "../Rect.h"
#include "../TextureManager.h"
#include "TransformComponent.h"

class ColliderComponent : public Component {
public:
	Rect collider{};

	ColliderComponent(TransformComponent* transformC);

	ColliderComponent(float xpos, float ypos, float size);

	ColliderComponent(SDL_Rect rect);

	static Rect getCollider(TransformComponent* transformC) {
		auto position = transformC->getPosition();
		return { position.x, position.y, transformC->getWidth(), transformC->getHeight() };
	}

	void update() override;
	void draw() override;
private:
	// Transform is optional.
	TransformComponent* transform = nullptr;
	// So can see collider on map.
	SDL_Rect dstRect = { 0,0 };

	void populateDstRect();

	void initColliderComponent(Rect collider);
};
