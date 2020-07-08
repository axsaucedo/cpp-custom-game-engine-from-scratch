#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include "./SpriteComponent.h"

class KeyboardControlComponent: public Component
{
private:
    

public:
    std::string upKey;
    std::string downKey;
    std::string rightKey;
    std::string leftKey;
    std::string shootKey;
    TransformComponent* transform;
    SpriteComponent* sprite;
    
    KeyboardControlComponent() {

    }

    KeyboardControlComponent(std::string upKey, std::string rightKey, std::string downKey, std::string leftKey, std::string shootKey) {
        this->upKey = this->GetSDLKeyStringCode(upKey);
        this->rightKey = this->GetSDLKeyStringCode(rightKey);
        this->downKey = this->GetSDLKeyStringCode(downKey);
        this->leftKey = this->GetSDLKeyStringCode(leftKey);
        this->shootKey = this->GetSDLKeyStringCode(shootKey);
    }

    std::string GetSDLKeyStringCode(std::string key) {
        if (key.compare("up") == 0) return "1073741906";
        if (key.compare("down") == 0) return "1073741905";
        if (key.compare("left") == 0) return "1073741904";
        if (key.compare("right") == 0) return "1073741903";
        if (key.compare("space") == 0) return "32";
        return std::to_string(static_cast<int>(key[0]));
    }

    void Initialize() override {
        this->transform = this->owner->GetComponent<TransformComponent>();
        this->sprite = this->owner->GetComponent<SpriteComponent>();
    }

    void Update(float deltaTime) override {
        if (Game::event.type == SDL_KEYDOWN) {
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);

            if (keyCode.compare(this->upKey) == 0) {
                this->transform->velocity.y = -30;
                this->transform->velocity.x = 0;
                this->sprite->Play("UpAnimation");

            } else if (keyCode.compare(this->rightKey) == 0) {
                this->transform->velocity.y = 0;
                this->transform->velocity.x = 30;
                this->sprite->Play("RightAnimation");

            } else if (keyCode.compare(this->downKey) == 0) {
                this->transform->velocity.y = 30;
                this->transform->velocity.x = 0;
                this->sprite->Play("DownAnimation");

            } else if (keyCode.compare(this->leftKey) == 0) {
                this->transform->velocity.y = 0;
                this->transform->velocity.x = -30;
                this->sprite->Play("LeftAnimation");

            } else if (keyCode.compare(this->shootKey) == 0) {
                
            }
        }

        if (Game::event.type == SDL_KEYUP) {
            std::string keyCode = std::to_string(Game::event.key.keysym.sym);
            if (keyCode.compare(this->upKey) == 0) {
                this->transform->velocity.y = 0;

            } else if (keyCode.compare(this->rightKey) == 0) {
                this->transform->velocity.x = 0;

            } else if (keyCode.compare(this->downKey) == 0) {
                this->transform->velocity.y = 0;

            } else if (keyCode.compare(this->leftKey) == 0) {
                this->transform->velocity.x = 0;

            } else if (keyCode.compare(this->shootKey) == 0) {

            }
        }
    }

    std::string ToString() override {
        return "KeyboardControlComponent";
    }
};

#endif /* KEYBOARDCONTROLCOMPONENT_H */
