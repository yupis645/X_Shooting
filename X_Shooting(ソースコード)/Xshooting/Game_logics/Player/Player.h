#pragma once

//#include "../../common.h"
#include "../../Game.h"
#include "../Data/GameData.h"


void Shot_Create(P_SHOT_ST _s[], RECT hitbox, int* count);
void Bom_Create(P_BOM_ST* _b, RECT hitbox, float cx, float cy);

void Shot_Update(P_SHOT_ST _s[], int* count);
void Bom_Update(P_BOM_ST* _b, int size,  int _frameC);

void Shot_Draw(P_SHOT_ST _s[], Bmp _bmpR, Bmp _bmpL);
void Bom_Draw(P_BOM_ST* _b, Bmp _bmp);