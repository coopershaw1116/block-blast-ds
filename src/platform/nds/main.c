/*
    NDS hardware entry point (flashcart .nds build).
*/

#include <stdint.h>
#include <nds.h>

#include "../../core/game.h"
#include "../platform.h"

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    Platform_Init();

    GameState game;
    {
        volatile uint16_t* keyinput = (volatile uint16_t*)0x04000130;
        volatile uint16_t* vcount = (volatile uint16_t*)0x04000006;
        unsigned int seed = 0xB10C4A57u ^
            ((unsigned int)*vcount | ((unsigned int)*keyinput << 16));
        Game_InitSeeded(&game, seed);
    }

    while (1)
    {
        Input_Update(&game);
        Game_Update(&game);
        Render_Draw(&game);
        Platform_EndFrame();
    }

    return 0;
}
