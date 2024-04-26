#pragma once

//#include "../../common.h"
#include "../../Game.h"
#include "../Data/GameData.h"

void G_Enemy_Moving(PLAYER*, ENEMY_ST*, ENEMY_SHOT*, int _frame,MAP* _m);
void G_Enemy_Anim(ENEMY_ST*, int _frame, int cycle);

void BARRA();
void ZOLBAK(ENEMY_ST*,int _frame);
void LOGRAM(float cx, float cy, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame);
void DOMOGRAM(float cx, float cy, ENEMY_ST* _e, ENEMY_SHOT* _s, MAP* _m, int _frame);
void DEROTA(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame);
void GROBDA(float px, float py,P_BOM_ST _pb,RECT sight_hitbox , ENEMY_ST* _e,  int _frame);
void BOZALOGRAM();
void SOL(ENEMY_ST*, int _frame);
void GARUBARRA(ENEMY_ST* _e);
void GARUDEROTA(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame);
void SPECIALFLAG(RECT p_hitbox, RECT sight_hitbox, bool* lookon, ENEMY_ST* _e);

//ÉhÉÇÉOÉâÉÄêÍópÇÃèàóù
void AroundSearch(ENEMY_ST* _e, MAP* _m, int pat);
