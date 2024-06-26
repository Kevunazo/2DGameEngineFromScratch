#ifndef GAME_H
#define GAME_H

# include "../ECS/ECS.h"
# include <SDL2/SDL.h>

const int FPS = 120;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {

    private:
        bool isRunning;
        int count = 0;
        bool increasing = false;
        int endTimeAtPreviousFrame = 0;

        SDL_Window* window;
        SDL_Renderer* renderer;

        std::unique_ptr<Registry> registry;


    public:
        int windowWidth;
        int windowHeight;
        double deltaTimeSec = 0;


        Game();
        ~Game();

        void Initialize();
        void Setup();
        void Run();
        void ProcessInput();
        void Update();
        void Render();
        void RenderMovingColor();
        void Destroy();
};

#endif