/*
    NDS libnds rendering — MODE_5 16-bit bitmap on both screens.
*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <nds.h>

#include "../../core/game.h"
#include "../platform.h"

#define SCREEN_W 256
#define SCREEN_H 192

/*
 * Sub-screen: smaller tiles + extra top inset (real DS LCDs clip top scanlines).
 * 8*18=144px grid, Y=36 leaves 12px at bottom within the 192px framebuffer.
 */
#define NDS_TILE_PX 18
#define NDS_GRID_PX (GRID_SIZE * NDS_TILE_PX)
#define NDS_BOARD_X ((SCREEN_W - NDS_GRID_PX) / 2)
#define NDS_BOARD_Y 36

#define BMP_STRIDE 256
#define BMP_PAGE_WORDS (BMP_STRIDE * BMP_STRIDE)

static int s_bgMain;
static int s_bgSub;
static u16* s_mainBuf;
static u16* s_subDrawBuf;

/* Bmp16 pixels are aBBBBBGGGGGRRRRR — bit 15 must be set or the pixel is invisible */
static u16 ColorFrom8(u8 r, u8 g, u8 b)
{
    return ARGB16(1, r >> 3, g >> 3, b >> 3);
}

static void FillRect(u16* buf, int x, int y, int w, int h, u16 color)
{
    if (x < 0)
    {
        w += x;
        x = 0;
    }

    if (y < 0)
    {
        h += y;
        y = 0;
    }

    if (x + w > SCREEN_W)
    {
        w = SCREEN_W - x;
    }

    if (y + h > SCREEN_H)
    {
        h = SCREEN_H - y;
    }

    if (w <= 0 || h <= 0)
    {
        return;
    }

    for (int row = y; row < y + h; row++)
    {
        for (int col = x; col < x + w; col++)
        {
            buf[row * BMP_STRIDE + col] = color;
        }
    }
}

static void DrawRectOutline(u16* buf, int x, int y, int w, int h, u16 color)
{
    FillRect(buf, x, y, w, 1, color);
    FillRect(buf, x, y + h - 1, w, 1, color);
    FillRect(buf, x, y, 1, h, color);
    FillRect(buf, x + w - 1, y, 1, h, color);
}

/* alpha5 is 0..31 (Bmp16 has no real translucency; used only for clear FX) */
static u16 Blend15(u16 fg, u16 bg, int alpha5)
{
    if (alpha5 <= 0)
    {
        return bg;
    }

    if (alpha5 >= 31)
    {
        return fg;
    }

    int fr = fg & 31;
    int fg_g = (fg >> 5) & 31;
    int fb = (fg >> 10) & 31;
    int br = bg & 31;
    int bg_g = (bg >> 5) & 31;
    int bb = (bg >> 10) & 31;
    int inv = 31 - alpha5;

    int r = (fr * alpha5 + br * inv) / 31;
    int g = (fg_g * alpha5 + bg_g * inv) / 31;
    int b = (fb * alpha5 + bb * inv) / 31;

    return ARGB16(1, (u8)r, (u8)g, (u8)b);
}

static bool CellIsClearing(const GameState* game, int x, int y)
{
    return (game->clearingRows & (uint8_t)(1u << y)) != 0 ||
           (game->clearingCols & (uint8_t)(1u << x)) != 0;
}

/* Scale tray pieces as integer num/den (no soft-float on ARM9) */
static void GetPieceScale(Piece* piece, int* num, int* den)
{
    int maxDim = piece->width > piece->height ? piece->width : piece->height;

    if (maxDim <= 2)
    {
        *num = 3;
        *den = 4;
    }
    else if (maxDim == 3)
    {
        *num = 13;
        *den = 20;
    }
    else if (maxDim == 4)
    {
        *num = 11;
        *den = 20;
    }
    else
    {
        *num = 1;
        *den = 2;
    }
}

