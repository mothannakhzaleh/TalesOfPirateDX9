//=============================================================================
// FileName: ChaAction.h
// Creater: ZhangXuedong
// Date: 2005.09.15
// Comment: Character Action Control Type
//=============================================================================

#ifndef CHAACTION_H
#define CHAACTION_H

const long	ACTCONTROL_MOVE         = 0; // 移动位
const long	ACTCONTROL_USE_GSKILL   = 1; // 使用普通技能位
const long	ACTCONTROL_USE_MSKILL   = 2; // 使用魔法技能位
const long	ACTCONTROL_BEUSE_SKILL  = 3; // 被使用技能位
const long	ACTCONTROL_TRADE        = 4; // 交易位
const long	ACTCONTROL_USE_ITEM     = 5; // 使用物品位
const long	ACTCONTROL_BEUSE_ITEM   = 6; // 被使用物品位
const long	ACTCONTROL_INVINCIBLE   = 7; // 无敌位
const long	ACTCONTROL_EYESHOT      = 8; // 视野位（可以看到视野内可见的实体）
const long	ACTCONTROL_NOHIDE       = 9; // 不隐形（可以被看见）
const long	ACTCONTROL_NOSHOW       = 10; // 不被强制现形（如果有强制现形，则隐形被屏蔽）
const long	ACTCONTROL_ITEM_OPT     = 11; // 道具操作位
const long	ACTCONTROL_TALKTO_NPC   = 12; // 和NPC对话位

const long	ACTCONTROL_MAX          = 13;

#endif // CHAACTION_H