/*
    =====================================
    BLOCK BLAST DS - MAIN ENTRY POINT
    =====================================
    
    Handles SDL2 window/renderer initialization,
    game loop, and input/render coordination.
*/

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "../rendering/render.h"
#include "../input/input.h"

/*
    Main Game Loop
    ==============
    - Initializes SDL2 and window
    - Runs event loop with 60 FPS target
    - Coordinates rendering and input handling
    - Cleans up resources on exit
*/
int main(int argc, char* argv[])
{
    /*
        Initialize SDL2
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    /*
        Create Game Window
        ==================
        DS resolution (256x384) displayed at native size
    */
    SDL_Window* window = SDL_CreateWindow(
        "BLOCK BLAST DS",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DS_WIDTH,
        DS_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        SDL_Quit();
        return 1;
    }

    /*
        Create Renderer
        ===============
        Hardware accelerated rendering for better performance
    */
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    /*
        Initialize Game State
    */
    GameState game;
    Game_Init(&game);

    /*
        Main Game Loop
        ==============
        Runs until window is closed (SDL_QUIT event)
        Target: 60 FPS (16ms per frame)
    */
    bool running = true;

    while (running)
    {
        /*
            Process Input Events
        */
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            Input_HandleEvent(&event, &game);
        }

        /*
            Render Frame
        */
        Render_Draw(renderer, &game);

        /*
            Frame Timing
            ============
            16ms delay ≈ 60 FPS
        */
        SDL_Delay(16);
    }

    /*
        Cleanup Resources
    */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
