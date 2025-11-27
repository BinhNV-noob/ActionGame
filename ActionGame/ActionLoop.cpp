// ActionLoop.cpp

#include <windows.h>
#include "Def.h"
#include "Work.h"

void ActNull(void) {}

// Bảng function pointer cho từng ID
TBLJP ActJpTbl[] = {
    ActNull,     // 00 : không dùng
    ActPlayer,   // 01 : PLAYER (ID_PLAYER)
    ActEnemy,    // 02 : ENEMY  (ID_ENEMY)
    ActNull,     // 03 :
    ActNull,     // 04 :
    ActNull,     // 05 :
    ActNull,     // 06 :
    ActNull,     // 07 :
    ActNull,     // 08 :
    ActNull,     // 09 :
    ActNull,     // 10 :
    ActNull,     // 11 :
    ActNull,     // 12 :
    ActNull,     // 13 :
    ActNull,     // 14 :
    ActNull      // 15 :
};

void ActionLoop(void)
{
    int i;

    for (i = 0; i < MAXOBJ; i++) {
        if (obj[i].id != 0) {
            pp = &obj[i];
            ActJpTbl[pp->id]();
        }
    }
}

// プレイヤーの基本アクション
void ActPlayer(void)
{
    // đọc trạng thái phím mỗi frame
    KeyCheck();

    switch (pp->mode) {

        // 初期化
    case 0:
        pp->mode = PLAYERMODE_WAIT;
        break;

        // 待ち状態
    case PLAYERMODE_WAIT:
        // quản lý tiến trình animation
        pp->xspeed = 0;
        animManager(ANIM_WAIT_TIME, PLAYERMODE_WAIT, 1);

        // nhập trái/phải
        if ((keystatus & 0x0f) == KEYLEFT) {
            pp->idx = BMP_PLAYER_L;
            pp->xspeed = -5;
        }
        if ((keystatus & 0x0f) == KEYRIGHT) {
            pp->idx = BMP_PLAYER_R;
            pp->xspeed = 5;
        }

        // tấn công từ trạng thái đứng yên
        if ((keystatus & 0xf0) == KEYATTACK) {
            pp->animcnt = ANIM_ATTACK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_ATTACK_PATTERN;
            pp->animloop = ANIM_ATTACK_LOOP;
            pp->xboff = PLAYER_ATTACK_OFFSET;
            pp->xmoff = pp->xboff;      // マスク
            pp->mode = PLAYERMODE_ATTACK;
            pp->xspeed = 0;              // vô hiệu hóa di chuyển
            pp->xsize = PLAYER_ATK_W;   // Xサイズ攻撃用
        }
        // nếu có input di chuyển thì chuyển sang WALK
        else if (pp->xspeed != 0) {
            pp->animcnt = ANIM_WALK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WALK_PATTERN;
            pp->animloop = ANIM_WALK_LOOP;
            pp->xboff = pp->xsize * ANIM_WALK_OFFSET;
            pp->xmoff = pp->xboff;      // マスク
            pp->mode = PLAYERMODE_WALK;
        }
        break;

        // 歩き状態
    case PLAYERMODE_WALK:
        animManager(ANIM_WALK_TIME, PLAYERMODE_WALK, 1);

        pp->xspeed = 0.0;
        if ((keystatus & 0x0f) == KEYLEFT) {
            pp->xspeed = -5.0;
        }
        if ((keystatus & 0x0f) == KEYRIGHT) {
            pp->xspeed = 5.0;
        }
        pp->xposition += (int)pp->xspeed;

        // tấn công trong khi đang đi bộ
        if ((keystatus & 0xf0) == KEYATTACK) {
            pp->animcnt = ANIM_ATTACK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_ATTACK_PATTERN;
            pp->animloop = ANIM_ATTACK_LOOP;
            pp->xboff = PLAYER_ATTACK_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_ATTACK;
            pp->xspeed = 0;
            pp->xsize = PLAYER_ATK_W;
        }
        // không nhấn phím di chuyển nữa → quay về WAIT
        else if (pp->xspeed == 0.0) {
            pp->animcnt = ANIM_WAIT_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WAIT_PATTERN;
            pp->animloop = ANIM_WAIT_LOOP;
            pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_WAIT;
        }
        break;

        // 攻撃状態
    case PLAYERMODE_ATTACK:
        animManager(ANIM_ATTACK_TIME, PLAYERMODE_ATTACK_AFTER, 1);
        break;

        // 攻撃後状態
    case PLAYERMODE_ATTACK_AFTER:
        pp->animcnt = ANIM_WAIT_TIME;
        pp->animpatternnow = 0;
        pp->animpattern = ANIM_WAIT_PATTERN;
        pp->animloop = ANIM_WAIT_LOOP;
        pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
        pp->xmoff = pp->xboff;
        pp->mode = PLAYERMODE_WAIT;
        pp->xsize = PLAYER_W;   // trả lại size bình thường
        break;
    }
}

// まだ処理を書いていないのでダミー
void ActEnemy(void)
{
    // TODO: xử lý enemy sau
}

int animManager(int anim_time, int next_mode, int dispflg)
{
    pp->animcnt--;

    if (pp->animcnt <= 0) {

        // アニメ更新
        pp->animcnt = anim_time;
        pp->animpatternnow++;

        // 最終絵か確認
        if (pp->animpattern == pp->animpatternnow) {

            // アニメ終了？
            if (pp->animloop) {
                // 繰り返しあり
                pp->animpatternnow = 0;
            }
            else {
                // 繰り返しなし
                pp->mode = next_mode;
                pp->dspf = dispflg;
            }
        }
    }
    return 1;
}
