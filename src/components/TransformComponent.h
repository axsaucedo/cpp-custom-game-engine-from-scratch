#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../EntityManager.h"
#include "../Game.h"
#include "../../lib/glm/glm.hpp"
#include <SDL2/SDL.h>

class TransformComponent: public Component {
private:
    

public:
    TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
        position = glm::vec2(posX, posY);
        velocity = glm::vec2(velX, velY);
        width = w;
        height = h;
        scale = s;
    }
    void Initialize() override {

    }

    void Update(float deltaTime) override {
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
    }

    void Render() override {

    }

    std::string ToString() override {
        return "TransformComponent";
    }

    glm::vec2 position;
    glm::vec2 velocity;
    int width;
    int height;
    int scale;
};

#endif /* TRANSFORMCOMPONENT_H */
