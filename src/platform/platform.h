/*
    Platform abstraction for NDS and PC test builds.
    No SDL or libnds includes — portable game code only.
*/

#ifndef PLATFORM_H
#define PLATFORM_H

#include "../core/game.h"

void Platform_Init(void);
void Platform_Shutdown(void);
void Input_Update(GameState* game);
void Render_Draw(GameState* game);
void Platform_EndFrame(void);
void Platform_FrameWait(void);

#endif
