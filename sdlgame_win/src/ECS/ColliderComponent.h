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

    // So can see collider on map.
    SDL_Texture* tex;
    SDL_Rect srcRect, dstRect;

    TransformComponent* transform;

    ColliderComponent(std::string t) : tag(t) {}

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

        dstRect.x = collider.x - Game::camera.x;
        dstRect.y = collider.y - Game::camera.y;
    }

    void draw() override {
        TextureManager::draw(tex, srcRect, dstRect, SDL_FLIP_NONE);
        // SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
        // SDL_RenderDrawRect(Game::renderer, &collider );
    }
};
