#pragma once
#include <windows.h>

#define MAXOBJ     1000   // スプライトの数
#define WINDOW_W   800    // ウィンドウの幅
#define WINDOW_H   640    // ウィンドウの高さ

#define PLAYER_W   80
#define PLAYER_H   80

#define ENEMY_W    80
#define ENEMY_H    80

#define START_W    256
#define START_H    40

// ID管理
#define ID_PLAYER  1
#define ID_ENEMY   (ID_PLAYER + 1)

// 配列のindex管理
#define IDX_PLAYER 1
#define IDX_ENEMY  (IDX_PLAYER + 1)

// ビットマップ番号
#define BMP_BG         0
#define BMP_PLAYER_L   1
#define BMP_PLAYER_R   2
#define BMP_ENEMY_L    3
#define BMP_ENEMY_R    4
#define BMP_TITLE      5
#define BMP_START      6
#define BMP_SCORE      7
#define BMP_HOLE       8
#define BMP_BLOCK      9

#define BLOCK_W  32
#define BLOCK_H  32
#define X_LINE   (WINDOW_W / BLOCK_W)
#define Y_LINE   (WINDOW_H / BLOCK_H)

// 攻撃アニメ
#define PLAYER_ATK_W        88
#define ANIM_ATTACK_TIME    4
#define ANIM_ATTACK_PATTERN 2
#define ANIM_ATTACK_LOOP    0
#define ANIM_ATTACK_OFFSET  11
#define PLAYERMODE_ATTACK        (PLAYERMODE_WALK + 1)
#define PLAYERMODE_ATTACK_AFTER  (PLAYERMODE_ATTACK + 1)
#define PLAYER_ATTACK_OFFSET     968

// 待機
#define ANIM_WAIT_TIME    12    
#define ANIM_WAIT_PATTERN 4
#define ANIM_WAIT_LOOP    1
#define ANIM_WAIT_OFFSET  0

// 歩き
#define ANIM_WALK_TIME    4
#define ANIM_WALK_PATTERN 4
#define ANIM_WALK_LOOP    1
#define ANIM_WALK_OFFSET  4

// 入力ビット
#define KEYUP     0x01
#define KEYDOWN   0x02
#define KEYLEFT   0x04
#define KEYRIGHT  0x08
#define KEYJUMP   0x10
#define KEYATTACK 0x20

// プレイヤーモード
#define PLAYERMODE_WAIT 1
#define PLAYERMODE_WALK (PLAYERMODE_WAIT + 1)

// ヒットボックス
#define PLAYER_HIT_W 25
#define PLAYER_HIT_H 30

// 落下
#define ANIM_FALL_TIME    1
#define ANIM_FALL_PATTERN 1
#define ANIM_FALL_LOOP    1
#define ANIM_FALL_OFFSET  9
#define PLAYERMODE_FALL   (PLAYERMODE_ATTACK_AFTER + 1)

// ジャンプ
#define ANIM_JUMP_TIME    1
#define ANIM_JUMP_PATTERN 1
#define ANIM_JUMP_LOOP    1
#define ANIM_JUMP_OFFSET  8
#define PLAYERMODE_JUMP   (PLAYERMODE_FALL + 1)

// やられ（DOWN）
#define ANIM_DOWN_TIME    1
#define ANIM_DOWN_PATTERN 1
#define ANIM_DOWN_LOOP    1
#define ANIM_DOWN_OFFSET  11
#define PLAYERMODE_DOWN   (PLAYERMODE_JUMP + 1)

// 向き
#define DIR_LEFT  0  // 左向き
#define DIR_RIGHT 1  // 右向き

#define BMP_PLAYER_BASE 1
#define BMP_ENEMY_BASE  3

// 土管
#define HOLE_W 100
#define HOLE_H 100

// Enemy mode (No.9 用)
#define ENEMYMODE_WAIT         1
#define ENEMYMODE_WALK         (ENEMYMODE_WAIT + 1)
#define ENEMYMODE_ATTACK       (ENEMYMODE_WALK + 1)
#define ENEMYMODE_ATTACK_AFTER (ENEMYMODE_ATTACK + 1)
#define ENEMYMODE_FALL         (ENEMYMODE_ATTACK_AFTER + 1)
#define ENEMYMODE_JUMP         (ENEMYMODE_FALL + 1)

struct CharData {
    int   id;           // 識別番号
    int   mode;         // アクション管理番号
    BOOL  dspf;         // 0:非表示 1:表示
    int   xposition;    // Ｘ座標（中心）
    int   yposition;    // Ｙ座標（中心）
    double xspeed;
    double yspeed;
    int   xsize;        // Ｘサイズ
    int   ysize;        // Ｙサイズ
    int   xboff;        // Ｘオフセット
    int   yboff;        // Ｙオフセット
    int   xmoff;        // Ｘマスク
    int   ymoff;        // Ｙマスク
    int   actioncnt;
    int   idx;          // 画像番号
    int   animcnt;
    int   animpatternnow;
    int   animpattern;
    int   animloop;
    int   direction;
};

typedef void (*TBLJP)(void);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void GameLoop(void);
void ActionLoop(void);
void DrawLoop(void);
void KeyCheck(void);
void InitSet(void);
void InitPlayer(void);
void InitEnemy(void);
void ActEnemy(void);
void ActPlayer(void);
int  chkFix(void);
int  chkRoof(void);
int  holeWarp(void);


int PlyEneHitcheck(void);
int animManager(int anim_time, int next_mode, int dispflg);


#define ENEMYMODE_DOWN   ENEMYMODE_JUMP + 1


void EnemySet(void);
int  getEnemyObjID(void);
int  EnePsHitcheck(void);
void InitEnemy(int enemyID, int enemyOutPut);
