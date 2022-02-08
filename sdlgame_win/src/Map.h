#pragma once
#include <cassert>
#include <string>
#include <filesystem>
#include "ECS/ECS.h"
#include "ECS/ColliderComponent.h"
#include "ECS/TileComponent.h"

class Map {
public:
	std::vector<std::vector<int>> navMap;

	// mapPath should contain a file with a mapHeight x mapWidth grid with each item containing
	// a key into the texture file. Each key should be of the form: Y * 10 + X where X and Y
	// describe coordinates in the texture file.
	// followig this, the mapPath file should contain a mapHeight x mapWidth grid with each item representing
	// the presence of a collider with 1 or the absense with 0.
	// textureMappingPath should contain a file mapping each texture coordinate to a tag and navigatability
	// value.
	// texId should describe the stored texture file. tileSize is the size of each tile in the texture.
	Map(std::filesystem::path mapPath, int mapHeight, int mapWidth, std::string texId,
		std::filesystem::path textureMappingPath, int tileSize, float mapScale);

	Vector2D<int> getBounds() const {
		return Vector2D<int>{ boundsX, boundsY };
	}

	SDL_Rect getScaledTile(const Vector2D<int>& coords) const {
		return SDL_Rect{ static_cast<int>(coords.x * scaledSize),
						 static_cast<int>(coords.y * scaledSize),
						 static_cast<int>(scaledSize),
						 static_cast<int>(scaledSize) };
	}

	Vector2D<int> getCoords(const Vector2D<> pos) const {
		const auto xCord = static_cast<int>(pos.x / scaledSize);
		const auto yCord = static_cast<int>(pos.y / scaledSize);
		return Vector2D<int>(xCord, yCord);
	}

	std::vector<Vector2D<int>> neighborCoords(const Vector2D<int>& coords) const {
		assert(navMap.size() > 0);
		std::vector<Vector2D<int>> neighbors;
		std::vector<Vector2D<int>> directions = { {-1,-1}, {0,-1}, {0,1}, {-1,0}, {1,0}, {1,1} };
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
	float mapScale = 0;
	int tileSize = 0;
	float scaledSize = 0;
	void addTile(int srcX, int srcY, int xPos, int yPos, int navValue);
	void readMap(const std::filesystem::path& mapPath, int mapHeight,
		int mapWidth, const std::unordered_map<int, int>& navigatibility);
};