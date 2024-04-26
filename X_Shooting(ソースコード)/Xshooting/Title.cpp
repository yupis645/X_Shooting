

#include"common.h"
#include "Title.h"







// ----------------------------------------------------------------
// タイトル画面
// ----------------------------------------------------------------
//変数宣言
SCENE scTitle = { true,0,TITLE,0 ,{0,0 } };	//シーン構造体

Bmp* Titlepic = nullptr;
const wchar_t* texture =  L"res/STG_Title.png" ;
//タイトル画面
void Title(SCENE* _sc)
{
	Title_Init(_sc);
	while (_sc->loop) {
		Title_Input(_sc);
		Title_Update(_sc);
		Title_Draw(_sc);
		_sc->frame_count++;
	}
	Title_End(_sc);
}
//初期化処理
void Title_Init(SCENE* _sc)
{
	_sc->loop = true;	//ゲームループ継続フラグ
	_sc->frame_count = 0;
	_sc->next_scene = TITLE;
	_sc->scene_state = 0;	//進捗度合いの初期化

	LoadDivBmp(texture,
		0, 0, 198, 58, 1, 1, &Titlepic);

	/*光*/
	//LoadDivBmp(_t->filename,
		//208, 0, 8, 5, 4, 1, _t->flash);



}
//入力
void Title_Input(SCENE* _sc)
{
	GetKeyAll();	//全てのキー入力。

	//アプリ終了キーの判定
	if (ChkKeyPress(VK_ESCAPE)) {
		_sc->loop = false;	//ゲームループ終了
		_sc->next_scene = APP_EXIT;	//次の画面遷移先を「アプリ終了」にする
		return;	//直ちにこの関数を終了
	}
	if (ChkKeyEdge(VK_RETURN)) {
		//エンターキー画面を進捗させる
		_sc->scene_state++;
	}
	if (ChkKeyEdge(PK_O)) {
		//Oで次オプション画面に遷移
		_sc->next_scene = OPTION;	//次の遷移先画面IDをセット
		_sc->loop = false;	//タイトル終了
	}
	if (_sc->scene_state == 1) {
		if (ChkKeyEdge(PK_UP)) {
			_sc->carsor.y--;
		}
		if (ChkKeyEdge(PK_DOWN)) {
			_sc->carsor.y++;
		}

	}

#if TestMode
	if (DebugOn) {
		//デバックモードの有効化 / 無効化
		if (ChkKeyEdge(PK_D)){		DebugEnable = DebugEnable ? false : true;		}
	}
#endif


}
//更新
void Title_Update(SCENE* _sc)
{
	if (_sc->scene_state >= 2) {
		switch (_sc->carsor.y) {
		case 0:
		case 1:
			_sc->next_scene = GAME;	//次の遷移先画面IDをセット
			state.life = 3;
			break;
		case 2:
			_sc->next_scene = GAME;	//次の遷移先画面IDをセット
			state.life = 6;
			break;

		}
		//次の画面に遷移
		_sc->loop = false;	//タイトル終了

	}

	//カーソルのループ
	REPEAT(_sc->carsor.y, 0, 2);		//カーソルが0以下になれば3に3を超えれば0に戻る

}
//描画
void Title_Draw(SCENE* _sc)
{
	MOJI Text[] = { {"PUSH START ENTER",1},{"1 PLAYER",1},{"2 PLAYERS",1} };		//テキストの文章
	int po[] = { 30, 30,0 };
	ClearScreen(0x00, 0x00, 0x00);	//画面消去：R,G,Bの指定色で塗りつぶす
	//▼▼▼ここから
	DrawBmp(SRN_W / 2 - 99, 100, Titlepic);		//タイトルの表示

	switch (_sc->scene_state) {
	case 0:
		WriteTextFA(SRN_W / 2 - ((WORD_W / 2) * strlen(Text[0].word)), SRN_H / 2, 0, "%s", Text[0]);		//"PUSH START ENTER"の表示
		break;
	case 1:
		for (int r = 1; r < 4; r++) {
			WriteTextFA(SRN_W / 2 - ((WORD_W / 2) * strlen(Text[2].word)), SRN_H / 2 - po[r], 0, "%s", Text[r]);	//テキストの表示位置
		}
		WriteTextFA(SRN_W / 2 - ((WORD_W / 2) * strlen(Text[0].word)) - WORD_W, SRN_H / 2 - po[_sc->carsor.y], 0, "→");	//カーソルの表示

	}

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
		WriteText(0, SRN_H - (WORD_H * 32), L"--------------------------------------------------------------------------");

		WriteText(0, SRN_H - (WORD_H * 12), L"----------------------デバックモード操作説明------------------------------");
		WriteText(0, SRN_H - (WORD_H * 10), L"・[1,2,3,4,5,5,6,7,8,9キー , Qキー : 各空中敵の出現] ");
		WriteText(0, SRN_H - (WORD_H * 8), L"・[W,E,R,T,Y,U,I,O,P,A,Sキー: 各地上敵の出現]　");
		WriteText(0, SRN_H - (WORD_H * 6), L"・[Fキー : ボスの出現 ]　・[Gキー : 敵のタイプ変更 ]");
		WriteText(0, SRN_H - (WORD_H * 4), L"--------------------------------------------------------------------------");
	}
#endif
	//▲▲▲ここまで
	PrintFrameBuffer();	//画面バッファを転送
	RenderScreen();		//画面をレンダリング
	FrameSync();		//１フレームの同期
}
//終了処理
void Title_End(SCENE* _sc)
{
}