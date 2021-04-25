#pragma once
#include <string>
#include <filesystem>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern EntityManager entityManager;

class Map {
public:
	// Initialized when call loadMap.
	int boundsX = 0;
	int boundsY = 0;

	Map(std::string texId, int mScale, int tSize) :
		textureId(texId), mapScale(mScale), tileSize(tSize) {
		scaledSize = mScale * tSize;
	}

	std::vector<std::vector<int>> loadMap(std::filesystem::path path, std::filesystem::path mappingsPath, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xPos, int yPos, std::string tag);
private:
	std::string textureId = "";
	const char* mapFilePath = nullptr;
	int mapScale = 0;
	int tileSize = 0;
	int scaledSize = 0;
};