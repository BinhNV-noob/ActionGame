//HitCheck.cpp


#include <windows.h>
#include "def.h"
#include "work.h"

/* ********************************************	*/
/*			プレイヤー　ＶＳ　敵				*/
/*
	if( 0 != PlyEne( 8,16,8,16 ) )
	{
		pp->idno = ID_BOMB ;
		pp->mode = 0 ;
	}

/* ********************************************	*/
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
		if ((playeryup < enemyydown) && (playerydown > enemyyup)) {
			if ((playerxleft < enemyxright) && (playerxright > enemyxleft)) {
				hf = 1;
			}
		}
	}
	return hf;
}
int chkFix(void) {
	int xidx, yidx;
	int result = 0;
	xidx = pp->xposition / BLOCK_W;
	yidx = (pp->yposition + (pp->ysize / 2) + 5) / BLOCK_H;
	if (mapdata[yidx][xidx] >= 1 && mapdata[yidx][xidx] <= 4) {
		result = yidx * BLOCK_H;
	}
	return result;
}
int chkRoof(void) {
	int xidx, yidx;
	int result = 0;
	xidx = pp->xposition / BLOCK_W;
	yidx = (pp->yposition - (pp->ysize / 2) - 5) / BLOCK_H;
	if (mapdata[yidx][xidx] >= 1 && mapdata[yidx][xidx] <= 4) {
		result = 1;
	}
	return result;
}