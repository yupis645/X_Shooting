#pragma once

#include "common.h"
//#include "Game_logics/Date/GameData.h"

//-------------------------------------------------------------------------------------------
//			Game�Ŏg���\���̂�v���g�^�C�v�錾�����Ă���
// 
//�\���̂� �E�v���C���[ �E�󒆓G �E�n��G �E�{�X�@�E�}�b�v �E�e��摜�f�[�^ �Ȃ�
// �v���g�^�C�v�錾�� �Q�[�����[�v,�e�I�u�W�F�N�g�̏������A�����A�X�V�A�`��
//-------------------------------------------------------------------------------------------


//==============================================================
//					/*�v���C���[�̓��͔���*/
// 
//==============================================================

#define DIR_LEFT  (1)		//�ړ��L�[�̓���
#define DIR_RIGHT (2)
#define DIR_UP	  (3)
#define DIR_DOWN  (4)
#define INPUT_SHOT	(5)		//�V���b�g�̃{�^�����������ꍇ�̔���
#define INPUT_BOM	(6)		//�{���̃{�^�����������ꍇ�̔���


//==============================================================
//					/*�v���C���[�֌W�̒�`*/
// 
//==============================================================
#define MAX_SHOT (3)		//��ʓ��ɕ\���ł���V���b�g�̐�
#define SHOT_SPEED	(20)	//�V���b�g�̒e��
#define BOM_SPEED	(5)		//�V���b�g�̒e��
#define PL_SPEED	(5)		//�v���C���[�̈ړ����x	
#define BOM_ST	  (100)		//�{���̎˒�����

//==============================================================
//					/*�G�l�~�[���ʂ̒�`*/
// 
//==============================================================
#define A_ENEMY_PIC_LINE	(8)		//�󒆓G�̉摜�̍s�̐�
#define G_ENEMY_PIC_LINE	(4)		//�n��G�̉摜�̍s�̐�
#define MAX_ENEMY	(20)			//�G�̍ő�o����(�󒆂ƒn��͕ʃJ�E���g)
#define MAX_E_SHOT	(19)			//�G�e�̍ő�o����
#define E_SHOT_SPEED	(4)			//�G�e�̊�{�̑��x
#define MAX_LEVEL	(64)			//�G�̏o�����x���̏��




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
												
																/*�\���̂̐錾*/

//------------------------------------------------------/*�v���C���[�֘A*/-------------------------------------------------------

//============================================
// �V���b�g�̍\����
//============================================
typedef struct P_SHOT_ST {
	RECT hitbox;			//���W(x,y��	)
	float center_x;		//���S���Wx
	float center_y;		//���S���Wy
	bool F;				//�e����ʓ��ɑ��݂��邩�̔���
	int sp1;			//���ꏈ��������ꍇ�ɓK�X�g�p����ϐ�
	int sp2;			//����
}P_SHOT_ST;

//============================================
// �{���̍\����
//============================================
typedef struct P_BOM_ST {
	RECT hitbox;
	float center_x;		//���S���Wx
	float center_y;		//���S���Wy
	bool F;				//���˃t���O
	int range;			//�˒�����
	bool bomber;		//�����t���O(�U������)
}P_BOM_ST;

//============================================
// �v���C���[�\����
//============================================
typedef struct PLAYER {
	Bmp* pic[12];				//�摜�|�C���^(���@)
	Bmp* bullet[3];				//�摜�|�C���^(�e)
	RECT hitbox;				//�����蔻��
	int pic_x;					//�摜�̍���x���W
	int pic_y;					//�摜�̍���y���W
	int pic_size;				//�摜�T�C�Y
	int hitbox_size;			//�����蔻��̃T�C�Y
	float center_x, center_y;	//���@�̒��S���W
	int anim;					//�A�j���[�V�����p�^�[���̐؂�ւ��^�C�~���O
	int pat;					//�A�j���[�V�����p�^�[���ԍ�
	bool hit;					//��e����
	int shot_count;				//��ʂɕ\������e�̐�(define�ɂ���MAX_SHOT�̐��l���ő�)
	RECT sight_hitbox;			//�Ə��̓����蔻��(�{���Ŕj��ł��镨�ɔ�������)
	bool lookon;				//�Ə����j��\�X�v���C�g�Əd�Ȃ��Ă����Ԃ̔���
	P_SHOT_ST shot[MAX_SHOT];	//�ʏ�e(�΋�)
	P_BOM_ST bom;				//����e(�Βn)
}PLAYER_ST;

