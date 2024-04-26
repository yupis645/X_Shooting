
#include"common.h"
#include "Option.h"


// ----------------------------------------------------------------
// ���U���g���
// ----------------------------------------------------------------
//�ϐ��錾
SCENE scOption = { true,0,OPTION,0 ,{0,0 } };	//�V�[���\����

// �Q�[�����
void Option(SCENE* _sc)
{
	Option_Init(_sc);
	while (_sc->loop) {
		Option_Input(_sc);
		Option_Update(_sc);
		Option_Draw(_sc);
		_sc->frame_count++;
	}
	Option_End(_sc);
}
//����������
void Option_Init(SCENE* _sc)
{
	_sc->loop = true;	//�Q�[�����[�v�p���t���O
	_sc->frame_count = 0;
	_sc->next_scene = OPTION;
}
//����
void Option_Input(SCENE* _sc)
{
	GetKeyAll();	//�S�ẴL�[���́B
}
//�X�V
void Option_Update(SCENE* _sc)
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
void Option_Draw(SCENE* _sc)
{

	ClearScreen(0x11, 0x7f, 0x80);	//��ʏ����FR,G,B�̎w��F�œh��Ԃ�
	//��������������
	WriteText(0, 0, L"�I�v�V�������");

	//�����������܂�
	PrintFrameBuffer();	//��ʃo�b�t�@��]��
	RenderScreen();		//��ʂ������_�����O
	FrameSync();		//�P�t���[���̓���
	
	//fclose(fin);

}
//�I������
void Option_End(SCENE* _sc)
{
}