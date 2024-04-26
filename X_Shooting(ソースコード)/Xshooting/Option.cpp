
#include"common.h"
#include "Option.h"


// ----------------------------------------------------------------
// リザルト画面
// ----------------------------------------------------------------
//変数宣言
SCENE scOption = { true,0,OPTION,0 ,{0,0 } };	//シーン構造体

// ゲーム画面
void Option(SCENE* _sc)
{
	Option_Init(_sc);
	while (_sc->loop) {
		Option_Input(_sc);
		Option_Update(_sc);
		Option_Draw(_sc);
		_sc->frame_count++;
	}
	Option_End(_sc);
}
//初期化処理
void Option_Init(SCENE* _sc)
{
	_sc->loop = true;	//ゲームループ継続フラグ
	_sc->frame_count = 0;
	_sc->next_scene = OPTION;
}
//入力
void Option_Input(SCENE* _sc)
{
	GetKeyAll();	//全てのキー入力。
}
//更新
void Option_Update(SCENE* _sc)
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
void Option_Draw(SCENE* _sc)
{

	ClearScreen(0x11, 0x7f, 0x80);	//画面消去：R,G,Bの指定色で塗りつぶす
	//▼▼▼ここから
	WriteText(0, 0, L"オプション画面");

	//▲▲▲ここまで
	PrintFrameBuffer();	//画面バッファを転送
	RenderScreen();		//画面をレンダリング
	FrameSync();		//１フレームの同期
	
	//fclose(fin);

}
//終了処理
void Option_End(SCENE* _sc)
{
}