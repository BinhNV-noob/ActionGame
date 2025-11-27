
//work.h

extern HDC         hDCBack;				// device context cho back buffer (vẽ ngoài màn hình)
extern HBITMAP     hBmpTbl[10];			// mảng lưu các bitmap đã load
extern HWND        hwnd;				// handle cửa sổ chính
extern u_short     tmf;					// cờ timer (đánh dấu mỗi tick của timer)
extern CharData    obj[MAXOBJ];			// mảng quản lý toàn bộ object / sprite
extern CharData* pp;					// con trỏ trỏ tới object đang xử lý

extern int gameCount;					// bộ đếm thời gian / frame trong game

extern int         gmode;				// trạng thái game (title, play, gameover,...)
extern char mapdata[Y_LINE][X_LINE];

extern int keystatus;                   // trạng thái phím (bit flag)