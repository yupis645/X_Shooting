
//#include "../../common.h"
#include "../../Game.h"
#include "../Data/GameData.h"

#if TestMode


int DebugInput_A_EnemyAppear(ENEMY);
int DebugInput_G_EnemyAppear(ENEMY);
bool DebugInput_BossAppear(BOSS);


MOJI Text[] = { {"トーロイド",1},{"タルケン",1},{"ギドスパリオ",1},{"ジゾー",1},{"ジアラ",1},{"カピ",1},{"テラジ",1},{"ザカート",1}, {"ブラグザカート",1},{"ガルザカート",1},{"バキュラ",1},{"None",1}};
MOJI GText[] = { {"バーラ",1},{"ゾルバグ",1},{"ログラム",1},{"ドモグラム",1},{"デローダ",1},{"グロブダー",1},{"ボザログラム",1},{"ソル",1}, {"ガルバーラ",1},{"ガルデロータ",1},{"アルゴ",1},{"A/Gコア",1},{"SPフラッグ",1},{"撃破済み",1}, {"None",0}};
MOJI TypeText[] = { {"",1}, {"Type1",1},{"Type2",1},{"Type3",1},{"Type4",1},{"Type5",1},{"Type6",1},{"Type7",1},{"Type8",1}, {"Type9",1}};


static int typeNum = 0;


//=====================================================================
//				ゲーム全体の情報
//	フレームカウントやステージ番号など
//======================================================================
void DebugDraw_Status(MAP _m, SCENE _sc,bool _b_F) {

	static short time = 0;
	//リアルタイム
	if (_sc.frame_count % 60 == 0 && _sc.frame_count != 0) {
		time++;
	}

	//画面左上
	WriteTextFA(0, WORD_H, 0, "frame_count = %d", _sc.frame_count);					//(フレームカウントの値)
	WriteTextFA(0, WORD_H * 2, 0, "経過時間 = %dsec", time);								//sec(秒)
	
	WriteText(0, WORD_H * 3, L"ステージ情報");								//sec(秒)
	WriteTextFA(WORD_W, WORD_H * 4, 0, "マップ:stage%dの%d/4", _m.stage, _m.split_num);			//ステージ番号,１ステージの進行度合い
	WriteTextFA(WORD_W, WORD_H * 5, 0, "ステージ進行度%d", _m.y);	//現在の分割マップにおける進行度合い(1ステージ４分割されている)
	WriteTextFA(WORD_W, WORD_H * 6, 0, "分割マップの(0,0)までの距離 = %d", _m.backmap[_m.split_num].y);	//現在の分割マップにおける進行度合い(1ステージ４分割されている)
	
	

	if (_b_F) {
		WriteText(0, WORD_H * 9, L"エネミーテーブル,Ex");	//現在の分割マップにおける進行度合い(1ステージ４分割されている)

		WriteText(WORD_W, WORD_H * 9 + ( WORD_H * 1),L"出現する敵0:ザカート type1");	//現在の分割マップにおける進行度合い(1ステージ４分割されている)
		WriteText(WORD_W, WORD_H * 9 + ( WORD_H * 2), L"出現する敵1:ザカート type1");	//現在の分割マップにおける進行度合い(1ステージ４分割されている)
		WriteText(WORD_W, WORD_H * 9 + ( WORD_H* 3), L"出現する敵2:ザカート type1");	//現在の分割マップにおける進行度合い(1ステージ４分割されている)

	}
	else {
		WriteTextFA(0, WORD_H * 9, 0, "エネミーテーブル,Lv%d", A_Date_Table.Lv);	//現在の分割マップにおける進行度合い(1ステージ４分割されている)

		for (int r = 0; A_Date_Table.table[(A_Date_Table.Lv * 6) + r] != 99; r++) {		//テーブルでは '99' が出現最大数として扱っている(NULLのような扱い)
			int number = A_Date_Table.table[(A_Date_Table.Lv * 6) + r];
			int type = 0;

			if (A_Date_Table.table[(A_Date_Table.Lv * 6) + r] >= 100) { type = A_Date_Table.table[(A_Date_Table.Lv * 6) + r] / 100; number = A_Date_Table.table[(A_Date_Table.Lv * 6) + r] % 100; }
			WriteTextFA(WORD_W, WORD_H * 10 + (r * WORD_H), 0, "出現する敵%d:%s %s", r, Text[number], TypeText[type]);	//現在の分割マップにおける進行度合い(1ステージ４分割されている)

		}
	}
	WriteTextFA(SRN_W - (WORD_W * 15), WORD_H * 2, 0, "typeNum = %d", typeNum / 100);			//ステージ番号,１ステージの進行度合い
	WriteText(SRN_W - (WORD_W * 21), WORD_H * 3, L"<typeに対応できる敵>");			//ステージ番号,１ステージの進行度合い
	if(typeNum == 0){
		WriteText(SRN_W - (WORD_W * 13), WORD_H * 4,L"全ての敵");			//ステージ番号,１ステージの進行度合い
	}
	if (typeNum == 100) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "1キー:%s", Text[0]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 5, 0, "4キー:%s", Text[3]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 6, 0, "8キー:%s", Text[7]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 7, 0, "9キー:%s", Text[8]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 8, 0, "Uキー:%s", GText[5]);			//ステージ番号,１ステージの進行度合い

	}
	if (typeNum == 200) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "4キー:%s", Text[3]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 5, 0, "8キー:%s", Text[7]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 6, 0, "9キー:%s", Text[8]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 7, 0, "Uキー:%s", GText[5]);			//ステージ番号,１ステージの進行度合い
	}
	if (typeNum == 300) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "8キー:%s", Text[7]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 5, 0, "9キー:%s", Text[8]);			//ステージ番号,１ステージの進行度合い
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 6, 0, "Uキー:%s", GText[5]);			//ステージ番号,１ステージの進行度合い

	}
	if (typeNum >= 400) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "Uキー:%s", GText[5]);			//ステージ番号,１ステージの進行度合い

	}


}

