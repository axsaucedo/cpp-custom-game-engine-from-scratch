#include "../lib/glm/glm.hpp"
#include <iostream>

#include "./Constants.h"
#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./AssetManager.h"
#include "./components/TransformComponent.h"
#include "./components/SpriteComponent.h"
#include "./components/KeyboardControlComponent.h"
#include "./components/ColliderComponent.h"
#include "./components/LabelComponent.h"
#include "./components/ProjectileEmitterComponent.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;
Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

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

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
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
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);
    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());

    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
    player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

    //Include entitites
    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);

    Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
    projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    projectile.AddComponent<SpriteComponent>("projectile-image");
    projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4);
    projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);

    Entity& heliport(manager.AddEntity("Heliport", OBSTACLE_LAYER));
    heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
    heliport.AddComponent<SpriteComponent>("heliport-image");
    heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);

    Entity& radarEntity(manager.AddEntity("radar", UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

    Entity& labelLevelName(manager.AddEntity("LabelLevelName", UI_LAYER));
    labelLevelName.AddComponent<LabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);
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

    manager.Update(deltaTime);

    this->HandleCameraMovement();
    this->CheckCollisions();
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

void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();
    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    // Clamping camera to ensure it doesn't go outside boundaries of map
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    CollisionType collisionTagType = manager.CheckCollisions();
    if (collisionTagType == PLAYER_ENEMY_COLLISION) {
        this->ProcessGameOver();
    } else if (collisionTagType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        this->ProcessNextLevel(1);
    }
}

void Game::Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next level" << std::endl;
    isRunning = false;
}

void Game::Game::ProcessGameOver() {
    std::cout << "Game over" << std::endl;
    isRunning = false;
}

