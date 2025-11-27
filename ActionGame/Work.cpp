
#include <windows.h>
#include "Def.h"
#include "Work.h"

HDC			hDCBack;					// ����ʂ̃f�o�C�X�R���e�L�X�g	
HBITMAP		hBmpTbl[10];				// �r�b�g�}�b�v���[�h�p
HWND		hwnd;						// �E�C���h�E�n���h��
u_short		tmf = 0;					// �P�^�C�}�[�����`�F�b�N�t���O
CharData	obj[MAXOBJ];
CharData* pp;

int gameCount;

int			gmode = 0;					// �Q�[���Ǘ����[�h

