#pragma once

#include "common.h"
//#include "Game_logics/Date/GameData.h"

//-------------------------------------------------------------------------------------------
//			Gameで使う構造体やプロトタイプ宣言をしている
// 
//構造体は ・プレイヤー ・空中敵 ・地上敵 ・ボス　・マップ ・各種画像データ など
// プロトタイプ宣言は ゲームループ,各オブジェクトの初期化、生成、更新、描画
//-------------------------------------------------------------------------------------------


//==============================================================
//					/*プレイヤーの入力判定*/
// 
//==============================================================

#define DIR_LEFT  (1)		//移動キーの入力
#define DIR_RIGHT (2)
#define DIR_UP	  (3)
#define DIR_DOWN  (4)
#define INPUT_SHOT	(5)		//ショットのボタンを押した場合の判定
#define INPUT_BOM	(6)		//ボムのボタンを押した場合の判定


//==============================================================
//					/*プレイヤー関係の定義*/
// 
//==============================================================
#define MAX_SHOT (3)		//画面内に表示できるショットの数
#define SHOT_SPEED	(20)	//ショットの弾速
#define BOM_SPEED	(5)		//ショットの弾速
#define PL_SPEED	(5)		//プレイヤーの移動速度	
#define BOM_ST	  (100)		//ボムの射程距離

//==============================================================
//					/*エネミー共通の定義*/
// 
//==============================================================
#define A_ENEMY_PIC_LINE	(8)		//空中敵の画像の行の数
#define G_ENEMY_PIC_LINE	(4)		//地上敵の画像の行の数
#define MAX_ENEMY	(20)			//敵の最大出現数(空中と地上は別カウント)
#define MAX_E_SHOT	(19)			//敵弾の最大出現数
#define E_SHOT_SPEED	(4)			//敵弾の基本の速度
#define MAX_LEVEL	(64)			//敵の出現レベルの上限




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
												
																/*構造体の宣言*/

//------------------------------------------------------/*プレイヤー関連*/-------------------------------------------------------

//============================================
// ショットの構造体
//============================================
typedef struct P_SHOT_ST {
	RECT hitbox;			//座標(x,y軸	)
	float center_x;		//中心座標x
	float center_y;		//中心座標y
	bool F;				//弾が画面内に存在するかの判定
	int sp1;			//特殊処理がある場合に適宜使用する変数
	int sp2;			//同上
}P_SHOT_ST;

//============================================
// ボムの構造体
//============================================
typedef struct P_BOM_ST {
	RECT hitbox;
	float center_x;		//中心座標x
	float center_y;		//中心座標y
	bool F;				//発射フラグ
	int range;			//射程距離
	bool bomber;		//爆発フラグ(攻撃判定)
}P_BOM_ST;

//============================================
// プレイヤー構造体
//============================================
typedef struct PLAYER {
	Bmp* pic[12];				//画像ポインタ(自機)
	Bmp* bullet[3];				//画像ポインタ(弾)
	RECT hitbox;				//当たり判定
	int pic_x;					//画像の左上x座標
	int pic_y;					//画像の左上y座標
	int pic_size;				//画像サイズ
	int hitbox_size;			//当たり判定のサイズ
	float center_x, center_y;	//自機の中心座標
	int anim;					//アニメーションパターンの切り替えタイミング
	int pat;					//アニメーションパターン番号
	bool hit;					//被弾判定
	int shot_count;				//画面に表示する弾の数(defineにあるMAX_SHOTの数値が最大)
	RECT sight_hitbox;			//照準の当たり判定(ボムで破壊できる物に反応する)
	bool lookon;				//照準が破壊可能スプライトと重なっている状態の判定
	P_SHOT_ST shot[MAX_SHOT];	//通常弾(対空)
	P_BOM_ST bom;				//特殊弾(対地)
}PLAYER_ST;

//------------------------------------------------------/*敵関連*/-------------------------------------------------------


//============================================
// 空中、地上共通の弾の構造体
//============================================
typedef struct E_SHOT_ST {
	Bmp* pic[2];		//画像ポインタ(弾)
	bool F;				 //使用:1  未使用:0
	RECT hitbox;		//弾の当たり判定
	float center_x;		//中心座標x
	float center_y;		//中心座標y
	int pl_x, pl_y;		//発射した瞬間の自機の位置を記憶する
	float rad;			//進む角度(発射する際のプレイヤーに対する角度)
	float distance;		
}E_SHOT_ST;

