#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#define SCREEN_WIDTH    600
#define SCREEN_HEIGHT   600

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderSystem : public System {
    public:
        RenderSystem(){
            RequireComponent<SpriteComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Renderer* renderer) {
            // Loop thru all entities that the movement system is interested on
            for (Entity entity: GetSystemEntities()) {
                Logger::Log("Rendering entity ID = " + entity.toString());
                // TODO RENDERING
                SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
                TransformComponent& transform = entity.GetComponent<TransformComponent>();

                SDL_Rect dstRect = { 
                    static_cast<int>(transform.position.x) % SCREEN_WIDTH, 
                    static_cast<int>(transform.position.y) % SCREEN_HEIGHT, 
                    16, 
                    32
                };

                SDL_RenderCopy(renderer, sprite.texture, NULL, &dstRect);
            }
            // SDL_Surface* surface = IMG_Load("./assets/images/tree.png");
            // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            // SDL_FreeSurface(surface);

            // SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            
            // SDL_DestroyTexture(texture);
        }
};

#endif