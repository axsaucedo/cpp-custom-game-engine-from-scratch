#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./EntityManager.h"
#include "./AssetManager.h"
#include "./components/TransformComponent.h"
#include "./components/SpriteComponent.h"
#include "./components/KeyboardControlComponent.h"

#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {

}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    window = SDL_CreateWindow(NULL, 
        0, 
        SDL_WINDOWPOS_CENTERED, 
        width, 
        height, 
        SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    Game::renderer = SDL_CreateRenderer(window, -1, 0);
    if (!Game::renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    this->LoadLevel(0);

    isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber) {
    // Include assets
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());

    Entity& chopperEntity(manager.AddEntity("chopper"));
    chopperEntity.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    chopperEntity.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    chopperEntity.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");

    //Include entitites
    Entity& tankEntity(manager.AddEntity("projectile"));
    tankEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");

    Entity& radarEntity(manager.AddEntity("radar"));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);
}

void Game::ProcessInput() {
    SDL_PollEvent(&Game::event);

    switch(Game::event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (Game::event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
        default:
            break;
    }
}

void Game::Update() {
    // Wait until 16ms has elasped since last time
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    // Delta time is the difference in ticks from the last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // Clamp deltaTime to maximum value
    deltaTime = (deltaTime > 0.05) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    // TODO:
    // Here we call the manager.update

    manager.Update(deltaTime);
}

void Game::Render() {
    SDL_SetRenderDrawColor(Game::renderer, 21, 21, 21, 255);
    SDL_RenderClear(Game::renderer);
    
    if (manager.HasNoEntities()) {
        return;
    }

    manager.Render();

    SDL_RenderPresent(Game::renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(Game::renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

