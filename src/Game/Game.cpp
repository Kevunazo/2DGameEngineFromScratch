#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>


#include <iostream>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

// Define screen dimensions
#define SCREEN_WIDTH    600
#define SCREEN_HEIGHT   600

Game::Game() {
    isRunning = false;
    registry = std::make_unique<Registry>();
    Logger::Success("Game constructor called!");

}

Game::~Game() {
    // TODO...
    Logger::Err("Game destructor called!");;

}

/**
 * Initializing SDL components
*/
void Game::Initialize() {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Err("Error initializing SDL. ");
        return;
    }

    #if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
        // Disable compositor bypass
        if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
        {
            printf("SDL can not disable compositor bypass!\n");
            return;
        }
    #endif



    // Continue initialization
    window = SDL_CreateWindow(
        "My Game... my rules",
        2600,
        300,
        // SDL_WINDOWPOS_CENTERED,
        // SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        Logger::Err("Error creating SDL window.");
        return;
    }

    renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!renderer) {
        Logger::Err("Error creating SDL renderer.");
        return;
    }
}

/**
 * Initialize Game Objects
*/
void Game::Setup() {
    Entity e1 = registry->SpawnEntity();
    Entity e2 = registry->SpawnEntity();
    Entity e3 = registry->SpawnEntity();


    registry->AddComponentToEntity<TransformComponent>(e1, glm::vec2(10, 5), glm::vec2(1, 2), 180.0);
    registry->AddComponentToEntity<RigidBodyComponent>(e1, glm::vec2(100.0, 50.0));


}

/**
 * Main Loop
*/
void Game::Run() {
    Setup();
    isRunning = true;

    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        if (sdlEvent.type == SDL_QUIT) {
                isRunning = false;
                break;
        } else if (sdlEvent.type == SDL_KEYDOWN) {
            Logger::Log("A key was pressed");
            switch (sdlEvent.key.keysym.sym) {
                // Escape Key
                case SDLK_ESCAPE: {
                    Logger::Log("The scape key was pressed");
                    isRunning = false;
                    break;
                }
                // case SDLK_LEFT: {
                //     playerPosition.x -= playerVelocity.x * deltaTime;
                //     break;
                // }
                // case SDLK_RIGHT: {
                //     playerPosition.x += playerVelocity.x * deltaTime;
                //     break;
                // }
                // case SDLK_UP: {
                //     playerPosition.y -= playerVelocity.y * deltaTime;
                //     break;
                // }
                // case SDLK_DOWN: {
                //     playerPosition.y += playerVelocity.y * deltaTime;
                //     break;
                // }
            }
        }
    }
}

void Game::Update() {
    // Make sure we wait enough between frames to keep a consistent FPS
    int32_t milisecondsSinceLastUpdate = SDL_GetTicks() - endTimeAtPreviousFrame;
    // Logger::Log("milisecondsSinceLastUpdate -> " + std::to_string(milisecondsSinceLastUpdate));

    // If it has been less than the ms assigned per frame (16 ms @ 60fps), wait a bit
    int timeToWait = MILLISECONDS_PER_FRAME - milisecondsSinceLastUpdate;
    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
        // Logger::Log("Waiting miliseconds -> " + std::to_string(timeToWait));
        SDL_Delay(timeToWait);
    }

    
    deltaTimeSec = (SDL_GetTicks() - endTimeAtPreviousFrame) / 1000.0;
    // Logger::Log(std::to_string(deltaTimeSec));
    endTimeAtPreviousFrame = SDL_GetTicks();

    // Update Registry
    registry->Update();


    // TODO: Update all the systems that have to be run every frame
    // MovementSystem.Update();
    // CollisionSystem.Update();
    // DamageSystem.Update();
}

void Game::Render() {
    // BG Color Mechanism :)
    RenderMovingColor();

    // TODO: Render Game Objects    
    SDL_RenderPresent(renderer);
}

void Game::RenderMovingColor() {
    if (count == 0) increasing = true;
    if (count == 250) increasing = false;
    count = increasing ? count + 1 : count - 1;
    SDL_SetRenderDrawColor(renderer, count, 120, 90, 1);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, count -50, 100, 90, 200);
    SDL_Rect dstRect = { 
        static_cast<int>(100), 
        static_cast<int>(100), 
        16, 
        32
    };

    SDL_RenderFillRect(renderer, &dstRect);

    // Drawing a PNG Texture
    SDL_Surface* surface = IMG_Load("./assets/images/tree.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    
    SDL_DestroyTexture(texture);
}

/**
 * Destroy SDL components
*/
void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}