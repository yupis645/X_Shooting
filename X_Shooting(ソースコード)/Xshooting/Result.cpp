

#include"common.h"
#include "Result.h"






// ----------------------------------------------------------------
// リザルト画面
// ----------------------------------------------------------------
//変数宣言
SCENE scResult = { true,0,RESULT,0 ,{0,0 } };	//シーン構造体

// ゲーム画面
void Result(SCENE* _sc)
{
	Result_Init(_sc);
	while (_sc->loop) {
		Result_Input(_sc);
		Result_Update(_sc);
		Result_Draw(_sc);
		_sc->frame_count++;
	}
	Result_End(_sc);
}
//初期化処理
void Result_Init(SCENE* _sc)
{
	_sc->loop = true;	//ゲームループ継続フラグ
	_sc->frame_count = 0;
	_sc->next_scene = RESULT;
}
//入力
void Result_Input(SCENE* _sc)
{
	GetKeyAll();	//全てのキー入力。
}
//更新
void Result_Update(SCENE* _sc)
{
	//アプリ終了キーの判定
	if (ChkKeyPress(VK_ESCAPE)) {
		_sc->loop = false;	//ゲームループ終了
		_sc->next_scene = APP_EXIT;	//次の画面遷移先を「アプリ終了」にする
		return;	//直ちにこの関数を終了
	}
	if (ChkKeyEdge(VK_RETURN)) {
		//エンターキーで次の画面に遷移
		_sc->next_scene = TITLE;	//次の遷移先画面IDをセット
		_sc->loop = false;	//タイトル終了
	}
}
//描画
void Result_Draw(SCENE* _sc)
{
	ClearScreen(0x10, 0x10, 0x10);	//画面消去：R,G,Bの指定色で塗りつぶす
	//▼▼▼ここから
	WriteText(CENTER_X - (8 * WORD_W), CENTER_Y - (WORD_H * 10),  L"G A M E O V E R");
	WriteText(CENTER_X - (5 * WORD_W), CENTER_Y - (WORD_H * 6), L"S C O R E");
	WriteTextFA(CENTER_X - (11 * WORD_W), CENTER_Y - (WORD_H * 4), 0, "	%5d ",state.score);
	//▲▲▲ここまで
	PrintFrameBuffer();	//画面バッファを転送
	RenderScreen();		//画面をレンダリング
	FrameSync();		//１フレームの同期
}
//終了処理
void Result_End(SCENE* _sc)
{
}
