#pragma once



// ----------------------------------------------------------------
// タイトル画面
// ----------------------------------------------------------------
// 関数プロトタイプ宣言
void Title_Init(SCENE* _sc);	//初期化
void Title_Input(SCENE* _sc);	//入力
void Title_Update(SCENE* _sc);	//更新
void Title_Draw(SCENE* _sc);	//描画
void Title_End(SCENE* _sc);	//終了処理
void Title(SCENE* _sc);	//タイトル画面

//変数のextern宣言
extern SCENE scTitle;