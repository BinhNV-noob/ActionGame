#include <windows.h>
#include "Def.h"
#include "Work.h"

int PlyEneHitcheck(void)
{
    int hf = 0;
    int playerxleft, playerxright, playeryup, playerydown;
    int enemyxleft, enemyxright, enemyyup, enemyydown;

    enemyxleft = pp->xposition - (pp->xsize / 2);
    enemyxright = pp->xposition + (pp->xsize / 2);
    enemyyup = pp->yposition - (pp->ysize / 2);
    enemyydown = pp->yposition + (pp->ysize / 2);

    playerxleft = obj[IDX_PLAYER].xposition - PLAYER_HIT_W;
    playerxright = obj[IDX_PLAYER].xposition + PLAYER_HIT_W;
    playeryup = obj[IDX_PLAYER].yposition - PLAYER_HIT_H;
    playerydown = obj[IDX_PLAYER].yposition + PLAYER_HIT_H;

    if (obj[IDX_PLAYER].id == ID_PLAYER) {
        if (obj[IDX_PLAYER].mode != PLAYERMODE_DOWN) {     // đang không ở DOWN
            if ((playeryup < enemyydown) && (playerydown > enemyyup)) {
                if ((playerxleft < enemyxright) && (playerxright > enemyxleft)) {
                    hf = 1;
                }
            }
        }
    }
    return hf;
}


int chkFix(void)
{
    int xidx, yidx;
    int result = 0;

    xidx = pp->xposition / BLOCK_W;
    yidx = (pp->yposition + (pp->ysize / 2) + 5) / BLOCK_H;
    if (mapdata[yidx][xidx] >= 1 && mapdata[yidx][xidx] <= 4) {
        result = yidx * BLOCK_H;
    }
    return result;
}

int chkRoof(void)
{
    int xidx, yidx;
    int result = 0;

    xidx = pp->xposition / BLOCK_W;
    yidx = (pp->yposition - (pp->ysize / 2) - 5) / BLOCK_H;
    if (mapdata[yidx][xidx] >= 1 && mapdata[yidx][xidx] <= 4) {
        result = 1;
    }
    return result;
}
int EnePsHitcheck(void)
{
    int i;
    int hf = 0;
    int playerxleft, playerxright, playeryup, playerydown;
    int enemyxleft, enemyxright, enemyyup, enemyydown;
    int atkarea = 10;    // 攻撃範囲の伸び

    // hướng tấn công
    if (pp->direction == DIR_LEFT) {
        playerxleft = pp->xposition - (pp->xsize / 2) - atkarea;
        playerxright = pp->xposition - (pp->xsize / 2);
    }
    else {
        playerxleft = pp->xposition + (pp->xsize / 2);
        playerxright = pp->xposition + (pp->xsize / 2) + atkarea;
    }
    playeryup = pp->yposition - (pp->ysize / 2);
    playerydown = pp->yposition + (pp->ysize / 2);

    for (i = IDX_ENEMY; i < MAXOBJ; i++) {
        enemyxleft = obj[i].xposition - (obj[i].xsize / 2);
        enemyxright = obj[i].xposition + (obj[i].xsize / 2);  
        enemyyup = obj[i].yposition - (obj[i].ysize / 2);
        enemyydown = obj[i].yposition + (obj[i].ysize / 2);

        if (obj[i].mode != 0 && obj[i].mode != ENEMYMODE_DOWN) {
            if ((playeryup < enemyydown) && (playerydown > enemyyup)) {
                if ((playerxleft < enemyxright) && (playerxright > enemyxleft)) {
                    // 敵をやられに
                    obj[i].mode = ENEMYMODE_DOWN;
                    obj[i].animcnt = ANIM_DOWN_TIME;
                    obj[i].animpatternnow = 0;
                    obj[i].animpattern = ANIM_DOWN_PATTERN;
                    obj[i].animloop = ANIM_DOWN_LOOP;
                    obj[i].xboff = obj[i].xsize * ANIM_DOWN_OFFSET;
                    obj[i].xmoff = obj[i].xboff;
                    obj[i].xspeed = 0;
                    obj[i].yspeed = -10;

                    killCount++;
                    hitCount++;
                }
            }
        }

    }
    return hf;
}
