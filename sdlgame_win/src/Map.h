#pragma once
#include <string>
#include <filesystem>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;

class Map {
public:
	// Initialized when call loadMap.
	int boundsX = 0;
	int boundsY = 0;

	Map(std::string texId, int mScale, int tSize) :
			textureId(texId), mapScale(mScale), tileSize(tSize) {
		scaledSize = mScale * tSize;
	}

	void loadMap(std::filesystem::path path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xPos, int yPos);
private:
	std::string textureId = "";
	const char* mapFilePath = nullptr;
	int mapScale = 0;
	int tileSize = 0;
	int scaledSize = 0;
};