//============================================
//  空中、地上共通のステータス
//============================================
typedef struct ENEMY_ST {
	int number;			//エネミーの種類を区別するための番号
	RECT hitbox;		//座標
	int hitbox_size;	//当たり判定のサイズ
	bool frame_in;		//画面内にいるかを判別するためのフラグ
	bool hit;			//被弾判定
	float rad;			//角度
	float center_x;		//中心のx座標
	float center_y;		//中心のy座標
	int pic_x;			//画像左上のx座標
	int pic_y;			//画像左上のy座標
	int pic_size;		//画像サイズ
	int back_num;		//配置されている裏マップの番号
	int back_coord;		//裏マップにおける配置
	int type;		//同一個体の違う挙動
	int dir;		//向き
	int anim;		//アニメーションパターンの切り替えタイミング
	int anim_sum;	//行動パターンの数
	int pat;		//行動パターン番号
	int points;		//ポイント
	float speed;	//移動速度
	int ac_count;	//初期化された瞬間からカウントを開始する(行動パターンに使用する)
	int move_x;		//x軸の移動量
	int move_y;		//y軸の移動量
	float acc;		//加速度
	Bmp* pic[A_ENEMY_PIC_LINE * 11];		//画像ポインタ
}ENEMY_ST;


//============================================
//出現する空中敵の出現テーブル
//============================================
typedef struct A_ENEMY_TABLE {
	int Lv;
	int count;
	int table[MAX_LEVEL * 6];
}A_ENEMY_TABLE;


//============================================
//ボス & 砲台、コアのステータス
//============================================
typedef struct BOSS {
	bool F;				//出現フラグ
	bool down;			//撃墜判定(コア破壊)
	ENEMY_ST enemy[5];	//砲台×4とコアの情報
	float center_x;		//x軸中心座標
	float center_y;		//y軸中心座標
	int pic_x;			//左上x座標(スプライト用)
	int pic_y;			//左上y座標(スプライト用)
	Bmp* pic[120];				//本体を分割したスプライト
	Bmp* pats[5];				//撃破前の砲台とコアの画像
}BOSS;

//============================================
// 敵を一括管理する構造体
//============================================
typedef struct ENEMY {
	bool pop_F;			//出現フラグ
	int count;			//敵の出現数カウント
	ENEMY_ST enemy[MAX_ENEMY];	//敵の設定。MAX_ENEMYの値で最大出現数が変化する
}ENEMY;


//============================================
// 敵の弾を一括管理する構造体
//============================================
typedef struct ENEMY_SHOT {
	E_SHOT_ST shot[MAX_E_SHOT];		//弾の設定。MAXの値で弾の画面に出現する最大個数が変動する
}ENEMY_SHOT;

//------------------------------------------------------/*マップ関連*/-------------------------------------------------------


//============================================
// マップデータを保存する構造体
//============================================
typedef struct MAP_DATE {
	bool F;						//描写フラグ
	int data[MAP_H * MAP_W];	//エリア情報
	int x, y;					//マップの左上座標
}MAP_TILE;

//============================================
//　マップの情報を一括管理する構造体
//============================================
typedef struct MAP {
	int stage;					//現在のステージ番号を記憶する
	int split_num;				//現在のエリア番号
	int x;						//ステージ全体の座標
	int y;
	MAP_DATE split[5];			//ステージを5つに分割したマップデータを記憶する
	MAP_DATE backmap[5];		//splitと同じように動き、敵の配置や固定出現などを記憶する
	int px;						
	int py;
	float rate;				
	Bmp* pic[12 * 10];			//マップチップ格納
	const wchar_t* filename[5];//画像を保存する
}MAP_DATA;

//============================================
//マップの画像を保存する構造体
//============================================
typedef struct MAP_PIC_DATA{
	Bmp* pic[5];				//1エリア(ステージ全体を５つに分割したデータ)を記憶する
	const wchar_t* filename[5];	//画像
}MAP_PIC_DATA;


//============================================
//1ステージを4分割した内の 1/4 を示すデータ構造体
//============================================
typedef struct ARIA_SPLIT_DATE {
	int date[MAP_H * MAP_W];
}ARIA_SPLIT_DATE;

//================================================
//1ステージを4分割した構造体 + 一面森でできたAREA0
//================================================
typedef struct ARIA_SPLIT {
	ARIA_SPLIT_DATE split[5];
}ARIA_SPLIT;


//============================================
//CSVファイルを読み込む構造体
//============================================
typedef struct CSV_MAPDATA {
	const char* Front[5];
	const char* Back[5];
}CSV_MAPDATA;


//-----------------------------------------------------/*汎用*/--------------------------------------------------------------

//============================================
//プレイヤー、敵で共通のエフェクト
//============================================
typedef struct EFFECT {
	Bmp* DieEffect[12];
	int pat;
}EFFECT;

//============================================
//画像データを保管する構造体
//============================================
struct PNGDATA {
	const wchar_t* Player;
	const wchar_t* Shot;
	const wchar_t* A_Enemy;
	const wchar_t* G_Enemy;
	const wchar_t* Boss;
	const wchar_t* BossSub;
	const wchar_t* bomber;

};

