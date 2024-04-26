
#include "Air_Enemy.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

															/*�󒆓G�̏�����*/
//==================================================================================================================================
//								�󒆓G�̏�����
// 
// �G��S�ď��������A���̕ϐ�������������
//==================================================================================================================================

void Air_Enemy_Init(ENEMY* _e) {
	for (int i = 0; i < MAX_ENEMY; i++) {
		memcpy(&_e->enemy[i], &E_Init, sizeof(ENEMY_ST));		//�f�[�^��������
	}
	_e->count = 0;
	_e->pop_F = false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

															/*�󒆓G�̃f�[�^�Z�b�g*/



//==================================================================================================================================
//											�󒆓G�̐���
// 
//  �����͓G�̔ԍ��A�v���C���[��X�����W
// �Enumber��100�𒴂��Ă���ꍇ�͕S�̈ʂ̒l��type�Ƃ��Ď󂯎��A�\�ƈ�̈ʂ̒l��number�Ƃ��Ď擾����
// �E�擾����number�����Ɏ�ނ��Ƃ̏����X�e�[�^�X��������
// �E�����Ŏ󂯎�����f�[�^���X�e�[�^�X�ɑ�����Ă���
// �E�o���ʒu�̐ݒ�
// �Etype�ɂ���ē��X�e�[�^�X���ω�����ꍇ�͂����Őݒ�
// �E�O���t�B�b�N�f�[�^�̑��
// �E�����蔻��Ɖ摜�̈ʒu�𓯊�������
// �E���E�ǂ���ɐi�ނ��̌��������߂�
//		�쐬�����X�e�[�^�X��Ԃ�l�ƂĕԂ�
//==================================================================================================================================
void Air_Enemy_Create(ENEMY* _e,int number, int Player_x) {
	

	int EmptyNum = SetEmptyNum(*_e);
	if (EmptyNum == 99) { return; }		//�G���[(ENEMY�̒���1�̂�frame_in��false�o�Ȃ������ꍇ)

	//������
	_e->enemy[EmptyNum] = E_Init;

	int type = 0;			//�����X�e�[�^�X������type��n�����߂̕ϐ�
	
	//number�̕S�̈ʂ̓G�l�~�[��Type��\��(100 = Type:1 , 200 = Type:2)
	if (number >= 100) {
		type = number / 100;								//�i���o�[��100�Ŋ�����Type�����o��
		number = number - (100 * type);						//100�̈ʂ���菜�����G�̃i���o�[�����o��
	}

	//[�Z�[�t�e�B] : �i���o�[��0�`10�łȂ���Ώ������~�߂�
	if (number < 0 && number > 10) {
		_e->enemy[EmptyNum].frame_in = false;				//framein��false�ɂ��Ă����΋󂢂Ă���X�e�[�^�X�ƔF�������
		return;								//�������~�߂�
	}		

	//_e->enemy[EmptyNum] = A_enemyDate[number];
	//��b�X�e�[�^�X���R�s�[ or �e�l�̃Z�b�g
	memcpy(&_e->enemy[EmptyNum], &A_enemyDate[number], sizeof(ENEMY_ST));//�f�[�^���Z�b�g����

	//[�Z�[�t�e�B] : type�������l�ȏ�̒l���o�Ă���ꍇ��0(�ʏ퓮��)��type�Ƃ��Đ�������
	if (_e->enemy[EmptyNum].type < type) {	
		type = 0;	
	}

	_e->enemy[EmptyNum].type = type;										//�f�[�^�Ƀ^�C�v����

	//�o���ʒu�̐ݒ�
	if (number == 0 || number == 4 || number == 6) {		//�Ώ�:�g�[���C�h,�W�A��,�e���W
		_e->enemy[EmptyNum].center_x = Player_x + 250 + (rand() % 200);		//Player���� 250 ���ꂽ�ʒu����X�� 0�`200 �̗��������Z����X���W���w�� 
	}		
	else {													//����ȊO
		_e->enemy[EmptyNum].center_x = (float)(rand() % SRN_W + 1); 			//��ʂ̉��T�C�Y�Ɏ��܂�͈͂Ń����_����X���W���w��
	}

	//���������Ŏ擾����X���W����ʂ̉��T�C�Y�𒴂��Ă�����
	if (_e->enemy[EmptyNum].center_x >= SRN_W) { _e->enemy[EmptyNum].center_x = Player_x - 350 + (rand() % 200); }

	/*�����̍s���p�^�[���̂���G�̃X�e�[�^�X�ύX*/
	//�W�\�[
	if (_e->enemy[EmptyNum].number == 3) {
		if (_e->enemy[EmptyNum].type > 0 && _e->enemy[EmptyNum].type < 3) { _e->enemy[EmptyNum].points = 100; }	//Type�ɂ�链�_�̕ϓ�
	}
	//�U�K�[�g
	else if (_e->enemy[EmptyNum].number == 7) {
		if (_e->enemy[EmptyNum].type == 0) { _e->enemy[EmptyNum].points = 100; }
		if (_e->enemy[EmptyNum].type == 1) { _e->enemy[EmptyNum].points = 150;	_e->enemy[EmptyNum].speed = 4.0; }
		if (_e->enemy[EmptyNum].type == 2) { _e->enemy[EmptyNum].points = 200; }
		if (_e->enemy[EmptyNum].type == 3) { _e->enemy[EmptyNum].points = 300;	_e->enemy[EmptyNum].speed = 4.0; }
	}

	/*�摜��������*/
	LoadDivBmp(PngData.A_Enemy, 0, 0, _e->enemy[EmptyNum].pic_size, _e->enemy[EmptyNum].pic_size, A_ENEMY_PIC_LINE, 11, _e->enemy[EmptyNum].pic);

	/*���S���W����'�����蔻��'��'�摜'�̍���ƉE���̍��W*/
	Hitbox_Sync(&_e->enemy[EmptyNum].hitbox, _e->enemy[EmptyNum].hitbox_size,				//RECT:�����蔻�� , int:�����蔻��̒��S���W���猩���T�C�Y
		&_e->enemy[EmptyNum].center_x, &_e->enemy[EmptyNum].center_y,						//float:���S���W(X) , ���S���W(Y)
		&_e->enemy[EmptyNum].pic_x, &_e->enemy[EmptyNum].pic_y, _e->enemy[EmptyNum].pic_size);	//int:�摜�����x���W , �摜�����y���W , int:�摜�̃T�C�Y


	/*��ʂ̒��S�����ɍ��E�ǂ���Ɋ���Ă��邩�ɂ���čŏ��̐i�s�����������߂�*/
	_e->enemy[EmptyNum].dir = _e->enemy[EmptyNum].hitbox.left < CENTER_X ? -1 : 1;


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*�󒆂̓G�̏����X�V����*/


//==================================================================================================================================
//											�󒆓G�̍X�V
// 
// ��ʓ��ɏo�����Ă���Ǘ��ԍ��ɂ��Ĉړ��������e���蓙���s��
// �E��e���肪�o�Ă���ꍇ�͐��t���[����1�x�A�j���[�V�����ԍ���i�߂āA���̒l�ɒB�����炻�̔ԍ��̓G�f�[�^������������
// �E���j���肪�o�Ă��Ȃ��Ԃ͓G�̎�ނɉ������ŗL�̓��������Ȃ�����W�Ɠ����蔻��̓����A��ʓ��ɂ��邩�̔��������
// �E���j���肪�o�Ă��Ȃ��Ԃ̓V���b�g�ɑ΂����e������s���Ă���B�ꕔ�G�͓���ȏ���������
//==================================================================================================================================
void Air_Enemy_Update(PLAYER* _p, ENEMY* _e, ENEMY_SHOT* _s, SCENE* _sc) {


	for (int r = 0; r < MAX_ENEMY; r++) {
		if (_e->enemy[r].frame_in == true) {	//�o���t���O��ON�Ȃ珈���ɐi��

			bool frame_out = false;					//��ʂ���������邩�𔻒������(�����̒���true�ɂȂ����ꍇ�A�����̍Ŋ��ŉ�ʂ���폜 & ���̏��������s��)

			_e->enemy[r].ac_count++;				//�G��̂ɂ��Ă���ʂ̃^�C�}�[��i�߂�(�s�������Ɏg��)

			//��e���肪true�̏ꍇ
			if (_e->enemy[r].hit == true) {
				if (_e->enemy[r].ac_count % 2 == 0) {		//2�t���[���Ɉ�x����
					_e->enemy[r].anim++;					//anim�ϐ������Z����(�����A�j���[�V�����̐i�s)

				}
				if (_e->enemy[r].anim > 11) {				//anim�ϐ��� 11�����傫���Ȃ�����
					memcpy(&_e->enemy[r], &E_Init, sizeof(ENEMY_ST));		//�G�̏�������������
					_e->count--;
				}

				continue;		//�ȍ~�̏����͂������̊Ǘ��ԍ��̃��[�v��
			}

			//�ȉ��͔�e���肪�o�Ă��Ȃ��ꍇ�ɏ����ɐi��

			A_Enemy_Moving(_p, &_e->enemy[r], _s, _sc->frame_count);		//�G�̍s��(number�ɂ���ĈقȂ鋓��������)

			if (!_e->enemy[r].frame_in) { _e->count--; continue; }			//�ŗL�̍s���ŉ�ʂ��炢�Ȃ��Ȃ����ꍇ�̓J�E���g�����炵���̊Ǘ��ԍ��֐i��

			//�����蔻��̓���
			Hitbox_Sync(&_e->enemy[r].hitbox, _e->enemy[r].hitbox_size,				//RECT:�����蔻�� , int:�����蔻��̒��S���W���猩���T�C�Y
				&_e->enemy[r].center_x, &_e->enemy[r].center_y,						//float:���S���W(X) , ���S���W(Y)
				&_e->enemy[r].pic_x, &_e->enemy[r].pic_y, _e->enemy[r].pic_size);


			//���W����ʊO�ɏo���ꍇ�̍��W�̏�����
			if (_e->enemy[r].ac_count > 50) {				//�o�����Ă���50�t���[�������Ă��画��ɓ���(�o�����Ă��΂炭�͉�ʒ[�ɓ������Ă��Ă��������Ȃ�)
				if (!Hit_box(_e->enemy[r].hitbox, winView))		//�����蔻��ƃE�B���h�E�T�C�Y��RECT�Əd�Ȃ��Ă��Ȃ� = ��ʊO �Ȃ�
				{
					memcpy(&_e->enemy[r], &E_Init, sizeof(ENEMY_ST));		//�G�̏�������������
					_e->count--;
				}
			}

			/*�V���b�g�ɑ΂����e����*/
			for (int shotNum = 0; shotNum < MAX_SHOT; shotNum++) {											//��ʓ��ɑ��݂���V���b�g�̐����[�v����
				if ( _p->shot[shotNum].F == true) {						//�V���b�g�̏o���t���O��true�Ȃ�
					_e->enemy[r].hit = Hit_box(_p->shot[shotNum].hitbox, _e->enemy[r].hitbox);		//���@�̃V���b�g�ɑ΂����e����(Hit_box�֐��ɂ�蔻��B�߂�l��ture�Ȃ��e)
					if (_e->enemy[r].hit == true) {											//�V���b�g�ɔ�e���Ă�����
						//�V���b�g�̏�����
						memcpy(&_p->shot[shotNum], &P_SHOT_init, sizeof(P_SHOT_ST));					//���������ԍ��̃V���b�g�̏�������������
						_p->shot_count--;														//��ʂɑ��݂���V���b�g�̐������炷

						/*���j���̏���*/
						if (_e->enemy[r].number != 10) {		//�u�o�L�����v�ȊO�̓G�Ȃ�
							_e->enemy[r].ac_count = 0;				//�ŗL�̃^�C�}�[������������(���j�A�j���[�V�����Ɏg�p���邽��)
							_e->enemy[r].anim = 6;					//���j�A�j���[�V�����ԍ��Ɉڍs����
							state.score += _e->enemy[r].points;		//�G�l�~�[�̎��_�����X�R�A�ɉ��Z����
						}
						//��O����
						else {
							_e->enemy[r].hit = false;			//number == 10�u�o�L�����v�̂ݒe�̃q�b�g������󂯂Ă����ł��Ȃ�(hit���肪�o�Ă��Ă�false�ɖ߂�)
						}
						break;								//���[�v�𔲂���
					}
				}
			}
		


		}
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

																	/*�󒆂̓G�̃X�v���C�g��\������*/

//==================================================================================================================================
//											�󒆓G�̕`��
// 
// ���j���肪�o��܂ł͓G�̃X�v���C�g��\������B���j���肪�o�Ă���Ԃ͔����A�j���[�V�������Đ�����
// �E�G�̍ő吔����������frame_in��true�̓G�̃X�v���C�g��`�ʂ���
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

																	/*�G�̏o���e�[�u��*/

//==================================================================================================================================
//											�󒆓G�̏o���e�[�u��
// 
//GameDate�ɂ���ꎟ���z�񂩂�G�̔ԍ����Q�Ƃ��A�ő�5�̂̓G���o������B�e�[�u����int�z��� int table[MAX_LEVEL * 6]; �ƂȂ��Ă���
// �EPop�t���O�����ďo�������邩�m�F����
// �ELv��x�A���[�v�ŉ��Z����l�� r�Ƃ����Ƃ��A x���r�s���m�F���Ă��̏ꏊ�̓G�̎�ޔԍ����擾����
// �E�󂢂Ă���Ǘ��ԍ��𒲂ׁA���������ꍇ�͂��̔ԍ��œG�𐶐��B������Ȃ������ꍇ�͓G�𐶐����Ȃ�
//==================================================================================================================================
void Air_Enemy_table(ENEMY* _e, PLAYER* _p, int _frame,int splitnum,bool boss_F,bool boss_down) {

	//�{�X�̏o���t���O�������Ă���Ԃ͓���̓G���������Ȃ� �܂� ���j�t���O�������Ă���ꍇ�͓G�̏o�����~�߂�
	if (boss_F ) {
		if (rand() % 100 == 0 && !boss_down) {
			if (3 > _e->count) {

				Air_Enemy_Create(_e, 107, _p->center_x);		//�G�̐���
				_e->count++;
			}
		}
		return;
	}

	//���x����0�̂ƕ����}�b�v�ԍ���0�̎��͏o�������A����ȊO�Ȃ�o�����͂��߂�
	_e->pop_F = A_Date_Table.Lv == 0 || splitnum == 0 ? false : true;

	if (_frame % (SRN_H / 2) == 0) {	//��莞�ԂŃ��x�����㏸����(�󒆓G�̏o���e�[�u�������߂�)
		A_Date_Table.Lv++;
	}

	REPEAT(A_Date_Table.Lv,0,MAX_LEVEL - 1)


	int levellow = (A_Date_Table.Lv * 6);		//�G�̃��x���e�[�u���̗�

	int count = 0;


	if (rand() % 35 == 0) {

		/*Lv�ɂ��G�̎�ނƏo�����̊Ǘ�*/
		if (_e->pop_F == true) {

			//�o���e�[�u���͈ꎟ���z��̂��߁A[���x�� * �s]�œG�̃i���o�[�𒲂ׂĂ��� 
			for (int r = 0; A_Date_Table.table[levellow + r] != 99; r++) {		//�e�[�u���ł� '99' ���o���ő吔�Ƃ��Ĉ����Ă���(NULL�̂悤�Ȉ���)	
				if (r >= 6) { break; }	//[�Z�[�t�e�B]:�z�肳�ꂽ�ő�o�����𒴂���ꍇ�͋����Ń��[�v�𔲂���
				count = r + 1;				//���̃��x���ŏo������G�̐����擾
			}
			int s = (rand() % count);

			//(�󒆓G�̔���(�o�����ȏ�ɓG�͔��������Ȃ�)
			if (count > _e->count) {
				int enemynumber = A_Date_Table.table[levellow + (rand() % count)];		//�G�̎�ނ������i���o�[

				Air_Enemy_Create(_e, enemynumber, _p->center_x);		//�G�̐���
				_e->count++;

			}
		}
	}
}


//==================================================================================================================================
//											�󒆓G��1�t���[���̓���
// 
//�ԍ���50�ȉ��͋󒆓G�A50�ȏ�͒n��G
//
// 0 = �g�[���C�h , 1 = �^���P�� , 2 = �M�h�X�p���I , 3 = �W�]�[ , 4 =  �W�A�� , 5 =  �U�J�[�g ,
// 6 =  �J�s , 7 =  �e���W , 8 =  �u���O�U�J�[�g , 9  =  �K���U�J�[�g , 10 =  �o�L����
//==================================================================================================================================
void A_Enemy_Moving(PLAYER* _p, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame) {

	//�G�e�̔��˂��\���̔���
	bool shot_ok = true;				//true�Ȃ�e�̔��˂��ł���
	int s = E_shotGetEmptyNum(*_s);		//�t���O�̗����Ă��Ȃ��G�e��T��
	if (s == 99) { shot_ok = false; }	//s��99 -> �e�̏���������Ă��邽�ߒe�����˂ł��Ȃ��悤�ɂ���
	
	/*��ʂɓo�ꂵ��5�t���[���ڂ܂ł͎��@�ւ̊p�x���Z�o����**/
	if (_e->ac_count < 5) {
		_e->rad = _e->rad = atan2(_p->center_y - _e->center_y, _p->center_x - _e->center_x);		//�O�p�`�̊p�x���Z�o����
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

																	/*�󒆓G�̌ŗL����*/



//===================================================================================================
//								�g�[���C�h�@[�����O�^�̓G]
//
//  �T�v: X�����v���C���[�Əd�Ȃ�����i�s�����Ƃ͋t�̕����ɓ�������
// �Etype 0:�e�������Ȃ�
// �Etype 1:�e������
// 
// �Epat 0: �o�ꂵ���Ƃ��Ƀv���C���[���������W�Ɍ������Đi��
//			�v���C���[��Y�����d�Ȃ����玟�̃p�^�[����(type 1�Ȃ�ˌ�������)
// �Epat 1: �ݒ肵�����x�ɒB����܂Ō�������
// �Epat 2: �t�����ɉ������Ȃ��瓦������
//===================================================================================================
void TOROID(float px, float py,int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	/*�e�����O�̓���*/

	switch (_e->pat) {
	case 0://�o�����Ƀv���C���[���������W�Ɍ������Đi��

		Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����

		/*Y���̋����֌W�Ȃ��v���C���[�̂̓����蔻���X�����d�Ȃ�����*/
		if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//���@��Y������v�����Ƃ�
			if (_e->type == 1) {
				if (shot_ok) {
					 E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//���@�Ɍ������Ēe�𔭎˂���
				}
			}

			//�p�^�[���̕ύX
			_e->pat = 1;			//�����p�^�[���ֈڍs����
			
			//��������������߂�
			_e->dir = _e->center_x > px ? -1 : 1;
		}
		break;
	case 1:	//��葬�x�ɗ�����܂Ō�������
			Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����

			_e->anim = A_Enemy_Anim(*_e, _frame, 5);	//�A�j���[�V�����̐i�s
			_e->speed -= (float)0.1;					//����
			if (_e->speed < 0.6) { _e->pat = 2; }		//���x��0.6����������瓦���p�^�[���Ɉڍs����

		break;
	case 2:	//���Ε����Ɍ������ē�������
			Enemy_Patterns(_e,  5);		//�s���p�^�[�� 5:�����x�����Z�����������ɓ�����

			_e->anim = A_Enemy_Anim(*_e, _frame, 5);
		break;

	}
}

//===================================================================================================
//						�^���P�� [�L�l�퓬�@]
//
// �T�v:���t���[���Ŏˌ������A��ʂ̏�����ɓ�������
// �Epat 0:��葬�x�œo�ꎞ�̃v���C���[���������W�ɑO�i�B
//			�G�̓����^�C�}�[��100�t���[���ڂɒB�����瓮�����~�߂Ďˌ�����
// �Epat 1:���]�A�j���[�V�������Đ����I�������ʂ̏�����Ɍ������ē�������
//===================================================================================================
void TORKAN(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	
	switch (_e->pat) {
	//�e�����O�̓���
	case 0:			
		Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
		if (_e->ac_count >= 100) {

			if (shot_ok) {
				E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//���@�Ɍ������Ēe�𔭎˂���
			}
			_e->pat++;
			_e->rad = atan2(py - _e->center_y, px - _e->center_x);		//�O�p�`�̊p�x���Z�o����
		}
		break;
	//�e����������̓���
	case 1:		
			if (_e->anim < 14 && _frame % 5 == 0) {
				_e->anim++;
			}
			if (_e->anim >= 14) {
				_e->center_x -= (float)_e->speed * cos(_e->rad);		//X���̈ړ�
				_e->center_y -= (float)_e->speed * sin(_e->rad);		//Y���̈ړ�
			}

		break;

	}
}

//===================================================================================================
//								�M�h�X�p���I [�j��\�ȓG�e]
//
// �T�v:�����Ŕ��ł���e�B
// �E��葬�x�œo�ꎞ�̃v���C���[���������W�ɑO�i����
//===================================================================================================
void GIDDOSPARIO(ENEMY_ST* _e,int _frame) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);
	Enemy_Patterns(_e,  0);		//�s���p�^�[�� ��4���� 0:���@�Ɍ�����
}

//===================================================================================================
//									�]�V�[ [��]����M�U�M�U]
//
// �ŏ��̓v���C���[�Ɍ������Ă��邪�r���ŕ����]������B���̕�����type�ɂ���ĕω�����
// �Etype 0 : �v���C���[�Ɍ������đO�i�B���t���[���o�ߌ�A�����_���ȕ����֑O�i����
// �Etype 1 : �v���C���[�Ɍ������đO�i�B���t���[���o�ߌ�A�v���C���[�̍��W���Č������A���̕����ɑO�i����
// �Etype 2 : ��ʂ̉�����o�ꂵ�A�v���C���[�Ɍ������đO�i�B���t���[���o�ߌ�A
//			�v���C���[�̈ʒu���Č������邪Y���̈ړ��Ŋp�x���g��Ȃ����߁A���E�̌��������v���C���[�̕�������(�������ɂ͌�����Ȃ�)
// 
// �E�ݒ肳�ꂽ�����ɑO�i�B
// �E�����t���[���^�C�}�[�����̒l�ɒB����ƕ����]�� & �ˌ�
//===================================================================================================
void ZOSHI(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	
	//�A�j���[�V�����i�s(�S�̃^�C�v����)
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);

	/*�^�C�v2�̂݉�ʂ̉������֌���������ȍs���p�^�[��*/
	if (_e->type == 2) {		_e->pat = 2;	}

	Enemy_Patterns(_e,  _e->pat);		//pat0:���@�Ɍ�����,pat2:��ʂ̉�����

	//150�t���[���Œe������ & �i�s�������Đݒ肷��
	if (_e->ac_count % 150 == 0 && _e->ac_count != 0) {
		//�e������
		if (shot_ok) {
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//���@�Ɍ������Ēe�𔭎˂���
		}		
		
		//�i�s�����̍Đݒ�
		if (_e->type == 0) {		//�^�C�v0:�����_���Ɉړ�
			_e->rad = (float)(rand() % 360);
		}
		else {						//�^�C�v0�ȊO:�v���C���[�̕���������
			_e->rad = (float)atan2(py - _e->center_y, px - _e->center_x);		//�O�p�`�̊p�x���Z�o����
		}
	}
}

//===================================================================================================
//									�W�A�� [H�݂����ȓz]
//
//  �T�v: X�����v���C���[�Əd�Ȃ�����i�s�����Ƃ͋t�̕����ɓ�������B�g�[���C�h�������m�͈͂��L��
// �Etype 0:�e�������Ȃ�
// �Etype 1:�e������
// 
// �Epat 0: �o�ꂵ���Ƃ��Ƀv���C���[���������W�Ɍ������Đi�ށB
//			�v���C���[��Y�����d�Ȃ����玟�̃p�^�[����(type 1�Ȃ�ˌ�������)
// �Epat 1: �ݒ肵�����x�ɒB����܂Ō�������
// �Epat 2: �t�����ɉ������Ȃ��瓦������
//===================================================================================================
void JARA(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {

	switch (_e->pat) {

	case 0:
		Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����

		/*�v���C���[��Y������v�����Ƃ��̏���*/
		if (px - (p_hitoboxsize * 2) <= _e->center_x && px + (p_hitoboxsize * 2) >= _e->center_x && py > _e->center_y) {		//���@��Y������v�����Ƃ�
			if (_e->type == 1) {
				if (shot_ok) {
					E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//���@�Ɍ������Ēe�𔭎˂���
				}
			}
			_e->pat = 1;			//�p�^�[����ύX����

			_e->dir = _e->center_x > px ? -1 : 1;

		}
		break;
	//����
	case 1:

			_e->anim = A_Enemy_Anim(*_e, _frame, 5);
			Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
			_e->speed -= (float)0.1;		//���x��1�ɂ���
			if (_e->speed <= 1.5) {
				_e->pat++;
			}
		break;
	//���@�Ƌt�����ɓ�����
	case 2:
			Enemy_Patterns(_e,  5);		//�s���p�^�[�� 5:�����x�����Z�����������ɓ�����
			_e->center_y +=_e->speed ;	//Y���ړ��̕␳�l
			_e->anim = A_Enemy_Anim(*_e, _frame, 5);
		break;

	}
}

//===================================================================================================
//									�J�s [�قڃr�b�N�o�C�p�[]
//
// �T�v:���t���[���Ŏˌ������A��ʂ̏�����ɓ�������B�ˌ����������~�߂Ȃ�
// �Epat 0:��葬�x�œo�ꎞ�̃v���C���[���������W�ɑO�i�B
//			�G�̓����^�C�}�[��100�t���[���ڂɒB������ˌ�����
// �Epat 1:�������Ȃ����ʂ̏�����Ɍ������ē�������B���t���[���Ŏˌ�������
//===================================================================================================
void KAPI(float px, float py, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	
	switch (_e->pat) {
	
	//�e�����O�̓���
	case 0:
		Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
		if (_e->ac_count >= 100) {

			_e->pat++;
			_e->rad = atan2(py - _e->center_y, px - _e->center_x);		//�������鎞�͎΂߂Ɉړ�����
			_e->speed = 0.5;											//���]�����Ƃ��̏����x

			//���E�ǂ���Ɍ����������߂�
			_e->dir = _e->center_x > px ? -1 : 1;
			_e->ac_count = 0;
		}
		break;
	//�e����������̓���
	case 1:
			if (_e->ac_count % 30 == 0) {
				if (shot_ok) {
					E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//���@�Ɍ������Ēe�𔭎˂���
				}
			}
			if (_e->anim < 14 && _frame % 5 == 0) {
				//_e->anim++;
			}
			if (_e->anim >= 14) {
				_e->center_x -= (float)1 * cos(_e->rad) * _e->dir;		//X���̈ړ�
				_e->center_y -= (float)_e->speed * _e->acc;		//Y���̈ړ�
			}
			/*����*/
			if (_frame % 2 == 0) {
				_e->speed += _e->speed * _e->acc;
			}
		break;
	}
		
}

//===================================================================================================
//									�e���W ['U'�݂����Ȍ`�������퓬�@]
//
// �T�v:�ˌ����Ȃ���O�i���A�v���C���[��Y���Əd�Ȃ������ʂ̏�����ɓ�������
// 
// �Epat 0:�o�ꎞ�̃v���C���[���������W�ɑO�i�B
//			���Ԋu�Ŏˌ�����
// �Epat 1:��葬�x�܂Ō�������
// �Epat 2:�������Ȃ����ʂ̏�����Ɍ������ē�������B���t���[���Ŏˌ�������
//===================================================================================================
void TERRAZI(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	if (_e->ac_count % 40 == 0) {
		if (shot_ok) {
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);		//���@�Ɍ������Ēe�𔭎˂���
		}
	}
	//�e�����O�̓���
	if (_e->pat == 0) {
		Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
		if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//���@��Y������v�����Ƃ�
			_e->pat++;
		}
	}
	/*���m�͈͂Ɏ��@�������ꍇ*/
	else if (_e->pat == 1) {
		Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
		_e->speed -= (float)0.1;		//��������
		if (_e->speed <= 0.8) {				//��葬�x�܂ŉ���������
			_e->pat++;						//�����p�^�[���ֈڍs
			_e->rad = atan2(py - _e->center_y, px - _e->center_x);		//������p�x���Z�o(�ړ��ł�X�����̂ݍ̗p)

			//���E�̌��������肷��
			_e->dir = _e->center_x > px ? -1 : 1;
		
		}
	}
	/*�G�l�~�[�̑��x�����l�܂ŉ���������*/
	else if (_e->pat == 2) {
		Enemy_Patterns(_e,  4);		//�s���p�^�[�� 0:���@�Ɍ�����

	}
}

//===================================================================================================
//									�U�K�[�g [������]
//
// �T�v:type�ɂ���ď����͈Ⴄ���A�����ɒB����ƒe�𐶐����ĉ�ʂ��������
// 
// �E�O�i���A�����Ŏˌ������ď�����
// 
// �Etype 0:���������ɑO�i���A		  �u���t���[����v�A�e�𔭎˂��ĉ�ʂ��������B
// �Etype 1:�v���C���[�̕����ɑO�i���A�u���t���[����v�A�e�𔭎˂��ĉ�ʂ��������B
// 
// �Etype 2:���������ɑO�i���A		  �u�v���C���[��Y�����d�Ȃ�Ɓv�e�𔭎˂��ĉ�ʂ��������
// �Etype 3:�v���C���[�̕����ɑO�i���A�u�v���C���[��Y�����d�Ȃ�Ɓv�e�𔭎˂��ĉ�ʂ��������B
//===================================================================================================
void ZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);

	/*�e�����O�̓���*/
	if (_e->pat == 0) {
		if (_e->type % 2 != 0) {			//type 0 or type 2
			Enemy_Patterns(_e,  0);		//�s���p�^�[�� 3:��ʉ��Ɍ����Ē��i����
		}
		else {								//type 1 or type 3
			Enemy_Patterns(_e,  3);		//�s���p�^�[�� 0:���@�Ɍ�����
		}

		//�e��������
		if (_e->type < 2) {					//type 0 or type 1
			if (_e->ac_count > 75) {		//75�t���[���o�߂�����
				_e->pat = 1;				//�ˌ��p�^�[����
			}
		}
		else {								//type 2 or type 3
			if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//���@��Y������v�����Ƃ�
				_e->pat = 1;				//�ˌ��p�^�[����
			}
		}
	}

	/*�e�������ĉ�ʂ��������*/
	else if (_e->pat == 1) {
		if (shot_ok) {
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, 0);	//���@�Ɍ������Ēe�𔭎˂���
		}
		_e->frame_in = false;			//��ʂɂ���Ƃ����t���O��false�ɂ���
	}
}

