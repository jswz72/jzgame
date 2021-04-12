#include "Map.h"
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <iostream>

void Map::loadMap(std::filesystem::path mapPath, std::filesystem::path mappingsPath, int sizeX, int sizeY) {
	std::unordered_map<int, std::string> mappings;
	std::ifstream mappingsFile;
	mappingsFile.open(mappingsPath);
	int mappingNum;
	std::string mappingVal;
	while (!mappingsFile.eof()) {
		mappingsFile >> mappingNum;
		// Ignore comma.
		mappingsFile.ignore();
		mappingsFile >> mappingVal;
		mappings[mappingNum] = mappingVal;
	}
	mappingsFile.close();

	char c;
	std::fstream mapFile;
	mapFile.open(mapPath);
	int srcX, srcY, rawX, rawY;
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			rawY = atoi(&c);
			srcY = rawY * tileSize;
			mapFile.get(c);
			rawX = atoi(&c);
			srcX =  rawX * tileSize;
			auto tag = mappings.at(rawY * 10 + rawX);
			addTile(srcX, srcY, x * scaledSize, y * scaledSize, tag);
			// Ignore comma.
			mapFile.ignore();
		}
	}
	boundsX = sizeX * scaledSize;
	boundsY = sizeY * scaledSize;

	// Make sure ignore blank line.
	mapFile.ignore();
	// Load colliders.
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			if (c == '1') {
				auto& tileCol = entityManager.addEntity();
				tileCol.setTag("tileCollider");
				auto scale = tileSize * mapScale;
				auto xpos = x * scale;
				auto ypos = y * scale;
				tileCol.addComponent<ColliderComponent>("terrain", xpos, ypos, scale);
				tileCol.addGroup(Game::groupColliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

void Map::addTile(int srcX, int srcY, int xPos, int yPos, std::string tag) {
	auto& tile = entityManager.addEntity();
	// TODO, this is specific to the given map.
	tile.addComponent<TileComponent>(
		srcX, srcY, xPos, yPos, tileSize, mapScale, textureId);
	tile.addGroup(Game::groupMap);
}