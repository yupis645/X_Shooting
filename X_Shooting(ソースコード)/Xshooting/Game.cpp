
#include"common.h"
#include "Game.h"
#include "Game_logics/Data/GameData.h"


// ----------------------------------------------------------------
// ゲーム画面
// ----------------------------------------------------------------
//変数宣言
SCENE scGame = { true,0,GAME,0 ,{0,0 } };	//シーン構造体
PLAYER Player;									//プレイヤー構造体
ENEMY A_Enemy;								//敵(空中)の構造体
ENEMY_SHOT EnemyShot;							//敵弾の構造体
ENEMY G_Enemy;								//敵(地上)構造体
BOSS Boss;									//ボスの構造体
MAP Map;									//ステージ一つ分の情報を格納する
EFFECT Down = { { nullptr},0};				//撃破エフェクトが入る
bool PoseIn = false;						//一時停止

// ゲーム画面(メインループ)
void Game(SCENE* _sc)
{
	srand((unsigned int)time(NULL));		//乱数生成
	state.score = 0;						//スコアの初期化
	A_Date_Table.Lv = 63;
	Game_Init(_sc);
	while (_sc->loop) {
		/*初期化*/
		Game_Input(_sc);
		/*更新*/
		Game_Update(_sc);
		/*描画*/
		Game_Draw(_sc);
		
		
	}
	Game_End(_sc);
}
//初期化処理
void Game_Init(SCENE* _sc)
{
	_sc->loop = true;			//ゲームループ継続フラグ
	_sc->frame_count = 0;		//フレームカウントの初期化
	_sc->next_scene = GAME;		//ゲーム画面を継続
	_sc->scene_state = 0;		//画面遷移やゲーム画面のリセットするときに使う変数


	//各データの初期化
	Player_Init(&Player);									//プレイヤー情報の初期化
	Air_Enemy_Init(&A_Enemy);
	G_Enemy_Init(&G_Enemy);
	Boss_Init(&Boss);
	E_Shot_Init(&EnemyShot);
	MAP_Init(&Map);										//マップの初期化

	//プレイヤーの作成
	Player_Create(&Player);
	
	//爆発エフェクトのロード
	LoadDivBmp(PngData.bomber, 0, 0, 47, 47, 6, 2, Down.DieEffect);
	A_Date_Table;
}
//入力
void Game_Input(SCENE* _sc)
{
	int inputkey = 0;

	GetKeyAll();	//全てのキー入力。

	//一時停止
	if (ChkKeyEdge(PK_SP)) {
		PoseIn = PoseIn ? false : true;

	}
	if (PoseIn || Player.hit == true) { return; }

	//キーの入力受付
	if (ChkKeyPress(PK_LEFT)) {
		inputkey = DIR_LEFT;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyPress(PK_RIGHT)) {
		inputkey = DIR_RIGHT;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyPress(PK_UP)) {
		inputkey = DIR_UP;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyPress(PK_DOWN)) {
		inputkey = DIR_DOWN;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyEdge(PK_Z)) {
		inputkey = INPUT_SHOT;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyEdge(PK_X)){
		inputkey = INPUT_BOM;
		Player_Move(&Player, inputkey);
	}

	
//テストモード有効時のみ作用
#if TestMode
	if (DebugOn) {				//
		if (ChkKeyEdge(PK_D)) {
			DebugEnable = DebugEnable ? false : true;
		}
	}
	if (DebugEnable) {		//デバックモード有効時のみ作動
		DebugInput(&G_Enemy,&A_Enemy, &Boss, Player.center_x, Map.split_num);
	}

#endif

}
//更新
void Game_Update(SCENE* _sc)
{
	//ポーズが有効時は更新処理はここで終了
	if (PoseIn) { return; }

	/*情報の更新*/
	Air_Enemy_Update(&Player, &A_Enemy, &EnemyShot, _sc);		//空中の敵の情報更新
	G_Enemy_Update(&Player, &G_Enemy, &EnemyShot, _sc,&Map,&Down);	//地上の敵の情報更新
	Boss_Update(&Player, &Boss, &EnemyShot, _sc,  &Down);		//ボスの情報更新
	Player_Update(&Player, &A_Enemy,&G_Enemy, &EnemyShot, _sc);	//プレイヤーの情報更新
	E_Shot_Update(&Player, &EnemyShot, _sc);						//敵の弾の情報更新

	
	/*空中敵の出現ルーチン*/
	Air_Enemy_table(&A_Enemy, &Player, _sc->frame_count, Map.split_num,Boss.F,Boss.down);			//空中の敵の出現テーブル
	
	/*地上物の出現ルーチン*/
	BackMap_Enemy_table(&G_Enemy,&A_Enemy, &Player,&Map,  _sc->frame_count,&Boss);

	/*マップのスクロール*/
	MAP_Update(&Map, _sc);
	
	GameStateChange(_sc);

	scGame.frame_count++;				//フレームカウント

}
//描画
void Game_Draw(SCENE* _sc)
{
	ClearScreen(0x19, 0x53, 0x5F);	//画面消去：R,G,Bの指定色で塗りつぶす
	//▼▼▼ここから

	if (PoseIn) {
		Pose();
	}
	else {
		MAP_Draw(&Map);
		G_Enemy_Draw(&G_Enemy, &Down);		//地上の敵表示
		Boss_Draw(&Boss, &Down);			//地上の敵表示
		Air_Enemy_Draw(&A_Enemy, &Down);	//空中敵の表示
		Player_Draw(&Player, &Down);			//プレイヤーの表示

		E_Shot_Draw(&EnemyShot);					//敵弾の表示

		//情報の表示
		WriteTextFA(0, (SRN_H)-(WORD_H * 4), 0, "life");					//残機の表示(仮)
		for (int r = 0; r < state.life; r++) {
			DrawBmp(Player.hitbox_size * r, (SRN_H)-(WORD_H * 3), Player.pic[5]);				//自機の描写
		}

		WriteTextFA(SRN_W - WORD_W * 20, 0, 0, "SCORE %10d", state.score);			//スコアの表示


		/*検証用*/
#if TestMode
		if (DebugEnable) {
			DebugDraw_Status(Map, *_sc,Boss.F);
			DebugDraw_PlayerDraw(Player);
			DebugDraw_A_EnemyDraw(A_Enemy);
			DebugDraw_G_EnemyDraw(G_Enemy);
			DebugDraw_BossDraw(Boss);
		}
#endif
	}

	//▲▲▲ここまで
	PrintFrameBuffer();	//画面バッファを転送
	RenderScreen();		//画面をレンダリング
	FrameSync();		//１フレームの同期
}
//終了処理
void Game_End(SCENE* _sc)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

									/*関数*/


