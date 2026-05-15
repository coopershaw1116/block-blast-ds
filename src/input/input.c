/*
    =====================================
    INPUT IMPLEMENTATION
    =====================================
    
    Handles keyboard input for game control.
*/

#include "input.h"

/*
    Input_HandleEvent
    =================
    Processes keyboard input and updates game state.
    
    Key Bindings:
    - LEFT/RIGHT: Move cursor horizontally
    - UP/DOWN: Move cursor vertically
    - Q: Previous piece (cycle backwards)
    - E: Next piece (cycle forwards)
*/
void Input_HandleEvent(SDL_Event* event, GameState* game)
{
    /* Only process key down events */
    if (event->type != SDL_KEYDOWN)
    {
        return;
    }

    switch (event->key.keysym.sym)
    {
        /*
            Cursor Movement
            ===============
        */
        case SDLK_LEFT:
            if (game->cursorX > 0)
                game->cursorX--;
            break;

        case SDLK_RIGHT:
            if (game->cursorX < GRID_SIZE - 1)
                game->cursorX++;
            break;

        case SDLK_UP:
            if (game->cursorY > 0)
                game->cursorY--;
            break;

        case SDLK_DOWN:
            if (game->cursorY < GRID_SIZE - 1)
                game->cursorY++;
            break;

        /*
            Piece Selection Cycling
            ======================
            Skip pieces that have already been used
        */
        case SDLK_q:
        {
            int prevSelected = game->selectedPiece;
            game->selectedPiece--;

            if (game->selectedPiece < 0)
                game->selectedPiece = 2;

            /* Skip if piece is already used */
            if (game->pieceUsed[game->selectedPiece])
            {
                game->selectedPiece = prevSelected;
            }
            break;
        }

        case SDLK_e:
        {
            int prevSelected = game->selectedPiece;
            game->selectedPiece++;

            if (game->selectedPiece > 2)
                game->selectedPiece = 0;

            /* Skip if piece is already used */
            if (game->pieceUsed[game->selectedPiece])
            {
                game->selectedPiece = prevSelected;
            }
            break;
        }

        /*
            Piece Placement
            ===============
        */
        case SDLK_SPACE:
            Game_PlacePiece(game);
            break;
    }
}
