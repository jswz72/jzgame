#pragma once
#include <string>
#include <filesystem>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;

class Map {
public:
	Map(std::string texId, int mScale, int tSize);
	~Map();

	void loadMap(std::filesystem::path path, int sizeX, int sizeY);
	void addTile(int srcX, int srcY, int xPos, int yPos);
private:
	std::string textureId;
	const char* mapFilePath;
	int mapScale;
	int tileSize;
	int scaledSize;
};