//===================================================================================================
//									�v���O�U�K�[�g [�ԓ_�̓�����������]
//
// �T�v:�����̓U�K�[�g�Ɠ����B�e�̔��ː����Ⴄ
// 
// �E�O�i���A�����Ő��Ɏˌ������ď�����B
// 
// �Etype 0:���������ɑO�i���A		  �u���t���[����v�A���ɒe�𔭎˂��ĉ�ʂ��������B
// �Etype 1:�v���C���[�̕����ɑO�i���A�u���t���[����v�A���ɒe�𔭎˂��ĉ�ʂ��������B
// 
// �Etype 2:���������ɑO�i���A		  �u�v���C���[��Y�����d�Ȃ�Ɓv���ɒe�𔭎˂��ĉ�ʂ��������
// �Etype 3:�v���C���[�̕����ɑO�i���A�u�v���C���[��Y�����d�Ȃ�Ɓv���ɒe�𔭎˂��ĉ�ʂ��������B
//===================================================================================================
void BRAGZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);
	/*�e�����O�̓���*/
	if (_e->pat == 0) {
		if (_e->type % 2 != 0) {
			Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
		}
		else {
			Enemy_Patterns(_e,  3);		//�s���p�^�[�� 3:��ʉ��Ɍ����Ē��i����
		}

		if (_e->type < 2) {
			if (_e->ac_count > 75) {
				_e->pat = 1;
			}
		}
		else {
			if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//���@��Y������v�����Ƃ�
				_e->pat = 1;
			}
		}

	}
	/*�e����������̓���(�e��5����ł��ď��ł���)*/
	else if (_e->pat == 1) {
		for (float radoffset = -1.0; radoffset <= 1.0; radoffset += 0.5) {
			int Nextshotnum = E_shotGetEmptyNum(*_s);		//�t���O�̗����Ă��Ȃ��G�e��T��
			if (Nextshotnum != 99){							//99�G���[���o�ĂȂ���Ώ�����i�߂�
				E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, radoffset);		//���@�Ɍ������Ēe�𔭎˂���
			}
		}
		_e->frame_in = false;			//��ʂ��������
		
	}
}

