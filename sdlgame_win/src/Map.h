#pragma once
#include <string>
#include <filesystem>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern EntityManager entityManager;

class Map {
public:
	Map(std::string texId, int mScale, int tSize) :
		textureId(texId), mapScale(mScale), tileSize(tSize) {
		scaledSize = mScale * tSize;
	}

	std::vector<std::vector<int>> loadMap(std::filesystem::path path,
		std::filesystem::path mappingPath, int sizeX, int sizeY);

	Vector2D getBounds() const {
		return Vector2D{ boundsX, boundsY };
	}
private:
	// Initialized in loadMap.
	int boundsX = 0;
	// Initialized in loadMap.
	int boundsY = 0;
	std::string textureId = "";
	const char* mapFilePath = nullptr;
	int mapScale = 0;
	int tileSize = 0;
	int scaledSize = 0;

	void addTile(int srcX, int srcY, int xPos, int yPos, std::string tag);
};