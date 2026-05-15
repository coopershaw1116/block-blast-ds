/*
    PC/SDL test build entry point (desktop only).
*/

#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "../../core/game.h"
#include "../platform.h"

static SDL_Window* s_window;
static SDL_Renderer* s_renderer;
static bool s_running = true;

void Platform_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return;
    }

    s_window = SDL_CreateWindow(
        "BLOCK BLAST DS (PC test)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DS_WIDTH,
        DS_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!s_window)
    {
        return;
    }

    s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_ACCELERATED);
}

void Platform_Shutdown(void)
{
    if (s_renderer)
    {
        SDL_DestroyRenderer(s_renderer);
        s_renderer = NULL;
    }

    if (s_window)
    {
        SDL_DestroyWindow(s_window);
        s_window = NULL;
    }

    SDL_Quit();
}

void Platform_FrameWait(void)
{
    SDL_Delay(16);
}

void Platform_EndFrame(void)
{
}

SDL_Renderer* PC_GetRenderer(void)
{
    return s_renderer;
}

bool PC_IsRunning(void)
{
    return s_running;
}

void PC_SetRunning(bool running)
{
    s_running = running;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    Platform_Init();

    if (!s_renderer)
    {
        Platform_Shutdown();
        return 1;
    }

    GameState game;
    Game_InitSeeded(&game, (unsigned int)time(NULL));

    while (PC_IsRunning())
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                PC_SetRunning(false);
            }
        }

        Input_Update(&game);
        Game_Update(&game);
        Render_Draw(&game);
        Platform_FrameWait();
    }

    Platform_Shutdown();
    return 0;
}
