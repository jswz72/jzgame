#pragma once
#include <SDL.h>
#include <string>
#include <filesystem>
#include "ECS.h"
#include "TransformComponent.h"
#include "../Game.h"
#include "../TextureManager.h"

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

    TransformComponent* transform;

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

    void init() override {
        if (!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
        auto assetPath = std::filesystem::current_path() / "assets";
        tex = TextureManager::loadTexture(assetPath / "ColTex.png");
        srcRect = { 0, 0, 32, 32 };
        dstRect = { collider.x, collider.y, collider.w, collider.h };

        Game::colliders.push_back(this);
    }

    void update() override {
        // Terrain doesn't have transform.
        if (tag != "terrain") {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }
		collider.x += xOffset;
		collider.y += yOffset;
		if (widthScaleFactor)
			collider.w *= widthScaleFactor;
		if (heightScaleFactor)
			collider.h *= heightScaleFactor;

        dstRect.x = collider.x - Game::camera.x;
        dstRect.y = collider.y - Game::camera.y;
    }

    void draw() override {
        TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
        if (tag == "player") {
			SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
        } else if (tag == "terrain") {
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawRect(Game::renderer, &collider );
    }
};
