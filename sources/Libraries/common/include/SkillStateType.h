//=============================================================================
// FileName: SkillStateType.h
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Type
//=============================================================================

#ifndef SKILLSTATETYPE_H
#define SKILLSTATETYPE_H

const unsigned char	SSTATE_NONE			= 0;	// 空状态

// 前八种也用于地面的状态
const unsigned char	SSTATE_CUT			= 1;	// 烈焰斩
const unsigned char	SSTATE_FERVOR_ARROR	= 2;	// 炽热之箭
const unsigned char	SSTATE_FROST_ARROR	= 3;	// 霜冻之箭
const unsigned char	SSTATE_SKYROCKET	= 4;	// 焰火表演
const unsigned char	SSTATE_MURRAIN		= 5;	// 瘟神
const unsigned char	SSTATE_GIDDY		= 6;	// 眩晕
const unsigned char	SSTATE_FREEZE		= 7;	// 僵硬
const unsigned char	SSTATE_SLEEP		= 8;	// 昏睡
const unsigned char	SSTATE_BIND			= 9;	// 束缚
const unsigned char	SSTATE_FROST		= 10;	// 冰冻
const unsigned char	SSTATE_BEAT_BACK	= 11;	// 击退
const unsigned char	SSTATE_UNBEATABLE	= 12;	// 无敌
const unsigned char	SSTATE_TOXIN		= 13;	// 巫毒
const unsigned char	SSTATE_REBOUND		= 14;	// 反弹
const unsigned char	SSTATE_AVATAR		= 15;	// 变身
const unsigned char	SSTATE_TITAN		= 16;	// 巨人
const unsigned char	SSTATE_BLINDNESS	= 17;	// 失明
const unsigned char	SSTATE_HAIR			= 18;	// 理发
const unsigned char	SSTATE_FLOAT		= 19;	// 浮空
const unsigned char	SSTATE_CALL			= 20;	// 召唤
const unsigned char	SSTATE_SHIELD		= 21;	// 盾牌熟练
const unsigned char	SSTATE_TIGER		= 22;	// 虎啸
const unsigned char	SSTATE_HIDE			= 43;	// 隐身
const unsigned char	SSTATE_TRADE		= 85;	// 交易
const unsigned char	SSTATE_STALL		= 99;	// 摆摊
const unsigned char	SSTATE_REPAIR		= 100;	// 修理
const unsigned char	SSTATE_FORGE		= 101;	// 精炼


const unsigned char	AREA_STATE_MAXID	= 255;

// Modify by lark.li 20080828 begin
//const unsigned char	SKILL_STATE_MAXID	= 240;
const unsigned char	SKILL_STATE_MAXID	= 254;
// End

const unsigned char	SKILL_STATE_LEVEL	= 20;

const long	SSTATE_SIGN_BYTE_NUM	= (SKILL_STATE_MAXID + sizeof(char) * 8 - 1) / (sizeof(char) * 8);

const unsigned char	AREA_STATE_NUM		= AREA_STATE_MAXID;
const unsigned char	CHA_STATE_NUM		= 16;

#endif // SKILLSTATETYPE_H
