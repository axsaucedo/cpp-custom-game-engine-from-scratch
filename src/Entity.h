#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

class Component;
class EntityManager;

class Entity
{
private:
    EntityManager &manager;
    bool isActive;
    std::vector<Component*> components;

public:
    Entity(EntityManager &manager);
    Entity(EntityyManager &manager, std::string name);
    void Update(float deltaTime);
    void Render();
    void Destroy();
    void IsActive() const;
};

#endif /* ENTITY_H */
