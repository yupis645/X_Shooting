#pragma once
#include "conioex.h"
#include "STG.h"
#include "Game.h"


extern PLAYER P_init;
extern P_SHOT_ST P_SHOT_init;

extern ENEMY_ST  E_Init;
extern E_SHOT_ST  E_SHOT_Init;

extern ENEMY_ST A_enemyDate[];
extern ENEMY_ST G_enemyData[];

extern A_ENEMY_TABLE A_Date_Table;

extern CSV_MAPDATA Map_csvDate[];

extern int date0[MAP_H * MAP_W];

extern  ARIA_SPLIT ARIA_front[];

extern  ARIA_SPLIT ARIA_back[];

extern PNGDATA PngData;
