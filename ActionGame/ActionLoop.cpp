// ActionLoop.cpp

#include <windows.h>
#include "Def.h"
#include "Work.h"

// keystatus đã extern trong Work.cpp, prototype KeyCheck đã có trong Def.h
// → không cần khai báo lại ở đây

void ActNull(void) {}

// bảng hàm action cho từng ID
TBLJP ActJpTbl[] = {
    ActNull,    // 00 : không dùng
    ActPlayer,  // 01 : PLAYER (ID_PLAYER)
    ActEnemy,   // 02 : ENEMY  (ID_ENEMY)
    ActNull,    // 03 :
    ActNull,    // 04 :
    ActNull,    // 05 :
    ActNull,    // 06 :
    ActNull,    // 07 :
    ActNull,    // 08 :
    ActNull,    // 09 :
    ActNull,    // 10 :
    ActNull,    // 11 :
    ActNull,    // 12 :
    ActNull,    // 13 :
    ActNull,    // 14 :
    ActNull     // 15 :
};

void ActionLoop(void)
{
    for (int i = 0; i < MAXOBJ; i++) {
        if (obj[i].id != 0) {
            pp = &obj[i];
            ActJpTbl[pp->id]();
        }
    }
}

// =======================
// プレイヤーの基本アクション
// =======================
void ActPlayer(void)
{
    // đọc trạng thái phím, gán vào keystatus
    KeyCheck();

    switch (pp->mode) {

        // khởi tạo lần đầu (trường hợp nào đó reset về mode 0)
    case 0:
        pp->mode = PLAYERMODE_WAIT;
        pp->xspeed = 0.0;
        pp->yspeed = 0.0;

        // anim đứng yên
        pp->animcnt = ANIM_WAIT_TIME;
        pp->animpatternnow = 0;
        pp->animpattern = ANIM_WAIT_PATTERN;
        pp->animloop = ANIM_WAIT_LOOP;

        pp->xsize = PLAYER_W;
        pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
        pp->xmoff = pp->xboff;

        pp->idx = BMP_PLAYER_R;   // ban đầu quay phải
        break;

        // trạng thái đứng yên
    case PLAYERMODE_WAIT:
        // tiến trình anim (loop trong mode WAIT)
        animManager(ANIM_WAIT_TIME, PLAYERMODE_WAIT, 1);

        pp->xspeed = 0.0;

        // di chuyển trái / phải
        if ((keystatus & 0x0f) == KEYLEFT) {
            pp->idx = BMP_PLAYER_L;
            pp->xspeed = -5.0;
        }
        else if ((keystatus & 0x0f) == KEYRIGHT) {
            pp->idx = BMP_PLAYER_R;
            pp->xspeed = 5.0;
        }

        // nếu nhấn attack khi đang đứng
        if ((keystatus & 0xf0) == KEYATTACK) {

            pp->animcnt = ANIM_ATTACK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_ATTACK_PATTERN;
            pp->animloop = ANIM_ATTACK_LOOP;

            // FIX: set xsize trước rồi mới tính xboff, tránh lệch frame
            pp->xsize = PLAYER_W;  // dùng cùng chiều rộng với frame thường

            // offset frame đầu tiên của anim attack (tính theo frame-index)
            pp->xboff = pp->xsize * ANIM_ATTACK_OFFSET;
            pp->xmoff = pp->xboff;

            pp->mode = PLAYERMODE_ATTACK;
            pp->xspeed = 0.0;

            break;
        }

        // nếu có input di chuyển thì chuyển sang WALK
        if (pp->xspeed != 0.0) {
            pp->animcnt = ANIM_WALK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WALK_PATTERN;
            pp->animloop = ANIM_WALK_LOOP;

            pp->xboff = pp->xsize * ANIM_WALK_OFFSET;
            pp->xmoff = pp->xboff;

            pp->mode = PLAYERMODE_WALK;
        }
        break;

        // trạng thái đi bộ
    case PLAYERMODE_WALK:
        // anim đi bộ
        animManager(ANIM_WALK_TIME, PLAYERMODE_WALK, 1);

        pp->xspeed = 0.0;

        if ((keystatus & 0x0f) == KEYLEFT) {
            pp->idx = BMP_PLAYER_L;
            pp->xspeed = -5.0;
        }
        else if ((keystatus & 0x0f) == KEYRIGHT) {
            pp->idx = BMP_PLAYER_R;
            pp->xspeed = 5.0;
        }

        // cập nhật vị trí
        pp->xposition += (int)pp->xspeed;

        // giới hạn trong màn hình
        if (pp->xposition < pp->xsize / 2) {
            pp->xposition = pp->xsize / 2;
        }
        if (pp->xposition > WINDOW_W - pp->xsize / 2) {
            pp->xposition = WINDOW_W - pp->xsize / 2;
        }

        // attack khi đang đi bộ
        if ((keystatus & 0xf0) == KEYATTACK) {

            pp->animcnt = ANIM_ATTACK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_ATTACK_PATTERN;
            pp->animloop = ANIM_ATTACK_LOOP;

            // FIX: tương tự WAIT, set xsize trước rồi mới tính offset
            pp->xsize = PLAYER_W;

            pp->xboff = pp->xsize * ANIM_ATTACK_OFFSET;
            pp->xmoff = pp->xboff;

            pp->mode = PLAYERMODE_ATTACK;
            pp->xspeed = 0.0;

            break;
        }

        // nếu không còn input di chuyển thì quay về WAIT
        if (pp->xspeed == 0.0) {
            pp->animcnt = ANIM_WAIT_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WAIT_PATTERN;
            pp->animloop = ANIM_WAIT_LOOP;

            pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
            pp->xmoff = pp->xboff;

            pp->mode = PLAYERMODE_WAIT;
        }
        break;

        // đang chém
    case PLAYERMODE_ATTACK:
        // khi anim chém kết thúc sẽ nhảy sang PLAYERMODE_ATTACK_AFTER
        animManager(ANIM_ATTACK_TIME, PLAYERMODE_ATTACK_AFTER, 1);
        break;

        // sau khi chém xong, reset về trạng thái đứng
    case PLAYERMODE_ATTACK_AFTER:
        // FIX: trả xsize về trước rồi mới set offset theo WAIT
        pp->xsize = PLAYER_W;      // trả lại chiều rộng sprite thường

        pp->animcnt = ANIM_WAIT_TIME;
        pp->animpatternnow = 0;
        pp->animpattern = ANIM_WAIT_PATTERN;
        pp->animloop = ANIM_WAIT_LOOP;

        pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
        pp->xmoff = pp->xboff;

        pp->mode = PLAYERMODE_WAIT;
        break;
    }
}

