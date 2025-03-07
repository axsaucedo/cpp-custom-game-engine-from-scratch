#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class Entity;

class Component {
public:
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    virtual std::string ToString() {}
    Entity *owner;
};

#endif
