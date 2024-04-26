#pragma once
// ----------------------------------------------------------------
// リザルト画面
// ----------------------------------------------------------------
// 関数プロトタイプ宣言
void Result_Init(SCENE* _sc);	//初期化
void Result_Input(SCENE* _sc);	//入力
void Result_Update(SCENE* _sc);	//更新
void Result_Draw(SCENE* _sc);	//描画
void Result_End(SCENE* _sc);		//終了処理
void Result(SCENE* _sc);	//ゲーム画面
//変数のextern宣言
extern SCENE scResult;