static void DrawPieceOnBuffer(
    u16* buf,
    Piece* piece,
    int startX,
    int startY,
    int scaleNum,
    int scaleDen,
    bool drawOutline,
    u16 fillColor,
    u16 outlineColor
)
{
    for (int y = 0; y < piece->height; y++)
    {
        for (int x = 0; x < piece->width; x++)
        {
            if (!piece->cells[y][x])
            {
                continue;
            }

            /*
             * Use adjacent cell boundaries so scaled blocks tile with no gaps
             * (independent rounding per cell was leaving holes in the shape).
             */
            int bx = startX + (x * NDS_TILE_PX * scaleNum) / scaleDen;
            int by = startY + (y * NDS_TILE_PX * scaleNum) / scaleDen;
            int bx2 = startX + ((x + 1) * NDS_TILE_PX * scaleNum) / scaleDen;
            int by2 = startY + ((y + 1) * NDS_TILE_PX * scaleNum) / scaleDen;
            int bw = bx2 - bx;
            int bh = by2 - by;

            if (bw < 1)
            {
                bw = 1;
            }

            if (bh < 1)
            {
                bh = 1;
            }

            FillRect(buf, bx, by, bw, bh, fillColor);

            if (drawOutline)
            {
                DrawRectOutline(buf, bx, by, bw, bh, outlineColor);
            }
        }
    }
}

void Platform_Init(void)
{
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);
    lcdMainOnTop();

    /* Map VRAM banks for main/sub 16-bit bitmap backgrounds (devkitPro double-buffer layout) */
    vramSetPrimaryBanks(
        VRAM_A_MAIN_BG_0x06000000,
        VRAM_B_MAIN_BG_0x06020000,
        VRAM_C_SUB_BG,
        VRAM_D_LCD
    );

    /* BG3 is the standard 256x256 16-bit bitmap layer in MODE_5 (see devkitPro examples) */
    s_bgMain = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    s_bgSub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    s_mainBuf = (u16*)bgGetGfxPtr(s_bgMain);

    bgSetScroll(s_bgMain, 0, 0);
    bgSetScroll(s_bgSub, 0, 0);

    /* Draw to the back page; map base 0 is shown first */
    s_subDrawBuf = (u16*)bgGetGfxPtr(s_bgSub) + BMP_PAGE_WORDS;

    /* ~0.5s before repeat, then ~8 repeats per second when held */
    keysSetRepeat(30, 8);
}

static u16* SubDrawTarget(void)
{
    u16* front = (u16*)bgGetGfxPtr(s_bgSub);

    if (bgGetMapBase(s_bgSub) == 0)
    {
        return front + BMP_PAGE_WORDS;
    }

    return front;
}

void Platform_Shutdown(void)
{
}

void Platform_FrameWait(void)
{
    swiWaitForVBlank();
}

void Platform_EndFrame(void)
{
    swiWaitForVBlank();

    if (bgGetMapBase(s_bgSub) == 8)
    {
        bgSetMapBase(s_bgSub, 0);
    }
    else
    {
        bgSetMapBase(s_bgSub, 8);
    }
}

