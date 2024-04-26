#pragma once


//#include "../../common.h"
#include "../../Game.h"
#include "../Data/GameData.h"


void Boss_Moving(PLAYER*, ENEMY_ST*, ENEMY_SHOT*, int);

void ALG(float cx, float cy, ENEMY_ST* _e, ENEMY_SHOT* _s);
void CORE(ENEMY_ST* _e);