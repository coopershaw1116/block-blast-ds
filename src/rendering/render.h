/*
    =====================================
    RENDERING INTERFACE
    =====================================
    
    Handles all visual output to the screen including:
    - Top screen piece selection UI
    - Bottom screen game board
    - Grid rendering
    - Cursor and preview visualization
*/

#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "../core/game.h"

/*
    Render_Draw
    ===========
    Main rendering function called once per frame.
    Draws both DS screens with appropriate layout.
*/
void Render_Draw(SDL_Renderer* renderer, GameState* game);

#endif
