
//#include "../../common.h"
//#include "../../Game.h"
#include "GameData.h"

//---------------------------------------------------------------------------------------
//							/*初期化などゲーム中に参照するデータ群*/
// 
// プレイヤーや敵の構造体を初期ステータスをもった変数群
// ゲームステータスにあるレベルによって出現する敵を変えるエネミーテーブル
// CSVのマップを取り出し、保管するマップデータ変数
//---------------------------------------------------------------------------------------



////////////////////////////////////////////////////////////////////*各ステータスの初期化*///////////////////////////////////////////////////////////////////////////////////

//===========================================================================
//					Playerの初期化
//===========================================================================
PLAYER P_init = {
	{nullptr,},{nullptr,},	//画像ポインタ(自機,弾)
	{0,0,0,0},				//座標(初期配置)
	0 , 0 ,					//画像の座標(x,y)
	32 , 16 ,					//画像サイズ , 当たり判定サイズ
	CENTER_X , CENTER_Y + 150,				//中心x,y座標(float型)		
	2,						//アニメーションパターンの切り替えタイミング
	2,						//アニメーションパターン番号
	false,					//被弾判定
	0,						//画面に表示する弾の数(defineにあるMAX_SHOTの数値が最大)
	{0,0,0,0},				//照準器の判定
	false,					//照準が地上の敵を捉えているかの判定
	{0,0,false,0,0,0,0, false,0,0 , 0, 0, false, 0, 0, },	//通常弾
	0,0,false,0,false									//特殊弾
};

//===========================================================================
//					Playerの弾の初期化
//===========================================================================
P_SHOT_ST P_SHOT_init{
	{0,0,0,0 },			//座標(x,y軸	)
	0.0,		//中心座標x
	0.0,	//中心座標y
	false,				//弾が画面内に存在するかの判定
	0,			//特殊処理がある場合に適宜使用する変数
	0			//同上
};

//===========================================================================
//					Enemyの初期化
//===========================================================================
ENEMY_ST E_Init = {
		0,				//エネミーの種類を区別するための番号
		{0,0,0,0},		//座標(左上x,y軸と右下のx,y軸)
		0,				//hitboxサイズ
		false,			//画面にいるかを判別するためのフラグ
		false,			//被弾判定
		0.0,			//角度
		0.0 , 0.0,		//中心座標XY
		0,
		0 , 0 ,			//画像左上のx,y座標
		0,				//画像サイズ
		0,
		0,
		0,				//向き
		7,				//アニメーションパターンの切り替えタイミング
		0,				//アニメーションパターン番号
		0 ,				//アニメーション番号の合計
		0,				//得点
		0,				//速度
		0,0,			//軸の進む向き
		0,				//カウント
		0.0,				//加速度
		{ nullptr,},
};

//===========================================================================
//					Enemyの弾の初期化
//===========================================================================
E_SHOT_ST E_SHOT_Init = {
	{ nullptr},
	false,    //使用:1  未使用:0
	{0,0,0,0},
	0.0 , 0.0 ,		//中心座標
	0,0,
	0.0,    //進む角度
	0.0,
};

//===========================================================================
//					画像データ
//===========================================================================
PNGDATA PngData = {
	{L"res/EDGE STG自機.png"},
	{L"res/STG 弾.png"},
	{ L"res/STG_A_enemy.png"},
	{ L"res/STG_G_enemy.png"  },
	{ L"res/BOSS.png"},
	{ L"res/BOSS_PATS.png"},
	{L"res/bomber.png"},
};
 

////////////////////////////////////////////////////////////////////*敵のデータテーブル*///////////////////////////////////////////////////////////////////////////////////

