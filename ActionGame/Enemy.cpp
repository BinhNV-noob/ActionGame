#include <windows.h>
#include "Def.h"
#include "Work.h"

//--------------------------------------
// bảng xuất hiện enemy
// ESTBL[ time , id , 1:leftPipe 2:rightPipe ]
//--------------------------------------
int ESTBL[][3] = {
    { 10 , ID_ENEMY , 1 },
    { 50 , ID_ENEMY , 1 },
    { 100, ID_ENEMY , 2 },
    { 150, ID_ENEMY , 2 },
    { 200, ID_ENEMY , 1 },
    { 250, ID_ENEMY , 1 },
    { 300, ID_ENEMY , 2 },
    { 350, ID_ENEMY , 1 },
    { 400, ID_ENEMY , 2 },
    { 450, ID_ENEMY , 2 },

    { -1 , 0 , 0 },   // kết thúc
};

//--------------------------------------
// tìm index trống trong obj[] cho enemy
//--------------------------------------
int getEnemyObjID(void)
{
    int result = 0;

    for (int i = IDX_ENEMY; i < MAXOBJ; i++) {
        if (obj[i].mode == 0) {   // slot trống
            result = i;
            break;
        }
    }
    return result;
}

//--------------------------------------
// spawn enemy theo thời gian gameCount
//--------------------------------------
void EnemySet(void)
{
    if (ESTBL[enemyCount][0] < 0) {
        // hết dữ liệu spawn
        return;
    }

    if (gameCount == ESTBL[enemyCount][0]) {
        // thêm enemy mới
        int idx = getEnemyObjID();
        if (idx != 0) {
            InitEnemy(idx, ESTBL[enemyCount][2]);
        }
        enemyCount++;
    }
}

//--------------------------------------
// khởi tạo 1 enemy theo ống trái/ phải
//--------------------------------------
void InitEnemy(int enemyID, int enemyOutPut)
{
    // thông tin cơ bản
    obj[enemyID].id = ID_ENEMY;
    obj[enemyID].mode = ENEMYMODE_WAIT;
    obj[enemyID].dspf = 1;

    obj[enemyID].xsize = ENEMY_W;
    obj[enemyID].ysize = ENEMY_H;

    obj[enemyID].xspeed = 0;
    obj[enemyID].yspeed = 0;

    // vị trí Y cố định (miệng ống)
    obj[enemyID].yposition = 100;

    if (enemyOutPut == 1) {
        // từ ống trái ra: đi sang phải
        obj[enemyID].direction = DIR_RIGHT;
        obj[enemyID].xposition = 50;
    }
    else {
        // từ ống phải ra: đi sang trái
        obj[enemyID].direction = DIR_LEFT;
        obj[enemyID].xposition = WINDOW_W - 50;
    }

    // sprite offset
    obj[enemyID].xboff = 0;
    obj[enemyID].yboff = 0;
    obj[enemyID].xmoff = 0;
    obj[enemyID].ymoff = ENEMY_H;

    obj[enemyID].actioncnt = 0;

    obj[enemyID].idx = BMP_ENEMY_BASE + obj[enemyID].direction;
    obj[enemyID].animcnt = ANIM_WAIT_TIME;
    obj[enemyID].animpatternnow = 0;
    obj[enemyID].animpattern = ANIM_WAIT_PATTERN;
    obj[enemyID].animloop = ANIM_WAIT_LOOP;
}

