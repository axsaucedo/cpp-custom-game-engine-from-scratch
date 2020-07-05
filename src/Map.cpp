#include <fstream>

#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./components/TileComponent.h"

extern EntityManager manager;

int atoi(std::string);

Map::Map(std::string textureId, int scale, int tileSize) {
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

Map::~Map() {

}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    // TODO: Add check for no file map found
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;
            this->AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int sourceRectX, int sourceRectY, int x, int y) {
    Entity& newTile(manager.AddEntity("Title", TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceRectX, sourceRectY, x, y, tileSize, this->scale, this->textureId);
}


