/*
    =====================================
    RENDERING IMPLEMENTATION
    =====================================
    
    Handles all game rendering:
    - UI piece selection display
    - Game board grid rendering
    - Cursor and piece preview
    - Color coding and visual feedback
*/

#include <stdbool.h>
#include "render.h"

/*
    =====================================
    UI RENDERING HELPERS
    =====================================
*/

/*
    DrawPieceUI
    ===========
    Renders a piece in UI space with optional scaling and outline.
    
    Parameters:
    - renderer: SDL renderer
    - piece: The piece to draw
    - startX, startY: Screen position in pixels
    - alpha: Transparency (0-255)
    - scale: Size multiplier for UI pieces
    - drawOutline: Draw selection outline around piece
*/
void DrawPieceUI(
    SDL_Renderer* renderer,
    Piece* piece,
    int startX,
    int startY,
    int alpha,
    float scale,
    bool drawOutline
)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int y = 0; y < piece->height; y++)
    {
        for (int x = 0; x < piece->width; x++)
        {
            if (!piece->cells[y][x])
                continue;

            SDL_Rect block =
            {
                startX + (int)(x * TILE_PIXEL_SIZE * scale),
                startY + (int)(y * TILE_PIXEL_SIZE * scale),
                (int)(TILE_PIXEL_SIZE * scale),
                (int)(TILE_PIXEL_SIZE * scale)
            };

            SDL_SetRenderDrawColor(renderer, 0, 200, 255, alpha);
            SDL_RenderFillRect(renderer, &block);

            if (drawOutline)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }
}

/*
    GetPieceScale
    ==============
    Computes appropriate scaling for pieces in the UI selection area.
    Larger pieces are scaled down to fit nicely in their slots.
    
    Scaling logic:
    - 1x1 or 2x2: 75% size
    - 3x3: 65% size
    - 4x4: 55% size
    - Larger: 50% size
*/
float GetPieceScale(Piece* piece)
{
    int maxDim = piece->width > piece->height
        ? piece->width
        : piece->height;

    if (maxDim <= 2) return 0.75f;
    if (maxDim == 3) return 0.65f;
    if (maxDim == 4) return 0.55f;
    return 0.5f;
}

/*
    =====================================
    MAIN RENDERING FUNCTION
    =====================================
*/

/*
    Render_Draw
    ===========
    Complete frame rendering function.
    
    Layout:
    - Top screen (0-192px): Piece selection UI
    - Bottom screen (192-384px): Game board with grid
*/
void Render_Draw(SDL_Renderer* renderer, GameState* game)
{
    /*
        Clear Screen with background color
    */
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    /*
        =====================================
        TOP SCREEN: PIECE SELECTION UI
        =====================================
    */
    SDL_Rect topScreen =
    {
        0,
        0,
        DS_WIDTH,
        DS_SCREEN_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &topScreen);

    /*
        Piece Layout Configuration
        ==========================
        Three slots horizontally centered:
        [Slot 1] [Slot 2 - Selected] [Slot 3]
    */
    int centerX = DS_WIDTH / 2;
    int baseY = 32;
    int slotWidth = TILE_PIXEL_SIZE * 3;

    for (int i = 0; i < 3; i++)
    {
        /* Skip rendering used pieces */
        if (game->pieceUsed[i])
            continue;

        Piece* p = &game->activePieces[i];
        float scale = GetPieceScale(p);

        int pieceWidth = (int)(p->width * TILE_PIXEL_SIZE * scale);
        int pieceHeight = (int)(p->height * TILE_PIXEL_SIZE * scale);
        int slotCenterX = centerX + (i - 1) * slotWidth;
        int startX = slotCenterX - pieceWidth / 2;
        int startY = baseY;

        /*
            Draw Selection Highlight
            ========================
            Yellow border around selected piece
        */
        if (i == game->selectedPiece)
        {
            SDL_Rect highlight =
            {
                startX - 8,
                startY - 8,
                pieceWidth + 16,
                pieceHeight + 16
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawRect(renderer, &highlight);
        }

        /*
            Draw Piece
        */
        DrawPieceUI(
            renderer,
            p,
            startX,
            startY,
            255,
            scale,
            false
        );
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    /*
        =====================================
        SCREEN HINGE DIVIDER
        =====================================
        Pure black line representing the DS hinge
        between top and bottom screens
    */
    SDL_Rect hinge =
    {
        0,
        DS_SCREEN_HEIGHT,
        DS_WIDTH,
        4  /* 4 pixel thick hinge */
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hinge);

    /*
        =====================================
        BOTTOM SCREEN: GAME BOARD
        =====================================
    */

    /*
        Board Background
        ================
        Full bottom screen background
    */
    SDL_Rect bottomScreen =
    {
        0,
        BOARD_OFFSET_Y,
        DS_WIDTH,
        DS_SCREEN_HEIGHT
    };

    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderFillRect(renderer, &bottomScreen);

    /*
        Grid Lines
        ==========
        Draw grid cells for the 8x8 game board
        Each cell is TILE_PIXEL_SIZE pixels
    */
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            SDL_Rect cell =
            {
                BOARD_OFFSET_X + x * TILE_PIXEL_SIZE,
                BOARD_OFFSET_Y + y * TILE_PIXEL_SIZE,
                TILE_PIXEL_SIZE,
                TILE_PIXEL_SIZE
            };

            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

    /*
        Placed Pieces
        =============
        Render all placed pieces on the board grid
    */
    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (game->board.cells[y][x] != 0)
            {
                SDL_Rect block =
                {
                    BOARD_OFFSET_X + x * TILE_PIXEL_SIZE,
                    BOARD_OFFSET_Y + y * TILE_PIXEL_SIZE,
                    TILE_PIXEL_SIZE,
                    TILE_PIXEL_SIZE
                };

                SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
                SDL_RenderFillRect(renderer, &block);

                SDL_SetRenderDrawColor(renderer, 60, 100, 200, 255);
                SDL_RenderDrawRect(renderer, &block);
            }
        }
    }

    /*
        =====================================
        PIECE PREVIEW & CURSOR
        =====================================
    */

    /*
        Translucent Piece Preview
        =========================
        Shows where the selected piece will be placed
        Rendered at semi-transparent (alpha=120)
        Only show preview if piece hasn't been used yet
    */
    if (!game->pieceUsed[game->selectedPiece])
    {
        Piece* selected = &game->activePieces[game->selectedPiece];

        DrawPieceUI(
            renderer,
            selected,
            BOARD_OFFSET_X + game->cursorX * TILE_PIXEL_SIZE,
            BOARD_OFFSET_Y + game->cursorY * TILE_PIXEL_SIZE,
            120,
            1.0f,
            true
        );
    }

    /*
        Cursor Outline
        ==============
        Yellow highlight box showing current cursor position
    */
    SDL_Rect cursor =
    {
        BOARD_OFFSET_X + game->cursorX * TILE_PIXEL_SIZE,
        BOARD_OFFSET_Y + game->cursorY * TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE,
        TILE_PIXEL_SIZE
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &cursor);

    /*
        Present Frame
    */
    SDL_RenderPresent(renderer);
}