//============================================
//デバック用の構造体
//============================================
struct DenugState {
	bool G_enemy_stop;
	bool A_enemy_stop;
	bool Player_invalid;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////関数プロトタイプ宣言///////////////////////////////////////////////////////////////


//=========================================================
//					ゲームループ
//
//=========================================================


void Game_Init(SCENE* _sc);	//初期化
void Game_Input(SCENE* _sc);				//入力
void Game_Update(SCENE* _sc);	//更新
void Game_Draw(SCENE* _sc);	//描画
void Game_End(SCENE* _sc);					//終了処理
void Game(SCENE* _sc);						//ゲーム画面

//=========================================================
//					プレイヤー
// 初期化、オブジェクト作成、入力処理、更新、描写
//=========================================================
void Player_Init(PLAYER*);
void Player_Create(PLAYER*);
void Player_Move(PLAYER*, int);
void Player_Update(PLAYER*, ENEMY*_ae,ENEMY*_ge, ENEMY_SHOT*, SCENE* _sc);
void Player_Draw(PLAYER*,EFFECT*);

//=========================================================
//					敵の出現テーブル
//
//=========================================================
void Air_Enemy_table(ENEMY*, PLAYER*,int level, int map_splitNum,bool boss_flag,bool boss_down);
void BackMap_Enemy_table(ENEMY*, ENEMY*, PLAYER*, MAP* _m, int _frame, BOSS* _b);

//=========================================================
//					   空中敵
//初期化、オブジェクト作成、更新、描画
//=========================================================
void Air_Enemy_Init(ENEMY*);
void Air_Enemy_Create(ENEMY* _e,int number, int Player_x);
void Air_Enemy_Update(PLAYER* _p, ENEMY*, ENEMY_SHOT*, SCENE* _sc);
void Air_Enemy_Draw(ENEMY*, EFFECT*);

//=========================================================
//					   地上敵
//初期化、オブジェクト作成、更新、描画
//=========================================================
void G_Enemy_Init(ENEMY*);
void G_Enemy_Create(ENEMY* _e,int number,int back_coord,int back_num,float center_x);
void G_Enemy_Update(PLAYER* _p, ENEMY*, ENEMY_SHOT*, SCENE* _sc, MAP*, EFFECT*);
void G_Enemy_Draw(ENEMY*, EFFECT*);

//=========================================================
//					   ボス
//初期化、オブジェクト作成、更新、描画
//=========================================================
void Boss_Init(BOSS* _b);
void Boss_Create(BOSS* _b);
void Boss_Update(PLAYER* _p, BOSS*, ENEMY_SHOT*, SCENE* _sc,  EFFECT*);
void Boss_Draw(BOSS*, EFFECT*);

//=========================================================
//					   敵弾
//初期化、オブジェクト作成、更新、描画
//=========================================================
void E_Shot_Init(ENEMY_SHOT*);
void E_Shot_Create(ENEMY_SHOT*, float px,float py, float x, float y, float rad);
void E_Shot_Update(PLAYER* _p, ENEMY_SHOT*, SCENE* _sc);
void E_Shot_Draw(ENEMY_SHOT*);


/*敵の種類によって動きを変える*/
void Enemy_Patterns(ENEMY_ST*,  int pat);

//=========================================================
//					マップの処理
// 初期化、更新、描画処理
//=========================================================

void MAP_Init(MAP*);
void MAP_Update(MAP*, SCENE*);
void MAP_Draw(MAP*);
//void MAP_Init(GAME_STATE*);
//void MAP_Update(GAME_STATE*, SCENE*);
//void MAP_Draw(GAME_STATE*);

//=========================================================
//			ゲームのリトライやリザルト画面への遷移
// 初期化、更新、描画処理
//=========================================================
void GameStateChange( SCENE* _SC);

//=========================================================
//					ポーズ(一時停止処理)
//=========================================================
void Pose();

//=========================================================
//			２つのRECT構造体が接触しているか判別する
//=========================================================
bool Hit_box(RECT, RECT);


//=========================================================」
//			//当たり判定と画像の同期
//=========================================================

void Hitbox_Sync(RECT* hitbox, int size, float* x, float* y,int* pic_x,int* pic_y,int pic_size);

//=========================================================」
//					デバック関連
// ゲームのステータス表示、オブジェクトのhitbox可視化
// 任意敵を出現させるコマンド
//=========================================================
void DebugDraw_Status(MAP, SCENE,bool _b_F);
void DebugDraw_PlayerDraw(PLAYER _p);
void DebugDraw_A_EnemyDraw(ENEMY _ae);
void DebugDraw_G_EnemyDraw(ENEMY _ge);
void DebugDraw_BossDraw(BOSS _b);

void DebugInput(ENEMY* ge, ENEMY* ae, BOSS* b, int Pl_x, int map_y);

//=========================================================」
//					空き番号を探す
//=========================================================
int SetEmptyNum(ENEMY _e);
int E_shotGetEmptyNum(ENEMY_SHOT _es);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

											//変数のextern宣言
extern SCENE scGame;
