
//#include "../../common.h"
#include "../../Game.h"
#include "../Data/GameData.h"

#if TestMode


int DebugInput_A_EnemyAppear(ENEMY);
int DebugInput_G_EnemyAppear(ENEMY);
bool DebugInput_BossAppear(BOSS);


MOJI Text[] = { {"�g�[���C�h",1},{"�^���P��",1},{"�M�h�X�p���I",1},{"�W�]�[",1},{"�W�A��",1},{"�J�s",1},{"�e���W",1},{"�U�J�[�g",1}, {"�u���O�U�J�[�g",1},{"�K���U�J�[�g",1},{"�o�L����",1},{"None",1}};
MOJI GText[] = { {"�o�[��",1},{"�]���o�O",1},{"���O����",1},{"�h���O����",1},{"�f���[�_",1},{"�O���u�_�[",1},{"�{�U���O����",1},{"�\��",1}, {"�K���o�[��",1},{"�K���f���[�^",1},{"�A���S",1},{"A/G�R�A",1},{"SP�t���b�O",1},{"���j�ς�",1}, {"None",0}};
MOJI TypeText[] = { {"",1}, {"Type1",1},{"Type2",1},{"Type3",1},{"Type4",1},{"Type5",1},{"Type6",1},{"Type7",1},{"Type8",1}, {"Type9",1}};


static int typeNum = 0;


//=====================================================================
//				�Q�[���S�̂̏��
//	�t���[���J�E���g��X�e�[�W�ԍ��Ȃ�
//======================================================================
void DebugDraw_Status(MAP _m, SCENE _sc,bool _b_F) {

	static short time = 0;
	//���A���^�C��
	if (_sc.frame_count % 60 == 0 && _sc.frame_count != 0) {
		time++;
	}

	//��ʍ���
	WriteTextFA(0, WORD_H, 0, "frame_count = %d", _sc.frame_count);					//(�t���[���J�E���g�̒l)
	WriteTextFA(0, WORD_H * 2, 0, "�o�ߎ��� = %dsec", time);								//sec(�b)
	
	WriteText(0, WORD_H * 3, L"�X�e�[�W���");								//sec(�b)
	WriteTextFA(WORD_W, WORD_H * 4, 0, "�}�b�v:stage%d��%d/4", _m.stage, _m.split_num);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
	WriteTextFA(WORD_W, WORD_H * 5, 0, "�X�e�[�W�i�s�x%d", _m.y);	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)
	WriteTextFA(WORD_W, WORD_H * 6, 0, "�����}�b�v��(0,0)�܂ł̋��� = %d", _m.backmap[_m.split_num].y);	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)
	
	

	if (_b_F) {
		WriteText(0, WORD_H * 9, L"�G�l�~�[�e�[�u��,Ex");	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)

		WriteText(WORD_W, WORD_H * 9 + ( WORD_H * 1),L"�o������G0:�U�J�[�g type1");	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)
		WriteText(WORD_W, WORD_H * 9 + ( WORD_H * 2), L"�o������G1:�U�J�[�g type1");	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)
		WriteText(WORD_W, WORD_H * 9 + ( WORD_H* 3), L"�o������G2:�U�J�[�g type1");	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)

	}
	else {
		WriteTextFA(0, WORD_H * 9, 0, "�G�l�~�[�e�[�u��,Lv%d", A_Date_Table.Lv);	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)

		for (int r = 0; A_Date_Table.table[(A_Date_Table.Lv * 6) + r] != 99; r++) {		//�e�[�u���ł� '99' ���o���ő吔�Ƃ��Ĉ����Ă���(NULL�̂悤�Ȉ���)
			int number = A_Date_Table.table[(A_Date_Table.Lv * 6) + r];
			int type = 0;

			if (A_Date_Table.table[(A_Date_Table.Lv * 6) + r] >= 100) { type = A_Date_Table.table[(A_Date_Table.Lv * 6) + r] / 100; number = A_Date_Table.table[(A_Date_Table.Lv * 6) + r] % 100; }
			WriteTextFA(WORD_W, WORD_H * 10 + (r * WORD_H), 0, "�o������G%d:%s %s", r, Text[number], TypeText[type]);	//���݂̕����}�b�v�ɂ�����i�s�x����(1�X�e�[�W�S��������Ă���)

		}
	}
	WriteTextFA(SRN_W - (WORD_W * 15), WORD_H * 2, 0, "typeNum = %d", typeNum / 100);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
	WriteText(SRN_W - (WORD_W * 21), WORD_H * 3, L"<type�ɑΉ��ł���G>");			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
	if(typeNum == 0){
		WriteText(SRN_W - (WORD_W * 13), WORD_H * 4,L"�S�Ă̓G");			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
	}
	if (typeNum == 100) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "1�L�[:%s", Text[0]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 5, 0, "4�L�[:%s", Text[3]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 6, 0, "8�L�[:%s", Text[7]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 7, 0, "9�L�[:%s", Text[8]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 8, 0, "U�L�[:%s", GText[5]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����

	}
	if (typeNum == 200) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "4�L�[:%s", Text[3]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 5, 0, "8�L�[:%s", Text[7]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 6, 0, "9�L�[:%s", Text[8]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 7, 0, "U�L�[:%s", GText[5]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
	}
	if (typeNum == 300) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "8�L�[:%s", Text[7]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 5, 0, "9�L�[:%s", Text[8]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 6, 0, "U�L�[:%s", GText[5]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����

	}
	if (typeNum >= 400) {
		WriteTextFA(SRN_W - (WORD_W * 19), WORD_H * 4, 0, "U�L�[:%s", GText[5]);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����

	}


}