//------------------------------------------------------/*�G�֘A*/-------------------------------------------------------


//============================================
// �󒆁A�n�㋤�ʂ̒e�̍\����
//============================================
typedef struct E_SHOT_ST {
	Bmp* pic[2];		//�摜�|�C���^(�e)
	bool F;				 //�g�p:1  ���g�p:0
	RECT hitbox;		//�e�̓����蔻��
	float center_x;		//���S���Wx
	float center_y;		//���S���Wy
	int pl_x, pl_y;		//���˂����u�Ԃ̎��@�̈ʒu���L������
	float rad;			//�i�ފp�x(���˂���ۂ̃v���C���[�ɑ΂���p�x)
	float distance;		
}E_SHOT_ST;

//============================================
//  �󒆁A�n�㋤�ʂ̃X�e�[�^�X
//============================================
typedef struct ENEMY_ST {
	int number;			//�G�l�~�[�̎�ނ���ʂ��邽�߂̔ԍ�
	RECT hitbox;		//���W
	int hitbox_size;	//�����蔻��̃T�C�Y
	bool frame_in;		//��ʓ��ɂ��邩�𔻕ʂ��邽�߂̃t���O
	bool hit;			//��e����
	float rad;			//�p�x
	float center_x;		//���S��x���W
	float center_y;		//���S��y���W
	int pic_x;			//�摜�����x���W
	int pic_y;			//�摜�����y���W
	int pic_size;		//�摜�T�C�Y
	int back_num;		//�z�u����Ă��闠�}�b�v�̔ԍ�
	int back_coord;		//���}�b�v�ɂ�����z�u
	int type;		//����̂̈Ⴄ����
	int dir;		//����
	int anim;		//�A�j���[�V�����p�^�[���̐؂�ւ��^�C�~���O
	int anim_sum;	//�s���p�^�[���̐�
	int pat;		//�s���p�^�[���ԍ�
	int points;		//�|�C���g
	float speed;	//�ړ����x
	int ac_count;	//���������ꂽ�u�Ԃ���J�E���g���J�n����(�s���p�^�[���Ɏg�p����)
	int move_x;		//x���̈ړ���
	int move_y;		//y���̈ړ���
	float acc;		//�����x
	Bmp* pic[A_ENEMY_PIC_LINE * 11];		//�摜�|�C���^
}ENEMY_ST;


//============================================
//�o������󒆓G�̏o���e�[�u��
//============================================
typedef struct A_ENEMY_TABLE {
	int Lv;
	int count;
	int table[MAX_LEVEL * 6];
}A_ENEMY_TABLE;


//============================================
//�{�X & �C��A�R�A�̃X�e�[�^�X
//============================================
typedef struct BOSS {
	bool F;				//�o���t���O
	bool down;			//���Ĕ���(�R�A�j��)
	ENEMY_ST enemy[5];	//�C��~4�ƃR�A�̏��
	float center_x;		//x�����S���W
	float center_y;		//y�����S���W
	int pic_x;			//����x���W(�X�v���C�g�p)
	int pic_y;			//����y���W(�X�v���C�g�p)
	Bmp* pic[120];				//�{�̂𕪊������X�v���C�g
	Bmp* pats[5];				//���j�O�̖C��ƃR�A�̉摜
}BOSS;

//============================================
// �G���ꊇ�Ǘ�����\����
//============================================
typedef struct ENEMY {
	bool pop_F;			//�o���t���O
	int count;			//�G�̏o�����J�E���g
	ENEMY_ST enemy[MAX_ENEMY];	//�G�̐ݒ�BMAX_ENEMY�̒l�ōő�o�������ω�����
}ENEMY;


//============================================
// �G�̒e���ꊇ�Ǘ�����\����
//============================================
typedef struct ENEMY_SHOT {
	E_SHOT_ST shot[MAX_E_SHOT];		//�e�̐ݒ�BMAX�̒l�Œe�̉�ʂɏo������ő�����ϓ�����
}ENEMY_SHOT;

