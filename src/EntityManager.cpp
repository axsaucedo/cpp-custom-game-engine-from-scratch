#include <iostream>

#include "EntityManager.h"
#include "./Collision.h"
#include "./components/ColliderComponent.h"

void EntityManager::ClearData() {
    for (auto& entity: entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() {
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime) {
    for (auto& entity: entities) {
        entity->Update(deltaTime);
    }
}

void EntityManager::Render() {
    for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
        for (Entity* entity: this->GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
            entity->Render();
        }
    }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

unsigned int EntityManager::GetEntityCount() {
    return entities.size();
}

void EntityManager::ListAllEntities() const {
    for (Entity* entity: entities) {
        std::cout << "Entity: " << entity->GetName() << std::endl;
        std::vector<Component*> components = entity->GetComponents();
        for (Component* component: components) {
            std::cout << "\t" << "Component: " << typeid(component).name() << std::endl;
        }
    }
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for (Entity* entity: entities) {
        if (entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

CollisionType EntityManager::CheckCollisions() const {
    for (Entity* thisEntity: entities) {
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (Entity* thatEntity: entities) {
                if (thisEntity->GetName().compare(thatEntity->GetName()) != 0 && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
                        std::string thisTag = thisCollider->colliderTag;
                        std::string thatTag = thatCollider->colliderTag;
                        if (thisTag.compare("PLAYER") == 0 && thatTag.compare("ENEMY") == 0) {
                            return PLAYER_ENEMY_COLLISION;
                        } else if (thisTag.compare("PLAYER") == 0 && thatTag.compare("PROJECTILE") == 0) {
                            return PLAYER_PROJECTILE_COLLISION;
                        } else if (thisTag.compare("ENEMY") == 0 && thatTag.compare("FRIENDLY_PROJECTILE") == 0) {
                            return ENEMY_PROJECTILE_COLLISION;
                        } else if (thisTag.compare("PLAYER") == 0 && thatTag.compare("VEGETATION") == 0) {
                            return PLAYER_VEGETATION_COLLISION;
                        } else if (thisTag.compare("PLAYER") == 0 && thatTag.compare("LEVEL_COMPLETE") == 0) {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }
    return NO_COLLISION;
}

