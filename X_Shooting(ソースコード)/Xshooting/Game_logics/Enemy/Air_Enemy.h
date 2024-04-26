#pragma once


//#include "../../common.h"
#include "../../Game.h"
#include "../Data/GameData.h"


void A_Enemy_Moving(PLAYER*, ENEMY_ST*, ENEMY_SHOT*, int);
int A_Enemy_Anim(ENEMY_ST, int _frame, int cycle);

void TOROID(float cx,float cy,int boxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int, bool shot_ok, int shotnum);
void TORKAN(float cx, float cy, ENEMY_ST* _e, ENEMY_SHOT* _s, int, bool shot_ok, int shotnum);
void GIDDOSPARIO(ENEMY_ST* _e , int _frame); 
void ZOSHI(float cx, float cy, ENEMY_ST* _e, ENEMY_SHOT* _s, int, bool shot_ok, int shotnum);
void JARA(float cx, float cy, int boxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int, bool shot_ok, int shotnum);
void KAPI(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame , bool shot_ok, int shotnum);
void TERRAZI(float cx, float cy, int boxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int, bool shot_ok, int shotnum);
void ZAKATO(float cx, float cy, int boxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int, bool shot_ok, int shotnum);
void BRAGZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame , bool shot_ok, int shotnum);

void GARUZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame , bool shot_ok, int shotnum);

void BACURA(ENEMY_ST* _e, int _frame );