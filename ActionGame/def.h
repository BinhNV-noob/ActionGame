//def.h

#define	MAXOBJ		1000	// スプライトの数
#define WINDOW_W	800		// ウィンドウの幅
#define WINDOW_H	640		// ウィンドウの高さ

//#define GM_		0		// 

//#define O_		0		// 

//#define ID_		1		// 

#define PLAYER_W 80
#define PLAYER_H 80

#define ENEMY_W 82
#define ENEMY_H 80

#define START_W 256
#define START_H 40

// ID管理
#define ID_PLAYER 1
#define ID_ENEMY  ID_PLAYER + 1

//配列のindex管理
#define IDX_PLAYER 1
#define IDX_ENEMY  IDX_PLAYER + 1

#define BMP_BG			0
#define BMP_PLAYER_L	1
#define BMP_PLAYER_R	2
#define BMP_ENEMY_L		3
#define BMP_ENEMY_R		4
#define BMP_TITLE		5
#define BMP_START		6
#define BMP_SCORE		7
#define BMP_HOLE		8
#define BMP_BLOCK		9

#define BLOCK_W 32
#define BLOCK_H 32
#define X_LINE (WINDOW_W / BLOCK_W)
#define Y_LINE (WINDOW_H / BLOCK_H)

#define PLAYER_ATK_W 89
#define ANIM_ATTACK_TIME 10
#define ANIM_ATTACK_PATTERN 2
#define ANIM_ATTACK_LOOP 0
#define ANIM_ATTACK_OFFSET 12
#define PLAYERMODE_ATTACK PLAYERMODE_WALK + 1
#define PLAYERMODE_ATTACK_AFTER PLAYERMODE_ATTACK + 1
#define PLAYER_ATTACK_OFFSET 968

#define ANIM_WAIT_TIME 10
#define ANIM_WAIT_PATTERN 4
#define ANIM_WAIT_LOOP 1
#define ANIM_WAIT_OFFSET 0 

#define ANIM_WALK_TIME 10
#define ANIM_WALK_PATTERN 4
#define ANIM_WALK_LOOP 1
#define ANIM_WALK_OFFSET 4

#define KEYUP 0x01
#define KEYDOWN 0x02
#define KEYLEFT 0x04
#define KEYRIGHT 0x08
#define KEYJUMP 0x10
#define KEYATTACK 0x20
#define PLAYERMODE_WAIT 1
#define PLAYERMODE_WALK PLAYERMODE_WAIT + 1
#define PLAYER_HIT_W 25
#define PLAYER_HIT_H 30

#define ANIM_FALL_TIME 1
#define ANIM_FALL_PATTERN 1
#define ANIM_FALL_LOOP 1
#define ANIM_FALL_OFFSET 9
#define PLAYERMODE_FALL PLAYERMODE_ATTACK_AFTER + 1

#define ANIM_JUMP_TIME 1
#define ANIM_JUMP_PATTERN 1
#define ANIM_JUMP_LOOP 1
#define ANIM_JUMP_OFFSET 8
#define PLAYERMODE_JUMP PLAYERMODE_FALL + 1
#define DIR_LEFT 0 // 左向き
#define DIR_RIGHT 1 // 右向き
#define BMP_PLAYER_BASE 1
#define BMP_ENEMY_BASE 3

struct CharData {
	int id;			// 識別番号
	int mode;			// アクション管理番号
	BOOL dspf;			// ０：非表示	1：表示
	int xposition;			// スクリーンＸ座標
	int yposition;			// スクリーンＹ座標
	double xspeed;
	double yspeed;
	int xsize;			// Ｘサイズ
	int ysize;			// Ｙサイズ
	int xboff;			// Ｘオフセット
	int yboff;			// Ｙオフセット
	int xmoff;			// Ｘマスク
	int ymoff;			// Ｙマスク
	int actioncnt;
	int idx;			// 画像番号
	int animcnt;
	int animpatternnow;
	int animpattern;
	int animloop;
	int direction;
};
typedef void (*TBLJP)(void);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GameLoop(void);
void ActionLoop(void);
void DrawLoop(void);
void KeyCheck(void);
void InitSet(void);

void ActPlayer(void);
int chkFix(void);
int chkRoof(void);