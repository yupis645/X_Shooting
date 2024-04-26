

#include"common.h"
#include "Result.h"






// ----------------------------------------------------------------
// ���U���g���
// ----------------------------------------------------------------
//�ϐ��錾
SCENE scResult = { true,0,RESULT,0 ,{0,0 } };	//�V�[���\����

// �Q�[�����
void Result(SCENE* _sc)
{
	Result_Init(_sc);
	while (_sc->loop) {
		Result_Input(_sc);
		Result_Update(_sc);
		Result_Draw(_sc);
		_sc->frame_count++;
	}
	Result_End(_sc);
}
//����������
void Result_Init(SCENE* _sc)
{
	_sc->loop = true;	//�Q�[�����[�v�p���t���O
	_sc->frame_count = 0;
	_sc->next_scene = RESULT;
}
//����
void Result_Input(SCENE* _sc)
{
	GetKeyAll();	//�S�ẴL�[���́B
}
//�X�V
void Result_Update(SCENE* _sc)
{
	//�A�v���I���L�[�̔���
	if (ChkKeyPress(VK_ESCAPE)) {
		_sc->loop = false;	//�Q�[�����[�v�I��
		_sc->next_scene = APP_EXIT;	//���̉�ʑJ�ڐ���u�A�v���I���v�ɂ���
		return;	//�����ɂ��̊֐����I��
	}
	if (ChkKeyEdge(VK_RETURN)) {
		//�G���^�[�L�[�Ŏ��̉�ʂɑJ��
		_sc->next_scene = TITLE;	//���̑J�ڐ���ID���Z�b�g
		_sc->loop = false;	//�^�C�g���I��
	}
}
//�`��
void Result_Draw(SCENE* _sc)
{
	ClearScreen(0x10, 0x10, 0x10);	//��ʏ����FR,G,B�̎w��F�œh��Ԃ�
	//��������������
	WriteText(CENTER_X - (8 * WORD_W), CENTER_Y - (WORD_H * 10),  L"G A M E O V E R");
	WriteText(CENTER_X - (5 * WORD_W), CENTER_Y - (WORD_H * 6), L"S C O R E");
	WriteTextFA(CENTER_X - (11 * WORD_W), CENTER_Y - (WORD_H * 4), 0, "	%5d ",state.score);
	//�����������܂�
	PrintFrameBuffer();	//��ʃo�b�t�@��]��
	RenderScreen();		//��ʂ������_�����O
	FrameSync();		//�P�t���[���̓���
}
//�I������
void Result_End(SCENE* _sc)
{
}