//===================================================================================================
//									�K���U�K�[�g [�ςȖ͗l�̓������e]
//
// �T�v:�����̓U�K�[�g�Ɠ����B�e�̔��ː����Ⴄ
// 
// �E�O�i���A�����ŋ���Ɏˌ������ď�����B
// 
// �Etype 0:���������ɑO�i���A		  �u���t���[����v�A����ɒe�𔭎˂��ĉ�ʂ��������B
// �Etype 1:�v���C���[�̕����ɑO�i���A�u���t���[����v�A����ɒe�𔭎˂��ĉ�ʂ��������B
// 
// �Etype 2:���������ɑO�i���A		  �u�v���C���[��Y�����d�Ȃ�Ɓv����ɒe�𔭎˂��ĉ�ʂ��������
// �Etype 3:�v���C���[�̕����ɑO�i���A�u�v���C���[��Y�����d�Ȃ�Ɓv����ɒe�𔭎˂��ĉ�ʂ��������B
//===================================================================================================
void GARUZAKATO(float px, float py, int p_hitoboxsize, ENEMY_ST* _e, ENEMY_SHOT* _s, int _frame,  bool shot_ok, int shotnum) {
	_e->anim = A_Enemy_Anim(*_e, _frame, 5);
	/*�e�����O�̓���*/
	if (_e->pat == 0) {
		if (_e->type % 2 != 0) {
			Enemy_Patterns(_e,  0);		//�s���p�^�[�� 0:���@�Ɍ�����
		}
		else {
			Enemy_Patterns(_e,  3);		//�s���p�^�[�� 3:��ʉ��Ɍ����Ē��i����
		}

		if (_e->type < 2) {
			if (_e->ac_count > 75) {
				_e->pat = 1;
			}
		}
		else {
			if (px - p_hitoboxsize <= _e->center_x && px + p_hitoboxsize >= _e->center_x && py > _e->center_y) {		//���@��Y������v�����Ƃ�
				_e->pat = 1;
			}
		}

	}
	/*�e����������̓���(�e��5����ł��ď��ł���)*/
	else if (_e->pat == 1) {
		for (float radoffset = -4.0; radoffset <= 4.0; radoffset += 0.5) {
			
			E_Shot_Create(_s, px, py, _e->center_x, _e->center_y, radoffset);		//���@�Ɍ������Ēe�𔭎˂���
			
		}
		_e->frame_in = false;		//��ʂ��������
	}

}