//=====================================================================
//				�v���C���[�֌W�̏��
//	�{�̂̓����蔻��,�Ə���͈̔�,�V���b�g�̓����蔻��,�{���̓����蔻��
//======================================================================
void DebugDraw_PlayerDraw(PLAYER _p) {
	if (_p.anim < 12) {
		DrawRect(_p.hitbox.left, _p.hitbox.top, _p.hitbox.right, _p.hitbox.bottom, DWRGB(255, 0, 0), true);								//�����蔻��
		DrawRect((int)_p.center_x - 1, (int)_p.center_y - 1, (int)_p.center_x + 1, (int)_p.center_y + 1, DWRGB(255, 255, 255), true);	//���S�_
		DrawRect(_p.sight_hitbox.left, _p.sight_hitbox.top, _p.sight_hitbox.right, _p.sight_hitbox.bottom, DWRGB(0, 255, 255), false);	//�����蔻��
	}

	/*�V���b�g�̕`��*/
	for (int r = 0; r < MAX_SHOT; r++) {
		if (_p.shot[r].F == true) {
			DrawRect(_p.shot[r].hitbox.left, _p.shot[r].hitbox.top, _p.shot[r].hitbox.right, _p.shot[r].hitbox.bottom, DWRGB(255, 0, 0), true);
		}
	}

	/*�{���̕`��*/
	if (_p.bom.bomber == true) {
		DrawRect(_p.bom.hitbox.left, _p.bom.hitbox.top, _p.bom.hitbox.right, _p.bom.hitbox.bottom, DWRGB(0, 255, 255), true);
	}
}

//=====================================================================
//				�󒆓G�֌W�̏��
//	�����蔻��̉���
//======================================================================
void DebugDraw_A_EnemyDraw(ENEMY _ae) {

	WriteText(0, WORD_H * 25, L"<�󒆂̓G>");						//(�t���[���J�E���g�̒l)
	WriteTextFA(0, WORD_H * 8, 0, "�G�̐� = %d", _ae.count);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����

	for (int r = 0; r < MAX_ENEMY; r++) {
		int num = 11;
		if (_ae.enemy[r].hit == false && _ae.enemy[r].frame_in == true) {
			DrawRect(_ae.enemy[r].hitbox.left, _ae.enemy[r].hitbox.top, _ae.enemy[r].hitbox.right, _ae.enemy[r].hitbox.bottom, DWRGB(255, 0, 0), false);
			num = _ae.enemy[r].number;
		}

		//��ʏ�ɓo�ꂵ�Ă���G�̖��O��\��
		WriteTextFA(0, WORD_H * (26 + r), 0, "num%2d:%s %s", r, Text[num], TypeText[_ae.enemy[r].type]);		//(�t���[���J�E���g�̒l)
	}
}

//=====================================================================
//				�n��I�̏��
//	�����蔻��̉���
//======================================================================
void DebugDraw_G_EnemyDraw(ENEMY _ge) {

	WriteText(SRN_W - WORD_W * 22, WORD_H * 25, L"<�n��̓G>");

	//WriteTextFA(0, WORD_H * 16, 0, "��ʏ�̒n��G�̐� = %d", _ge.count);			//�X�e�[�W�ԍ�,�P�X�e�[�W�̐i�s�x����

	for (int r = 0; r < MAX_ENEMY; r++) {
		int e = 14;
		if (_ge.enemy[r].frame_in == true) {
			if (_ge.enemy[r].hit == false) {
				DrawRect(_ge.enemy[r].hitbox.left, _ge.enemy[r].hitbox.top, _ge.enemy[r].hitbox.right, _ge.enemy[r].hitbox.bottom, DWRGB(0, 255, 255), false);		//�����蔻��̉���
				e = _ge.enemy[r].number - 50;
			}
			else {	e = 13;}
		}
		
		
		//��ʏ�ɓo�ꂵ�Ă���G�̖��O��\��
		WriteTextFA(SRN_W - WORD_W * 22, WORD_H * (26 + r), 0, "num%2d:%s %s",r, GText[e], TypeText[_ge.enemy[r].type]);
		if(_ge.enemy[r].type != 0)
		{

		}
	}

}

