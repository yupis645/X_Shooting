
#include "Air_Enemy.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

															/*空中敵の初期化*/
//==================================================================================================================================
//								空中敵の初期化
// 
// 敵を全て初期化し、他の変数も初期化する
//==================================================================================================================================

void Air_Enemy_Init(ENEMY* _e) {
	for (int i = 0; i < MAX_ENEMY; i++) {
		memcpy(&_e->enemy[i], &E_Init, sizeof(ENEMY_ST));		//データを初期化
	}
	_e->count = 0;
	_e->pop_F = false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

															/*空中敵のデータセット*/



//==================================================================================================================================
//											空中敵の生成
// 
//  引数は敵の番号、プレイヤーのX軸座標
// ・numberが100を超えている場合は百の位の値をtypeとして受け取り、十と一の位の値をnumberとして取得する
// ・取得したnumberを元に種類ごとの初期ステータスを代入する
// ・引数で受け取ったデータをステータスに代入していく
// ・出現位置の設定
// ・typeによって得ステータスが変化する場合はここで設定
// ・グラフィックデータの代入
// ・当たり判定と画像の位置を同期させる
// ・左右どちらに進むかの向きを決める
//		作成したステータスを返り値とて返す
//==================================================================================================================================
void Air_Enemy_Create(ENEMY* _e,int number, int Player_x) {
	

	int EmptyNum = SetEmptyNum(*_e);
	if (EmptyNum == 99) { return; }		//エラー(ENEMYの中で1体もframe_inがfalse出なかった場合)

	//初期化
	_e->enemy[EmptyNum] = E_Init;

	int type = 0;			//初期ステータス代入後にtypeを渡すための変数
	
	//numberの百の位はエネミーのTypeを表す(100 = Type:1 , 200 = Type:2)
	if (number >= 100) {
		type = number / 100;								//ナンバーを100で割ってTypeを取り出す
		number = number - (100 * type);						//100の位を取り除いた敵のナンバーを取り出す
	}

	//[セーフティ] : ナンバーが0～10でなければ処理を止める
	if (number < 0 && number > 10) {
		_e->enemy[EmptyNum].frame_in = false;				//frameinをfalseにしておけば空いているステータスと認識される
		return;								//処理を止める
	}		

	//_e->enemy[EmptyNum] = A_enemyDate[number];
	//基礎ステータスをコピー or 各値のセット
	memcpy(&_e->enemy[EmptyNum], &A_enemyDate[number], sizeof(ENEMY_ST));//データをセットする

	//[セーフティ] : typeが初期値以上の値が出ている場合は0(通常動作)のtypeとして生成する
	if (_e->enemy[EmptyNum].type < type) {	
		type = 0;	
	}

	_e->enemy[EmptyNum].type = type;										//データにタイプを代入

	//出現位置の設定
	if (number == 0 || number == 4 || number == 6) {		//対象:トーロイド,ジアラ,テラジ
		_e->enemy[EmptyNum].center_x = Player_x + 250 + (rand() % 200);		//Playerから 250 離れた位置から更に 0～200 の乱数を加算したX座標を指定 
	}		
	else {													//それ以外
		_e->enemy[EmptyNum].center_x = (float)(rand() % SRN_W + 1); 			//画面の横サイズに収まる範囲でランダムなX座標を指定
	}

	//もし乱数で取得したX座標が画面の横サイズを超えていたら
	if (_e->enemy[EmptyNum].center_x >= SRN_W) { _e->enemy[EmptyNum].center_x = Player_x - 350 + (rand() % 200); }

	/*複数の行動パターンのある敵のステータス変更*/
	//ジソー
	if (_e->enemy[EmptyNum].number == 3) {
		if (_e->enemy[EmptyNum].type > 0 && _e->enemy[EmptyNum].type < 3) { _e->enemy[EmptyNum].points = 100; }	//Typeによる得点の変動
	}
	//ザガート
	else if (_e->enemy[EmptyNum].number == 7) {
		if (_e->enemy[EmptyNum].type == 0) { _e->enemy[EmptyNum].points = 100; }
		if (_e->enemy[EmptyNum].type == 1) { _e->enemy[EmptyNum].points = 150;	_e->enemy[EmptyNum].speed = 4.0; }
		if (_e->enemy[EmptyNum].type == 2) { _e->enemy[EmptyNum].points = 200; }
		if (_e->enemy[EmptyNum].type == 3) { _e->enemy[EmptyNum].points = 300;	_e->enemy[EmptyNum].speed = 4.0; }
	}

	/*画像を代入する*/
	LoadDivBmp(PngData.A_Enemy, 0, 0, _e->enemy[EmptyNum].pic_size, _e->enemy[EmptyNum].pic_size, A_ENEMY_PIC_LINE, 11, _e->enemy[EmptyNum].pic);

	/*中心座標から'当たり判定'と'画像'の左上と右下の座標*/
	Hitbox_Sync(&_e->enemy[EmptyNum].hitbox, _e->enemy[EmptyNum].hitbox_size,				//RECT:当たり判定 , int:当たり判定の中心座標から見たサイズ
		&_e->enemy[EmptyNum].center_x, &_e->enemy[EmptyNum].center_y,						//float:中心座標(X) , 中心座標(Y)
		&_e->enemy[EmptyNum].pic_x, &_e->enemy[EmptyNum].pic_y, _e->enemy[EmptyNum].pic_size);	//int:画像左上のx座標 , 画像左上のy座標 , int:画像のサイズ


	/*画面の中心を軸に左右どちらに寄っているかによって最初の進行方向をさだめる*/
	_e->enemy[EmptyNum].dir = _e->enemy[EmptyNum].hitbox.left < CENTER_X ? -1 : 1;


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*空中の敵の情報を更新する*/


//==================================================================================================================================
//											空中敵の更新
// 
// 画面内に出現している管理番号について移動処理や被弾判定等を行う
// ・被弾判定が出ている場合は数フレームに1度アニメーション番号を進めて、一定の値に達したらその番号の敵データを初期化する
// ・撃破判定が出ていない間は敵の種類に応じた固有の動きをしながら座標と当たり判定の同期、画面内にいるかの判定をする
// ・撃破判定が出ていない間はショットに対する被弾判定を行っている。一部敵は特殊な処理をする
//==================================================================================================================================
void Air_Enemy_Update(PLAYER* _p, ENEMY* _e, ENEMY_SHOT* _s, SCENE* _sc) {


	for (int r = 0; r < MAX_ENEMY; r++) {
		if (_e->enemy[r].frame_in == true) {	//出現フラグがONなら処理に進む

			bool frame_out = false;					//画面から消去するかを判定をする(処理の中でtrueになった場合、処理の最期で画面から削除 & 情報の初期化を行う)

			_e->enemy[r].ac_count++;				//敵一体についている個別のタイマーを進める(行動処理に使う)

			//被弾判定がtrueの場合
			if (_e->enemy[r].hit == true) {
				if (_e->enemy[r].ac_count % 2 == 0) {		//2フレームに一度入る
					_e->enemy[r].anim++;					//anim変数を加算する(爆発アニメーションの進行)

				}
				if (_e->enemy[r].anim > 11) {				//anim変数が 11よりも大きくなったら
					memcpy(&_e->enemy[r], &E_Init, sizeof(ENEMY_ST));		//敵の情報を初期化する
					_e->count--;
				}

				continue;		//以降の処理はせず次の管理番号のループへ
			}

			//以下は被弾判定が出ていない場合に処理に進む

			A_Enemy_Moving(_p, &_e->enemy[r], _s, _sc->frame_count);		//敵の行動(numberによって異なる挙動をする)

			if (!_e->enemy[r].frame_in) { _e->count--; continue; }			//固有の行動で画面からいなくなった場合はカウントを減らし次の管理番号へ進む

			//当たり判定の同期
			Hitbox_Sync(&_e->enemy[r].hitbox, _e->enemy[r].hitbox_size,				//RECT:当たり判定 , int:当たり判定の中心座標から見たサイズ
				&_e->enemy[r].center_x, &_e->enemy[r].center_y,						//float:中心座標(X) , 中心座標(Y)
				&_e->enemy[r].pic_x, &_e->enemy[r].pic_y, _e->enemy[r].pic_size);


			//座標が画面外に出た場合の座標の初期化
			if (_e->enemy[r].ac_count > 50) {				//出現してから50フレーム立ってから判定に入る(出現してしばらくは画面端に当たっていても消去しない)
				if (!Hit_box(_e->enemy[r].hitbox, winView))		//当たり判定とウィンドウサイズのRECTと重なっていない = 画面外 なら
				{
					memcpy(&_e->enemy[r], &E_Init, sizeof(ENEMY_ST));		//敵の情報を初期化する
					_e->count--;
				}
			}

			/*ショットに対する被弾判定*/
			for (int shotNum = 0; shotNum < MAX_SHOT; shotNum++) {											//画面内に存在するショットの数ループする
				if ( _p->shot[shotNum].F == true) {						//ショットの出現フラグがtrueなら
					_e->enemy[r].hit = Hit_box(_p->shot[shotNum].hitbox, _e->enemy[r].hitbox);		//自機のショットに対する被弾判定(Hit_box関数により判定。戻り値がtureなら被弾)
					if (_e->enemy[r].hit == true) {											//ショットに被弾していたら
						//ショットの初期化
						memcpy(&_p->shot[shotNum], &P_SHOT_init, sizeof(P_SHOT_ST));					//当たった番号のショットの情報を初期化して
						_p->shot_count--;														//画面に存在するショットの数も減らす

						/*撃破時の処理*/
						if (_e->enemy[r].number != 10) {		//「バキュラ」以外の敵なら
							_e->enemy[r].ac_count = 0;				//固有のタイマーを初期化する(爆破アニメーションに使用するため)
							_e->enemy[r].anim = 6;					//爆破アニメーション番号に移行する
							state.score += _e->enemy[r].points;		//エネミーの持つ点数をスコアに加算する
						}
						//例外処理
						else {
							_e->enemy[r].hit = false;			//number == 10「バキュラ」のみ弾のヒット判定を受けても消滅しない(hit判定が出ていてもfalseに戻す)
						}
						break;								//ループを抜ける
					}
				}
			}
		


		}
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*空中の敵のスプライトを表示する*/

//==================================================================================================================================
//											空中敵の描写
// 
// 撃破判定が出るまでは敵のスプライトを表示する。撃破判定が出ている間は爆発アニメーションを再生する
// ・敵の最大数を検索してframe_inがtrueの敵のスプライトを描写する
//==================================================================================================================================
void Air_Enemy_Draw(ENEMY* _e, EFFECT* _ef) {


	for (int r = 0; r < MAX_ENEMY; r++) {
		if (_e->enemy[r].frame_in == true) {
			if (_e->enemy[r].hit == false) {
				DrawBmp(_e->enemy[r].hitbox.left, _e->enemy[r].hitbox.top, _e->enemy[r].pic[_e->enemy[r].anim]);
			}
			else {
				DrawBmp(_e->enemy[r].hitbox.left, _e->enemy[r].hitbox.top, _ef->DieEffect[_e->enemy[r].anim]);
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*敵の出現テーブル*/

//==================================================================================================================================
//											空中敵の出現テーブル
// 
//GameDateにある一次元配列から敵の番号を参照し、最大5体の敵が出現する。テーブルはint配列で int table[MAX_LEVEL * 6]; となっている
// ・Popフラグを見て出現させるか確認する
// ・Lvをx、ループで加算する値を rとしたとき、 x列のr行を確認してその場所の敵の種類番号を取得する
// ・空いている管理番号を調べ、見つかった場合はその番号で敵を生成。見つからなかった場合は敵を生成しない
//==================================================================================================================================
void Air_Enemy_table(ENEMY* _e, PLAYER* _p, int _frame,int splitnum,bool boss_F,bool boss_down) {

	//ボスの出現フラグが立っている間は特定の敵しか沸かない また 撃破フラグが立っている場合は敵の出現を止める
	if (boss_F ) {
		if (rand() % 100 == 0 && !boss_down) {
			if (3 > _e->count) {

				Air_Enemy_Create(_e, 107, _p->center_x);		//敵の生成
				_e->count++;
			}
		}
		return;
	}

	//レベルが0のと分割マップ番号が0の時は出現せず、それ以外なら出現をはじめる
	_e->pop_F = A_Date_Table.Lv == 0 || splitnum == 0 ? false : true;

	if (_frame % (SRN_H / 2) == 0) {	//一定時間でレベルが上昇する(空中敵の出現テーブルを決める)
		A_Date_Table.Lv++;
	}

	REPEAT(A_Date_Table.Lv,0,MAX_LEVEL - 1)


	int levellow = (A_Date_Table.Lv * 6);		//敵のレベルテーブルの列

	int count = 0;


	if (rand() % 35 == 0) {

		/*Lvによる敵の種類と出現数の管理*/
		if (_e->pop_F == true) {

			//出現テーブルは一次元配列のため、[レベル * 行]で敵のナンバーを調べている 
			for (int r = 0; A_Date_Table.table[levellow + r] != 99; r++) {		//テーブルでは '99' が出現最大数として扱っている(NULLのような扱い)	
				if (r >= 6) { break; }	//[セーフティ]:想定された最大出現数を超える場合は強制でループを抜ける
				count = r + 1;				//そのレベルで出現する敵の数を取得
			}
			int s = (rand() % count);

			//(空中敵の発生(出現数以上に敵は発生させない)
			if (count > _e->count) {
				int enemynumber = A_Date_Table.table[levellow + (rand() % count)];		//敵の種類を示すナンバー

				Air_Enemy_Create(_e, enemynumber, _p->center_x);		//敵の生成
				_e->count++;

			}
		}
	}
}


//==================================================================================================================================
//											空中敵の1フレームの動き
// 
//番号が50以下は空中敵、50以上は地上敵
//
// 0 = トーロイド , 1 = タルケン , 2 = ギドスパリオ , 3 = ジゾー , 4 =  ジアラ , 5 =  ザカート ,
// 6 =  カピ , 7 =  テラジ , 8 =  ブラグザカート , 9  =  ガルザカート , 10 =  バキュラ
//==================================================================================================================================
void A_Enemy_Moving(PLAYER* _p, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame) {

	//敵弾の発射が可能かの判定
	bool shot_ok = true;				//trueなら弾の発射ができる
	int s = E_shotGetEmptyNum(*_s);		//フラグの立っていない敵弾を探す
	if (s == 99) { shot_ok = false; }	//sが99 -> 弾の上限が超えているため弾が発射できないようにする
	
	/*画面に登場して5フレーム目までは自機への角度を算出する**/
	if (_e->ac_count < 5) {
		_e->rad = _e->rad = atan2(_p->center_y - _e->center_y, _p->center_x - _e->center_x);		//三角形の角度を算出する
	}

	if		(_e->number == 0) {		TOROID(_p->center_x, _p->center_y, _p->pic_size, _e, _s,_frame,shot_ok,s);	}
	else if (_e->number == 1) {		TORKAN(_p->center_x, _p->center_y, _e, _s, _frame,  shot_ok, s);		}
	else if (_e->number == 2) {		GIDDOSPARIO(_e,  _frame);	}
	else if (_e->number == 3) {		ZOSHI(_p->center_x, _p->center_y, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 4) {		JARA(_p->center_x, _p->center_y, _p->pic_size, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 5) {		KAPI(_p->center_x, _p->center_y, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 6) {		TERRAZI(_p->center_x, _p->center_y, _p->pic_size, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 7) {		ZAKATO(_p->center_x, _p->center_y, _p->pic_size, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 8) {		BRAGZAKATO(_p->center_x, _p->center_y, _p->pic_size, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 9) {		GARUZAKATO(_p->center_x, _p->center_y, _p->pic_size, _e, _s, _frame,  shot_ok, s);	}
	else if (_e->number == 10){		BACURA(_e, _frame);	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*空中敵の固有処理*/



//===================================================================================================
//								トーロイド　[リング型の敵]
//
//  概要: X軸がプレイヤーと重なったら進行方向とは逆の方向に逃走する
// ・type 0:弾を撃たない
// ・type 1:弾を撃つ
// 
// ・pat 0: 登場したときにプレイヤーがいた座標に向かって進む
//			プレイヤーのY軸が重なったら次のパターンへ(type 1なら射撃をする)
// ・pat 1: 設定した速度に達するまで減速する
// ・pat 2: 逆方向に加速しながら逃走する
//===================================================================================================
void TOROID(float px, float py,int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	/*弾を撃つ前の動き*/

	switch (_e->pat) {
	case 0://出現時にプレイヤーがいた座標に向かって進む

		Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう

		/*Y軸の距離関係なくプレイヤーのの当たり判定とX軸が重なったら*/
		if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//自機とY軸が一致したとき
			if (_e->type == 1) {
				if (shot_ok) {
					 E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//自機に向かって弾を発射する
				}
			}

			//パターンの変更
			_e->pat = 1;			//減速パターンへ移行する
			
			//逃げる方向を決める
			_e->dir = _e->center_x > px ? -1 : 1;
		}
		break;
	case 1:	//一定速度に落ちるまで減速する
			Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう

			_e->anim = A_Enemy_Anim(*_e, _frame, 5);	//アニメーションの進行
			_e->speed -= (float)0.1;					//減速
			if (_e->speed < 0.6) { _e->pat = 2; }		//速度が0.6を下回ったら逃走パターンに移行する

		break;
	case 2:	//反対方向に向かって逃走する
			Enemy_Patterns(_e,  5);		//行動パターン 5:加速度を加算しつつｘ軸方向に逃げる

			_e->anim = A_Enemy_Anim(*_e, _frame, 5);
		break;

	}
}

//===================================================================================================
//						タルケン [有人戦闘機]
//
// 概要:一定フレームで射撃をし、画面の上方向に逃走する
// ・pat 0:一定速度で登場時のプレイヤーがいた座標に前進。
//			敵の内部タイマーが100フレーム目に達したら動きを止めて射撃する
// ・pat 1:反転アニメーションが再生を終えたら画面の上方向に向かって逃走する
//===================================================================================================
void TORKAN(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	
	switch (_e->pat) {
	//弾を撃つ前の動き
	case 0:			
		Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
		if (_e->ac_count >= 100) {

			if (shot_ok) {
				E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//自機に向かって弾を発射する
			}
			_e->pat++;
			_e->rad = atan2(py - _e->center_y, px - _e->center_x);		//三角形の角度を算出する
		}
		break;
	//弾を撃った後の動き
	case 1:		
			if (_e->anim < 14 && _frame % 5 == 0) {
				_e->anim++;
			}
			if (_e->anim >= 14) {
				_e->center_x -= (float)_e->speed * cos(_e->rad);		//X軸の移動
				_e->center_y -= (float)_e->speed * sin(_e->rad);		//Y軸の移動
			}

		break;

	}
}

//===================================================================================================
//								ギドスパリオ [破壊可能な敵弾]
//
// 概要:高速で飛んでくる弾。
// ・一定速度で登場時のプレイヤーがいた座標に前進する
//===================================================================================================
void GIDDOSPARIO(ENEMY_ST* _e,int _frame) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);
	Enemy_Patterns(_e,  0);		//行動パターン 第4引数 0:自機に向かう
}

//===================================================================================================
//									ゾシー [回転するギザギザ]
//
// 最初はプレイヤーに向かってくるが途中で方向転換する。その方向はtypeによって変化する
// ・type 0 : プレイヤーに向かって前進。一定フレーム経過後、ランダムな方向へ前進する
// ・type 1 : プレイヤーに向かって前進。一定フレーム経過後、プレイヤーの座標を再検索し、その方向に前進する
// ・type 2 : 画面の下から登場し、プレイヤーに向かって前進。一定フレーム経過後、
//			プレイヤーの位置を再検索するがY軸の移動で角度を使わないため、左右の向きだけプレイヤーの方を向く(下方向には向かわない)
// 
// ・設定された方向に前進。
// ・内部フレームタイマーが一定の値に達すると方向転換 & 射撃
//===================================================================================================
void ZOSHI(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	
	//アニメーション進行(全体タイプ共通)
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);

	/*タイプ2のみ画面の下から上へ向かう特殊な行動パターン*/
	if (_e->type == 2) {		_e->pat = 2;	}

	Enemy_Patterns(_e,  _e->pat);		//pat0:自機に向かう,pat2:画面の下から

	//150フレームで弾を撃つ & 進行方向を再設定する
	if (_e->ac_count % 150 == 0 && _e->ac_count != 0) {
		//弾を撃つ
		if (shot_ok) {
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//自機に向かって弾を発射する
		}		
		
		//進行方向の再設定
		if (_e->type == 0) {		//タイプ0:ランダムに移動
			_e->rad = (float)(rand() % 360);
		}
		else {						//タイプ0以外:プレイヤーの方向を向く
			_e->rad = (float)atan2(py - _e->center_y, px - _e->center_x);		//三角形の角度を算出する
		}
	}
}

//===================================================================================================
//									ジアラ [Hみたいな奴]
//
//  概要: X軸がプレイヤーと重なったら進行方向とは逆の方向に逃走する。トーロイドよりも感知範囲が広い
// ・type 0:弾を撃たない
// ・type 1:弾を撃つ
// 
// ・pat 0: 登場したときにプレイヤーがいた座標に向かって進む。
//			プレイヤーのY軸が重なったら次のパターンへ(type 1なら射撃をする)
// ・pat 1: 設定した速度に達するまで減速する
// ・pat 2: 逆方向に加速しながら逃走する
//===================================================================================================
void JARA(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {

	switch (_e->pat) {

	case 0:
		Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう

		/*プレイヤーとY軸が一致したときの処理*/
		if (px - (p_hitoboxsize * 2) <= _e->center_x && px + (p_hitoboxsize * 2) >= _e->center_x && py > _e->center_y) {		//自機とY軸が一致したとき
			if (_e->type == 1) {
				if (shot_ok) {
					E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//自機に向かって弾を発射する
				}
			}
			_e->pat = 1;			//パターンを変更する

			_e->dir = _e->center_x > px ? -1 : 1;

		}
		break;
	//減速
	case 1:

			_e->anim = A_Enemy_Anim(*_e, _frame, 5);
			Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
			_e->speed -= (float)0.1;		//速度を1にする
			if (_e->speed <= 1.5) {
				_e->pat++;
			}
		break;
	//自機と逆方向に逃げる
	case 2:
			Enemy_Patterns(_e,  5);		//行動パターン 5:加速度を加算しつつｘ軸方向に逃げる
			_e->center_y +=_e->speed ;	//Y軸移動の補正値
			_e->anim = A_Enemy_Anim(*_e, _frame, 5);
		break;

	}
}

//===================================================================================================
//									カピ [ほぼビックバイパー]
//
// 概要:一定フレームで射撃をし、画面の上方向に逃走する。射撃時動きを止めない
// ・pat 0:一定速度で登場時のプレイヤーがいた座標に前進。
//			敵の内部タイマーが100フレーム目に達したら射撃する
// ・pat 1:加速しながら画面の上方向に向かって逃走する。一定フレームで射撃をする
//===================================================================================================
void KAPI(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	
	switch (_e->pat) {
	
	//弾を撃つ前の動き
	case 0:
		Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
		if (_e->ac_count >= 100) {

			_e->pat++;
			_e->rad = atan2(py - _e->center_y, px - _e->center_x);		//逃走する時は斜めに移動する
			_e->speed = 0.5;											//反転したときの初速度

			//左右どちらに向くかを決める
			_e->dir = _e->center_x > px ? -1 : 1;
			_e->ac_count = 0;
		}
		break;
	//弾を撃った後の動き
	case 1:
			if (_e->ac_count % 30 == 0) {
				if (shot_ok) {
					E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//自機に向かって弾を発射する
				}
			}
			if (_e->anim < 14 && _frame % 5 == 0) {
				//_e->anim++;
			}
			if (_e->anim >= 14) {
				_e->center_x -= (float)1 * cos(_e->rad) * _e->dir;		//X軸の移動
				_e->center_y -= (float)_e->speed * _e->acc;		//Y軸の移動
			}
			/*加速*/
			if (_frame % 2 == 0) {
				_e->speed += _e->speed * _e->acc;
			}
		break;
	}
		
}

//===================================================================================================
//									テラジ ['U'みたいな形をした戦闘機]
//
// 概要:射撃しながら前進し、プレイヤーのY軸と重なったら画面の上方向に逃走する
// 
// ・pat 0:登場時のプレイヤーがいた座標に前進。
//			一定間隔で射撃する
// ・pat 1:一定速度まで減速する
// ・pat 2:加速しながら画面の上方向に向かって逃走する。一定フレームで射撃をする
//===================================================================================================
void TERRAZI(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	if (_e->ac_count % 40 == 0) {
		if (shot_ok) {
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//自機に向かって弾を発射する
		}
	}
	//弾を撃つ前の動き
	if (_e->pat == 0) {
		Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
		if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//自機とY軸が一致したとき
			_e->pat++;
		}
	}
	/*感知範囲に自機が来た場合*/
	else if (_e->pat == 1) {
		Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
		_e->speed -= (float)0.1;		//減速する
		if (_e->speed <= 0.8) {				//一定速度まで下がったら
			_e->pat++;						//逃走パターンへ移行
			_e->rad = atan2(py - _e->center_y, px - _e->center_x);		//逃げる角度を算出(移動ではX方向のみ採用)

			//左右の向きを決定する
			_e->dir = _e->center_x > px ? -1 : 1;
		
		}
	}
	/*エネミーの速度が一定値まで下がったら*/
	else if (_e->pat == 2) {
		Enemy_Patterns(_e,  4);		//行動パターン 0:自機に向かう

	}
}

//===================================================================================================
//									ザガート [黒い球]
//
// 概要:typeによって条件は違うが、条件に達すると弾を生成して画面から消える
// 
// ・前進し、条件で射撃をして消える
// 
// ・type 0:垂直方向に前進し、		  「一定フレーム後」、弾を発射して画面から消える。
// ・type 1:プレイヤーの方向に前進し、「一定フレーム後」、弾を発射して画面から消える。
// 
// ・type 2:垂直方向に前進し、		  「プレイヤーとY軸が重なると」弾を発射して画面から消える
// ・type 3:プレイヤーの方向に前進し、「プレイヤーとY軸が重なると」弾を発射して画面から消える。
//===================================================================================================
void ZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);

	/*弾を撃つ前の動き*/
	if (_e->pat == 0) {
		if (_e->type % 2 != 0) {			//type 0 or type 2
			Enemy_Patterns(_e,  0);		//行動パターン 3:画面下に向けて直進する
		}
		else {								//type 1 or type 3
			Enemy_Patterns(_e,  3);		//行動パターン 0:自機に向かう
		}

		//弾を撃つ条件
		if (_e->type < 2) {					//type 0 or type 1
			if (_e->ac_count > 75) {		//75フレーム経過したら
				_e->pat = 1;				//射撃パターンへ
			}
		}
		else {								//type 2 or type 3
			if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//自機とY軸が一致したとき
				_e->pat = 1;				//射撃パターンへ
			}
		}
	}

	/*弾を撃って画面から消える*/
	else if (_e->pat == 1) {
		if (shot_ok) {
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);	//自機に向かって弾を発射する
		}
		_e->frame_in = false;			//画面にいるというフラグをfalseにする
	}
}

//===================================================================================================
//									プラグザガート [赤点の入った黒い球]
//
// 概要:挙動はザガートと同じ。弾の発射数が違う
// 
// ・前進し、条件で扇状に射撃をして消える。
// 
// ・type 0:垂直方向に前進し、		  「一定フレーム後」、扇状に弾を発射して画面から消える。
// ・type 1:プレイヤーの方向に前進し、「一定フレーム後」、扇状に弾を発射して画面から消える。
// 
// ・type 2:垂直方向に前進し、		  「プレイヤーとY軸が重なると」扇状に弾を発射して画面から消える
// ・type 3:プレイヤーの方向に前進し、「プレイヤーとY軸が重なると」扇状に弾を発射して画面から消える。
//===================================================================================================
void BRAGZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);
	/*弾を撃つ前の動き*/
	if (_e->pat == 0) {
		if (_e->type % 2 != 0) {
			Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
		}
		else {
			Enemy_Patterns(_e,  3);		//行動パターン 3:画面下に向けて直進する
		}

		if (_e->type < 2) {
			if (_e->ac_count > 75) {
				_e->pat = 1;
			}
		}
		else {
			if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//自機とY軸が一致したとき
				_e->pat = 1;
			}
		}

	}
	/*弾を撃った後の動き(弾を5発を打って消滅する)*/
	else if (_e->pat == 1) {
		for (float radoffset = -1.0; radoffset <= 1.0; radoffset += 0.5) {
			int Nextshotnum = E_shotGetEmptyNum(*_s);		//フラグの立っていない敵弾を探す
			if (Nextshotnum != 99){							//99エラーが出てなければ処理を進める
				E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, radoffset);		//自機に向かって弾を発射する
			}
		}
		_e->frame_in = false;			//画面から消える
		
	}
}

//===================================================================================================
//									ガルザガート [変な模様の入った弾]
//
// 概要:挙動はザガートと同じ。弾の発射数が違う
// 
// ・前進し、条件で球状に射撃をして消える。
// 
// ・type 0:垂直方向に前進し、		  「一定フレーム後」、球状に弾を発射して画面から消える。
// ・type 1:プレイヤーの方向に前進し、「一定フレーム後」、球状に弾を発射して画面から消える。
// 
// ・type 2:垂直方向に前進し、		  「プレイヤーとY軸が重なると」球状に弾を発射して画面から消える
// ・type 3:プレイヤーの方向に前進し、「プレイヤーとY軸が重なると」球状に弾を発射して画面から消える。
//===================================================================================================
void GARUZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);
	/*弾を撃つ前の動き*/
	if (_e->pat == 0) {
		if (_e->type % 2 != 0) {
			Enemy_Patterns(_e,  0);		//行動パターン 0:自機に向かう
		}
		else {
			Enemy_Patterns(_e,  3);		//行動パターン 3:画面下に向けて直進する
		}

		if (_e->type < 2) {
			if (_e->ac_count > 75) {
				_e->pat = 1;
			}
		}
		else {
			if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//自機とY軸が一致したとき
				_e->pat = 1;
			}
		}

	}
	/*弾を撃った後の動き(弾を5発を打って消滅する)*/
	else if (_e->pat == 1) {
		for (float radoffset = -4.0; radoffset <= 4.0; radoffset += 0.5) {
			
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, radoffset);		//自機に向かって弾を発射する
			
		}
		_e->frame_in = false;		//画面から消える
	}

}

