
#include"common.h"
#include "Game.h"
#include "Game_logics/Data/GameData.h"


// ----------------------------------------------------------------
// �Q�[�����
// ----------------------------------------------------------------
//�ϐ��錾
SCENE scGame = { true,0,GAME,0 ,{0,0 } };	//�V�[���\����
PLAYER Player;									//�v���C���[�\����
ENEMY A_Enemy;								//�G(��)�̍\����
ENEMY_SHOT EnemyShot;							//�G�e�̍\����
ENEMY G_Enemy;								//�G(�n��)�\����
BOSS Boss;									//�{�X�̍\����
MAP Map;									//�X�e�[�W����̏����i�[����
EFFECT Down = { { nullptr},0};				//���j�G�t�F�N�g������
bool PoseIn = false;						//�ꎞ��~

// �Q�[�����(���C�����[�v)
void Game(SCENE* _sc)
{
	srand((unsigned int)time(NULL));		//��������
	state.score = 0;						//�X�R�A�̏�����
	A_Date_Table.Lv = 63;
	Game_Init(_sc);
	while (_sc->loop) {
		/*������*/
		Game_Input(_sc);
		/*�X�V*/
		Game_Update(_sc);
		/*�`��*/
		Game_Draw(_sc);
		
		
	}
	Game_End(_sc);
}
//����������
void Game_Init(SCENE* _sc)
{
	_sc->loop = true;			//�Q�[�����[�v�p���t���O
	_sc->frame_count = 0;		//�t���[���J�E���g�̏�����
	_sc->next_scene = GAME;		//�Q�[����ʂ��p��
	_sc->scene_state = 0;		//��ʑJ�ڂ�Q�[����ʂ̃��Z�b�g����Ƃ��Ɏg���ϐ�


	//�e�f�[�^�̏�����
	Player_Init(&Player);									//�v���C���[���̏�����
	Air_Enemy_Init(&A_Enemy);
	G_Enemy_Init(&G_Enemy);
	Boss_Init(&Boss);
	E_Shot_Init(&EnemyShot);
	MAP_Init(&Map);										//�}�b�v�̏�����

	//�v���C���[�̍쐬
	Player_Create(&Player);
	
	//�����G�t�F�N�g�̃��[�h
	LoadDivBmp(PngData.bomber, 0, 0, 47, 47, 6, 2, Down.DieEffect);
	A_Date_Table;
}
//����
void Game_Input(SCENE* _sc)
{
	int inputkey = 0;

	GetKeyAll();	//�S�ẴL�[���́B

	//�ꎞ��~
	if (ChkKeyEdge(PK_SP)) {
		PoseIn = PoseIn ? false : true;

	}
	if (PoseIn || Player.hit == true) { return; }

	//�L�[�̓��͎�t
	if (ChkKeyPress(PK_LEFT)) {
		inputkey = DIR_LEFT;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyPress(PK_RIGHT)) {
		inputkey = DIR_RIGHT;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyPress(PK_UP)) {
		inputkey = DIR_UP;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyPress(PK_DOWN)) {
		inputkey = DIR_DOWN;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyEdge(PK_Z)) {
		inputkey = INPUT_SHOT;
		Player_Move(&Player, inputkey);
	}
	if (ChkKeyEdge(PK_X)){
		inputkey = INPUT_BOM;
		Player_Move(&Player, inputkey);
	}

	
//�e�X�g���[�h�L�����̂ݍ�p
#if TestMode
	if (DebugOn) {				//
		if (ChkKeyEdge(PK_D)) {
			DebugEnable = DebugEnable ? false : true;
		}
	}
	if (DebugEnable) {		//�f�o�b�N���[�h�L�����̂ݍ쓮
		DebugInput(&G_Enemy,&A_Enemy, &Boss, Player.center_x, Map.split_num);
	}

#endif

}
//�X�V
void Game_Update(SCENE* _sc)
{
	//�|�[�Y���L�����͍X�V�����͂����ŏI��
	if (PoseIn) { return; }

	/*���̍X�V*/
	Air_Enemy_Update(&Player, &A_Enemy, &EnemyShot, _sc);		//�󒆂̓G�̏��X�V
	G_Enemy_Update(&Player, &G_Enemy, &EnemyShot, _sc,&Map,&Down);	//�n��̓G�̏��X�V
	Boss_Update(&Player, &Boss, &EnemyShot, _sc,  &Down);		//�{�X�̏��X�V
	Player_Update(&Player, &A_Enemy,&G_Enemy, &EnemyShot, _sc);	//�v���C���[�̏��X�V
	E_Shot_Update(&Player, &EnemyShot, _sc);						//�G�̒e�̏��X�V

	
	/*�󒆓G�̏o�����[�`��*/
	Air_Enemy_table(&A_Enemy, &Player, _sc->frame_count, Map.split_num,Boss.F,Boss.down);			//�󒆂̓G�̏o���e�[�u��
	
	/*�n�㕨�̏o�����[�`��*/
	BackMap_Enemy_table(&G_Enemy,&A_Enemy, &Player,&Map,  _sc->frame_count,&Boss);

	/*�}�b�v�̃X�N���[��*/
	MAP_Update(&Map, _sc);
	
	GameStateChange(_sc);

	scGame.frame_count++;				//�t���[���J�E���g

}
//�`��
void Game_Draw(SCENE* _sc)
{
	ClearScreen(0x19, 0x53, 0x5F);	//��ʏ����FR,G,B�̎w��F�œh��Ԃ�
	//��������������

	if (PoseIn) {
		Pose();
	}
	else {
		MAP_Draw(&Map);
		G_Enemy_Draw(&G_Enemy, &Down);		//�n��̓G�\��
		Boss_Draw(&Boss, &Down);			//�n��̓G�\��
		Air_Enemy_Draw(&A_Enemy, &Down);	//�󒆓G�̕\��
		Player_Draw(&Player, &Down);			//�v���C���[�̕\��

		E_Shot_Draw(&EnemyShot);					//�G�e�̕\��

		//���̕\��
		WriteTextFA(0, (SRN_H)-(WORD_H * 4), 0, "life");					//�c�@�̕\��(��)
		for (int r = 0; r < state.life; r++) {
			DrawBmp(Player.hitbox_size * r, (SRN_H)-(WORD_H * 3), Player.pic[5]);				//���@�̕`��
		}

		WriteTextFA(SRN_W - WORD_W * 20, 0, 0, "SCORE %10d", state.score);			//�X�R�A�̕\��


		/*���ؗp*/
#if TestMode
		if (DebugEnable) {
			DebugDraw_Status(Map, *_sc,Boss.F);
			DebugDraw_PlayerDraw(Player);
			DebugDraw_A_EnemyDraw(A_Enemy);
			DebugDraw_G_EnemyDraw(G_Enemy);
			DebugDraw_BossDraw(Boss);
		}
#endif
	}

	//�����������܂�
	PrintFrameBuffer();	//��ʃo�b�t�@��]��
	RenderScreen();		//��ʂ������_�����O
	FrameSync();		//�P�t���[���̓���
}
//�I������
void Game_End(SCENE* _sc)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

									/*�֐�*/