//=====================================================================
//				プレイヤー関係の情報
//	本体の当たり判定,照準器の範囲,ショットの当たり判定,ボムの当たり判定
//======================================================================
void DebugDraw_PlayerDraw(PLAYER _p) {
	if (_p.anim < 12) {
		DrawRect(_p.hitbox.left, _p.hitbox.top, _p.hitbox.right, _p.hitbox.bottom, DWRGB(255, 0, 0), true);								//当たり判定
		DrawRect((int)_p.center_x - 1, (int)_p.center_y - 1, (int)_p.center_x + 1, (int)_p.center_y + 1, DWRGB(255, 255, 255), true);	//中心点
		DrawRect(_p.sight_hitbox.left, _p.sight_hitbox.top, _p.sight_hitbox.right, _p.sight_hitbox.bottom, DWRGB(0, 255, 255), false);	//当たり判定
	}

	/*ショットの描写*/
	for (int r = 0; r < MAX_SHOT; r++) {
		if (_p.shot[r].F == true) {
			DrawRect(_p.shot[r].hitbox.left, _p.shot[r].hitbox.top, _p.shot[r].hitbox.right, _p.shot[r].hitbox.bottom, DWRGB(255, 0, 0), true);
		}
	}

	/*ボムの描写*/
	if (_p.bom.bomber == true) {
		DrawRect(_p.bom.hitbox.left, _p.bom.hitbox.top, _p.bom.hitbox.right, _p.bom.hitbox.bottom, DWRGB(0, 255, 255), true);
	}
}

