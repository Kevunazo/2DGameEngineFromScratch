#include "Game/Game.h"
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_mixer.h>
// #include <glm/glm.hpp>
// #include <imgui/imgui.h>
// #include <sol/sol.hpp>


int main(int argc, char* argv[]) {
	
    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}