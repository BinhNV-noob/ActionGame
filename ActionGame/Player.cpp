// Player.cpp
#include <windows.h>
#include "Def.h"
#include "Work.h"

// ====== InitPlayer ======
void InitPlayer(void)
{
    obj[IDX_PLAYER].id = ID_PLAYER;
    obj[IDX_PLAYER].mode = PLAYERMODE_WAIT;
    obj[IDX_PLAYER].dspf = 1;

    obj[IDX_PLAYER].xsize = PLAYER_W;
    obj[IDX_PLAYER].ysize = PLAYER_H;

    obj[IDX_PLAYER].xposition = 170;

    int px = obj[IDX_PLAYER].xposition / BLOCK_W;
    int centerY = WINDOW_H / 2;
    int groundY = -1;
    int bestDist = 999999;

    for (int y = 0; y < Y_LINE; ++y) {
        if (mapdata[y][px] >= 1 && mapdata[y][px] <= 4) {
            int by = y * BLOCK_H;
            int dist = (by > centerY) ? (by - centerY) : (centerY - by);
            if (dist < bestDist) {
                bestDist = dist;
                groundY = by;
            }
        }
    }

    if (groundY == -1) {
        groundY = (Y_LINE - 1) * BLOCK_H;  // fallback
    }

    obj[IDX_PLAYER].yposition = groundY - (PLAYER_H / 2) + 5;

    obj[IDX_PLAYER].xspeed = 0;
    obj[IDX_PLAYER].yspeed = 0;

    obj[IDX_PLAYER].direction = DIR_RIGHT;
    obj[IDX_PLAYER].idx = BMP_PLAYER_BASE + obj[IDX_PLAYER].direction;

    obj[IDX_PLAYER].animcnt = ANIM_WAIT_TIME;
    obj[IDX_PLAYER].animpatternnow = 0;
    obj[IDX_PLAYER].animpattern = ANIM_WAIT_PATTERN;
    obj[IDX_PLAYER].animloop = ANIM_WAIT_LOOP;

    obj[IDX_PLAYER].xboff = obj[IDX_PLAYER].xsize * ANIM_WAIT_OFFSET;
    obj[IDX_PLAYER].xmoff = obj[IDX_PLAYER].xboff;
    obj[IDX_PLAYER].yboff = 0;
    obj[IDX_PLAYER].ymoff = PLAYER_H;

    obj[IDX_PLAYER].actioncnt = 0;
}