//------------------------------------------------------/*�}�b�v�֘A*/-------------------------------------------------------


//============================================
// �}�b�v�f�[�^��ۑ�����\����
//============================================
typedef struct MAP_DATE {
	bool F;						//�`�ʃt���O
	int data[MAP_H * MAP_W];	//�G���A���
	int x, y;					//�}�b�v�̍�����W
}MAP_TILE;

//============================================
//�@�}�b�v�̏����ꊇ�Ǘ�����\����
//============================================
typedef struct MAP {
	int stage;					//���݂̃X�e�[�W�ԍ����L������
	int split_num;				//���݂̃G���A�ԍ�
	int x;						//�X�e�[�W�S�̂̍��W
	int y;
	MAP_DATE split[5];			//�X�e�[�W��5�ɕ��������}�b�v�f�[�^���L������
	MAP_DATE backmap[5];		//split�Ɠ����悤�ɓ����A�G�̔z�u��Œ�o���Ȃǂ��L������
	int px;						
	int py;
	float rate;				
	Bmp* pic[12 * 10];			//�}�b�v�`�b�v�i�[
	const wchar_t* filename[5];//�摜��ۑ�����
}MAP_DATA;

//============================================
//�}�b�v�̉摜��ۑ�����\����
//============================================
typedef struct MAP_PIC_DATA{
	Bmp* pic[5];				//1�G���A(�X�e�[�W�S�̂��T�ɕ��������f�[�^)���L������
	const wchar_t* filename[5];	//�摜
}MAP_PIC_DATA;


//============================================
//1�X�e�[�W��4������������ 1/4 �������f�[�^�\����
//============================================
typedef struct ARIA_SPLIT_DATE {
	int date[MAP_H * MAP_W];
}ARIA_SPLIT_DATE;

//================================================
//1�X�e�[�W��4���������\���� + ��ʐX�łł���AREA0
//================================================
typedef struct ARIA_SPLIT {
	ARIA_SPLIT_DATE split[5];
}ARIA_SPLIT;


//============================================
//CSV�t�@�C����ǂݍ��ލ\����
//============================================
typedef struct CSV_MAPDATA {
	const char* Front[5];
	const char* Back[5];
}CSV_MAPDATA;


//-----------------------------------------------------/*�ėp*/--------------------------------------------------------------

//============================================
//�v���C���[�A�G�ŋ��ʂ̃G�t�F�N�g
//============================================
typedef struct EFFECT {
	Bmp* DieEffect[12];
	int pat;
}EFFECT;

//============================================
//�摜�f�[�^��ۊǂ���\����
//============================================
struct PNGDATA {
	const wchar_t* Player;
	const wchar_t* Shot;
	const wchar_t* A_Enemy;
	const wchar_t* G_Enemy;
	const wchar_t* Boss;
	const wchar_t* BossSub;
	const wchar_t* bomber;

};