//===================================================================================================
//									�o�L���� [���Ă����]
//
// �T�v:�����Ɉړ�����B�V���b�g��256�������Ă����܂���B
// 
// �E���������ɐi�ށB�e���������Ă��e�̃t���O������I�ɂ��������B
//===================================================================================================
void BACURA(ENEMY_ST* _e, int _frame) {
	if (_frame % 7 == 0) {
		_e->anim++;
	}
	if (_e->anim >= 62) {
		_e->anim = 56;
	}
	Enemy_Patterns(_e,  3);		//pat0:���@�Ɍ�����,pat2:��ʂ̉�����
}


//===================================================================================================
//									�A�j���[�V�����̐i�s
//
// ENEMY_ST�\���̂Ɋ܂܂��anim�ԍ���anim_sum,number�𗘗p���Ĉ��͈͓���anim�ԍ������[�v������
// 
// �Ecycle:anim�̐؂�ւ����ԁ@�E_frame:�t���[���J�E���g
// �Edir�ɂ����anim�ԍ���i�߂���A�t�Ɍ��炵����ł���
// �EREPEAT(�������l, �ŏ��l, �ő�l)�ɂ����anim�ԍ����ő�l�𒴂�����ŏ��l�ɁA�t�Ȃ�ő�l�ɍ��킹��
//===================================================================================================
int A_Enemy_Anim(ENEMY_ST _e, int _frame, int cycle)
{
	int animNum = _e.anim;

	int StartAnimNum = _e.number * A_ENEMY_PIC_LINE;		//_e.enemy(��) * A_ENEMY_PIC_LINE(�s)
	int EndAnimNum = StartAnimNum + _e.anim_sum  - 1;		//�A�j���[�V�����ԍ� + �����ŃA�j���[�V�����̍ŏI�ԍ�������o�� -1�͕␳�l

	//�A�j���[�V�����ԍ���i�߂�
	if (_frame % cycle == 0) {		animNum += 1 * _e.dir;		}

	REPEAT(animNum, StartAnimNum, EndAnimNum);


	return animNum;
}
