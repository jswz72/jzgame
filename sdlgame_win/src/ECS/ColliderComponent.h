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
    std::string tag = "";
	
    ColliderComponent(std::string t, TransformComponent *transformC=nullptr) : tag(t), transform(transformC) {}

    ColliderComponent(std::string t, int xpos, int ypos, int size, TransformComponent *transformC=nullptr)
        : transform(transformC) {
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.h = collider.w = size;
    }

    void init() override; 
    void update() override;
    void draw() override;
private:
	TransformComponent *transform = nullptr;
	// So can see collider on map.
    SDL_Texture* tex = nullptr;
    SDL_Rect srcRect, dstRect = {0,0};
};
