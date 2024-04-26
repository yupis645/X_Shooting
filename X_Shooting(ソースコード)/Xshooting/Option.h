#pragma once

// ----------------------------------------------------------------
// オプション画面
// ----------------------------------------------------------------
// 関数プロトタイプ宣言
void Option_Init(SCENE* _sc);	//初期化
void Option_Input(SCENE* _sc);	//入力
void Option_Update(SCENE* _sc);	//更新
void Option_Draw(SCENE* _sc);	//描画
void Option_End(SCENE* _sc);		//終了処理
void Option(SCENE* _sc);	//ゲーム画面

//変数のextern宣言
extern SCENE scOption;