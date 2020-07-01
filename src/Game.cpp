#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"
#include "EntityManager.h"

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {

}

bool Game::IsRunning() const {
    return this->isRunning;
}

EntityManager manager;
SDL_Renderer* Game::renderer;

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

    isRunning = true;
    return;
}

void Game::ProcessInput() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
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
}

void Game::Render() {
    SDL_SetRenderDrawColor(Game::renderer, 21, 21, 21, 255);
    SDL_RenderClear(Game::renderer);
    
    // TODO:
    // Here we call the manager render to render all entities

    SDL_RenderPresent(Game::renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(Game::renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

