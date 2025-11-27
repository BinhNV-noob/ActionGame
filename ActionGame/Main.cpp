//Main.cpp

#include <windows.h>
#include "resource.h"
#include "Def.h"
#include "Work.h"


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;
	RECT sPos;

	SetRect(&sPos, 0, 0, WINDOW_W, WINDOW_H);
	AdjustWindowRect(&sPos, WS_OVERLAPPEDWINDOW, FALSE);

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("ModelApp");
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(wcex.lpszClassName,
		TEXT("Action Game"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		sPos.right - sPos.left,
		sPos.bottom - sPos.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	hwnd = hWnd;

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);


	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/* ----------------------------------------------------	*/
/*					�E�B���h�E�v���V�[�W��				*/
LRESULT CALLBACK WndProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	static HINSTANCE	hInst;
	HBITMAP				hBitmap;
	PAINTSTRUCT			ps;
	HDC					hDC;
	//	TCHAR				s[50] ;

	switch (message)
	{
	case WM_CREATE:
		hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

		hBmpTbl[BMP_BG] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BG));
		hBmpTbl[BMP_PLAYER_L] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_L));
		hBmpTbl[BMP_PLAYER_R] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PLAYER_R));
		hBmpTbl[BMP_ENEMY_L] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENEMY_L));
		hBmpTbl[BMP_ENEMY_R] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENEMY_R));
		hBmpTbl[BMP_TITLE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TITLE));
		hBmpTbl[BMP_START] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_START));
		hBmpTbl[BMP_SCORE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SCORE));
		hBmpTbl[BMP_HOLE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HOLE));
		hBmpTbl[BMP_BLOCK] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BLOCK));

		hDC = GetDC(hWnd);
		hDCBack = CreateCompatibleDC(hDC);
		hBitmap = CreateCompatibleBitmap(hDC, WINDOW_W, WINDOW_H);
		SelectObject(hDCBack, hBitmap);
		DeleteObject(hBitmap);
		ReleaseDC(hWnd, hDC);

		SetTimer(hWnd, 1, 50, NULL);
		break;

	case WM_TIMER:
		if (tmf == 0)
		{
			tmf++;
			GameLoop();
			ActionLoop();
			DrawLoop();
			InvalidateRect(hWnd, NULL, FALSE);
		}
		gameCount++;
		if (gameCount >= 10000) {
			gameCount = 0;
		}

		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		BitBlt(hDC, 0, 0, WINDOW_W, WINDOW_H, hDCBack, 0, 0, SRCCOPY);
		tmf = 0;
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		DeleteDC(hDCBack);			//����ʂ̍폜
		//�r�b�g�}�b�v�̍폜
//			DeleteObject( hBmpTbl[0] ) ;
//			DeleteObject( hBmpTbl[1] ) ;

		KillTimer(hWnd, 1);			//�^�C�}�̍폜
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/* -------------------------------------------- */
/*						�֐�					*/
/* -------------------------------------------- */
/*	�����@�Z�b�g	*/
void InitSet(void)
{
    // ===== PLAYER INIT =====
    obj[IDX_PLAYER].id        = ID_PLAYER;
    obj[IDX_PLAYER].mode      = PLAYERMODE_WAIT;      // trạng thái ban đầu
    obj[IDX_PLAYER].dspf      = 1;                    // hiển thị ON

    obj[IDX_PLAYER].xsize     = PLAYER_W;             // size sprite
    obj[IDX_PLAYER].ysize     = PLAYER_H;

    obj[IDX_PLAYER].xposition = 200;
    obj[IDX_PLAYER].yposition = 577 - (PLAYER_H / 2);

    obj[IDX_PLAYER].xspeed    = 0;
    obj[IDX_PLAYER].yspeed    = 0;

    obj[IDX_PLAYER].xboff     = 0;
    obj[IDX_PLAYER].yboff     = 0;
    obj[IDX_PLAYER].xmoff     = 0;
    obj[IDX_PLAYER].ymoff     = PLAYER_H;

    obj[IDX_PLAYER].actioncnt = 0;
    obj[IDX_PLAYER].idx       = BMP_PLAYER_R;         // hướng mặt ban đầu

    // ====== Player Animation Init (bắt buộc) ======
    obj[IDX_PLAYER].animcnt        = ANIM_WAIT_TIME;     // thời gian chờ mỗi frame
    obj[IDX_PLAYER].animpatternnow = 0;                  // frame hiện tại
    obj[IDX_PLAYER].animpattern    = ANIM_WAIT_PATTERN;  // số frame của WAIT
    obj[IDX_PLAYER].animloop       = ANIM_WAIT_LOOP;     // có lặp hay không


    // ===== ENEMY INIT =====
    obj[IDX_ENEMY].id        = ID_ENEMY;
    obj[IDX_ENEMY].mode      = 1;
    obj[IDX_ENEMY].dspf      = 1;

    obj[IDX_ENEMY].xsize     = ENEMY_W;
    obj[IDX_ENEMY].ysize     = ENEMY_H;

    obj[IDX_ENEMY].xposition = 400;
    obj[IDX_ENEMY].yposition = 577 - (ENEMY_H / 2);

    obj[IDX_ENEMY].xspeed    = 0;
    obj[IDX_ENEMY].yspeed    = 0;

    obj[IDX_ENEMY].xboff     = 0;
    obj[IDX_ENEMY].yboff     = 0;
    obj[IDX_ENEMY].xmoff     = 0;
    obj[IDX_ENEMY].ymoff     = ENEMY_H;

    obj[IDX_ENEMY].actioncnt = 0;
    obj[IDX_ENEMY].idx       = BMP_ENEMY_L;

    // (Enemy CHƯA được yêu cầu animation nên không set animcnt/pattern cho enemy)


    // ===== RESET GAME COUNTER =====
    gameCount = 0;
}


void KeyCheck(void)
{
	keystatus = 0;

	if (GetKeyState(VK_UP) < 0)        // 0x01
		keystatus |= KEYUP;
	if (GetKeyState(VK_DOWN) < 0)      // 0x02
		keystatus |= KEYDOWN;
	if (GetKeyState(VK_LEFT) < 0)      // 0x04
		keystatus |= KEYLEFT;
	if (GetKeyState(VK_RIGHT) < 0)     // 0x08
		keystatus |= KEYRIGHT;

	if (GetKeyState(VK_SPACE) < 0)     // 0x10
		keystatus |= KEYJUMP;
	if (GetKeyState(VK_F1) < 0)        // 0x20
		keystatus |= KEYATTACK;
}
