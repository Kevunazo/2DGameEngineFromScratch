#include "./AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>



AssetStore::AssetStore() {
    Logger::Success("Registry constructor called!");

}

AssetStore::~AssetStore() {
    Logger::Success("Registry destructor called!");
}

void AssetStore::ClearAssests() {
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& path) {

    SDL_Surface* surface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    textures.insert(std::make_pair(assetId, texture));

}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    return textures.at(assetId);
}