#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using vec2 = glm::vec2;


struct SpriteComponent {
    SDL_Texture* texture;

    SpriteComponent(
        const char * fileLocation = "",
        SDL_Renderer* renderer = nullptr
    ) {
        SDL_Surface* surface = IMG_Load(fileLocation);
        this->texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);
    }
};

#endif