void GameStateChange(SCENE* _sc) {

	//被弾した時の処理
	if (_sc->scene_state == 1) {
		state.life--;		//残機を減らす
		//残機が0以上ならゲームをリセットする
		if (state.life > 0)
		{
			Game_Init(_sc);					//プレイヤー、敵、敵の弾などを初期化して再スタート
		}
		else {				//lifeが0以下なら
			_sc->next_scene = RESULT;	//次の遷移先画面IDをセット
			_sc->loop = false;			//ゲーム画面終了
			_sc->scene_state = 0;
		}
	}
	if (_sc->scene_state == 2) {
		_sc->next_scene = RESULT;	//次の遷移先画面IDをセット
		_sc->loop = false;			//ゲーム画面終了
		_sc->scene_state = 0;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

															/*引数で持ってきた２つのRECT構造体を比べて接触していないかを判別する*/


bool Hit_box(RECT a, RECT b) {


	//それぞれの辺の重なり判定
	bool left = false; 
	bool top = false;
	bool right = false;
	bool bottom = false;

	//線と線での重なり判定
	if (a.left <= b.right){	right = true;	}
	if (a.right >= b.left){	left = true;	}
	if (a.top <= b.bottom){	bottom = true;	}
	if (a.bottom >= b.top){	top = true; }

	//4辺全て true(重なっていた)ならtureを返す 
	if (right&& left&& bottom && top) {
		return true;
	}

	return false;

}





//==================================================================================================
// 						/*あたり判定と画像の座標を同期させる*/
//「ヒットボックスシンクロ」
// 左上座標とサイズを受け取り、当たり判定の大きさを設定する (画像と当たり判定の大きさが違う敵がいるため)
// この関数は正方形のみ作成可能
//==================================================================================================
void Hitbox_Sync(RECT* _r, int r_size, float* cx, float* cy, int* px, int* py, int p_size){

	//画像サイズが0の場合は無視
	if (p_size != 0) {						 //画像サイズが0の場合は無視
		int pic_size_half = (p_size / 2 - 1); //RECTの半分の長さ()

		/*スプライトの左上座標設定*/
		*px = (int)*cx - pic_size_half;		 //中心座標 - 画像サイズの半径 - 1(補正)
		*py = (int)*cy - pic_size_half;
	}

	int hitbox_size_half = (r_size / 2); //ステータスの当たり判定の半分の長さ()

	/*当たり判定の同期*/
	_r->left   = (int)*cx - hitbox_size_half;
	_r->top    = (int)*cy - hitbox_size_half;
	_r->right  = (int)*cx + hitbox_size_half;
	_r->bottom = (int)*cy + hitbox_size_half;
}



//==================================================================================================
//									/*ポーズ画面*/
// 
// スペースキーで作動。PoseIn変数がOnの状態では更新と描画の関数を止める
// 操作説明を描写
//==================================================================================================
void Pose()
{
	WriteTextFA(WORD_W * 30, SRN_H - (WORD_H * 28), 0, "[  P O S E  ]");

#if TestMode
	WriteTextFA(0, SRN_H - (WORD_H * 22), 0, "---------------------------------操作説明---------------------------------");
	WriteTextFA(0, SRN_H - (WORD_H * 20), 0, "・[Enterキー : 決定] ・[Esc : ゲームの終了]");
	WriteTextFA(0, SRN_H - (WORD_H * 18), 0, "・[方向キー : 移動] ・[Zキー : ショット]　・[Xキー : 爆弾]　　");
	WriteTextFA(0, SRN_H - (WORD_H * 16), 0, "・[Spaceキー : 一時停止] ・[Dキー : デバックモード切り替え] ");
	WriteTextFA(0, SRN_H - (WORD_H * 14), 0, "--------------------------------------------------------------------------");
	if (DebugEnable) {
		WriteText(CENTER_X - (WORD_W * 9), SRN_H - (WORD_H * 38), L"デバックモード ON");
		WriteText(0, SRN_H - (WORD_H * 36), L"----------------------デバックモードの表示物------------------------------");
		WriteText(0, SRN_H - (WORD_H * 34), L"・Player無敵 ・Player,EnemyのHItBoxの可視化 ・ゲーム中のステータスの表示");
		WriteText(0, SRN_H - (WORD_H * 32), L"※敵出現は上限を超えると本来出現するはずの敵がいなくなったりするので注意!");
		WriteText(0, SRN_H - (WORD_H * 30), L"--------------------------------------------------------------------------");

		WriteText(0, SRN_H - (WORD_H * 12), L"----------------------デバックモード操作説明------------------------------");
		WriteText(0, SRN_H - (WORD_H * 10), L"・[1,2,3,4,5,5,6,7,8,9キー , Qキー : 各空中敵の出現] ");
		WriteText(0, SRN_H - (WORD_H * 8), L"・[W,E,R,T,Y,U,I,O,P,A,Sキー: 各地上敵の出現]　");
		WriteText(0, SRN_H - (WORD_H * 6), L"・[Fキー : ボスの出現 ] ・[Gキー : 敵のタイプ変更 ]");
		WriteText(0, SRN_H - (WORD_H * 4), L"--------------------------------------------------------------------------");
	}
#endif
}