//===================================================================================================
//									バキュラ [飛翔する板]
//
// 概要:垂直に移動する板。ショットを256発撃っても壊れません。
// 
// ・垂直方向に進む。弾が当たっても弾のフラグを一方的にかき消す。
//===================================================================================================
void BACURA(ENEMY_ST* _e, int _frame) {
	if (_frame % 7 == 0) {
		_e->anim++;
	}
	if (_e->anim >= 62) {
		_e->anim = 56;
	}
	Enemy_Patterns(_e,  3);		//pat0:自機に向かう,pat2:画面の下から
}


//===================================================================================================
//									アニメーションの進行
//
// ENEMY_ST構造体に含まれるanim番号とanim_sum,numberを利用して一定範囲内でanim番号をループさせる
// 
// ・cycle:animの切り替え時間　・_frame:フレームカウント
// ・dirによってanim番号を進めたり、逆に減らしたりできる
// ・REPEAT(代入する値, 最小値, 最大値)によってanim番号が最大値を超えたら最小値に、逆なら最大値に合わせる
//===================================================================================================
int A_Enemy_Anim(ENEMY_ST _e, int _frame, int cycle)
{
	int animNum = _e.anim;

	int StartAnimNum = _e.number * A_ENEMY_PIC_LINE;		//_e.enemy(列) * A_ENEMY_PIC_LINE(行)
	int EndAnimNum = StartAnimNum + _e.anim_sum  - 1;		//アニメーション番号 + 枚数でアニメーションの最終番号を割り出す -1は補正値

	//アニメーション番号を進める
	if (_frame % cycle == 0) {		animNum += 1 * _e.dir;		}

	REPEAT(animNum, StartAnimNum, EndAnimNum);


	return animNum;
}
