#include "conioex.h" 
#include <math.h>
#include <time.h>
#include"STG.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Option.h"



void Debug_Draw(PLAYER _p, ENEMY _ae, ENEMY _ge,  GAME_STATE _g, SCENE _sc,BOSS _b) {

	static short time = 0;

	//���A���^�C��
	if (_sc.frame_count % 60 == 0 && _sc.frame_count != 0) {
		time++;
	}

	//��ʍ���
	WriteTextFA(0, WORD_H, 0, "frame_count = %d", _sc.frame_count);		//(�t���[���J�E���g�̒l)
	WriteTextFA(0, WORD_H * 2, 0, "time = %d", time);					//�c�@�̕\��(��)
	WriteTextFA(0, WORD_H * 4, 0, "stage%d,y = %d", _g.map.stage,_g.map.y);					//�c�@�̕\��(��)
	WriteTextFA(0, WORD_H * 5, 0, "mF1::%d", _g.map.backmap[1].y);					//�c�@�̕\��(��)

	

	//��ʉE��
	WriteTextFA(SRN_W - WORD_W * 25, WORD_H, 0, "ac_count = %d,anim = %d", A_ENEMY.enemy[0].ac_count);		//(�t���[���J�E���g�̒l)
	WriteTextFA(SRN_W - WORD_W * 25, WORD_H * 2, 0, "stage:%d",_g.map.stage);						//(�t���[���J�E���g�̒l)
	WriteTextFA(SRN_W - WORD_W * 25, WORD_H * 3, 0, "Lv = %d �󒆓G�̐� = %d", _g.level,_ae.count);		//�G�̃��x��



/////*�v���C���[*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (_p.anim < 12) {
		DrawRect(_p.hitbox.left, _p.hitbox.top, _p.hitbox.right, _p.hitbox.bottom, DWRGB(255, 0, 0), true);								//�����蔻��
		DrawRect((int)_p.center_x - 1, (int)_p.center_y - 1, (int)_p.center_x + 1, (int)_p.center_y + 1, DWRGB(255, 255, 255), true);	//���S�_
		DrawRect(_p.sight_hitbox.left, _p.sight_hitbox.top, _p.sight_hitbox.right, _p.sight_hitbox.bottom, DWRGB(0,255,255), false);	//�����蔻��
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

/////*�󒆓G*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (int r = 0; r < MAX_ENEMY; r++) {
		if (_ae.enemy[r].hit == false && _ae.enemy[r].frame_in == true) {
			//DrawBmp(_ae.enemy[r].hitbox.left, _ae.enemy[r].hitbox.top, _ae.enemy[r].pic[_ae.enemy[r].anim]);
			DrawRect(_ae.enemy[r].hitbox.left, _ae.enemy[r].hitbox.top, _ae.enemy[r].hitbox.right, _ae.enemy[r].hitbox.bottom, DWRGB(255, 0, 0), false);
			DrawRect((int)_ae.enemy->center_x - 1, (int)_ae.enemy->center_y - 1, (int)_ae.enemy->center_x + 1, (int)_ae.enemy->center_y + 1, DWRGB(0, 255, 255), true);		//���S�_
		}

/////*�n��G*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (_ge.enemy[r].hit == false && _ge.enemy[r].frame_in == true) {
			//DrawBmp(_ge.enemy[r].hitbox.left, _ge.enemy[r].hitbox.top, _ge.enemy[r].pic[_ge.enemy[r].anim]);
			DrawRect(_ge.enemy[r].hitbox.left, _ge.enemy[r].hitbox.top, _ge.enemy[r].hitbox.right, _ge.enemy[r].hitbox.bottom, DWRGB(0, 255, 255), false);
		}
		if (r < 5 && _b.enemy[r].hit == false) {
			DrawRect(_b.enemy[r].hitbox.left, _b.enemy[r].hitbox.top, _b.enemy[r].hitbox.right, _b.enemy[r].hitbox.bottom, DWRGB(0, 255, 255), false);
		}
	}

	


}
