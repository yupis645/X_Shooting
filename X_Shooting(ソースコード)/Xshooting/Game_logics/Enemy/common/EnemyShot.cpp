
//#include "../../../common.h"
#include "../../../Game.h"
#include "../../Data/GameData.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*敵の弾の情報をセット*/


//=================================================================
//							初期化
// 
// ループでENEMY構造体に入っている全ての敵ステータスを初期化する
//=================================================================
void E_Shot_Init(ENEMY_SHOT* _e){
	for (int i = 0; i < MAX_E_SHOT; i++) {
		memcpy(&_e->shot[i], &E_Init, sizeof(E_SHOT_Init));		//データを初期化
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*敵の弾の情報をセット*/

//=================================================================
//							敵の生成
// 
// 空のステータスに情報を入れて返す
// ・発射フラグを立てて、発射される座標を引数から設定する
// ・当たり判定のサイズからRECTに値を入れる
// ・プレイヤーへの角度をradに入れる
// ・dir(向き)を乗算することで符号を整える
// ・画像を読み込む
//=================================================================
void E_Shot_Create(ENEMY_SHOT* _s,float px,float py,float x,float y, float _dir) {

	int Nextshotnum = E_shotGetEmptyNum(*_s);		//フラグの立っていない敵弾を探す
	if (Nextshotnum == 99) { return; }						//99エラーが出てなければ処理を進める


	_s->shot[Nextshotnum] = E_SHOT_Init;

	_s->shot[Nextshotnum].F = true;
	_s->shot[Nextshotnum].center_x = x;
	_s->shot[Nextshotnum].center_y = y;


	/*hitbox初期化*/
	Hitbox_Sync(&_s->shot[Nextshotnum].hitbox, BULLET_SIZE, &x, &y, 0, 0, 0);

	//_s->shot[Nextshotnum].pl_x = (int)_p.center_x;				//この関数に入った瞬間のプレイヤー座標を記する(X軸)
	//_s->shot[Nextshotnum].pl_y = (int)_p.center_y;				//この関数に入った瞬間のプレイヤー座標を記する(Y軸)
	_s->shot[Nextshotnum].rad = atan2(py - (float)_s->shot[Nextshotnum].hitbox.top, px - (float)_s->shot[Nextshotnum].hitbox.left);		//三角形の角度を算出する

	//発射する弾の向きを正負によって方向を絞る
	if (_dir != 0) {
		_s->shot[Nextshotnum].rad += _dir;
	}

	//画像の読み込み
	LoadDivBmp(PngData.Shot,
		0, 0, 7, 7, 2, 1, _s->shot[Nextshotnum].pic);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*更新*/


//=================================================================
//				画面に存在する敵弾の情報更新
// 
/// 発射フラグが立っている弾のみ処理に入る
// ・弾が画面内にある場合は弾の座標を移動させる
// ・座標と当たり判定を同期させる
// ・dir(向き)を乗算することで符号を整える
// ・画像を読み込む
//=================================================================
void E_Shot_Update(PLAYER* _p, ENEMY_SHOT* _s, SCENE* _sc) {

	for (int r = 0; r < MAX_E_SHOT; r++) {
		if (_s->shot[r].F == true) {
			//弾が画面内にある場合
			if (_s->shot[r].hitbox.left >= 0 && _s->shot[r].hitbox.top >= 0 && _s->shot[r].hitbox.left <= SRN_W && _s->shot[r].hitbox.top <= SRN_H) {
				_s->shot[r].center_x += E_SHOT_SPEED * cos(_s->shot[r].rad);		//X軸の移動
				_s->shot[r].center_y += E_SHOT_SPEED * sin(_s->shot[r].rad);		//Y軸の移動

				/*弾の当たり判定の同期*/
				Hitbox_Sync(&_s->shot[r].hitbox, BULLET_SIZE, &_s->shot[r].center_x, &_s->shot[r].center_y, 0, 0, 0);
			}

			//弾が画面外に出た場合
			else {
				_s->shot[r].hitbox.left = 0;		//位置を初期化する
				_s->shot[r].hitbox.top = 0;		//同上
				_s->shot[r].F = false;				//フラグをOFFにする
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*敵の弾のスプライトを表示する*/


void E_Shot_Draw(ENEMY_SHOT* _s) {

	for (int r = 0; r < MAX_E_SHOT; r++) {
		if (_s->shot[r].F == true) {
			DrawBmp(_s->shot[r].hitbox.left, _s->shot[r].hitbox.top, _s->shot[r].pic[0]);
		}
	}
}


