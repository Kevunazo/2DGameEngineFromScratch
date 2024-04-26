#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#define SCREEN_WIDTH    600
#define SCREEN_HEIGHT   600

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderSystem : public System {
    public:
        RenderSystem(){
            RequireComponent<SpriteComponent>();
            RequireComponent<TransformComponent>();
        }

        void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            // Loop thru all entities that the movement system is interested on
            for (Entity entity: GetSystemEntities()) {

                SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
                TransformComponent& transform = entity.GetComponent<TransformComponent>();

                int posx = static_cast<int>(transform.position.x);
                int posy = static_cast<int>(transform.position.y);

                SDL_Rect dstRect = { 
                    posx, 
                    posy, 
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };

                SDL_RenderCopyEx(
                    renderer, 
                    assetStore->GetTexture(sprite.assetId),
                    &sprite.srcRect, 
                    &dstRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE);
            }
            // SDL_Surface* surface = IMG_Load("./assets/images/tree.png");
            // SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            // SDL_FreeSurface(surface);

            // SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            
            // SDL_DestroyTexture(texture);
        }
};

#endif