/*
    =====================================
    INPUT HANDLING INTERFACE
    =====================================
    
    Processes keyboard input and updates game state.
    Supports cursor movement, piece selection, and actions.
*/

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "../core/game.h"

/*
    Input_HandleEvent
    =================
    Processes SDL events and updates game state accordingly.
    
    Controls:
    - Arrow Keys: Move cursor
    - Q/E: Rotate through selected pieces
*/
void Input_HandleEvent(SDL_Event* event, GameState* game);

#endif
