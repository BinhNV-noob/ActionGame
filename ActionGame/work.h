//work.h

extern HDC hDCBack;				// 裏画面のデバイスコンテキスト	
extern HBITMAP hBmpTbl[10];			// ビットマップロード用
extern HWND	hwnd;					// ウインドウハンドル
extern u_short tmf;					// １タイマー処理チェックフラグ
extern CharData	obj[MAXOBJ];
extern CharData* pp;
extern char mapdata[Y_LINE][X_LINE];
extern int gameCount;
extern int gmode;					// ゲーム管理モード
extern int fixYposition;
extern int keystatus;