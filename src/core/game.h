/*
    =====================================
    GAME STATE & CONSTANTS
    =====================================
    
    Core game state management and DS-accurate screen dimensions.
    This module manages the cursor position, selected piece,
    and active pieces in the player's hand.
*/

#ifndef GAME_H
#define GAME_H

#include "piece.h"

/*
    =========================
    DS SCREEN DIMENSIONS
    =========================
    Nintendo DS logical resolution.
    Total screen (both top + bottom): 256x384
    Each screen: 256x192
*/
#define DS_WIDTH 256
#define DS_HEIGHT 384
#define DS_SCREEN_HEIGHT 192

/*
    =========================
    GAME GRID CONFIGURATION
    =========================
    The actual game board grid size and dimensions.
    Note: This is in logical grid units, NOT pixels.
    Each cell is scaled to TILE_PIXEL_SIZE pixels on screen.
*/
#define GRID_SIZE 8

/*
    =========================
    RENDERING SCALE
    =========================
    How many pixels each grid cell occupies on the screen.
    24 pixels per cell = 192x192 pixel game board.
    Fits perfectly on the 256x192 bottom screen with margins.
    (8 cells × 24 pixels = 192 pixels)
    
    Adjustment formula:
      On screen pixels = Grid cells × TILE_PIXEL_SIZE
*/
#define TILE_PIXEL_SIZE 24

/*
    =========================
    SCREEN REGIONS
    =========================
    Screen split between top (UI/pieces) and bottom (game board).
    Grid is centered horizontally on the bottom screen.
*/
#define TOP_Y 0                         /* Top screen start */
#define BOTTOM_Y DS_SCREEN_HEIGHT      /* Bottom screen start */
#define BOARD_OFFSET_Y DS_SCREEN_HEIGHT /* Game board Y offset */
#define BOARD_OFFSET_X ((DS_WIDTH - GRID_PIXEL_SIZE) / 2) /* Center grid horizontally */

/*
    =========================
    DERIVED HELPER
    =========================
    Total pixel size of the game board grid.
*/
#define GRID_PIXEL_SIZE (GRID_SIZE * TILE_PIXEL_SIZE)

/*
    =========================
    BOARD GRID
    =========================
    Tracks placed pieces on the game board.
    0 = empty, 1 = occupied
*/
typedef struct
{
    int cells[GRID_SIZE][GRID_SIZE]; /* Board grid */
} GameBoard;

/*
    Game State Structure
    ====================
    Holds the current state of the game including:
    - Player cursor position on the board
    - Three active piece selections
    - Currently selected piece index
    - The game board grid
*/
typedef struct
{
    int cursorX;                    /* Cursor X position (grid cells) */
    int cursorY;                    /* Cursor Y position (grid cells) */
    
    Piece activePieces[3];          /* Three pieces in player's hand */
    int selectedPiece;              /* Index of currently selected piece (0-2) */
    int pieceUsed[3];               /* Track which pieces have been placed (0=available, 1=used) */
    int usedCount;                  /* Number of pieces placed from current hand */
    
    GameBoard board;                /* Game board grid */
} GameState;

/*
    Initialization
    ==============
*/
void Game_Init(GameState* game);

/*
    Piece Placement
    ===============
    Attempts to place the selected piece at cursor position.
    Returns 1 if successful, 0 if placement is invalid.
    When a piece is successfully placed, it is marked as used.
    When all 3 pieces are used, new pieces are automatically generated.
*/
int Game_PlacePiece(GameState* game);

/*
    Generate New Pieces
    ===================
    Randomly generates 3 new pieces for the player's hand.
    Resets the used piece tracking.
*/
void Game_GenerateNewPieces(GameState* game);

#endif
