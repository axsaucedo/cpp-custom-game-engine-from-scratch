#include "./AssetManager.h"

AssetManager::AssetManager(EntityManager* manager): manager(manager) {

}

AssetManager::~AssetManager() {

}

void AssetManager::ClearData() {
    this->textures.clear();
}

void AssetManager::AddTexture(std::string textureId, const char*filePath) {
    this->textures.emplace(textureId, TextureManager::LoadTexture(filePath));
}

SDL_Texture* AssetManager::GetTexture(std::string textureId) {
    return textures[textureId];
}

