#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <SDL_ttf.h>
#include "TextureManager.h"

class AssetManager {
public:
	// Texture management
	void addTexture(std::string id, std::filesystem::path path, uint32_t backgroundColor[] = nullptr);
	SDL_Texture* getTexture(std::string id);

	void addFont(std::string id, std::filesystem::path path, int fontSize);
	TTF_Font* getFont(std::string id);

private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};