#ifndef SPRITECOMPONENTS_H
#define SPRITECOMPONENTS_H

#include <SDL2/SDL.h>

#include "../TextureManager.h"
#include "../AssetManager.h"

class SpriteComponents: public Component
{
private:
    SDL_Texture* texture; 
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;

public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
};

#endif /* SPRITECOMPONENTS_H */