void ActPlayer(void)
{
    KeyCheck();

    switch (pp->mode) {
    case 0:
        pp->mode = PLAYERMODE_WAIT;
        break;

        // 待ち
    case PLAYERMODE_WAIT:
        pp->xspeed = 0;
        animManager(ANIM_WAIT_TIME, PLAYERMODE_WAIT, 1);

        if (!chkFix()) {
            pp->animcnt = ANIM_FALL_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_FALL_PATTERN;
            pp->animloop = ANIM_FALL_LOOP;
            pp->xboff = pp->xsize * ANIM_FALL_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_FALL;
            pp->yspeed = 0.0;
        }

        if ((keystatus & 0xf0) == KEYJUMP) {
            pp->animcnt = ANIM_JUMP_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_JUMP_PATTERN;
            pp->animloop = ANIM_JUMP_LOOP;
            pp->xboff = pp->xsize * ANIM_JUMP_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_JUMP;
            pp->yspeed = -10.0;
        }

        if ((keystatus & 0x0f) == KEYLEFT) {
            pp->direction = DIR_LEFT;
            pp->xspeed = -3;
        }
        if ((keystatus & 0x0f) == KEYRIGHT) {
            pp->direction = DIR_RIGHT;
            pp->xspeed = 3;
        }

        // 攻撃開始
        if ((keystatus & 0xf0) == KEYATTACK) {
            pp->idx = BMP_PLAYER_BASE + pp->direction;
            pp->xsize = PLAYER_ATK_W;
            pp->animcnt = ANIM_ATTACK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_ATTACK_PATTERN;
            pp->animloop = ANIM_ATTACK_LOOP;
            pp->xboff = pp->xsize * ANIM_ATTACK_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_ATTACK;
            break;
        }

        if (pp->xspeed != 0) {
            pp->animcnt = ANIM_WALK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WALK_PATTERN;
            pp->animloop = ANIM_WALK_LOOP;
            pp->xboff = pp->xsize * ANIM_WALK_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_WALK;
        }
        break;

        // 歩き
    case PLAYERMODE_WALK:
        animManager(ANIM_WALK_TIME, PLAYERMODE_WALK, 1);
        pp->xspeed = 0.0;

        if ((keystatus & 0x0f) == KEYLEFT) {
            pp->direction = DIR_LEFT;
            pp->idx = BMP_PLAYER_BASE + pp->direction;
            pp->xspeed = -10.0;
        }
        if ((keystatus & 0x0f) == KEYRIGHT) {
            pp->direction = DIR_RIGHT;
            pp->idx = BMP_PLAYER_BASE + pp->direction;
            pp->xspeed = 10.0;
        }

        if ((keystatus & 0xf0) == KEYATTACK) {
            pp->idx = BMP_PLAYER_BASE + pp->direction;
            pp->xsize = PLAYER_ATK_W;
            pp->animcnt = ANIM_ATTACK_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_ATTACK_PATTERN;
            pp->animloop = ANIM_ATTACK_LOOP;
            pp->xboff = pp->xsize * ANIM_ATTACK_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_ATTACK;
            break;
        }

        if ((keystatus & 0xf0) == KEYJUMP) {
            pp->animcnt = ANIM_JUMP_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_JUMP_PATTERN;
            pp->animloop = ANIM_JUMP_LOOP;
            pp->xboff = pp->xsize * ANIM_JUMP_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_JUMP;
            pp->yspeed = -10.0;
        }

        if (!holeWarp()) {
            pp->xposition += (int)pp->xspeed;
        }

        if (pp->xspeed == 0.0) {
            pp->animcnt = ANIM_WAIT_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WAIT_PATTERN;
            pp->animloop = ANIM_WAIT_LOOP;
            pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_WAIT;
        }

        if (!chkFix()) {
            pp->animcnt = ANIM_FALL_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_FALL_PATTERN;
            pp->animloop = ANIM_FALL_LOOP;
            pp->xboff = pp->xsize * ANIM_FALL_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_FALL;
            pp->yspeed = 0.0;
        }
        break;

        // ジャンプ
    case PLAYERMODE_JUMP:
        if (chkRoof()) {
            pp->xposition += (int)pp->xspeed;
            pp->yposition += (int)pp->yspeed;
            pp->yspeed = 0;
        }

        if (pp->yspeed >= 0) {
            pp->animcnt = ANIM_FALL_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_FALL_PATTERN;
            pp->animloop = ANIM_FALL_LOOP;
            pp->xboff = pp->xsize * ANIM_FALL_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_FALL;
        }
        else {
            holeWarp();
            pp->yspeed += 0.3;
            pp->xposition += (int)pp->xspeed;
            pp->yposition += (int)pp->yspeed;
        }
        break;

        // 攻撃
    case PLAYERMODE_ATTACK:
        animManager(ANIM_ATTACK_TIME, PLAYERMODE_ATTACK_AFTER, 1);
        EnePsHitcheck();
        break;

    case PLAYERMODE_ATTACK_AFTER:
        pp->animcnt = ANIM_WAIT_TIME;
        pp->animpatternnow = 0;
        pp->animpattern = ANIM_WAIT_PATTERN;
        pp->animloop = ANIM_WAIT_LOOP;
        pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
        pp->xmoff = pp->xboff;
        pp->mode = PLAYERMODE_WAIT;
        pp->xsize = PLAYER_W;
        break;

        // 落下
    case PLAYERMODE_FALL:
        fixYposition = chkFix();
        if (fixYposition) {
            pp->animcnt = ANIM_WAIT_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WAIT_PATTERN;
            pp->animloop = ANIM_WAIT_LOOP;
            pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = PLAYERMODE_WAIT;
            pp->yposition = fixYposition - (pp->ysize / 2) + 5;
            pp->yspeed = 0;
        }
        else {
            holeWarp();
            pp->yspeed += 0.3;
            pp->xposition += (int)pp->xspeed;
            pp->yposition += (int)pp->yspeed;
        }
        break;

        // やられ
    case PLAYERMODE_DOWN:
        if (pp->yposition >= WINDOW_H + pp->ysize) {
            // 画面外まで落ちたらプレイヤーだけ復活
            InitPlayer();
        }
        else {
            pp->yspeed += 0.3;
            pp->xposition += (int)pp->xspeed;
            pp->yposition += (int)pp->yspeed;
        }
        break;
    }
}