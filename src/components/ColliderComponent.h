#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"

class ColliderComponent: public Component {
private:
    

public:
    ColliderComponent(std::string colliderTag, int x, int y, int width, int height) {
        this->colliderTag = colliderTag;
        this->collider = SDL_Rect{x, y, width, height};
    }

    std::string colliderTag;
    SDL_Rect collider;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    TransformComponent* transform;

    void Initialize() override {
        if (this->owner->HasComponent<TransformComponent>()) {
            this->transform = this->owner->GetComponent<TransformComponent>();
            this->sourceRectangle = SDL_Rect{0, 0, this->transform->width, this->transform->height};
            this->destinationRectangle = SDL_Rect{collider.x, collider.y, collider.w, collider.h};
        }
    }

    void Update(float deltaTime) override {
        this->collider.x = static_cast<int>(this->transform->position.x);
        this->collider.y = static_cast<int>(this->transform->position.y);
        this->collider.w = this->transform->width * this->transform->scale;
        this->collider.h = this->transform->height * this->transform->scale;
        this->destinationRectangle.x = this->collider.x - Game::camera.x;
        this->destinationRectangle.y = this->collider.y - Game::camera.y;

    }

    std::string ToString() override {
        return "ColliderComponent - Tag: " + this->colliderTag;
    }
};

#endif /* COLLIDERCOMPONENT_H */
