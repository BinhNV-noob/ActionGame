#include <windows.h>
#include "Def.h"
#include "Work.h"

void ActNull(void) {}

TBLJP ActJpTbl[] = {
    ActNull ,   // 00
    ActPlayer,  // 01 : ID_PLAYER
    ActEnemy ,  // 02 : ID_ENEMY
    ActNull ,   // 03
    ActNull ,   // 04
    ActNull ,   // 05
    ActNull ,   // 06
    ActNull ,   // 07
    ActNull ,   // 08
    ActNull ,   // 09
    ActNull ,   // 10
    ActNull ,   // 11
    ActNull ,   // 12
    ActNull ,   // 13
    ActNull ,   // 14
    ActNull ,   // 15
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


