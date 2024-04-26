

#include"common.h"
#include "Title.h"







// ----------------------------------------------------------------
// �^�C�g�����
// ----------------------------------------------------------------
//�ϐ��錾
SCENE scTitle = { true,0,TITLE,0 ,{0,0 } };	//�V�[���\����

Bmp* Titlepic = nullptr;
const wchar_t* texture =  L"res/STG_Title.png" ;
//�^�C�g�����
void Title(SCENE* _sc)
{
	Title_Init(_sc);
	while (_sc->loop) {
		Title_Input(_sc);
		Title_Update(_sc);
		Title_Draw(_sc);
		_sc->frame_count++;
	}
	Title_End(_sc);
}
//����������
void Title_Init(SCENE* _sc)
{
	_sc->loop = true;	//�Q�[�����[�v�p���t���O
	_sc->frame_count = 0;
	_sc->next_scene = TITLE;
	_sc->scene_state = 0;	//�i���x�����̏�����

	LoadDivBmp(texture,
		0, 0, 198, 58, 1, 1, &Titlepic);

	/*��*/
	//LoadDivBmp(_t->filename,
		//208, 0, 8, 5, 4, 1, _t->flash);



}
//����
void Title_Input(SCENE* _sc)
{
	GetKeyAll();	//�S�ẴL�[���́B

	//�A�v���I���L�[�̔���
	if (ChkKeyPress(VK_ESCAPE)) {
		_sc->loop = false;	//�Q�[�����[�v�I��
		_sc->next_scene = APP_EXIT;	//���̉�ʑJ�ڐ���u�A�v���I���v�ɂ���
		return;	//�����ɂ��̊֐����I��
	}
	if (ChkKeyEdge(VK_RETURN)) {
		//�G���^�[�L�[��ʂ�i��������
		_sc->scene_state++;
	}
	if (ChkKeyEdge(PK_O)) {
		//O�Ŏ��I�v�V������ʂɑJ��
		_sc->next_scene = OPTION;	//���̑J�ڐ���ID���Z�b�g
		_sc->loop = false;	//�^�C�g���I��
	}
	if (_sc->scene_state == 1) {
		if (ChkKeyEdge(PK_UP)) {
			_sc->carsor.y--;
		}
		if (ChkKeyEdge(PK_DOWN)) {
			_sc->carsor.y++;
		}

	}

#if TestMode
	if (DebugOn) {
		//�f�o�b�N���[�h�̗L���� / ������
		if (ChkKeyEdge(PK_D)){		DebugEnable = DebugEnable ? false : true;		}
	}
#endif


}
//�X�V
void Title_Update(SCENE* _sc)
{
	if (_sc->scene_state >= 2) {
		switch (_sc->carsor.y) {
		case 0:
		case 1:
			_sc->next_scene = GAME;	//���̑J�ڐ���ID���Z�b�g
			state.life = 3;
			break;
		case 2:
			_sc->next_scene = GAME;	//���̑J�ڐ���ID���Z�b�g
			state.life = 6;
			break;

		}
		//���̉�ʂɑJ��
		_sc->loop = false;	//�^�C�g���I��

	}

	//�J�[�\���̃��[�v
	REPEAT(_sc->carsor.y, 0, 2);		//�J�[�\����0�ȉ��ɂȂ��3��3�𒴂����0�ɖ߂�

}
//�`��
void Title_Draw(SCENE* _sc)
{
	MOJI Text[] = { {"PUSH START ENTER",1},{"1 PLAYER",1},{"2 PLAYERS",1} };		//�e�L�X�g�̕���
	int po[] = { 30, 30,0 };
	ClearScreen(0x00, 0x00, 0x00);	//��ʏ����FR,G,B�̎w��F�œh��Ԃ�
	//��������������
	DrawBmp(SRN_W / 2 - 99, 100, Titlepic);		//�^�C�g���̕\��

	switch (_sc->scene_state) {
	case 0:
		WriteTextFA(SRN_W / 2 - ((WORD_W / 2) * strlen(Text[0].word)), SRN_H / 2, 0, "%s", Text[0]);		//"PUSH START ENTER"�̕\��
		break;
	case 1:
		for (int r = 1; r < 4; r++) {
			WriteTextFA(SRN_W / 2 - ((WORD_W / 2) * strlen(Text[2].word)), SRN_H / 2 - po[r], 0, "%s", Text[r]);	//�e�L�X�g�̕\���ʒu
		}
		WriteTextFA(SRN_W / 2 - ((WORD_W / 2) * strlen(Text[0].word)) - WORD_W, SRN_H / 2 - po[_sc->carsor.y], 0, "��");	//�J�[�\���̕\��

	}

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
		WriteText(0, SRN_H - (WORD_H * 32), L"--------------------------------------------------------------------------");

		WriteText(0, SRN_H - (WORD_H * 12), L"----------------------�f�o�b�N���[�h�������------------------------------");
		WriteText(0, SRN_H - (WORD_H * 10), L"�E[1,2,3,4,5,5,6,7,8,9�L�[ , Q�L�[ : �e�󒆓G�̏o��] ");
		WriteText(0, SRN_H - (WORD_H * 8), L"�E[W,E,R,T,Y,U,I,O,P,A,S�L�[: �e�n��G�̏o��]�@");
		WriteText(0, SRN_H - (WORD_H * 6), L"�E[F�L�[ : �{�X�̏o�� ]�@�E[G�L�[ : �G�̃^�C�v�ύX ]");
		WriteText(0, SRN_H - (WORD_H * 4), L"--------------------------------------------------------------------------");
	}
#endif
	//�����������܂�
	PrintFrameBuffer();	//��ʃo�b�t�@��]��
	RenderScreen();		//��ʂ������_�����O
	FrameSync();		//�P�t���[���̓���
}
//�I������
void Title_End(SCENE* _sc)
{
}