//===========================================================================
//					空中敵の初期ステータス
//===========================================================================
ENEMY_ST  A_enemyDate[] = {
	//					番号			hitboxサイズ					 画像サイズ,  anim番号と枚数,得点,速度,        加速度
	//		　　		↓					↓								↓,  　　　     ↓  ↓　　 ↓  ↓            ↓
	/*トーロイド*/		{0  , {0 ,0 ,0 ,0 },32 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0, 0 ,1 ,1  ,0 , 8 ,0 ,30  ,2 ,0 , 0, 0,0.04,{nullptr,}  },
	/*タルケン*/		{1  , {0 ,0 ,0 ,0 },32 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,8  ,7 ,0 ,50  ,4 ,0 , 0, 0,0.0  ,{nullptr,}  },
	/*ギドスパリオ*/	{2  , {0 ,0 ,0 ,0 },16 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 0 ,1  ,16 ,8 ,0 ,10  ,7 ,0 , 0, 0,0.0  ,{nullptr,}  },
	/*ゾシー*/			{3  , {0 ,0 ,0 ,0 },32 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 2 ,1  ,24 ,4 ,0 ,70  ,3 ,0 , 0, 0,0.0  ,{nullptr,}  },
	/*ジアラ*/			{4  , {0 ,0 ,0 ,0 },32 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 0 ,1  ,32 ,6 ,0 ,150 ,4 ,0 , 0, 0,0.04 ,{nullptr,}  },
	/*カピ*/			{5  , {0 ,0 ,0 ,0 },32 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 0 ,1  ,40 ,7 ,0 ,300 ,4 ,0 , 0, 0,0.15 ,{nullptr,}  },
	/*テラジ*/			{6  , {0 ,0 ,0 ,0 },32 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 0 ,1  ,48 ,7 ,0 ,700 ,5 ,0 , 0, 0,0.08  ,{nullptr,}  },
	/*ザカート*/		{7  , {0 ,0 ,0 ,0 },16 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 3 ,1  ,56 ,1 ,0 ,100 ,3 ,0 , 0, 0,0.0  ,{nullptr,}  },
	/*ブラグザカート*/	{8  , {0 ,0 ,0 ,0 },16 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 3 ,1  ,64 ,1 ,0 ,600 ,3 ,0 , 0, 0,0.0  ,{nullptr,}  },
	/*ガルザカート*/	{9  , {0 ,0 ,0 ,0 },16 ,true ,false ,0 ,0 ,0 ,0 ,0 ,32, 0 ,0, 0 ,1  ,72 ,1 ,0 ,1000,3 ,0 , 0, 0,0.0  ,{nullptr,}  },
	/*バキュラ*/		{10 , {0 ,0 ,0 ,0 },48 ,true ,false ,0 ,0 ,0 ,0 ,0 ,48, 0 ,0, 0 ,1  ,56 ,8 ,0 ,   0,2 ,0 , 0, 0,0.0  ,{nullptr,}  },
};
//int number;			//エネミーの種類を区別するための番号
//RECT hitbox;		//座標
//int hitbox_size;	//当たり判定のサイズ
//bool frame_in;		//画面内にいるかを判別するためのフラグ
//bool hit;			//被弾判定
//float rad;			//角度
//float center_x;		//中心のx座標
//float center_y;		//中心のy座標
//int pic_x;			//画像左上のx座標
//int pic_y;			//画像左上のy座標
//int pic_size;		//画像サイズ
//int back_num;		//配置されている裏マップの番号
//int back_coord;		//裏マップにおける配置
//int type;		//同一個体の違う挙動
//int dir;		//向き
//int anim;		//アニメーションパターンの切り替えタイミング
//int anim_sum;	//行動パターンの数
//int pat;		//行動パターン番号
//int points;		//ポイント
//float speed;	//移動速度
//int ac_count;	//初期化された瞬間からカウントを開始する(行動パターンに使用する)
//int move_x;		//x軸の移動量
//int move_y;		//y軸の移動量
//float acc;		//加速度
//Bmp* pic[A_ENEMY_PIC_LINE * 11];		//画像ポインタ


