#include "Map.h"
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <iostream>


Map::Map(std::string texId, int mScale, int tSize) :
    textureId(texId), mapScale(mScale), tileSize(tSize) {
    scaledSize = mScale * tSize;
}

Map::~Map() {
}

void Map::loadMap(std::filesystem::path path, int sizeX, int sizeY) {
    char c;
    std::fstream mapFile;
    mapFile.open(path);
    int srcX, srcY;
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            mapFile.get(c);
            char cy = c;
            srcY = atoi(&c) * tileSize;
            mapFile.get(c);
            char cx = c;
            srcX = atoi(&c) * tileSize;
            addTile(srcX, srcY, x * scaledSize, y * scaledSize);
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
                auto& tileCol(manager.addEntity());
                auto scale = tileSize * mapScale;
                tileCol.addComponent<ColliderComponent>("terrain", x * scale, y * scale,
                    tileSize * mapScale);
                tileCol.addGroup(Game::groupColliders);
            }
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::addTile(int srcX, int srcY, int xPos, int yPos) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(
        srcX, srcY, xPos, yPos, tileSize, mapScale, textureId);
    tile.addGroup(Game::groupMap);

}
