
#include"common.h"
#include "Title.h"
#include "Game.h"
#include "Result.h"
#include "Option.h"



// �f�o�b�N���[�h�̏�����ԁB�f�o�b�N���[�h���L���Ȃ�ŏ�����true
bool DebugEnable = DebugOn;

//�X�R�A�Ǝc�@�ȂǃV�[�����ׂ��Ŏg�p������
 GAMESTATE state = {0,0};
/*
* @brief	main()�֐�
*
* @note	�A�v���̊J�n�ʒu�B
*	��ʑJ�ڂ̗��������Đ��䂵�Ă���
*/
int main(void)
{
	//�y�A�v���S�̂̏������z
	SetCaption("2023 HAL Osaka.");	//�E�B���h�E�^�C�g��
	InitConioEx(SRN_W, SRN_H, PXW, PXH);	//�������F�^�e���R�T�C�Y�ƃh�b�g�̑傫��

	//�y��ʑJ�ڂׂ̈̃��[�v�z
	int scene_id = TITLE;

	while (true) {
		switch (scene_id) {
		case	TITLE:	//�^�C�g�����
			Title(&scTitle);
			scene_id = scTitle.next_scene;	//���̉��ID���󂯎��
			break;
		case	GAME:	//�Q�[�����
			Game(&scGame);
			scene_id = scGame.next_scene;	//���̉��ID���󂯎��
			break;
		case	RESULT:	//���U���g���
			Result(&scResult);
			scene_id = scResult.next_scene;	//���̉��ID���󂯎��
		}
		//�A�v���I������
		if (scene_id == APP_EXIT) {
			break;
		}
	}
	//�y�A�v���S�̂̏I�������z
	EndConioEx();	//ConioEx�̏I������
	return 0;
}






//----------------------------------------------------------------
//	2023 HAL osaka.
//----------------------------------------------------------------
