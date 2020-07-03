#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>

#include "./TransformComponent.h"
#include "../TextureManager.h"
#include "../AssetManager.h"

class SpriteComponent: public Component
{
private:
    TransformComponent* transform;
    SDL_Texture* texture; 
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;

public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(const char* assetTextureId) {
        texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void SetTexture(std::string assetTextureId) {
        this->texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void Initialize() override {
        this->transform = owner->GetComponent<TransformComponent>();
        this->sourceRectangle.x = 0;
        this->sourceRectangle.y = 0;
        this->sourceRectangle.w = this->transform->width;
        this->sourceRectangle.h = this->transform->height;
    }

    void Update(float deltaTime) override {
        this->destinationRectangle.x = this->transform->position.x;
        this->destinationRectangle.y = this->transform->position.y;
        this->destinationRectangle.w = this->transform->width * this->transform->scale;
        this->destinationRectangle.h = this->transform->height * this->transform->scale;
    }
    
    void Render() override {
        TextureManager::Draw(
                this->texture,
                this->sourceRectangle,
                this->destinationRectangle,
                this->spriteFlip);
    }
};

#endif /* SPRITECOMPONENT_H */
