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
	
    ColliderComponent(std::string t, float xoffset, float yoffset, float wScale, float hScale) :
        tag(t), xOffset(xoffset), yOffset(yoffset), widthScaleFactor(wScale),
        heightScaleFactor(hScale) {}

    ColliderComponent(std::string t, int xpos, int ypos, int size) {
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.h = collider.w = size;
    }

    ColliderComponent(std::string t) : tag(t) {}

    void init() override; 
    void update() override;
    void draw() override;
private:
	TransformComponent* transform = nullptr;
	float yOffset = 0;
    float xOffset = 0;
    float widthScaleFactor = 0;
    float heightScaleFactor = 0;
	// So can see collider on map.
    SDL_Texture* tex = nullptr;
    SDL_Rect srcRect, dstRect = {0,0};

	int getWidthFromTransform();
    int getHeightFromTransform();
    int getXFromTransform();
    int getYFromTransform();
};