// =======================
// Enemy: đi qua lại, có anim
// =======================
void ActEnemy(void)
{
    switch (pp->mode) {

        // khởi tạo
    case 0:
        pp->mode = 1;          // mode đi bộ
        pp->xspeed = -2.0;       // bắt đầu đi sang trái
        pp->idx = BMP_ENEMY_L;

        pp->animcnt = ENE_WALK_TIME;
        pp->animpatternnow = 0;
        pp->animpattern = ENE_WALK_PATTERN;
        pp->animloop = ENE_WALK_LOOP;

        pp->xboff = pp->xsize * ENE_WALK_OFFSET;
        pp->xmoff = pp->xboff;
        break;

        // đi bộ
    case 1:
        // cập nhật anim (loop trong mode 1)
        animManager(ENE_WALK_TIME, 1, 1);

        // di chuyển
        pp->xposition += (int)pp->xspeed;

        // chạm biên trái/phải màn hình thì quay đầu
        if (pp->xposition < pp->xsize / 2) {
            pp->xposition = pp->xsize / 2;
            pp->xspeed = 2.0;
            pp->idx = BMP_ENEMY_R;
        }
        else if (pp->xposition > WINDOW_W - pp->xsize / 2) {
            pp->xposition = WINDOW_W - pp->xsize / 2;
            pp->xspeed = -2.0;
            pp->idx = BMP_ENEMY_L;
        }
        break;
    }
}

// =======================
// アニメーション管理
// =======================
int animManager(int anim_time, int next_mode, int dispflg)
{
    pp->animcnt--;

    if (pp->animcnt <= 0) {

        pp->animcnt = anim_time;
        pp->animpatternnow++;

        if (pp->animpattern == pp->animpatternnow) {

            if (pp->animloop) {
                pp->animpatternnow = 0;
            }
            else {
                pp->mode = next_mode;
                pp->dspf = dispflg;
            }
        }
    }
    return 1;
}
