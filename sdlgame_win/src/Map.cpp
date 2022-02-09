#include "Map.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "GroupLabel.h"

using Coordinates = Vector2D<int>;
using CoordinatesMap = std::unordered_map<Coordinates, int, Coordinates::HashFunction>;

// Populate and return a map of { textureIndexKey : navigatibility }, given a path of
// a file that follows the pattern:
// key navigatibility\n
CoordinatesMap getNavigatibility(const std::filesystem::path& path) {
	CoordinatesMap navigatibilityMap;
	std::ifstream mappingFile;
	mappingFile.open(path);
	std::string xs;
	std::string ys;
	std::string navStr;
	while (!mappingFile.eof()) {
		std::getline(mappingFile, xs, ':');
		std::getline(mappingFile, ys, ' ');
		std::getline(mappingFile, navStr);
		int x = stoi(xs);
		int y = stoi(ys);
		int nav = stoi(navStr);
		Coordinates coords = { x,y };
		navigatibilityMap[coords] = nav;
	}
	mappingFile.close();
	return navigatibilityMap;
}

void Map::readMap(const std::filesystem::path& mapPath, int mapHeight, int mapWidth,
				  const CoordinatesMap& navigatibility) {
	std::fstream mapFile;
	mapFile.open(mapPath);
	std::string xs;
	std::string ys;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			std::getline(mapFile, xs, ':');
			char delimiter = (x == mapWidth - 1) ? '\n' : ',';
			std::getline(mapFile, ys, delimiter);

			Coordinates coords = { stoi(xs), stoi(ys) };
			int srcX =  coords.x * tileSize;
			int srcY = coords.y * tileSize;

			addTile(srcX, srcY, x, y, navigatibility.at(coords));
		}
	}
	// TODO: probably should separate out colliders to separate file?
	// Load colliders.
	// Make sure ignore blank line.
	mapFile.ignore();
	std::string colStr;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			char delimiter = (x == mapWidth - 1) ? '\n' : ',';
			std::getline(mapFile, colStr, delimiter);
			// 1 denotes collider.
			if (stoi(colStr) == 1) {
				auto& tileCol = Globals::get().entityManager.addEntity();
				const auto scaledTile = getScaledTile(Vector2D<int>(x, y));
				tileCol.addComponent<ColliderComponent>(scaledTile);
				tileCol.addGroup(GroupLabel::Colliders);
			}
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