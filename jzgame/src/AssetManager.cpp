#include <filesystem>
#include <stdexcept>
#include "AssetManager.h"
#include "Components/Components.h"

void AssetManager::addTexture(std::string id, std::filesystem::path path, uint32_t backgroundColor[]) {
	auto texture = TextureManager::loadTexture(path, backgroundColor);
	textures.emplace(id, texture);
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