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
Entity& player(manager.AddEntity("chopper-texture", PLAYER_LAYER));

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

    this->LoadLevel(1);

    isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];

    // Reading all assets from Lua
    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if (assetType.compare("texture") == 0) {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            } else if (assetType.compare("font") == 0) {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->AddFont(assetId, assetFile.c_str(), fontSize);
            }
        }
        assetIndex++;
    }

    // Loads map from Lua config
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(mapTextureId, static_cast<int>(levelMap["scale"]), static_cast<int>(levelMap["tileSize"]));
    map->LoadMap(mapFile, static_cast<int>(levelMap["mapSizeX"]), static_cast<int>(levelMap["mapSizeY"]));

    // Load entities
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        sol::optional<sol::table> existsEntityIndexMode = levelEntities[entityIndex];
        if (existsEntityIndexMode == sol::nullopt) {
            break;
        } else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));
            Entity& newEntity(manager.AddEntity(entityName, entityLayerType));

            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if (existsTransformComponent != sol::nullopt) {
                sol::table transformComponent = entity["components"]["transform"];
                newEntity.AddComponent<TransformComponent>(
                    static_cast<int>(transformComponent["position"]["x"]),
                    static_cast<int>(transformComponent["position"]["y"]),
                    static_cast<int>(transformComponent["velocity"]["x"]),
                    static_cast<int>(transformComponent["velocity"]["y"]),
                    static_cast<int>(transformComponent["width"]),
                    static_cast<int>(transformComponent["height"]),
                    static_cast<int>(transformComponent["scale"])
                );
            }

            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if (existsSpriteComponent != sol::nullopt) {
                sol::table sprite = entity["components"]["sprite"];
                std::string textureId = sprite["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if (isAnimated) {
                    newEntity.AddComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(sprite["frameCount"]),
                        static_cast<int>(sprite["animationSpeed"]),
                        static_cast<bool>(sprite["hasDirections"]),
                        static_cast<bool>(sprite["fixed"])
                    );
                } else {
                    newEntity.AddComponent<SpriteComponent>(textureId.c_str());
                }
            }

            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if (existsInputComponent != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardComponent = entity["components"]["input"]["keyboard"];
                if (existsKeyboardComponent != sol::nullopt) {
                    sol::table keyboard = entity["components"]["input"]["keyboard"];
                    newEntity.AddComponent<KeyboardControlComponent>(
                        keyboard["up"],
                        keyboard["right"],
                        keyboard["down"],
                        keyboard["left"],
                        keyboard["shoot"]
                    );
                }
            }

            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if (existsColliderComponent != sol::nullopt) {
                std::string colliderTag = entity["components"]["collider"]["tag"];
                sol::table transform = entity["components"]["transform"];
                newEntity.AddComponent<ColliderComponent>(
                    colliderTag,
                    static_cast<int>(transform["position"]["x"]),
                    static_cast<int>(transform["position"]["y"]),
                    static_cast<int>(transform["width"]),
                    static_cast<int>(transform["height"])
                );
            }

            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectileEmitter"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                sol::table transform = entity["components"]["transform"];
                sol::table emitter = entity["components"]["projectileEmitter"];

                int parentEntityXPos = transform["position"]["x"];
                int parentEntityYPos = transform["position"]["y"];
                int parentEntityWidth = transform["width"];
                int parentEntityHeight = transform["height"];
                int projectileWidth = emitter["width"];
                int projectileHeight = emitter["height"];
                int projectileSpeed = emitter["speed"];
                int projectileRange = emitter["range"];
                int projectileAngle = emitter["angle"];
                bool projectileShouldLoop = emitter["shouldLoop"];
                std::string projectileTextureId = emitter["textureAssetId"];

                Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
                projectile.AddComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.AddComponent<SpriteComponent>(projectileTextureId.c_str());
                projectile.AddComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.AddComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }
        }
        entityIndex++;
    }
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
    if (mainPlayerTransform != nullptr) {
        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        // Clamping camera to ensure it doesn't go outside boundaries of map
        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;
        camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y > camera.h ? camera.h : camera.y;
    } else {
        camera.x = 0;
        camera.y = 0;
    }
}

void Game::CheckCollisions() {
    CollisionType collisionTagType = manager.CheckCollisions();
    if (collisionTagType == PLAYER_ENEMY_COLLISION) {
        this->ProcessGameOver();
    } else if (collisionTagType == PLAYER_PROJECTILE_COLLISION) {
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

