/*
    =====================================
    PIECE DEFINITIONS & DATA STRUCTURES
    =====================================
    
    Defines the piece types and shapes available in the game.
    Pieces are represented as 2D grids with cells marked as
    active (1) or inactive (0).
    
    Max dimensions support shapes up to 5x5 cells.
*/

#ifndef PIECE_H
#define PIECE_H

/*
    Piece Structure
    ===============
    Stores the dimensions and cell layout of a game piece.
*/
#define PIECE_MAX_SIZE 5

typedef struct
{
    int width;                                  /* Piece width in cells */
    int height;                                 /* Piece height in cells */
    int cells[PIECE_MAX_SIZE][PIECE_MAX_SIZE]; /* Cell grid: 1=filled, 0=empty */
} Piece;

/*
    Global Piece Library
    ====================
    Array of all available piece definitions and count.
    These are randomly selected when the game initializes.
*/
extern Piece pieceDefinitions[];
extern int pieceDefinitionCount;

#endif