//=====================================================================
//				�{�X�̏��
//	�����蔻��̉���
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
	else if (ChkKeyEdge(PK_4)) {	number =  3;	}		//103	�W�\�[ �^�C�v0::�e���ˌド���_��,�^�C�v1:���@�Ɍ�����,�^�C�v2:���@�̌�납��o��
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
	if (ChkKeyEdge(PK_W)) {		 number = 50; }		//�o�[��	   : �b�s���~�b�h�^�̍\�����B
	else if (ChkKeyEdge(PK_E)) { number = 51; }		//�]���o�O	   : �ۂ��\�����B�󂷂ƃ��x����������
	else if (ChkKeyEdge(PK_R)) { number = 52; }		//���O����	   : �C��B���Ԋu�Œe�����
	else if (ChkKeyEdge(PK_T)) { number = 53; }		//�h���O����   : �ړ��C��B�ړ��̓o�b�N�}�b�v�̒l��20,21,22�̏ꍇ�ɂ���ɉ���������������
	else if (ChkKeyEdge(PK_Y)) { number = 54; }		//�f���[�_	   : ���p�x�Œe������
	else if (ChkKeyEdge(PK_U)) { number = 55; }		//�O���u�^�[   : ��ԁB�^�C�v��9����A���ꂼ�ꋓ���Ɗl���X�R�A���Ⴄ
	else if (ChkKeyEdge(PK_I)) { number = 152; }	//�{�U���O���� : �S�̃��O�����Ɉ͂܂ꂽ�G�B�������󂹂Α������邪�A���ŉ�ꂽ���O�����̃X�R�A�͓���Ȃ�
	else if (ChkKeyEdge(PK_O)) { number = 57; }		//�\��		   ; �{���𓖂Ă�܂Ō����Ȃ��G�B�o�����Ă��猂�j����΍��X�R�A
	else if (ChkKeyEdge(PK_P)) { number = 58; }		//�K���o�[�� �@: �傫�ȃo�[�� 
	else if (ChkKeyEdge(PK_A)) { number = 59; }		//�K���f���[�^ : �傫�ȃf���[�_
	else if (ChkKeyEdge(PK_S)) { number = 62; }		//SP�t���b�O �@: �c�@��������

	return number;
}

bool DebugInput_BossAppear(BOSS _b){
	//�{�X�̌Ăяo��
	if (ChkKeyEdge(PK_F)) {
		if (_b.F == false) {
			return true;
		}
		//else {							//�{�X�����ɉ�ʂɓo�ꂵ�Ă���Ȃ�
		//	_b.down = true;			//�{�X�����j����
		//}
	}
	return false;
}


//�f�o�b�N�p�̓���
void DebugInput(ENEMY* ge,ENEMY* ae,BOSS* b, int Pl_x, int map_y)
{

	if (ChkKeyEdge(PK_G)) { typeNum += 100; }
	if (typeNum > 800) { typeNum = 0; }

	//�C�ӂ̋󒆓G�̏o��
	int a_enemyNum = DebugInput_A_EnemyAppear(*ae);
	if (a_enemyNum != 99) {
		a_enemyNum += typeNum;
		ae->count++;
		Air_Enemy_Create(ae,a_enemyNum, Pl_x);
	}
	//�C�ӂ̒n��G�̏o��
	int g_enemyNum = DebugInput_G_EnemyAppear(*ge);
	if (g_enemyNum != 99) {

		//�{�U���O�����ł͂�����type�̉��Z��؂�(���O������type�œǂݎ���Ă��邽��type�����Z����ƃ��O�������o�����Ă��܂�����)
		if (g_enemyNum != 152) {
			g_enemyNum += typeNum;
		}

		ge->count++;

		G_Enemy_Create(ge,g_enemyNum, 0, map_y, 300);				//�n��̓G���o������

		//�{�U���O������p����
		if (g_enemyNum == 152) {
			ge->count += 4;		//4�̕����Z����
			G_Enemy_Create(ge, 252, 0, map_y, 300 + CHIP_SIZE);				//�{�U���O���������̃��O����	300 + CHIP_SIZE�̈�����X���W
			G_Enemy_Create(ge, 56, 0, map_y, 300);							//�{�U���O�����{��
			G_Enemy_Create(ge, 352, 0, map_y, 300 - CHIP_SIZE);				//�{�U���O�����E���̃��O����
			G_Enemy_Create(ge, 452, 0, map_y, 300);							//�{�U���O��������̃��O����

			int g_EmptyNum = SetEmptyNum(*ge);
			if (g_EmptyNum == 99) { return; }		//�G���[(ENEMY�̒���1�̂�frame_in��false�o�Ȃ������ꍇ)
			ge->enemy[g_EmptyNum - 1].center_y -= (CHIP_SIZE * 2);			//�{�U���O��������̃��O����
			ge->enemy[g_EmptyNum - 2].center_y -= CHIP_SIZE;				//�{�U���O�����E���̃��O����
			ge->enemy[g_EmptyNum - 3].center_y -= CHIP_SIZE;				//�{�U���O�����{��
			ge->enemy[g_EmptyNum - 4].center_y -= CHIP_SIZE;				//�{�U���O���������̃��O����
		}
	}
	//�{�X�̏o��
	if (DebugInput_BossAppear(*b)) { Boss_Create(b); }
}


#endif