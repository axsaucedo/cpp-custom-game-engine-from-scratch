#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/lua/sol.hpp"

class AssetManager;

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
    void HandleCameraMovement();
    void CheckCollisions();
    void ProcessNextLevel(int levelNumber);
    void ProcessGameOver();

    static SDL_Renderer *renderer;
    static AssetManager* assetManager;
    static SDL_Event event;
    static SDL_Rect camera;

private:
    bool isRunning;
    int ticksLastFrame;
    SDL_Window *window;
};

#endif
