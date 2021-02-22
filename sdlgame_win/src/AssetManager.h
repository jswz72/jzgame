#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <SDL_ttf.h>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"

class AssetManager {
public:
	AssetManager(Manager* man);
	~AssetManager();

	// Game objects
	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id,
		Entity *source);

	// Texture management
	void addTexture(std::string id, std::filesystem::path path);
	SDL_Texture* getTexture(std::string id);

	void addFont(std::string id, std::filesystem::path path, int fontSize);
	TTF_Font* getFont(std::string id);

private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};