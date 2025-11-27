
#include <windows.h>
#include "Def.h"
#include "Work.h"

void ActNull(void) {		}

TBLJP ActJpTbl[] = {
	ActNull ,				//  00 : 
	ActNull ,				//  01 : 
	ActNull ,				//  02 : 
	ActNull ,				//  03 : 
	ActNull ,				//  04 : 
	ActNull ,				//  05 : 
	ActNull ,				//  06 : 
	ActNull ,				//  07 : 
	ActNull ,				//  08 : 
	ActNull ,				//  09 : 
	ActNull ,				//  10 : 
	ActNull ,				//  11 : 
	ActNull ,				//  12 : 
	ActNull ,				//  13 : 
	ActNull ,				//  14 : 
	ActNull ,				//  15 : 
};


void ActionLoop(void)
{
	int i;

	for (i = 0; i < MAXOBJ; i++)
	{
		if (obj[i].id != 0)
		{
			pp = &obj[i];
			ActJpTbl[pp->id]();
		}
	}
}

