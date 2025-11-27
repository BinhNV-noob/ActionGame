//GameLoop.cpp


#include <windows.h>
#include "Def.h"
#include "Work.h"

void GameNull(void) {		}
void GameOP(void) {		}

TBLJP GameJpTbl[] = {
	GameNull ,				//  00 : 
	GameNull ,				//  01 : 
	GameNull ,				//  02 : 
	GameNull ,				//  03 : 
	GameNull ,				//  04 : 
	GameNull ,				//  05 : 
	GameNull ,				//  06 : 
	GameNull ,				//  07 : 
	GameNull ,				//  08 : 
	GameNull ,				//  09 : 
	GameNull ,				//  10 : 
	GameNull ,				//  11 : 
	GameNull ,				//  12 : 
	GameNull ,				//  13 : 
	GameNull ,				//  14 : 
	GameNull ,				//  15 : 
};

void GameLoop(void)
{
	switch (gmode)
	{
	case 0:
		gmode = 1;
		break;
	case 1:
	
		if (GetKeyState(VK_SPACE) < 0) {
			gmode = 2;
		}
	
		break;
	case 2:
		InitSet();
		gmode = 3;
		break;
	case 3:
		//gmode = 6;
		break;
	case 6:
		gmode = 7;
		break;
	case 7:
		gmode = 8;
		break;

	case 8:
		gmode = 10;
		break;

	case 10:
		gmode = 11;
		break;

	case 11:
		gmode = 12;
		break;

	case 12:
		break;

	case 13:
		break;
	}
}

