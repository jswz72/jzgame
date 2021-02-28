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
    SDL_Rect collider;
    std::string tag;
    int yOffset = 0;
    int xOffset = 0;
    double widthScaleFactor = 0;
    double heightScaleFactor = 0;

    // So can see collider on map.
    SDL_Texture* tex;
    SDL_Rect srcRect, dstRect;

    TransformComponent* transform = nullptr;

    ColliderComponent(std::string t) : tag(t) {}

    ColliderComponent(std::string t, int xoffset, int yoffset, double wScale, double hScale) :
        tag(t), xOffset(xoffset), yOffset(yoffset), widthScaleFactor(wScale),
        heightScaleFactor(hScale) {}

    ColliderComponent(std::string t, int xpos, int ypos, int size) {
        tag = t;
        collider.x = xpos;
        collider.y = ypos;
        collider.h = collider.w = size;
    }

    int getWidthFromTransform() {
        assert(transform);
        auto colW = transform->width * transform->scale;
		if (widthScaleFactor)
			colW *= widthScaleFactor;
        return colW;
    }

	int getHeightFromTransform() {
        assert(transform);
        auto colH = transform->height * transform->scale;
		if (heightScaleFactor)
			colH *= heightScaleFactor;
        return colH;
    }

    void init() override {
        if (entity->hasComponent<TransformComponent>()) {
			transform = &entity->getComponent<TransformComponent>();
			collider.w = getWidthFromTransform();
            collider.h = getHeightFromTransform();
            collider.x = static_cast<int>(transform->position.x) + xOffset;
            collider.y = static_cast<int>(transform->position.y) + yOffset;

        }
        auto assetPath = std::filesystem::current_path() / "assets";
        tex = TextureManager::loadTexture(assetPath / "ColTex.png");
        srcRect = { 0, 0, 32, 32 };
        int colW = (collider.w) ? collider.w : getWidthFromTransform();
        int colH = (collider.h) ? collider.h : getHeightFromTransform();
        dstRect = { collider.x, collider.y, colW, colH };

        Game::colliders.push_back(this);
    }

    void update() override {
        if (transform) {
            collider.w = getWidthFromTransform();
            collider.h = getHeightFromTransform();
            collider.x = static_cast<int>(transform->position.x) + xOffset;
            collider.y = static_cast<int>(transform->position.y) + yOffset;
        }

        dstRect.x = collider.x - Game::camera.x;
        dstRect.y = collider.y - Game::camera.y;
    }

    void draw() override {
        // This should be turned off during the actual gameplay.
        TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
        // Turn this on if colliders aren't acting as expected.
        /*if (tag == "player") {
			SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
        } else if (tag == "terrain") {
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawRect(Game::renderer, &collider );*/
    }
};