//=====================================================================
//				空中敵関係の情報
//	当たり判定の可視化
//======================================================================
void DebugDraw_A_EnemyDraw(ENEMY _ae) {

	WriteText(0, WORD_H * 25, L"<空中の敵>");						//(フレームカウントの値)
	WriteTextFA(0, WORD_H * 8, 0, "敵の数 = %d", _ae.count);			//ステージ番号,１ステージの進行度合い

	for (int r = 0; r < MAX_ENEMY; r++) {
		int num = 11;
		if (_ae.enemy[r].hit == false && _ae.enemy[r].frame_in == true) {
			DrawRect(_ae.enemy[r].hitbox.left, _ae.enemy[r].hitbox.top, _ae.enemy[r].hitbox.right, _ae.enemy[r].hitbox.bottom, DWRGB(255, 0, 0), false);
			num = _ae.enemy[r].number;
		}

		//画面上に登場している敵の名前を表示
		WriteTextFA(0, WORD_H * (26 + r), 0, "num%2d:%s %s", r, Text[num], TypeText[_ae.enemy[r].type]);		//(フレームカウントの値)
	}
}

//=====================================================================
//				地上的の情報
//	当たり判定の可視化
//======================================================================
void DebugDraw_G_EnemyDraw(ENEMY _ge) {

	WriteText(SRN_W - WORD_W * 22, WORD_H * 25, L"<地上の敵>");

	//WriteTextFA(0, WORD_H * 16, 0, "画面上の地上敵の数 = %d", _ge.count);			//ステージ番号,１ステージの進行度合い

	for (int r = 0; r < MAX_ENEMY; r++) {
		int e = 14;
		if (_ge.enemy[r].frame_in == true) {
			if (_ge.enemy[r].hit == false) {
				DrawRect(_ge.enemy[r].hitbox.left, _ge.enemy[r].hitbox.top, _ge.enemy[r].hitbox.right, _ge.enemy[r].hitbox.bottom, DWRGB(0, 255, 255), false);		//当たり判定の可視化
				e = _ge.enemy[r].number - 50;
			}
			else {	e = 13;}
		}
		
		
		//画面上に登場している敵の名前を表示
		WriteTextFA(SRN_W - WORD_W * 22, WORD_H * (26 + r), 0, "num%2d:%s %s",r, GText[e], TypeText[_ge.enemy[r].type]);
		if(_ge.enemy[r].type != 0)
		{

		}
	}

}

//=====================================================================
//				ボスの情報
//	当たり判定の可視化
//======================================================================
void DebugDraw_BossDraw(BOSS _b) {
	for (int r = 0; r < 5; r++) {
		if (r < 5 && _b.enemy[r].hit == false) {
			DrawRect(_b.enemy[r].hitbox.left, _b.enemy[r].hitbox.top, _b.enemy[r].hitbox.right, _b.enemy[r].hitbox.bottom, DWRGB(0, 255, 255), false);
		}
	}
}

int DebugInput_A_EnemyAppear(ENEMY){

	int number = 99;
	if (ChkKeyEdge(PK_1)) {		 	number =  0;	}		//100
	else if (ChkKeyEdge(PK_2)) {	number =  1;	}
	else if (ChkKeyEdge(PK_3)) {	number =  2;	}
	else if (ChkKeyEdge(PK_4)) {	number =  3;	}		//103	ジソー タイプ0::弾発射後ランダム,タイプ1:自機に向かう,タイプ2:自機の後ろから出現
	else if (ChkKeyEdge(PK_5)) {	number =  4;	}		//
	else if (ChkKeyEdge(PK_6)) {	number =  5;	}
	else if (ChkKeyEdge(PK_7)) {	number =  6;	}
	else if (ChkKeyEdge(PK_8)) {	number =  7;	}		//307
	else if (ChkKeyEdge(PK_9)) {	number =  8;	}
	else if (ChkKeyEdge(PK_0)) {	number =  9;	}
	else if (ChkKeyEdge(PK_Q)) {	number = 10;	}

	return number;
}
int DebugInput_G_EnemyAppear(ENEMY) {
	int number = 99;
	if (ChkKeyEdge(PK_W)) {		 number = 50; }		//バーラ	   : ッピラミッド型の構造物。
	else if (ChkKeyEdge(PK_E)) { number = 51; }		//ゾルバグ	   : 丸い構造物。壊すとレベルが下がる
	else if (ChkKeyEdge(PK_R)) { number = 52; }		//ログラム	   : 砲台。一定間隔で弾を放つ
	else if (ChkKeyEdge(PK_T)) { number = 53; }		//ドモグラム   : 移動砲台。移動はバックマップの値が20,21,22の場合にそれに沿った挙動をする
	else if (ChkKeyEdge(PK_Y)) { number = 54; }		//デローダ	   : 高頻度で弾を撃つ
	else if (ChkKeyEdge(PK_U)) { number = 55; }		//グロブター   : 戦車。タイプが9つあり、それぞれ挙動と獲得スコアが違う
	else if (ChkKeyEdge(PK_I)) { number = 152; }	//ボザログラム : ４つのログラムに囲まれた敵。中央を壊せば他も壊れるが連鎖で壊れたログラムのスコアは入らない
	else if (ChkKeyEdge(PK_O)) { number = 57; }		//ソル		   ; ボムを当てるまで見えない敵。出現してから撃破すれば高スコア
	else if (ChkKeyEdge(PK_P)) { number = 58; }		//ガルバーラ 　: 大きなバーラ 
	else if (ChkKeyEdge(PK_A)) { number = 59; }		//ガルデロータ : 大きなデローダ
	else if (ChkKeyEdge(PK_S)) { number = 62; }		//SPフラッグ 　: 残機が増える

	return number;
}