void GameStateChange(SCENE* _sc) {

	//��e�������̏���
	if (_sc->scene_state == 1) {
		state.life--;		//�c�@�����炷
		//�c�@��0�ȏ�Ȃ�Q�[�������Z�b�g����
		if (state.life > 0)
		{
			Game_Init(_sc);					//�v���C���[�A�G�A�G�̒e�Ȃǂ����������čăX�^�[�g
		}
		else {				//life��0�ȉ��Ȃ�
			_sc->next_scene = RESULT;	//���̑J�ڐ���ID���Z�b�g
			_sc->loop = false;			//�Q�[����ʏI��
			_sc->scene_state = 0;
		}
	}
	if (_sc->scene_state == 2) {
		_sc->next_scene = RESULT;	//���̑J�ڐ���ID���Z�b�g
		_sc->loop = false;			//�Q�[����ʏI��
		_sc->scene_state = 0;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

															/*�����Ŏ����Ă����Q��RECT�\���̂��ׂĐڐG���Ă��Ȃ����𔻕ʂ���*/


bool Hit_box(RECT a, RECT b) {


	//���ꂼ��̕ӂ̏d�Ȃ蔻��
	bool left = false; 
	bool top = false;
	bool right = false;
	bool bottom = false;

	//���Ɛ��ł̏d�Ȃ蔻��
	if (a.left <= b.right){	right = true;	}
	if (a.right >= b.left){	left = true;	}
	if (a.top <= b.bottom){	bottom = true;	}
	if (a.bottom >= b.top){	top = true; }

	//4�ӑS�� true(�d�Ȃ��Ă���)�Ȃ�ture��Ԃ� 
	if (right&& left&& bottom && top) {
		return true;
	}

	return false;

}





//==================================================================================================
// 						/*�����蔻��Ɖ摜�̍��W�𓯊�������*/
//�u�q�b�g�{�b�N�X�V���N���v
// ������W�ƃT�C�Y���󂯎��A�����蔻��̑傫����ݒ肷�� (�摜�Ɠ����蔻��̑傫�����Ⴄ�G�����邽��)
// ���̊֐��͐����`�̂ݍ쐬�\
//==================================================================================================
void Hitbox_Sync(RECT* _r, int r_size, float* cx, float* cy, int* px, int* py, int p_size){

	//�摜�T�C�Y��0�̏ꍇ�͖���
	if (p_size != 0) {						 //�摜�T�C�Y��0�̏ꍇ�͖���
		int pic_size_half = (p_size / 2 - 1); //RECT�̔����̒���()

		/*�X�v���C�g�̍�����W�ݒ�*/
		*px = (int)*cx - pic_size_half;		 //���S���W - �摜�T�C�Y�̔��a - 1(�␳)
		*py = (int)*cy - pic_size_half;
	}

	int hitbox_size_half = (r_size / 2); //�X�e�[�^�X�̓����蔻��̔����̒���()

	/*�����蔻��̓���*/
	_r->left   = (int)*cx - hitbox_size_half;
	_r->top    = (int)*cy - hitbox_size_half;
	_r->right  = (int)*cx + hitbox_size_half;
	_r->bottom = (int)*cy + hitbox_size_half;
}



//==================================================================================================
//									/*�|�[�Y���*/
// 
// �X�y�[�X�L�[�ō쓮�BPoseIn�ϐ���On�̏�Ԃł͍X�V�ƕ`��̊֐����~�߂�
// ���������`��
//==================================================================================================
void Pose()
{
	WriteTextFA(WORD_W * 30, SRN_H - (WORD_H * 28), 0, "[  P O S E  ]");

#if TestMode
	WriteTextFA(0, SRN_H - (WORD_H * 22), 0, "---------------------------------�������---------------------------------");
	WriteTextFA(0, SRN_H - (WORD_H * 20), 0, "�E[Enter�L�[ : ����] �E[Esc : �Q�[���̏I��]");
	WriteTextFA(0, SRN_H - (WORD_H * 18), 0, "�E[�����L�[ : �ړ�] �E[Z�L�[ : �V���b�g]�@�E[X�L�[ : ���e]�@�@");
	WriteTextFA(0, SRN_H - (WORD_H * 16), 0, "�E[Space�L�[ : �ꎞ��~] �E[D�L�[ : �f�o�b�N���[�h�؂�ւ�] ");
	WriteTextFA(0, SRN_H - (WORD_H * 14), 0, "--------------------------------------------------------------------------");
	if (DebugEnable) {
		WriteText(CENTER_X - (WORD_W * 9), SRN_H - (WORD_H * 38), L"�f�o�b�N���[�h ON");
		WriteText(0, SRN_H - (WORD_H * 36), L"----------------------�f�o�b�N���[�h�̕\����------------------------------");
		WriteText(0, SRN_H - (WORD_H * 34), L"�EPlayer���G �EPlayer,Enemy��HItBox�̉��� �E�Q�[�����̃X�e�[�^�X�̕\��");
		WriteText(0, SRN_H - (WORD_H * 32), L"���G�o���͏���𒴂���Ɩ{���o������͂��̓G�����Ȃ��Ȃ����肷��̂Œ���!");
		WriteText(0, SRN_H - (WORD_H * 30), L"--------------------------------------------------------------------------");

		WriteText(0, SRN_H - (WORD_H * 12), L"----------------------�f�o�b�N���[�h�������------------------------------");
		WriteText(0, SRN_H - (WORD_H * 10), L"�E[1,2,3,4,5,5,6,7,8,9�L�[ , Q�L�[ : �e�󒆓G�̏o��] ");
		WriteText(0, SRN_H - (WORD_H * 8), L"�E[W,E,R,T,Y,U,I,O,P,A,S�L�[: �e�n��G�̏o��]�@");
		WriteText(0, SRN_H - (WORD_H * 6), L"�E[F�L�[ : �{�X�̏o�� ] �E[G�L�[ : �G�̃^�C�v�ύX ]");
		WriteText(0, SRN_H - (WORD_H * 4), L"--------------------------------------------------------------------------");
	}
#endif
}