//===========================================================================
//					地上敵の初期ステータス
//===========================================================================
ENEMY_ST G_enemyData[] = {
	//通常の建造物
	//					番号			hitboxサイズ					画像のサイズ,  anim番号と枚数,  得点,速度,          加速度
	//		　　		↓					↓								  ↓   　　　     ↓  ↓　　 ↓   ↓              ↓
	/*バーラ*/			{50 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,1 , 1 ,0 ,100  ,0   , 0, 0 ,0 ,0  ,{nullptr,}  },
	/*ゾルバグ*/		{51 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,4 , 4 ,0 ,200  ,0   , 0, 0 ,0 ,0  ,{nullptr,}  },
	/*ログラム*/		{52 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,4 ,1  ,8 , 4 ,0 ,300  ,0   , 0, 0 ,0 ,0  ,{nullptr,}  },
	/*ドモグラム*/		{53 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,12, 4 ,0 ,800  ,0.75, 0, 0 ,0 ,0.5,{nullptr,}  },
	/*デローダ**/		{54 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,16, 4 ,0 ,200  ,0   , 0, 0 ,0 ,0  ,{nullptr,}  },
	/*グロブダー*/		{55 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,8 ,1  ,20, 4 ,0 ,200 ,0.5 , 0, 0 ,0 ,0.5,{nullptr,}  },
	/*ボザログラム*/	{56 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,24, 1 ,0 ,600  ,0   , 0, 0 ,0 ,0  ,{nullptr,}  },
	/*ソル*/			{57 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,28, 4 ,0 ,2000 ,0   , 0, 0 ,0 ,0  ,{nullptr,}  },

	//大きい建造物
	/*ガルバーラ*/		{58 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,63, 0, 0 ,0 ,1  ,20 , 1 ,0 ,300  ,0  , 0, 0 ,0 ,0 ,{nullptr,}  },
	/*ガルデロータ*/	{59 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,64, 0, 0 ,0 ,1  ,24 , 4 ,0 ,2000 ,0  , 0, 0, 0, 0 ,0 ,0 ,{nullptr,}  },

	//ボス専用
	/*アルゴ*/			{60 , {0 ,0 ,0 ,0 },16, true, false ,0  ,0  ,0 ,0 ,0, 48, 0, 0, 0, 1  , 0, 4, 0, 1000 ,0   , 0, 0, 0, 0, { nullptr, }, },
	/*A/Gコア*/			{61 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,64, 0, 0 ,0 ,1  ,4 , 1 ,0 ,4000 ,0 , 0, 0,0 ,0 ,{nullptr,}  },

	//隠しキャラ
	/*SPフラッグ*/		{62 , {0 ,0 ,0 ,0 },32 ,true ,false ,0  ,0  ,0 ,0 ,0 ,32, 0, 0 ,0 ,1  ,3 , 1 ,0 ,1000 ,0 , 0, 0,0 ,0 ,{nullptr,}  }

};


