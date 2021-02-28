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
    SDL_Rect collider{ 0,0 };
    std::string tag;
	
    ColliderComponent(std::string t, int xoffset, int yoffset, double wScale, double hScale) :
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
	int yOffset = 0;
    int xOffset = 0;
    double widthScaleFactor = 0;
    double heightScaleFactor = 0;
	// So can see collider on map.
    SDL_Texture* tex = nullptr;
    SDL_Rect srcRect, dstRect = {0,0};

	int getWidthFromTransform();
    int getHeightFromTransform();
};
