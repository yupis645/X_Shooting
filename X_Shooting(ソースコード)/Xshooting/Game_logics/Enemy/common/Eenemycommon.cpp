

//#include "../../../common.h"
#include "../../../Game.h"
//#include "../../Data/GameData.h"

//------------------------------------------------------------------------------------------------------------
// 空中敵や地上敵、ボスなどが2つ以上が参照する関数の集まり。
// ・バックマップから敵を出現させる関数
// ・軸移動の処理が書かれた関数
// ・使っていない管理番号を探し、新しく敵を作る時に代入する管理番号を見つけだす関数
//------------------------------------------------------------------------------------------------------------





//==================================================================================================
// 								/*バックマップのエネミー読み込み*/
// 
// 主に地上敵が参照する関数。
// 固定シンボル敵(ボスや特殊な挙動を持った空中敵)なども参照する
// 
//バックマップは表画面と同時に動き、マップチップの大きさと同じ数を動いたときに画面の一番上端一列を左から順に読み出し、敵の番号がある場合は敵を出現させる
// チップサイズは 32 * 32 
//敵の番号の割り振りは
//空中の敵 0～10　および　100～110		
//地上の敵 50～53 および  150～163		

//==================================================================================================
	
void BackMap_Enemy_table(ENEMY* _ge, ENEMY* _ae, PLAYER* _p, MAP* _m, int _frame, BOSS* _b) {


	MAP_DATE BackMap = _m->backmap[_m->split_num];		//現在のバックマップの情報(1ステージにつき5つに分割されているうちの一つ)


	if (_m->split_num > 0) {
		//マップの進行度がマップチップのサイズ分進んだ時	　&	スクロール速度に関わらず一回だけ通る &	 マップが更新された瞬間のy = 0の時は無視する
		if (BackMap.y % CHIP_SIZE == 0 && _frame % SCROLL_SPEED == 0 && BackMap.y > 0) {

			//マップの左上(0,0)から→(行)を見て行く
			for (int r = 0; r < MAP_W; r++)
			{
				//変数の作成
				int SeachY_Axis = (BackMap.y / CHIP_SIZE * MAP_W) + r;	//y軸の進行度合い(何列目か + r行目)
				int enemyNum = BackMap.data[SeachY_Axis];				//ヒットした敵の番号

				//ボスのチップがあったら///////////
				if (enemyNum == 60) {
					Boss_Create(_b);		//ボスの初期設定
				}

				//地上敵(50で割り切れる)//
				else if ((enemyNum / 50) % 2 == 1) {
					int emptynum = SetEmptyNum(*_ge);

					//地上敵の出現位置等の設定
					int back_coord = SeachY_Axis;									//裏マップにおける座標を記憶する
					int back_num = _m->split_num;								//裏マップ番号を記憶する
					float center_x = (float)((CHIP_SIZE * r) + (CHIP_SIZE / 2));	//敵の中心座標を設定[(0座標からの距離) + 敵の大きさの半分]

					//敵の生成
					_ge->count++;
					 G_Enemy_Create(_ge,enemyNum, back_coord, back_num, center_x);
					 
				}



				//固定空中敵(必ず出現する)
				else if ((enemyNum / 50) % 2 == 0)		//番号が50以下
				{
					bool init = false;
				
					Air_Enemy_Create(_ae,enemyNum, _p->center_x);



				}
			}
		}
	}
}




//==================================================================================================
// 								/*敵の移動パターン*/
// 
//　空中敵が主に参照する関数。
//   一部の地上敵(スクロール以外での移動をするグロプターやドモグラムなど)も参照する
//==================================================================================================
void Enemy_Patterns(ENEMY_ST* _e,  int move_pat) {

	int dir_search = CHIP_SIZE + (CHIP_SIZE - (CHIP_SIZE * _e->speed));

	switch (move_pat) {
		/*自機に向かって進む*/
	case 0:

		_e->center_x += (float)_e->speed * cos(_e->rad);		//X軸の移動
		_e->center_y += (float)_e->speed * sin(_e->rad);		//Y軸の移動
		break;


		/*Y軸の位置に関係なく自機の方向に向かう*/
	case 1:
		_e->center_x += (float)_e->speed * cos(_e->rad);		//X軸の移動
		_e->center_y += _e->speed;
		break;

		/*画面の下から出現する*/
	case 2:
		if (_e->ac_count <= 5) {
			_e->center_y = SRN_H;
		}
		_e->center_x += (float)_e->speed * cos(_e->rad);		//X軸の移動
		_e->center_y -= _e->speed;		//Y軸の移動
		break;
		/*そのまま直進する*/
	case 3:
		_e->center_y += (float)_e->speed;		//Y軸の移動
		break;
		/*加速度を加算しつつdirの方へ移動する*/
	case 4:
		_e->center_x -= (float)1 * cos(_e->rad) * _e->dir;		//X軸の移動
		_e->center_y -= (float)_e->speed * _e->acc;		//Y軸の移動

		/*加速*/
		_e->speed += _e->speed * _e->acc;

		break;

		/*加速度を加算しつつｘ軸方向に逃げる*/
	case 5:
		_e->center_y++;
		_e->center_x -= (float)_e->speed * _e->dir;		//X軸の移動
		/*加速*/
		_e->speed += _e->speed * _e->acc;
		break;

	}
}




//==================================================================================================
// 						/*画面に出現していない管理番号を見つける*/
// 
//　新しく生成する際に画面内にいない事を示すフラグが立っていない管理番号を探し、その番号を返す
// 最大出現数を超えても見つからない場合は'99'を返す(numberで'0'の値を使っているため)
//==================================================================================================
//空中、地上それぞれを管理している構造体から空いている管理番号を探す
int SetEmptyNum(ENEMY _e) {
	for (int i = 0; i < MAX_ENEMY; i++) {
		if (_e.enemy[i].frame_in == false) {
			return i;
		}
	}
	return 99;
}

//敵弾で空いている管理番号を探す
int E_shotGetEmptyNum(ENEMY_SHOT _es)
{
	for (int i = 0; i < MAX_E_SHOT; i++) {
		if (_es.shot[i].F == false) {
			return i;
		}
	}
	return 99;
}


