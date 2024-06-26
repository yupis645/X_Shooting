
#include "Player.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									
																		//初期化//

//===============================================================
//					プレイヤーの情報初期化
// 
// ・memcpyを利用して初期化する
//===============================================================
void Player_Init(PLAYER* _p) {
	memcpy(_p, &P_init, sizeof(PLAYER));		//PLAYER構造体の中身を一気にコピーする
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*プレイヤーの作成*/


//===============================================================
//					プレイヤーの作成
// 
// 仮に作ったPLAYERに必要なデータをセットして返す
// ・中心座標から画像と当たり判定の左上座標を求める
// ・照準器の左右をプレイヤーの左右と揃える
// ・自機、ショット&ボム、照準器の画像を取得する
//===============================================================
void Player_Create(PLAYER* _p) {


	memcpy(_p, &P_init, sizeof(PLAYER));		//PLAYER構造体の中身を一気にコピーする

	/*中心座標から当たり判定の左上と右下の座標を求める*/
	Hitbox_Sync(&_p->hitbox, _p->hitbox_size, &_p->center_x,&_p->center_y, &_p->pic_x, &_p->pic_y, _p->pic_size);

	/*照準の当たり判定の更新*/
	_p->sight_hitbox.left = _p->pic_x; _p->sight_hitbox.top = _p->pic_y - BOM_ST;
	_p->sight_hitbox.right = _p->pic_x + _p->pic_size; _p->sight_hitbox.bottom = _p->pic_y + _p->pic_size - BOM_ST;


	/*プレイヤー画像読み込み*/
	LoadDivBmp(PngData.Player, 0, 0, _p->pic_size, _p->pic_size, 6, 2,_p->pic);		//自機 or 照準の画像ロード
	LoadDivBmp(PngData.Shot, 0, 0, BULLET_SIZE, BULLET_SIZE, 3, 1, _p->bullet);			//ザッパー or ブラスターの画像ロード	

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*入力された情報を処理する*/

//===============================================================
//					入力情報の処理
// 
// ・移動キーによる移動
// ・ショットキーによる射撃
// ・ボムキーによる対地攻撃
//===============================================================
void Player_Move(PLAYER* _p, int _input) {

	/*プレイヤーの移動*/
	if (_input == DIR_LEFT && _p->hitbox.left > 0) {
		_p->center_x -= PL_SPEED;		//左
	}
	if (_input == DIR_RIGHT && _p->hitbox.right < SRN_W) {
		_p->center_x += PL_SPEED;		//右	
	}
	if (_input == DIR_UP && _p->hitbox.top > 0) {
		_p->center_y -= PL_SPEED;		//上
	}
	if (_input == DIR_DOWN && _p->hitbox.bottom < SRN_H) {
		_p->center_y += PL_SPEED;		//下
	}

	/*ショット(対空弾)の設定*/
	if (_input == INPUT_SHOT) {
		Shot_Create(_p->shot, _p->hitbox,&_p->shot_count);
	}

	/*ボム(対地弾)の設定*/
	if (_input == INPUT_BOM){
		if (!_p->bom.F) {
			Bom_Create(&_p->bom,_p->hitbox,_p->center_x,_p->center_y);
		}
	}

}

//===============================================================
//					ショットの作成
// 
// 発射最大数以下なら座標や当たり判定を設定して弾を生成する
// ・発射フラグを建てる
// ・当たり判定を設定する
// ・発射カウントを加算する
//===============================================================
void Shot_Create(P_SHOT_ST _s[], RECT hitbox, int* count) {

	int emptyNum = 99;						//空いている管理番号を格納する変数。99は見つからなかった場合の値

	for (int r = 0; r < MAX_SHOT; r++) {	//発射フラグが立っていないショット番号を探す
		if (_s[r].F == false) {				//ショットの発射フラグが立っていない番号を見つけたら
			emptyNum = r;					//空いている管理番号を取得する
		}
	}
	if (emptyNum == 99) { return; }			//フラグがfalseな管理番号が見つからなかった場合は処理を終了する

	_s[emptyNum].F = true;					//ショット発射フラグを立てる
	_s[emptyNum].hitbox = hitbox;			//当たり判定を設定する(自機の当たり判定と同じ大きさ)

	count++;								//ショットの弾数を管理する変数を + 1する
}

//===============================================================
//					ボムの作成
// 
// 画面上に一つしか存在できない
// ・爆発範囲の設定
// ・発射座標の設定
// ・発射したY座標をrangeに保存する
//===============================================================
void Bom_Create(P_BOM_ST* _b, RECT hitbox, float cx, float cy) {

	_b->F = true;					//フラグをONにする
	_b->bomber = false;				//着弾フラグをOFFにする
	_b->hitbox = hitbox;			//爆発範囲の設定:自機の当たり判定と同じ大きさ
	_b->range = hitbox.top;			//ボムの射程距離を管理する変数にボムを発射した瞬間の自機の座標を代入する
	_b->center_x = cx;				//X座標の設定(初期位置 = 自機の位置)
	_b->center_y = cy;				//Y座標の設定
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*更新処理*/

//===============================================================
//					プレイヤーの更新
// 
// ・座標移動に合わせた画像や当たり判定の移動
// ・アニメーション番号の管理
// ・ショットの更新(座標移動 & ヒット処理)
// ・ボムの更新(座標移動 & ヒット処理)
// ・敵と敵弾に対する被弾判定
// 
//  ※TestMode true & DebugEnable trueの場合は被弾処理を無効化
//===============================================================
void Player_Update(PLAYER* _p, ENEMY* _ae, ENEMY* _ge, ENEMY_SHOT* _s, SCENE* _sc) {

	static int out;		//自機が撃破された時のインターバル(時間)を制御する静的変数。

	/////*プレイヤーの当たり判定を更新する*/
	Hitbox_Sync(&_p->hitbox, _p->hitbox_size, &_p->center_x, &_p->center_y, &_p->pic_x, &_p->pic_y, _p->pic_size);

	/////*照準の当たり判定の更新*/
	_p->sight_hitbox.left = _p->pic_x; _p->sight_hitbox.top = _p->pic_y - BOM_ST;
	_p->sight_hitbox.right = _p->pic_x + _p->pic_size; _p->sight_hitbox.bottom = _p->pic_y + _p->pic_size - BOM_ST;

	_p->lookon = false;		//照準器の判定を初期化

	////*自機のアニメーション番号の進行*/
	if (_sc->frame_count % 12 == 0) {
		_p->anim++;		//アニメーション番号を進める

		//被弾していない場合
		if (_p->hit == false) {
			REPEAT(_p->anim,2,4)		//プレイヤーのanim番号をループさせる
		}

		//被弾した場合
		else{
			out++;	//撃破された後のインターバル
			if (out >= 20) {
				out = 0;					//撃破後のインターバルを管理する変数を初期化する
				_sc->scene_state = 1;		//ステートの値を変更する(ゲームオーバーかリトライか)
			}
		}
	}

	//ショットの更新
	Shot_Update(_p->shot, &_p->shot_count);
		
	/*ボムの更新*/
	Bom_Update(&_p->bom, _p->hitbox_size, _sc->frame_count);

	/*照準と地上敵との接触判定*/
	for (int r = 0; r < MAX_ENEMY; r++) {
		if (_ge->enemy[r].frame_in == true && _ge->enemy[r].hit == false) {	//敵の出現フラグがONかつ自機が撃破されて無ければ
			_p->lookon = Hit_box(_p->sight_hitbox, _ge->enemy[r].hitbox);		//敵本体と自機との当たり判定
		}
		if (_p->lookon == true) {											//地上敵のどれか一体でも接触した場合
			break;																//ループを抜ける
		}
	}

#if TestMode
	if (DebugEnable) { _p->hit = false;  return; }			//テストモード及びデバックがONなら当たり判定に行かない
#endif

	//各当たり判定(被弾判定がtrueなら以下の判定には入らない)
	if (_p->hit) { return; }

	/*空中の敵との当たり判定*/
	for (int r = 0; r < MAX_ENEMY; r++) {
		if (_ae->enemy[r].frame_in == true) {						//エネミーの出現フラグがONなら
			if( Hit_box(_p->hitbox, _ae->enemy[r].hitbox)){	//敵本体と自機との当たり判定
				_p->hit = true; break;
			}
		}
	}

	//敵弾との当たり判定
	for (int r = 0; r < MAX_E_SHOT; r++) {
		if (_s->shot[r].F == true) {								//敵弾のフラグがONなら
			if (Hit_box(_p->hitbox, _s->shot[r].hitbox)) {	//敵弾と自機との当たり判定
				_p->hit = true; break;
			}
		}
	}
	
	//敵弾もしく敵本体どちらかに接触判定がでた場合はanimを0にする(爆発エフェクトの描写に使うため)
	if (_p->hit == true) {		_p->anim = 0;		}
}

//===============================================================
//					ショットの更新
// 
// フラグが建っているショットを全て更新する
// ・Y座標移動
// ・画面外に出たらその管理番号のショットを初期化する
//===============================================================
void Shot_Update(P_SHOT_ST _s[], int* count) {

	for (int r = 0; r < MAX_SHOT; r++) {
		if (_s[r].F == true) {					//ショットフラグがONなら
			_s[r].hitbox.top -= SHOT_SPEED;		//ショットの座標を設定した値分進める
			_s[r].hitbox.bottom -= SHOT_SPEED;	//ショットの座標を設定した値分進める

			if (_s[r].hitbox.top < 0) {								//ショットが画面外に出たら
				memcpy(&_s[r], &P_SHOT_init, sizeof(P_SHOT_ST));		//ショットの初期化
				count--;											//画面内に表示できる最大弾数を管理する変数を減らす
			}
		}
	}
}

//===============================================================
//					ボムの更新
// 
// 射程距離でボムが爆発する。その前後で処理が違う
// 爆発前 : Y軸方向移動する。射程距離に達している場合、爆発フラグを建てる
// 爆発後 : 一定時間、爆発フラグが建った瞬間の座標を中心に設定されている爆発判定を出す
// 
//・当たり判定と座標を同期させる
// ・爆発フラグが建つ前はボムの持つスピードで座標移動し、一定距離進むと
//		ボムの出現フラグをfalse、爆発フラグをtrueに、rangeを0にする
// 
// ・爆発フラグが建っている場合はrange変数を制限時間として利用し、
//		10フレームに達するまでは中心座標をｓ浮くロールに合わせて移動させ、
//		時間になったらフラグを消去する
// 
//===============================================================
void Bom_Update(P_BOM_ST* _b, int size,  int _frameC) {

	if (_b->bomber == true || _b->F == true) {		//ボムが画面内に出ているとき or　ボムの攻撃判定が出ている							
		Hitbox_Sync(&_b->hitbox, size,					//当たり判定の同期
			&_b->center_x, &_b->center_y, 0, 0, 0);		//ボムが爆発している間は画面のスクロールに合わせて爆発位置がずれないようにする
	}

	/*ボム(爆破前)の更新*/
	if (_b->F == true) {					//ボムの発射フラグがONなら
		_b->center_y -= BOM_SPEED;			//設定されたボムの速度もそって画面上に向かって進む

		if (_b->range - _b->hitbox.top >= BOM_ST) {		//ボムの発射地点から特定距離まで進んだら(画像のサイズもあるので判定はhitboxのtopからとっている)
			_b->F = false;									//ボムのフラグをOFFにする
			_b->bomber = true;								//攻撃判定をONにする
			_b->range = 0;									//爆発フラグが立っている間はフレームカウントとして利用するため初期化する
		}
	}

	/*ボム(爆発後)の更新*/
	if (_b->bomber == true) {					//ボムの攻撃判定が出ている間は
		_b->range++;							//初期化したrange変数を使って攻撃判定の出現時間を制御する
		if (_b->range % 10 == 0) {				//攻撃判定が出て10フレーム経過したら
			_b->bomber = false;					//攻撃判定をなくす
		}
		if (_frameC % SCROLL_SPEED == 0) {
			_b->center_y++;							//爆発位置をスクロールさせる
		}
	}

	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*プレイヤー関連のスプライトを表示させる*/
//===============================================================
//			プレイヤー、照準器、ショット、ボムの描写
// 
// anim番号にそってそれぞれ描写する。
// ・照準器は当たり判定が地上敵と重なっている場合はanim番号が変化する(通常は青で重なっていると赤になる)
// ・被弾するまではプレイヤーと照準器のスプライト。
//	   被弾後は爆発アニメーションを描画する
// ・ショットの描画
// ・ボムの描画
//===============================================================
void Player_Draw(PLAYER* _p, EFFECT* _ef) {

	int sighht_anim = 0;	//照準器のアニメーション番号を指定する。番号によって照準器の色が違う[0 = 青,1 = 赤]

	//照準器のヒット判定によるアニメーション番号の変更
	if (_p->lookon == true) {		sighht_anim = 1;		}		//照準器が地上の敵と被さっているなら使うアニメーションを変える


	/*プレイヤー or 照準の描写*/
	if (_p->hit == false) {
		DrawBmp(_p->pic_x, _p->pic_y, _p->pic[_p->anim]);				//自機の描写
		DrawBmp(_p->pic_x, _p->pic_y - BOM_ST, _p->pic[sighht_anim]);	//照準の描写(ボムの射程距離分、自機から離す)
	}

	/*プレイヤーが撃破された場合*/
	else if (_p->hit == true && _p->anim < 6) {						// _p->anim <= 6は爆発アニメーションが6なため
		DrawBmp(_p->pic_x, _p->pic_y, _ef->DieEffect[_p->anim]);		//自機の描写
	}

	Shot_Draw(_p->shot, *_p->bullet[1], *_p->bullet[0]);

	Bom_Draw(&_p->bom, *_p->bullet[2]);

}

//===============================================================
//				ショットの描画
// 
// フラグが建っているショットを描写する。見た目上は二つ弾があるが判定は一つだけ
// ・左の弾の描写
// ・右の弾の描写
//===============================================================
void Shot_Draw(P_SHOT_ST _s[], Bmp _bmpR, Bmp _bmpL) {
	for (int r = 0; r < MAX_SHOT; r++) {
		if (_s[r].F == true) {
			DrawBmp(_s[r].hitbox.left - BULLET_SIZE, _s[r].hitbox.top, &_bmpL);		//ショット(左)の描写
			DrawBmp(_s[r].hitbox.right, _s[r].hitbox.top, &_bmpR);		//ショット(右)の描写
		}
	}
}
//===============================================================
//				ボムの描写
// 
// フラグが建っている間のみボムを描写する。攻撃判定を出している間はなにも表示しない
// ・ボムの描写
//===============================================================
void Bom_Draw(P_BOM_ST* _b, Bmp _bmp) {
	if (_b->F == true) {
		DrawBmp(_b->center_x - BULLET_SIZE, _b->center_y - BULLET_SIZE, &_bmp);		//ボムの描写
	}
}