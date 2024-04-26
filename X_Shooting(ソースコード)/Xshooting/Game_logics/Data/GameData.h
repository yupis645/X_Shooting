#pragma once

#include "../../Game.h"


extern PLAYER P_init;				//プレイヤーの初期ステータス
extern P_SHOT_ST P_SHOT_init;		//プレイヤーの弾の初期ステータス

extern ENEMY_ST  E_Init;			//地上、空中敵共通の初期ステータス
extern E_SHOT_ST  E_SHOT_Init;		//敵弾の初期ステータス

extern ENEMY_ST A_enemyDate[];		//敵種類に応じた初期設定
extern ENEMY_ST G_enemyData[];		//敵種類に応じた初期設定

extern A_ENEMY_TABLE A_Date_Table;	//空中敵の出現テーブル

extern CSV_MAPDATA Map_csvDate[];	//CSVのデータ群

extern  ARIA_SPLIT ARIA_front[];	//フロントマップデータ(地上の絵)
			
extern  ARIA_SPLIT ARIA_back[];		//バックマップデータ(地上敵の配置や空中敵の固定シンボルの配置)

extern PNGDATA PngData;				//プレイヤーの絵など画像データ群
