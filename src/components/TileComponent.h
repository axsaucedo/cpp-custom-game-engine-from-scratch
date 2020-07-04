#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <string>
#include <SDL2/SDL.h>
#include <../../lib/glm/glm.hpp>

#include "../Component.h"
#include "../AssetManager.h"

class TileComponent: public Component {
private:
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    glm::vec2 position;
    
public:
    TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId) {
        
        texture = Game::assetManager->GetTexture(assetTextureId);

        this->sourceRectangle.x = sourceRectX;
        this->sourceRectangle.y = sourceRectY;
        this->sourceRectangle.w = tileSize;
        this->sourceRectangle.h = tileSize;

        this->destinationRectangle.x = x;
        this->destinationRectangle.y = y;
        this->destinationRectangle.w = tileSize * tileScale;
        this->destinationRectangle.h = tileSize * tileScale;

        this->position.x = x;
        this->position.y = y;
    }

    ~TileComponent() {
        SDL_DestroyTexture(texture);
    }

    void Update(float deltaTime) override {

    }

    void Render() override {
        TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
    }

};

#endif /* TILECOMPONENT_H */
