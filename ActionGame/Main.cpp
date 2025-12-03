#include <windows.h>
#include "resource.h"
#include "Def.h"
#include "Work.h"
#include <time.h>
int keystatus = 0;
/* ----------------------------------------------------	*/
/*					エントリポイント					*/
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	srand((unsigned)time(NULL));
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
		TEXT("ACTION GAME"),
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
/*					ウィンドウプロシージャ				*/
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
		hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

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
		SetTimer(hWnd, 1, 16, NULL);
		break;

	case WM_TIMER:
		if (tmf == 0)
		{
			tmf++;
			GameLoop();
			EnemySet();
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
		DeleteDC(hDCBack);			//裏画面の削除
		//ビットマップの削除
//			DeleteObject( hBmpTbl[0] ) ;
//			DeleteObject( hBmpTbl[1] ) ;

		KillTimer(hWnd, 1);			//タイマの削除
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/* -------------------------------------------- */
/*						関数					*/
/* -------------------------------------------- */
void InitSet(void)
{
	InitPlayer();
	//InitEnemy();
	gameCount = 0;
	enemyCount = 0;
	killCount = 0;
	hitCount = 0;
}

void KeyCheck(void) {
	keystatus = 0;
	if (GetKeyState(VK_UP) < 0) // 0x01
		keystatus |= KEYUP;
	if (GetKeyState(VK_DOWN) < 0) // 0x02
		keystatus |= KEYDOWN;
	if (GetKeyState(VK_LEFT) < 0) // 0x04
		keystatus |= KEYLEFT;
	if (GetKeyState(VK_RIGHT) < 0) // 0x08
		keystatus |= KEYRIGHT;
	if (GetKeyState(VK_SPACE) < 0) // 0x10
		keystatus |= KEYJUMP;
	if (GetKeyState(VK_F1) < 0) // 0x20
		keystatus |= KEYATTACK;
}

// 土管ワープ(画面端ループ)
int holeWarp(void)
{
	int result = 0;

	// 左側
	if (pp->xposition + (int)pp->xspeed < 0) {
		// 上 → 右下
		pp->xposition = WINDOW_W - 10;
		if (pp->yposition < BLOCK_H * 4) {
			// ワープ
			pp->yposition = WINDOW_H - (BLOCK_H * 3);
		}
		// 下 → 右上
		else if (pp->yposition > WINDOW_H - (BLOCK_H * 4)) {
			// ワープ
			pp->yposition = BLOCK_H * 3;
		}
		result = 1;
	}
	// 右側
	else if (pp->xposition + (int)pp->xspeed > WINDOW_W) {
		// 上 → 左下
		pp->xposition = 0 + 10;
		if (pp->yposition < BLOCK_H * 4) {
			// ワープ
			pp->yposition = WINDOW_H - (BLOCK_H * 3);
		}
		// 下 → 左上
		else if (pp->yposition > WINDOW_H - (BLOCK_H * 4)) {
			// ワープ
			pp->yposition = BLOCK_H * 3;
		}
		result = 1;
	}
	return result;
}
