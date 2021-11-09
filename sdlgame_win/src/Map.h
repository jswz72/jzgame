#pragma once
#include <cassert>
#include <string>
#include <filesystem>
#include "ECS/ECS.h"
#include "ECS/ColliderComponent.h"
#include "ECS/TileComponent.h"

extern EntityManager entityManager;

class Map {
public:
	std::vector<std::vector<int>> navMap;

	Map(std::string texId, int mScale, int tSize) :
		textureId(texId), mapScale(mScale), tileSize(tSize) {
		scaledSize = mScale * tSize;
	}

	void loadMap(std::filesystem::path path, std::filesystem::path mappingPath,
		int sizeX, int sizeY);

	Vector2D getBounds() {
		return Vector2D{ boundsX, boundsY };
	}

	SDL_Rect getScaledTile(const Vector2D& coords) const {
		return SDL_Rect{ static_cast<int>(coords.x * scaledSize), static_cast<int>(coords.y * scaledSize), scaledSize,
						 scaledSize };
	}

	Vector2D getCoords(const Vector2D pos) const {
		const auto xCord = static_cast<int>(pos.x) / scaledSize;
		const auto yCord = static_cast<int>(pos.y) / scaledSize;
		return Vector2D(xCord, yCord);
	}

	std::vector<Vector2D> neighborCoords(const Vector2D& coords) const {
		assert(navMap.size() > 0);
		std::vector<Vector2D> neighbors;
		std::vector<Vector2D> directions = { {0,-1}, {0,1}, {-1,0}, {1,0} };
		for (const auto& dir : directions) {
			const auto neighborPos = dir + coords;
			if (neighborPos.y < navMap.size() && neighborPos.x < navMap[0].size() &&
				neighborPos.y >= 0 && neighborPos.x >= 0) {
				neighbors.push_back(neighborPos);
			}
		}
		return neighbors;
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