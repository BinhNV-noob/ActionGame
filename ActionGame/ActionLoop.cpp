//ActionLoop.cpp


#include <windows.h>
#include "Def.h"
#include "Work.h"
void ActNull(void) {		}

TBLJP ActJpTbl[] = {
	ActNull ,				//  00 : 
	ActPlayer ,				//  01 : 
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
}int animManager(int anim_time, int next_mode, int dispflg) {
	pp->animcnt--;
	if (pp->animcnt <= 0) {
		// アニメ更新
		pp->animcnt = anim_time;
		pp->animpatternnow++;
		// 最終絵か確認
		if (pp->animpattern == pp->animpatternnow) {
			// アニメ終了
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
void ActPlayer(void) {
	KeyCheck();
	switch (pp->mode) {
		// 初期化
	case 0:
		pp->mode = 1;
		break;
		// 待ち状態
	case PLAYERMODE_WAIT:
		// アニメ進行管理
		pp->xspeed = 0;
		animManager(ANIM_WAIT_TIME, PLAYERMODE_WAIT, 1);
		if (!chkFix()) {
			// 空中状態
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
			// ジャンプ状態
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
			pp->xspeed = -5;
		}
		if ((keystatus & 0x0f) == KEYRIGHT) {
			pp->direction = DIR_RIGHT;
			pp->xspeed = 5;
		}
		if ((keystatus & 0xf0) == KEYATTACK) {
			pp->idx = BMP_PLAYER_BASE + pp->direction;
		}
		// 移動入力入力されたら共通部分のデータ代入
		if (pp->xspeed != 0) {
			pp->animcnt = ANIM_WALK_TIME; // その絵の状態を保持する時間 0になったら次の絵に移行
			pp->animpatternnow = 0; // 今何枚目の絵か
			pp->animpattern = ANIM_WALK_PATTERN; // そのアニメの絵の数
			pp->animloop = ANIM_WALK_LOOP;// そのアニメはループするタイプか 0:ループ無し 1:ループあり
			pp->xboff = pp->xsize * ANIM_WALK_OFFSET;
			pp->xmoff = pp->xboff; // Xマスク
			pp->mode = PLAYERMODE_WALK;
		}
		break;
		// 歩き状態
	case PLAYERMODE_WALK:
		animManager(ANIM_WALK_TIME, PLAYERMODE_WALK, 1);
		pp->xspeed = 0.0;
		if ((keystatus & 0x0f) == KEYLEFT) {
			pp->direction = DIR_LEFT;
			pp->idx = BMP_PLAYER_BASE + pp->direction;
			pp->xspeed = -5.0;
		}
		if ((keystatus & 0x0f) == KEYRIGHT) {
			pp->direction = DIR_RIGHT;
			pp->idx = BMP_PLAYER_BASE + pp->direction;
			pp->xspeed = 5.0;
		}
		if ((keystatus & 0xf0) == KEYATTACK) {
			pp->idx = BMP_PLAYER_BASE + pp->direction;
		}
		if ((keystatus & 0xf0) == KEYJUMP) {
			// ジャンプ状態
			pp->animcnt = ANIM_JUMP_TIME;
			pp->animpatternnow = 0;
			pp->animpattern = ANIM_JUMP_PATTERN;
			pp->animloop = ANIM_JUMP_LOOP;
			pp->xboff = pp->xsize * ANIM_JUMP_OFFSET;
			pp->xmoff = pp->xboff;
			pp->mode = PLAYERMODE_JUMP;
			pp->yspeed = -12.0;
		}
		pp->xposition += (int)pp->xspeed;
		if (pp->xspeed == 0.0) {
			pp->animcnt = ANIM_WAIT_TIME; // その絵の状態を保持する時間 0になったら次の絵に移行
			pp->animpatternnow = 0; // 今何枚目の絵か
			pp->animpattern = ANIM_WAIT_PATTERN; // そのアニメの絵の数
			pp->animloop = ANIM_WAIT_LOOP;// そのアニメはループするタイプか 0:ループ無し 1:ループあり
			pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
			pp->xmoff = pp->xboff; // Xマスク
			pp->mode = PLAYERMODE_WAIT;
		}
		if (!chkFix()) {
			// 空中状態
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
		// 攻撃状態
	case PLAYERMODE_ATTACK:
		animManager(ANIM_ATTACK_TIME, PLAYERMODE_ATTACK_AFTER, 1);
		break;
		// 攻撃状態
	case PLAYERMODE_ATTACK_AFTER: // その絵の状態を保持する時間 0になったら次の絵に移行
		pp->animcnt = ANIM_WAIT_TIME;
		pp->animpatternnow = 0; // 今何枚目の絵か
		pp->animpattern = ANIM_WAIT_PATTERN; // そのアニメの絵の数
		pp->animloop = ANIM_WAIT_LOOP; // そのアニメはループするタイプか 0:ループ無し 1:ループあり
		pp->xboff = pp->xsize * ANIM_WAIT_OFFSET;
		pp->xmoff = pp->xboff; // Xマスク
		pp->mode = PLAYERMODE_WAIT;
		pp->xsize = PLAYER_W; // Xサイズ
		break;
	case PLAYERMODE_FALL:
		fixYposition = chkFix();
		if (fixYposition) {
			// 着地状態
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
			pp->yspeed += 0.3;
			pp->xposition += (int)pp->xspeed;
			pp->yposition += (int)pp->yspeed;
		}
		break;
	}
}