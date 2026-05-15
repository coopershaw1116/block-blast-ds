/*
    =====================================
    PIECE IMPLEMENTATION
    =====================================
    
    Piece library containing all available block shapes.
*/

#include "piece.h"

/*
    Available Piece Definitions
    ===========================
    Each piece is defined with its width, height, and cell layout.
*/
Piece pieceDefinitions[] =
{
    /* 1x1 Square */
    {
        1, 1,
        {
            {1}
        }
    },

    /* 2x1 Horizontal Line */
    {
        2, 1,
        {
            {1, 1}
        }
    },

    /* 1x2 Vertical Line */
    {
        1, 2,
        {
            {1},
            {1}
        }
    },

    /* 2x2 Square */
    {
        2, 2,
        {
            {1, 1},
            {1, 1}
        }
    },

    /* 3x1 Horizontal Line */
    {
        3, 1,
        {
            {1, 1, 1}
        }
    },

    /* 3x2 L-Shaped Piece */
    {
        3, 2,
        {
            {1, 0, 0},
            {1, 1, 1}
        }
    }
};

/*
    Piece Count
    ===========
    Automatically calculated from the array size.
*/
int pieceDefinitionCount =
    sizeof(pieceDefinitions) / sizeof(Piece);
