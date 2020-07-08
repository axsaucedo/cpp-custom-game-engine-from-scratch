#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "../../lib/glm/glm.hpp"

#include "./TransformComponent.h"
#include "../EntityManager.h"

class ProjectileEmitterComponent: public Component {
private:
    TransformComponent* transform;
    glm::vec2 origin;
    int speed;
    int range;
    float angleRad;
    bool shouldLoop;

public:
    ProjectileEmitterComponent(int speed, int angleDeg, int range, bool shouldLoop) {
        this->speed = speed;
        this->range = range;
        this->shouldLoop = shouldLoop;
        this->angleRad = glm::radians(static_cast<float>(angleDeg));
    }

    void Initialize() override {
        this->transform = this->owner->GetComponent<TransformComponent>();
        this->origin = glm::vec2(this->transform->position.x, transform->position.y);
        this->transform->velocity = glm::vec2(glm::cos(angleRad) * this->speed,glm::sin(angleRad) * this->speed);
    }

    void Update(float deltaTime) {
        if (glm::distance(transform->position, this->origin) > this->range) {
            if (this->shouldLoop) {
                this->transform->position.x = this->origin.x;
                this->transform->position.y = this->origin.y;
            } else {
                this->owner->Destroy();
            }
        }
    }

};

#endif /* PROJECTILEEMITTERCOMPONENT_H */
