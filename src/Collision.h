#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>

class Collision
{
private:
    

public:
    Collision();

    static bool CheckRectangleCollision(const SDL_Rect& rectangleA, const SDL_Rect& rectangleB);

};

#endif /* COLLISION_H */