//===========================================================================
//					空中敵の出現テーブル
//
// 出現する敵は基本 1〜9 で 10のバキュラはテーブルにはない
// 出現するテーブルは 6行 1列 で x列のxにレベルが代入される
// 敵によっては複数の行動パターンがあり、100,200,300… といった百の位でTypeを判断する
// 99で打ち止め(NULL文字のようなもの)
//===========================================================================
A_ENEMY_TABLE A_Date_Table = {
	0,0,
	{   99,   99,   99,   99,   99,   99,		// Lv0:トーロイド　＊ 3
	     0,    0,    0,    0,   99,   99,		// Lv1:トーロイド　＊ 4
	     0,    0,    0,    0,    0,   99,		// Lv2:トーロイド　＊ 5
	     1,    1,   99,   99,   99,   99,		// Lv3:タルケン * 2
	     0,    0,    0,    0,   99,   99,		// Lv4:トーロイド * 4
	     0,    0,    0,  100,   99,   99,		// Lv1:トーロイド　＊ 3
	     0,    0,  100,  100,   99,   99,
	     0,    0,  100,  100,  100,   99,
	     0,    0,  100,  100,  100,   99,
	     1,    1,    1,   99,   99,   99,
	     1,    1,    1,    1,   99,   99,
	     1,    1,    1,    1,    1,   99,
	     2,    2,    2,   99,   99,   99,
	     2,    2,    2,    2,   99,   99,
	     2,    2,    2,    2,    2,   99,
	     3,    3,    3,   99,   99,   99,
	     3,    3,    3,    3,   99,   99,
	     3,    3,    3,    3,    3,   99,
	     4,    4,    4,   99,   99,   99,
	     4,    4,    4,    4,   99,   99,
	     4,    4,    4,    4,    4,   99,
	     7,    7,    7,   99,   99,   99,
	     7,    7,  107,   99,   99,   99,
	     7,    7,  107,  107,   99,   99,	//ザカートA*2 + ザカートB * 2
	   103,  103,  103,   99,   99,   99,
	   103,  103,  103,  103,   99,   99,
	   103,  103,  103,  103,  103,   99,
	   104,    4,    4,    4,   99,   99,
	   104,  104,    4,    4,   99,   99,
	   104,  104,    4,    4,    4,   99,
		 4,    4,    2,    2,    2,   99,
		 4,    4,    2,    2,    2,   99,
	   307,  307,   99,   99,   99,	  99,
	   307,  307,  307,   99,   99,	  99,
	   307,  307,  307,  307,   99,	  99,
	   307,  307,  307,  307,  307,   99,
		 2,    2,    2,    2,    2,   99,
		 2,    2,    2,    2,    2,   99,
		 5,    5,   99,   99,   99,   99,
		 5,    5,    5,   99,   99,   99,
		 5,    5,    5,    5,   99,   99,
		 5,    5,    5,    5,    5,   99,
	   103,  103,  103,  103,  103,   99,
	   103,  103,  103,  103,  103,   99,
	     2,    2,    2,    2,   99,   99,
		 2,    2,    2,    2,    2,   99,
	     2,    2,    2,    2,    2,   99,
	   104,  104,  104,  104,  104,   99,
	   104,  104,  104,  104,  104,   99,
	   307,  407,   99,   99,   99,   99,
	   307,  307,  407,   99,   99,   99,
	   307,  307,  407,  407,   99,   99,
	   307,  307,  407,  407,  407,   99,
	   104,  104,  104,  104,  104,   99,
	   100,  100,  100,  100,  100,   99,
	     6,    6,   99,   99,   99,   99,
	     6,    6,    6,   99,   99,   99,
	     6,    6,    6,    6,   99,   99,
	   100,  100,  100,  100,  100,   99,
	     8,   99,   99,   99,   99,   99,
	     8,    8,   99,   99,   99,   99,
	   100,  100,  100,  100,  100,   99,
	   108,   99,   99,   99,   99,   99,
	   108,  108,   99,   99,   99,   99}
	
};

 //===========================================================================
//					SCVのマップデータ
//===========================================================================
CSV_MAPDATA Map_csvDate[] = {
	//ステージ1
{
	 { "res/Map/ARIA0.csv" ,"res/Map/stage1/1-1Front.csv", "res/Map/stage1/1-2Front.csv", "res/Map/stage1/1-3Front.csv" , "res/Map/stage1/1-4Front.csv" },
	 { "res/Map/ARIA0.csv" ,"res/Map/stage1/1-1Back.csv", "res/Map/stage1/1-2Back.csv", "res/Map/stage1/1-3Back.csv" , "res/Map/stage1/1-4Back.csv" }
},
//ステージ2
{
	 { "res/Map/ARIA0.csv" ,"res/Map/stage2/2-1_Front.csv", "res/Map/stage2/2-2_Front.csv", "res/Map/stage2/2-3_Front.csv" , "res/Map/stage2/2-4_Front.csv" },
	 { "res/Map/ARIA0.csv" ,"res/Map/stage2/2-1_Back.csv", "res/Map/stage2/2-2_Back.csv", "res/Map/stage2/2-3_Back.csv" , "res/Map/stage2/2-4_Back.csv" }
 },
	//ステージ3
{
	 { "res/Map/ARIA0.csv" ,"res/Map/stage3/3-1_Front.csv", "res/Map/stage3/3-2_Front.csv", "res/Map/stage3/3-3_Front.csv" , "res/Map/stage3/3-4_Front.csv" },
	 { "res/Map/ARIA0.csv" ,"res/Map/stage3/3-1_Back.csv", "res/Map/stage3/3-2_Back.csv", "res/Map/stage3/3-3_Back.csv" , "res/Map/stage3/3-4_Back.csv" }
 },

};
