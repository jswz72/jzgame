#include "Map.h"
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <iostream>

std::vector<std::vector<int>> Map::loadMap(std::filesystem::path mapPath, std::filesystem::path mappingsPath, int sizeX, int sizeY) {
	std::unordered_map<int, std::string> tagMappings;
	std::unordered_map<int, int> navMappings;
	std::ifstream mappingsFile;
	mappingsFile.open(mappingsPath);
	int mappingKey;
	std::string tag;
	int navigatable;
	while (!mappingsFile.eof()) {
		mappingsFile >> mappingKey;
		// Ignore comma.
		mappingsFile.ignore();
		mappingsFile >> tag;
		tagMappings[mappingKey] = tag;
		// Ignore comma.
		mappingsFile.ignore();
		mappingsFile >> navigatable;
		navMappings[mappingKey] = navigatable;
	}
	mappingsFile.close();

	char c;
	std::fstream mapFile;
	mapFile.open(mapPath);
	int srcX, srcY, rawX, rawY;
	std::vector<std::vector<int>> navMap(sizeY, std::vector<int>(sizeX, 0));
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			rawY = atoi(&c);
			srcY = rawY * tileSize;
			mapFile.get(c);
			rawX = atoi(&c);
			srcX =  rawX * tileSize;
			auto key = rawY * 10 + rawX;
			auto tag = tagMappings.at(key);
			addTile(srcX, srcY, x * scaledSize, y * scaledSize, tag);
			if (y >= navMap.size() || x >= navMap[y].size()) {
				int i = 0;
				i++;
			}
			navMap[y][x] = navMappings.at(key);
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
	return navMap;
}

void Map::addTile(int srcX, int srcY, int xPos, int yPos, std::string tag) {
	auto& tile = entityManager.addEntity();
	tile.setTag("tile" + tag);
	// TODO, this is specific to the given map.
	tile.addComponent<TileComponent>(
		srcX, srcY, xPos, yPos, tileSize, mapScale, textureId);
	tile.addGroup(Game::groupMap);
}