bool DebugInput_BossAppear(BOSS _b){
	//ボスの呼び出し
	if (ChkKeyEdge(PK_F)) {
		if (_b.F == false) {
			return true;
		}
		//else {							//ボスが既に画面に登場しているなら
		//	_b.down = true;			//ボスを撃破する
		//}
	}
	return false;
}


//デバック用の入力
void DebugInput(ENEMY* ge,ENEMY* ae,BOSS* b, int Pl_x, int map_y)
{

	if (ChkKeyEdge(PK_G)) { typeNum += 100; }
	if (typeNum > 800) { typeNum = 0; }

	//任意の空中敵の出現
	int a_enemyNum = DebugInput_A_EnemyAppear(*ae);
	if (a_enemyNum != 99) {
		a_enemyNum += typeNum;
		ae->count++;
		Air_Enemy_Create(ae,a_enemyNum, Pl_x);
	}
	//任意の地上敵の出現
	int g_enemyNum = DebugInput_G_EnemyAppear(*ge);
	if (g_enemyNum != 99) {

		//ボザログラムではここでtypeの加算を切る(ログラムのtypeで読み取っているためtypeを加算するとログラムが出現してしまうため)
		if (g_enemyNum != 152) {
			g_enemyNum += typeNum;
		}

		ge->count++;

		G_Enemy_Create(ge,g_enemyNum, 0, map_y, 300);				//地上の敵が出現する

		//ボザログラム専用処理
		if (g_enemyNum == 152) {
			ge->count += 4;		//4体分加算する
			G_Enemy_Create(ge, 252, 0, map_y, 300 + CHIP_SIZE);				//ボザログラム左側のログラム	300 + CHIP_SIZEの引数はX座標
			G_Enemy_Create(ge, 56, 0, map_y, 300);							//ボザログラム本体
			G_Enemy_Create(ge, 352, 0, map_y, 300 - CHIP_SIZE);				//ボザログラム右側のログラム
			G_Enemy_Create(ge, 452, 0, map_y, 300);							//ボザログラムより上のログラム

			int g_EmptyNum = SetEmptyNum(*ge);
			if (g_EmptyNum == 99) { return; }		//エラー(ENEMYの中で1体もframe_inがfalse出なかった場合)
			ge->enemy[g_EmptyNum - 1].center_y -= (CHIP_SIZE * 2);			//ボザログラムより上のログラム
			ge->enemy[g_EmptyNum - 2].center_y -= CHIP_SIZE;				//ボザログラム右側のログラム
			ge->enemy[g_EmptyNum - 3].center_y -= CHIP_SIZE;				//ボザログラム本体
			ge->enemy[g_EmptyNum - 4].center_y -= CHIP_SIZE;				//ボザログラム左側のログラム
		}
	}
	//ボスの出現
	if (DebugInput_BossAppear(*b)) { Boss_Create(b); }
}


#endif