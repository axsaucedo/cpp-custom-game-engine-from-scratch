#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Game {
public:
    Game();
    ~Game();
    bool IsRunning() const;
    void Initialize(int width, int height);
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
    void LoadLevel(int levelNumber);

    static SDL_Renderer *renderer;

private:
    bool isRunning;
    int ticksLastFrame;
    SDL_Window *window;
};

#endif
