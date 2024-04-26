

//#include "../../../common.h"
#include "../../../Game.h"
//#include "../../Data/GameData.h"

//------------------------------------------------------------------------------------------------------------
// �󒆓G��n��G�A�{�X�Ȃǂ�2�ȏオ�Q�Ƃ���֐��̏W�܂�B
// �E�o�b�N�}�b�v����G���o��������֐�
// �E���ړ��̏����������ꂽ�֐�
// �E�g���Ă��Ȃ��Ǘ��ԍ���T���A�V�����G����鎞�ɑ������Ǘ��ԍ������������֐�
//------------------------------------------------------------------------------------------------------------





//==================================================================================================
// 								/*�o�b�N�}�b�v�̃G�l�~�[�ǂݍ���*/
// 
// ��ɒn��G���Q�Ƃ���֐��B
// �Œ�V���{���G(�{�X�����ȋ������������󒆓G)�Ȃǂ��Q�Ƃ���
// 
//�o�b�N�}�b�v�͕\��ʂƓ����ɓ����A�}�b�v�`�b�v�̑傫���Ɠ������𓮂����Ƃ��ɉ�ʂ̈�ԏ�[���������珇�ɓǂݏo���A�G�̔ԍ�������ꍇ�͓G���o��������
// �`�b�v�T�C�Y�� 32 * 32 
//�G�̔ԍ��̊���U���
//�󒆂̓G 0�`10�@����с@100�`110		
//�n��̓G 50�`53 �����  150�`163		

//==================================================================================================
	
void BackMap_Enemy_table(ENEMY* _ge, ENEMY* _ae, PLAYER* _p, MAP* _m, int _frame, BOSS* _b) {


	MAP_DATE BackMap = _m->backmap[_m->split_num];		//���݂̃o�b�N�}�b�v�̏��(1�X�e�[�W�ɂ�5�ɕ�������Ă��邤���̈��)


	if (_m->split_num > 0) {
		//�}�b�v�̐i�s�x���}�b�v�`�b�v�̃T�C�Y���i�񂾎�	�@&	�X�N���[�����x�Ɋւ�炸��񂾂��ʂ� &	 �}�b�v���X�V���ꂽ�u�Ԃ�y = 0�̎��͖�������
		if (BackMap.y % CHIP_SIZE == 0 && _frame % SCROLL_SPEED == 0 && BackMap.y > 0) {

			//�}�b�v�̍���(0,0)���灨(�s)�����čs��
			for (int r = 0; r < MAP_W; r++)
			{
				//�ϐ��̍쐬
				int SeachY_Axis = (BackMap.y / CHIP_SIZE * MAP_W) + r;	//y���̐i�s�x����(����ڂ� + r�s��)
				int enemyNum = BackMap.data[SeachY_Axis];				//�q�b�g�����G�̔ԍ�

				//�{�X�̃`�b�v����������///////////
				if (enemyNum == 60) {
					Boss_Create(_b);		//�{�X�̏����ݒ�
				}

				//�n��G(50�Ŋ���؂��)//
				else if ((enemyNum / 50) % 2 == 1) {
					int emptynum = SetEmptyNum(*_ge);

					//�n��G�̏o���ʒu���̐ݒ�
					int back_coord = SeachY_Axis;									//���}�b�v�ɂ�������W���L������
					int back_num = _m->split_num;								//���}�b�v�ԍ����L������
					float center_x = (float)((CHIP_SIZE * r) + (CHIP_SIZE / 2));	//�G�̒��S���W��ݒ�[(0���W����̋���) + �G�̑傫���̔���]

					//�G�̐���
					_ge->count++;
					 G_Enemy_Create(_ge,enemyNum, back_coord, back_num, center_x);
					 
				}



				//�Œ�󒆓G(�K���o������)
				else if ((enemyNum / 50) % 2 == 0)		//�ԍ���50�ȉ�
				{
					bool init = false;
				
					Air_Enemy_Create(_ae,enemyNum, _p->center_x);



				}
			}
		}
	}
}




//==================================================================================================
// 								/*�G�̈ړ��p�^�[��*/
// 
//�@�󒆓G����ɎQ�Ƃ���֐��B
//   �ꕔ�̒n��G(�X�N���[���ȊO�ł̈ړ�������O���v�^�[��h���O�����Ȃ�)���Q�Ƃ���
//==================================================================================================
void Enemy_Patterns(ENEMY_ST* _e,  int move_pat) {

	int dir_search = CHIP_SIZE + (CHIP_SIZE - (CHIP_SIZE * _e->speed));

	switch (move_pat) {
		/*���@�Ɍ������Đi��*/
	case 0:

		_e->center_x += (float)_e->speed * cos(_e->rad);		//X���̈ړ�
		_e->center_y += (float)_e->speed * sin(_e->rad);		//Y���̈ړ�
		break;


		/*Y���̈ʒu�Ɋ֌W�Ȃ����@�̕����Ɍ�����*/
	case 1:
		_e->center_x += (float)_e->speed * cos(_e->rad);		//X���̈ړ�
		_e->center_y += _e->speed;
		break;

		/*��ʂ̉�����o������*/
	case 2:
		if (_e->ac_count <= 5) {
			_e->center_y = SRN_H;
		}
		_e->center_x += (float)_e->speed * cos(_e->rad);		//X���̈ړ�
		_e->center_y -= _e->speed;		//Y���̈ړ�
		break;
		/*���̂܂ܒ��i����*/
	case 3:
		_e->center_y += (float)_e->speed;		//Y���̈ړ�
		break;
		/*�����x�����Z����dir�̕��ֈړ�����*/
	case 4:
		_e->center_x -= (float)1 * cos(_e->rad) * _e->dir;		//X���̈ړ�
		_e->center_y -= (float)_e->speed * _e->acc;		//Y���̈ړ�

		/*����*/
		_e->speed += _e->speed * _e->acc;

		break;

		/*�����x�����Z�����������ɓ�����*/
	case 5:
		_e->center_y++;
		_e->center_x -= (float)_e->speed * _e->dir;		//X���̈ړ�
		/*����*/
		_e->speed += _e->speed * _e->acc;
		break;

	}
}




//==================================================================================================
// 						/*��ʂɏo�����Ă��Ȃ��Ǘ��ԍ���������*/
// 
//�@�V������������ۂɉ�ʓ��ɂ��Ȃ����������t���O�������Ă��Ȃ��Ǘ��ԍ���T���A���̔ԍ���Ԃ�
// �ő�o�����𒴂��Ă�������Ȃ��ꍇ��'99'��Ԃ�(number��'0'�̒l���g���Ă��邽��)
//==================================================================================================
//�󒆁A�n�セ�ꂼ����Ǘ����Ă���\���̂���󂢂Ă���Ǘ��ԍ���T��
int SetEmptyNum(ENEMY _e) {
	for (int i = 0; i < MAX_ENEMY; i++) {
		if (_e.enemy[i].frame_in == false) {
			return i;
		}
	}
	return 99;
}

//�G�e�ŋ󂢂Ă���Ǘ��ԍ���T��
int E_shotGetEmptyNum(ENEMY_SHOT _es)
{
	for (int i = 0; i < MAX_E_SHOT; i++) {
		if (_es.shot[i].F == false) {
			return i;
		}
	}
	return 99;
}


