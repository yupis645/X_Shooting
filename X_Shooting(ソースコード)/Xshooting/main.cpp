
#include"common.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Option.h"



// デバックモードの初期状態。デバックモードが有効なら最初からtrue
bool DebugEnable = DebugOn;

//スコアと残機などシーンを跨いで使用する情報
 GAMESTATE state = {0,0};
/*
* @brief	main()関数
*
* @note	アプリの開始位置。
*	画面遷移の流れを作って制御している
*/
int main(void)
{
	//【アプリ全体の初期化】
	SetCaption("2023 HAL Osaka.");	//ウィンドウタイトル
	InitConioEx(SRN_W, SRN_H, PXW, PXH);	//初期化：タテヨコサイズとドットの大きさ

	//【画面遷移の為のループ】
	int scene_id = TITLE;

	while (true) {
		switch (scene_id) {
		case	TITLE:	//タイトル画面
			Title(&scTitle);
			scene_id = scTitle.next_scene;	//次の画面IDを受け取る
			break;
		case	GAME:	//ゲーム画面
			Game(&scGame);
			scene_id = scGame.next_scene;	//次の画面IDを受け取る
			break;
		case	RESULT:	//リザルト画面
			Result(&scResult);
			scene_id = scResult.next_scene;	//次の画面IDを受け取る
		}
		//アプリ終了判定
		if (scene_id == APP_EXIT) {
			break;
		}
	}
	//【アプリ全体の終了処理】
	EndConioEx();	//ConioExの終了処理
	return 0;
}






//----------------------------------------------------------------
//	2023 HAL osaka.
//----------------------------------------------------------------