//============================================
//�f�o�b�N�p�̍\����
//============================================
struct DenugState {
	bool G_enemy_stop;
	bool A_enemy_stop;
	bool Player_invalid;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////�֐��v���g�^�C�v�錾///////////////////////////////////////////////////////////////


//=========================================================
//					�Q�[�����[�v
//
//=========================================================


void Game_Init(SCENE* _sc);	//������
void Game_Input(SCENE* _sc);				//����
void Game_Update(SCENE* _sc);	//�X�V
void Game_Draw(SCENE* _sc);	//�`��
void Game_End(SCENE* _sc);					//�I������
void Game(SCENE* _sc);						//�Q�[�����

//=========================================================
//					�v���C���[
// �������A�I�u�W�F�N�g�쐬�A���͏����A�X�V�A�`��
//=========================================================
void Player_Init(PLAYER*);
void Player_Create(PLAYER*);
void Player_Move(PLAYER*, int);
void Player_Update(PLAYER*, ENEMY*_ae,ENEMY*_ge, ENEMY_SHOT*, SCENE* _sc);
void Player_Draw(PLAYER*,EFFECT*);

//=========================================================
//					�G�̏o���e�[�u��
//
//=========================================================
void Air_Enemy_table(ENEMY*, PLAYER*,int level, int map_splitNum,bool boss_flag,bool boss_down);
void BackMap_Enemy_table(ENEMY*, ENEMY*, PLAYER*, MAP* _m, int _frame, BOSS* _b);

//=========================================================
//					   �󒆓G
//�������A�I�u�W�F�N�g�쐬�A�X�V�A�`��
//=========================================================
void Air_Enemy_Init(ENEMY*);
void Air_Enemy_Create(ENEMY* _e,int number, int Player_x);
void Air_Enemy_Update(PLAYER* _p, ENEMY*, ENEMY_SHOT*, SCENE* _sc);
void Air_Enemy_Draw(ENEMY*, EFFECT*);

//=========================================================
//					   �n��G
//�������A�I�u�W�F�N�g�쐬�A�X�V�A�`��
//=========================================================
void G_Enemy_Init(ENEMY*);
void G_Enemy_Create(ENEMY* _e,int number,int back_coord,int back_num,float center_x);
void G_Enemy_Update(PLAYER* _p, ENEMY*, ENEMY_SHOT*, SCENE* _sc, MAP*, EFFECT*);
void G_Enemy_Draw(ENEMY*, EFFECT*);

//=========================================================
//					   �{�X
//�������A�I�u�W�F�N�g�쐬�A�X�V�A�`��
//=========================================================
void Boss_Init(BOSS* _b);
void Boss_Create(BOSS* _b);
void Boss_Update(PLAYER* _p, BOSS*, ENEMY_SHOT*, SCENE* _sc,  EFFECT*);
void Boss_Draw(BOSS*, EFFECT*);

//=========================================================
//					   �G�e
//�������A�I�u�W�F�N�g�쐬�A�X�V�A�`��
//=========================================================
void E_Shot_Init(ENEMY_SHOT*);
void E_Shot_Create(ENEMY_SHOT*, float px,float py, float x, float y, float rad);
void E_Shot_Update(PLAYER* _p, ENEMY_SHOT*, SCENE* _sc);
void E_Shot_Draw(ENEMY_SHOT*);


/*�G�̎�ނɂ���ē�����ς���*/
void Enemy_Patterns(ENEMY_ST*,  int pat);

//=========================================================
//					�}�b�v�̏���
// �������A�X�V�A�`�揈��
//=========================================================

void MAP_Init(MAP*);
void MAP_Update(MAP*, SCENE*);
void MAP_Draw(MAP*);
//void MAP_Init(GAME_STATE*);
//void MAP_Update(GAME_STATE*, SCENE*);
//void MAP_Draw(GAME_STATE*);

//=========================================================
//			�Q�[���̃��g���C�⃊�U���g��ʂւ̑J��
// �������A�X�V�A�`�揈��
//=========================================================
void GameStateChange( SCENE* _SC);

//=========================================================
//					�|�[�Y(�ꎞ��~����)
//=========================================================
void Pose();

//=========================================================
//			�Q��RECT�\���̂��ڐG���Ă��邩���ʂ���
//=========================================================
bool Hit_box(RECT, RECT);


//=========================================================�v
//			//�����蔻��Ɖ摜�̓���
//=========================================================

void Hitbox_Sync(RECT* hitbox, int size, float* x, float* y,int* pic_x,int* pic_y,int pic_size);

//=========================================================�v
//					�f�o�b�N�֘A
// �Q�[���̃X�e�[�^�X�\���A�I�u�W�F�N�g��hitbox����
// �C�ӓG���o��������R�}���h
//=========================================================
void DebugDraw_Status(MAP, SCENE,bool _b_F);
void DebugDraw_PlayerDraw(PLAYER _p);
void DebugDraw_A_EnemyDraw(ENEMY _ae);
void DebugDraw_G_EnemyDraw(ENEMY _ge);
void DebugDraw_BossDraw(BOSS _b);

void DebugInput(ENEMY* ge, ENEMY* ae, BOSS* b, int Pl_x, int map_y);

//=========================================================�v
//					�󂫔ԍ���T��
//=========================================================
int SetEmptyNum(ENEMY _e);
int E_shotGetEmptyNum(ENEMY_SHOT _es);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

											//�ϐ���extern�錾
extern SCENE scGame;
