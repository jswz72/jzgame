#include <filesystem>
#include <stdexcept>
#include "AssetManager.h"
#include "ECS/Components.h"

AssetManager::AssetManager(Manager* man) : manager(man) {
}

AssetManager::~AssetManager() {}


void AssetManager::addTexture(std::string id, std::filesystem::path path) {
    textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(std::string id) {
    return textures[id];
}

void AssetManager::addFont(std::string id, std::filesystem::path path, int fontSize)
{
    auto font = TTF_OpenFont(path.string().c_str(), fontSize);
    if (!font) {
        std::cout << TTF_GetError() << std::endl;
        throw std::exception("Error opening font");
    }
    fonts.emplace(id, font);
}

TTF_Font* AssetManager::getFont(std::string id)
{
    return fonts[id];
}
