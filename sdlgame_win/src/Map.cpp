#include "Map.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include "GroupLabel.h"

// Populate and return a map of { textureIndexKey : navigatibility }, given a path of
// a file that follows the pattern:
// key navigatibility\n
std::unordered_map<int, int> getNavigatibility(const std::filesystem::path& path) {
	std::unordered_map<int, int> navigatibilityMap;
	std::ifstream mappingFile;
	mappingFile.open(path);
	int mappingKey;
	int navigatability;
	while (!mappingFile.eof()) {
		mappingFile >> mappingKey;
		// Ignore comma.
		mappingFile.ignore();
		mappingFile >> navigatability;
		navigatibilityMap[mappingKey] = navigatability;
	}
	mappingFile.close();
	return navigatibilityMap;
}

void Map::readMap(const std::filesystem::path& mapPath, int mapHeight, int mapWidth,
				  const std::unordered_map<int, int>& navigatibility) {
	char c;
	std::fstream mapFile;
	mapFile.open(mapPath);
	int srcX, srcY, rawX, rawY;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			mapFile.get(c);
			rawY = atoi(&c);
			srcY = rawY * tileSize;

			mapFile.get(c);
			rawX = atoi(&c);
			srcX =  rawX * tileSize;

			// Multiply by 10 because navigatibility stores coordinate (2,1) at int 12.
			const auto key = rawY * 10 + rawX;
			addTile(srcX, srcY, x, y, navigatibility.at(key));
			// Ignore comma.
			mapFile.ignore();
		}
	}
	// TODO: probably should separate out colliders to separate file?
	// Make sure ignore blank line.
	mapFile.ignore();
	// Load colliders.
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			mapFile.get(c);
			// 1 denotes collider.
			if (c == '1') {
				auto& tileCol = Globals::get().entityManager.addEntity();
				const auto scaledTile = getScaledTile(Vector2D<int>(x, y));
				tileCol.addComponent<ColliderComponent>(scaledTile);
				tileCol.addGroup(GroupLabel::Colliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
}

Map::Map(std::filesystem::path mapPath, int mapHeight, int mapWidth, std::string texId,
		 std::filesystem::path textureMappingPath, int tileSize, float mapScale)
	: textureId(texId), tileSize(tileSize), mapScale(mapScale), scaledSize(mapScale * tileSize),
	  navMap(mapHeight, std::vector<int>(mapWidth, 0)) {
	boundsX = static_cast<int>(mapWidth * scaledSize);
	boundsY = static_cast<int>(mapHeight * scaledSize);
	const auto navigatibility = getNavigatibility(textureMappingPath);
	readMap(mapPath, mapHeight, mapWidth, navigatibility);
}

void Map::addTile(int srcX, int srcY, int x, int y, int navValue) {
	navMap[y][x] = navValue;
	const auto xPos = x * scaledSize;
	const auto yPos = y * scaledSize;
	auto& tile = Globals::get().entityManager.addEntity();
	tile.addComponent<TileComponent>(
		srcX, srcY, xPos, yPos, tileSize, mapScale, textureId, navValue);
	tile.addGroup(GroupLabel::Map);
}