void Render_Draw(GameState* game)
{
    s_subDrawBuf = SubDrawTarget();

    u16 colBgTop = ColorFrom8(30, 30, 30);
    u16 colBgSub = ColorFrom8(25, 25, 25);
    u16 colGrid = ColorFrom8(60, 60, 60);
    u16 colBlock = ColorFrom8(100, 150, 255);
    u16 colBlockEdge = ColorFrom8(60, 100, 200);
    u16 colPiece = ColorFrom8(0, 200, 255);
    u16 colPieceDim = ColorFrom8(0, 100, 128);
    u16 colPieceEdge = ColorFrom8(31, 31, 31);
    u16 colHighlight = ColorFrom8(31, 31, 0);
    u16 colFlash = ColorFrom8(31, 31, 25);

    dmaFillHalfWords(colBgTop, s_mainBuf, SCREEN_W * SCREEN_H * sizeof(u16));
    dmaFillHalfWords(colBgSub, s_subDrawBuf, SCREEN_W * SCREEN_H * sizeof(u16));

    int centerX = SCREEN_W / 2;
    int baseY = 32;
    int slotWidth = TILE_PIXEL_SIZE * 3;

    for (int i = 0; i < 3; i++)
    {
        if (game->pieceUsed[i])
        {
            continue;
        }

        Piece* p = &game->activePieces[i];
        int scaleNum;
        int scaleDen;
        GetPieceScale(p, &scaleNum, &scaleDen);
        int pieceWidth = (p->width * NDS_TILE_PX * scaleNum) / scaleDen;
        int pieceHeight = (p->height * NDS_TILE_PX * scaleNum) / scaleDen;
        int slotCenterX = centerX + (i - 1) * slotWidth;
        int startX = slotCenterX - pieceWidth / 2;
        int startY = baseY;

        if (i == game->selectedPiece)
        {
            DrawRectOutline(
                s_mainBuf,
                startX - 8,
                startY - 8,
                pieceWidth + 16,
                pieceHeight + 16,
                colHighlight
            );
        }

        DrawPieceOnBuffer(
            s_mainBuf,
            p,
            startX,
            startY,
            scaleNum,
            scaleDen,
            false,
            colPiece,
            colPieceEdge
        );
    }

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            int px = NDS_BOARD_X + x * NDS_TILE_PX;
            int py = NDS_BOARD_Y + y * NDS_TILE_PX;

            DrawRectOutline(s_subDrawBuf, px, py, NDS_TILE_PX, NDS_TILE_PX, colGrid);
        }
    }

    /* Ghost preview under placed blocks so it cannot erase them */
    if (!game->pieceUsed[game->selectedPiece])
    {
        Piece* selected = &game->activePieces[game->selectedPiece];

        DrawPieceOnBuffer(
            s_subDrawBuf,
            selected,
            NDS_BOARD_X + game->cursorX * NDS_TILE_PX,
            NDS_BOARD_Y + game->cursorY * NDS_TILE_PX,
            1,
            1,
            false,
            colPieceDim,
            colPieceEdge
        );
    }

    for (int y = 0; y < GRID_SIZE; y++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            if (game->board.cells[y][x] == 0)
            {
                continue;
            }

            int px = NDS_BOARD_X + x * NDS_TILE_PX;
            int py = NDS_BOARD_Y + y * NDS_TILE_PX;
            int size = NDS_TILE_PX;
            int inset = 0;
            u16 fill = colBlock;
            int alpha = 31;

            if (game->phase == GAME_PHASE_CLEARING && CellIsClearing(game, x, y))
            {
                int frame = game->clearAnimFrame;

                if (frame < 6)
                {
                    fill = colFlash;
                }
                else
                {
                    inset = (frame - 6) * 2;
                    if (inset > size / 2)
                    {
                        inset = size / 2;
                    }
                    alpha = 31 - ((frame - 6) * 31 / 5);
                    if (alpha < 0)
                    {
                        alpha = 0;
                    }
                }
            }

            int bw = size - inset * 2;
            int bh = size - inset * 2;

            if (bw > 0 && bh > 0)
            {
                if (alpha >= 31)
                {
                    FillRect(s_subDrawBuf, px + inset, py + inset, bw, bh, fill);
                    DrawRectOutline(s_subDrawBuf, px + inset, py + inset, bw, bh, colBlockEdge);
                }
                else
                {
                    for (int row = py + inset; row < py + inset + bh; row++)
                    {
                        for (int col = px + inset; col < px + inset + bw; col++)
                        {
                            if (col >= 0 && col < SCREEN_W && row >= 0 && row < SCREEN_H)
                            {
                                u16* pixel = &s_subDrawBuf[row * BMP_STRIDE + col];
                                *pixel = Blend15(fill, *pixel, alpha);
                            }
                        }
                    }
                }
            }
        }
    }

    DrawRectOutline(
        s_subDrawBuf,
        NDS_BOARD_X + game->cursorX * NDS_TILE_PX,
        NDS_BOARD_Y + game->cursorY * NDS_TILE_PX,
        NDS_TILE_PX,
        NDS_TILE_PX,
        colHighlight
    );
}
