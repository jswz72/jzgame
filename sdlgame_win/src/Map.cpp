#include "Map.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include "GroupLabel.h"

void Map::loadMap(std::filesystem::path mapPath, std::filesystem::path mappingPath,
		int sizeX, int sizeY) {
	// Tag for each tile.
	std::unordered_map<int, std::string> tagMap;
	// Navigatability for each tile.
	std::unordered_map<int, int> navigatibilityMap;

	std::ifstream mappingFile;
	mappingFile.open(mappingPath);
	int mappingKey;
	std::string tag;
	int navigatability;
	while (!mappingFile.eof()) {
		mappingFile >> mappingKey;
		// Ignore comma.
		mappingFile.ignore();
		mappingFile >> tag;
		tagMap[mappingKey] = tag;
		// Ignore comma.
		mappingFile.ignore();
		mappingFile >> navigatability;
		navigatibilityMap[mappingKey] = navigatability;
	}
	mappingFile.close();

	char c;
	std::fstream mapFile;
	mapFile.open(mapPath);
	int srcX, srcY, rawX, rawY;
	std::vector<std::vector<int>> newNav(sizeY, std::vector<int>(sizeX, 0));
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			rawY = atoi(&c);
			srcY = rawY * tileSize;

			mapFile.get(c);
			rawX = atoi(&c);
			srcX =  rawX * tileSize;

			// Multiply by 10 because tagMap stores coordinate (2,1) at int 12.
			const auto key = rawY * 10 + rawX;
			const auto tag = tagMap.at(key);
			addTile(srcX, srcY, x * scaledSize, y * scaledSize, tag);
			newNav[y][x] = navigatibilityMap.at(key);
			// Ignore comma.
			mapFile.ignore();
		}
	}
	/*for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			auto navChar = newNav[y][x];
			if (!navChar) {
				std::cout << " , ";
			}
			else {
				std::cout << navChar << ", ";
			}
		}
		std::cout << std::endl;
	}*/
	boundsX = sizeX * scaledSize;
	boundsY = sizeY * scaledSize;

	// Make sure ignore blank line.
	mapFile.ignore();
	// Load colliders.
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			// 1 denotes collider.
			if (c == '1') {
				auto& tileCol = entityManager.addEntity();
				tileCol.setTag("tileCollider");
				const auto scaledTile = getScaledTile(Vector2D(x, y));
				tileCol.addComponent<ColliderComponent>(scaledTile);
				tileCol.addGroup(GroupLabel::Colliders);
			}
			mapFile.ignore();
		}
	}

	mapFile.close();
	navMap = newNav;
}

void Map::addTile(int srcX, int srcY, int xPos, int yPos, std::string tag) {
	auto& tile = entityManager.addEntity();
	tile.setTag("tile" + tag);
	// TODO, this is specific to the given map.
	tile.addComponent<TileComponent>(
		srcX, srcY, xPos, yPos, tileSize, mapScale, textureId);
	tile.addGroup(GroupLabel::Map);
}