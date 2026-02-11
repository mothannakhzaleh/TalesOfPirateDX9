//----------------------------------------------------------------------
// 名称:游戏事件
// 作者:lh 2004-05-26
// 用途:用于响应事件
// 实现:事件必须依附于角色或道具存在
//----------------------------------------------------------------------
#pragma once

class CSkillRecord;
class CCharacter;

// 小工具,用于判断攻击单体的技能是否可使用
class CIsSkillUse
{
public:
	CIsSkillUse();

	// 仅判断技能是否可用
	bool		IsValid( CSkillRecord* pSkill, CCharacter* pSelf );

	// 用于综合判断是否可用
	bool		IsUse( CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget );

	// 判断技能是否可对某玩家使用
	bool		IsAttack( CSkillRecord* pSkill, CCharacter* pSelf, CCharacter* pTarget );		

	// 如果不能使用,得到错误原因
	const char*	GetError();	

private:
	enum eError
	{
		enumNone,

		enumSelf,
		enumFish,
		enumDieBoat,
		enumOnlyTeam,
		enumTree,
		enumMine,
		enumDie,
		enumRepair,

		enumAttackMain,
		enumAttackTeam,
		enumHelpMons,
		enumAttackDie,
		enumTargetError,
		enumAttackPlayer,

		enumInValid,
		enumNotEnergy,
		enumNotAttack,
		enumNotUse,
		enumNotMP,
		enumNotTarget,

	};
	eError			_eError;
	CSkillRecord*	_pSkill;

};

extern CIsSkillUse	g_SkillUse;
