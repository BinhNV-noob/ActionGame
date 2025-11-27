//HitCheck.cpp

#include <windows.h>
#include "def.h"
#include "work.h"

/* ********************************************	*/
/*			ƒvƒŒƒCƒ„[@‚u‚r@“G				*/
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