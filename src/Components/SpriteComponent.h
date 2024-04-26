#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <glm/glm.hpp>
#include "../ECS/ECS.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using vec2 = glm::vec2;


struct SpriteComponent {
    std::string assetId;
    double width;
    double height;
    SDL_Rect srcRect;

    SpriteComponent(
        std::string assetId = "",
        int width = 0,
        int height = 0,
        int srcRectX = 0,
        int srcRectY = 0
    ) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;

        this->srcRect = {srcRectX, srcRectY, width, height};
    }
};

#endif