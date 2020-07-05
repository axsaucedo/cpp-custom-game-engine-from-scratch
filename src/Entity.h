#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>

#include "./EntityManager.h"
#include "./Component.h"
#include "./Constants.h"

class EntityManager;
class Component;

class Entity
{
private:
    EntityManager &manager;
    bool isActive;
    std::string name;
    std::vector<Component*> components;
    std::map<const std::type_info*, Component*> componentTypeMap;

public:
    Entity(EntityManager& manager);
    Entity(EntityManager& manager, std::string name, LayerType layer);

    void Update(float deltaTime);
    void Render();
    void Destroy();
    bool IsActive() const;
    void ListAllComponents() const;
    std::string GetName() { return this->name; }
    std::vector<Component*>& GetComponents() { return components; }

    LayerType layer;

    template <typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args) {
        T* newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        this->components.emplace_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->Initialize();
        return *newComponent;
    }

    template <typename T>
    T* GetComponent() {
        return static_cast<T*>(this->componentTypeMap[&typeid(T)]);
    }

    template <typename T>
    T* HasComponent() {
        return this->componentTypeMap.count(&typeid(T)) > 0;
    }

};

#endif /* ENTITY_H */
