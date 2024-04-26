
//#include "../../../common.h"
#include "../../../Game.h"
#include "../../Data/GameData.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*�G�̒e�̏����Z�b�g*/


//=================================================================
//							������
// 
// ���[�v��ENEMY�\���̂ɓ����Ă���S�Ă̓G�X�e�[�^�X������������
//=================================================================
void E_Shot_Init(ENEMY_SHOT* _e){
	for (int i = 0; i < MAX_E_SHOT; i++) {
		memcpy(&_e->shot[i], &E_Init, sizeof(E_SHOT_Init));		//�f�[�^��������
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*�G�̒e�̏����Z�b�g*/

//=================================================================
//							�G�̐���
// 
// ��̃X�e�[�^�X�ɏ������ĕԂ�
// �E���˃t���O�𗧂ĂāA���˂������W����������ݒ肷��
// �E�����蔻��̃T�C�Y����RECT�ɒl������
// �E�v���C���[�ւ̊p�x��rad�ɓ����
// �Edir(����)����Z���邱�Ƃŕ����𐮂���
// �E�摜��ǂݍ���
//=================================================================
void E_Shot_Create(ENEMY_SHOT* _s,float px,float py,float x,float y, float _dir) {

	int Nextshotnum = E_shotGetEmptyNum(*_s);		//�t���O�̗����Ă��Ȃ��G�e��T��
	if (Nextshotnum == 99) { return; }						//99�G���[���o�ĂȂ���Ώ�����i�߂�


	_s->shot[Nextshotnum] = E_SHOT_Init;

	_s->shot[Nextshotnum].F = true;
	_s->shot[Nextshotnum].center_x = x;
	_s->shot[Nextshotnum].center_y = y;


	/*hitbox������*/
	Hitbox_Sync(&_s->shot[Nextshotnum].hitbox, BULLET_SIZE, &x, &y, 0, 0, 0);

	//_s->shot[Nextshotnum].pl_x = (int)_p.center_x;				//���̊֐��ɓ������u�Ԃ̃v���C���[���W���L����(X��)
	//_s->shot[Nextshotnum].pl_y = (int)_p.center_y;				//���̊֐��ɓ������u�Ԃ̃v���C���[���W���L����(Y��)
	_s->shot[Nextshotnum].rad = atan2(py - (float)_s->shot[Nextshotnum].hitbox.top, px - (float)_s->shot[Nextshotnum].hitbox.left);		//�O�p�`�̊p�x���Z�o����

	//���˂���e�̌����𐳕��ɂ���ĕ������i��
	if (_dir != 0) {
		_s->shot[Nextshotnum].rad += _dir;
	}

	//�摜�̓ǂݍ���
	LoadDivBmp(PngData.Shot,
		0, 0, 7, 7, 2, 1, _s->shot[Nextshotnum].pic);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*�X�V*/


//=================================================================
//				��ʂɑ��݂���G�e�̏��X�V
// 
/// ���˃t���O�������Ă���e�̂ݏ����ɓ���
// �E�e����ʓ��ɂ���ꍇ�͒e�̍��W���ړ�������
// �E���W�Ɠ����蔻��𓯊�������
// �Edir(����)����Z���邱�Ƃŕ����𐮂���
// �E�摜��ǂݍ���
//=================================================================
void E_Shot_Update(PLAYER* _p, ENEMY_SHOT* _s, SCENE* _sc) {

	for (int r = 0; r < MAX_E_SHOT; r++) {
		if (_s->shot[r].F == true) {
			//�e����ʓ��ɂ���ꍇ
			if (_s->shot[r].hitbox.left >= 0 && _s->shot[r].hitbox.top >= 0 && _s->shot[r].hitbox.left <= SRN_W && _s->shot[r].hitbox.top <= SRN_H) {
				_s->shot[r].center_x += E_SHOT_SPEED * cos(_s->shot[r].rad);		//X���̈ړ�
				_s->shot[r].center_y += E_SHOT_SPEED * sin(_s->shot[r].rad);		//Y���̈ړ�

				/*�e�̓����蔻��̓���*/
				Hitbox_Sync(&_s->shot[r].hitbox, BULLET_SIZE, &_s->shot[r].center_x, &_s->shot[r].center_y, 0, 0, 0);
			}

			//�e����ʊO�ɏo���ꍇ
			else {
				_s->shot[r].hitbox.left = 0;		//�ʒu������������
				_s->shot[r].hitbox.top = 0;		//����
				_s->shot[r].F = false;				//�t���O��OFF�ɂ���
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*�G�̒e�̃X�v���C�g��\������*/


void E_Shot_Draw(ENEMY_SHOT* _s) {

	for (int r = 0; r < MAX_E_SHOT; r++) {
		if (_s->shot[r].F == true) {
			DrawBmp(_s->shot[r].hitbox.left, _s->shot[r].hitbox.top, _s->shot[r].pic[0]);
		}
	}
}