//--------------------------------------
// enemy AI
//--------------------------------------
void ActEnemy(void)
{
    int fixYposition = 0;

    switch (pp->mode) {
        // 初期化
    case 0:
        pp->mode = ENEMYMODE_WAIT;
        pp->actioncnt = 100;
        break;

        // 通常状態
    case ENEMYMODE_WAIT:
        // アニメ進行管理
        pp->xspeed = 0;
        animManager(ANIM_WAIT_TIME, ENEMYMODE_WAIT, 1);

        pp->actioncnt--;
        if (pp->actioncnt <= 0) {
            if (rand() % 40 == 0) {
                // 歩き開始
                pp->mode = ENEMYMODE_WALK;
                pp->animcnt = ANIM_WALK_TIME;
                pp->animpatternnow = 0;
                pp->animpattern = ANIM_WALK_PATTERN;
                pp->animloop = ANIM_WALK_LOOP;
                pp->xboff = pp->xsize * ANIM_WALK_OFFSET;
                pp->xmoff = pp->xboff;
                pp->direction = rand() % 2;
                pp->idx = BMP_ENEMY_BASE + pp->direction;
                pp->actioncnt = 100;
            }
        }
        break;

        // 歩き状態
    case ENEMYMODE_WALK:
        animManager(ANIM_WALK_TIME, ENEMYMODE_WALK, 1);

        pp->actioncnt--;

        if (pp->direction == DIR_LEFT)
            pp->xspeed = -5;
        else
            pp->xspeed = 5;

        // 歩き終了 → WAITに戻す
        if (pp->actioncnt <= 0) {
            pp->mode = ENEMYMODE_WAIT;
            pp->animcnt = ANIM_WAIT_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WAIT_PATTERN;
            pp->animloop = ANIM_WAIT_LOOP;
            pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
            pp->xmoff = pp->xboff;
        }

        // 地面チェック
        if (!chkFix()) {
            // 空中状態
            pp->animcnt = ANIM_FALL_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_FALL_PATTERN;
            pp->animloop = ANIM_FALL_LOOP;
            pp->xboff = pp->xsize * ANIM_FALL_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = ENEMYMODE_FALL;
            pp->yspeed = 0.0;
        }

        if (!holeWarp()) {
            pp->xposition += (int)pp->xspeed;
        }
        break;

        // 落下状態
    case ENEMYMODE_FALL:
        fixYposition = chkFix();
        holeWarp();
        if (fixYposition) {
            // 着地 → WAIT
            pp->animcnt = ANIM_WAIT_TIME;
            pp->animpatternnow = 0;
            pp->animpattern = ANIM_WAIT_PATTERN;
            pp->animloop = ANIM_WAIT_LOOP;
            pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
            pp->xmoff = pp->xboff;
            pp->mode = ENEMYMODE_WAIT;
            pp->yposition = fixYposition - (pp->ysize / 2) + 5;
            pp->yspeed = 0;
            pp->actioncnt = 100;
        }
        else {
            pp->yspeed += 0.3;
            pp->xposition += (int)pp->xspeed;
            pp->yposition += (int)pp->yspeed;
        }
        break;

        // やられ落下
    case ENEMYMODE_DOWN:
        if (pp->yposition >= WINDOW_H + pp->ysize) {
            // màn hình ngoài → tắt enemy
            pp->mode = 0;
        }
        else {
            pp->yspeed += 0.3;
            pp->xposition += (int)pp->xspeed;
            pp->yposition += (int)pp->yspeed;
        }
        break;
    }
    if (pp->mode != ENEMYMODE_DOWN) {          // enemy không ở trạng thái chết
        if (PlyEneHitcheck()) {                // va chạm player
            obj[IDX_PLAYER].mode = PLAYERMODE_DOWN;

            obj[IDX_PLAYER].animcnt = ANIM_DOWN_TIME;
            obj[IDX_PLAYER].animpatternnow = 0;
            obj[IDX_PLAYER].animpattern = ANIM_DOWN_PATTERN;
            obj[IDX_PLAYER].animloop = ANIM_DOWN_LOOP;
            obj[IDX_PLAYER].xboff = obj[IDX_PLAYER].xsize * ANIM_DOWN_OFFSET;
            obj[IDX_PLAYER].xmoff = obj[IDX_PLAYER].xboff;

            obj[IDX_PLAYER].xspeed = 0;
            obj[IDX_PLAYER].yspeed = -10;      // bị hất lên một chút

            killCount = 0;
            hitCount++;
        }
    }
}
