#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "./TransformComponent.h"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"

#include <SDL2/SDL.h>
#include <iostream>

class Animation;

class SpriteComponent: public Component
{
private:
    TransformComponent* transform;
    SDL_Texture* texture; 
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    bool isAnimated;
    int animationSpeed;
    int numFrames;
    bool isFixed;
    std::map<std::string, Animation> animations;
    std::string currentAnimationName;
    unsigned int animationIndex = 0;

public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(const char* assetTextureId) {
        isAnimated = false;
        isFixed = false;
        this->texture = Game::assetManager->GetTexture(assetTextureId);
    }

    SpriteComponent(std::string id, int numFrames, int animationSpeed, bool hasDirections, bool isFixed) {
        this->isAnimated = true;
        this->numFrames = numFrames;
        this->animationSpeed = animationSpeed;
        this->isFixed = isFixed;

        if (hasDirections) {
            Animation downAnimation = Animation(0, numFrames, animationSpeed);
            Animation rightAnimation = Animation(1, numFrames, animationSpeed);
            Animation leftAnimation = Animation(2, numFrames, animationSpeed);
            Animation upAnimation = Animation(3, numFrames, animationSpeed);

            animations.emplace("DownAnimation", downAnimation);
            animations.emplace("RightAnimation", rightAnimation);
            animations.emplace("LeftAnimation", leftAnimation);
            animations.emplace("UpAnimation", upAnimation);

            this->animationIndex = 0;
            this->currentAnimationName = "DownAnimation";
        } else {
            Animation singleAnimation = Animation(0, numFrames, animationSpeed);
            animations.emplace("SingleAnimation", singleAnimation);
            this->animationIndex = 0;
            this->currentAnimationName = "SingleAnimation";
        }

        this->Play(this->currentAnimationName);
        this->SetTexture(id);
    }

    void Play(std::string animationName) {
        numFrames = animations[animationName].numFrames;
        animationIndex = animations[animationName].index;
        animationSpeed = animations[animationName].animationSpeed;
        currentAnimationName = animationName;
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
        if (this->isAnimated) {
            this->sourceRectangle.x = this->sourceRectangle.w * static_cast<int>((SDL_GetTicks() / this->animationSpeed) % this->numFrames);
        }
        this->sourceRectangle.y = this->animationIndex * this->transform->height;

        this->destinationRectangle.x = static_cast<int>(this->transform->position.x) - Game::camera.x;
        this->destinationRectangle.y = static_cast<int>(this->transform->position.y) - Game::camera.y;
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
