//======================================================================================================================
// FileName: SkillRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CSkillRecordSet class
//======================================================================================================================

#ifndef	SKILLRECORD_H
#define	SKILLRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "SkillBag.h"

const char cchSkillRecordKeyValue = (char)0xfe;				// 为-1时是都可以拿,为0时是空手,所以改为-2

#define	defSKILL_NAME_LEN	17
#define defSKILL_JOB_SELECT_NUM	9
#define defSKILL_ITEM_NEED_NUM	8
#define defSKILL_PRE_SKILL_NUM	3
#define defSKILL_ICON_NAME_LEN	17
#define defEFFECT_SELF_ATTR_NUM	2
#define defEFFECT_TAR_ATTR_NUM	2
#define defSELF_EFFECT_NUM		2
#define defEXPEND_ITEM_NUM	2
#define defSKILL_OPERATE_NUM	3
#define defSKILL_POSE_NUM       10
#define defSKILL_EFFECT_SCRIPT_LEN	33
#define defSKILL_RANGE_SET_SCRIPT	33
#define defSKILL_ACTION_EFFECT  3
#define defSKILL_ITEM_EFFECT    2

enum ESkillItemNeed
{
	enumSKILL_ITEM_NEED_TYPE	= 1,
	enumSKILL_ITEM_NEED_ID		= 2,
};

enum ESkillObjType
{
	// 该技能的施放目标选择1、自己；2、队友；3、场景；4、敌人；5、全体

	enumSKILL_TYPE_SELF		= 1,	// 自身
	enumSKILL_TYPE_TEAM		= 2,	// 队友
	enumSKILL_TYPE_SCENE	= 3,	// 场景特效
	enumSKILL_TYPE_ENEMY	= 4,	// 敌人
	enumSKILL_TYPE_ALL		= 5,	// 全体
	enumSKILL_TYPE_PLAYER_DIE	= 6,	// 玩家尸体
	enumSKILL_TYPE_EXCEPT_SELF  = 7,    // 除了自己以外的所有角色和怪物

	// 特殊技能类型
	enumSKILL_TYPE_REPAIR	= 17,	// 修补
	enumSKILL_TYPE_TREE		= 18,	// 砍树
	enumSKILL_TYPE_MINE		= 19,	// 开矿
	enumSKILL_TYPE_TRADE	= 22,	// 摆摊
	enumSKILL_TYPE_FISH		= 28,	// 捕鱼
	enumSKILL_TYPE_SALVAGE	= 29,	// 打捞沉船

};

enum eSelectCha		// 仅客户端用
{
	enumSC_NONE = 0,		// 不能对所有人使用
	enumSC_ALL,				// 对所有人使用
	enumSC_PLAYER,			// 其它玩家,不包括自己,用于菜单

	enumSC_ENEMY,			// 敌人:可PK则怪物+非同队玩家,否则仅为怪物
	enumSC_PLAYER_ASHES,	// 玩家尸体(已死亡的)

	enumSC_MONS,			// 普通怪物
	enumSC_MONS_REPAIRABLE, // 可修补怪物
	enumSC_MONS_TREE,       // 树怪物
	enumSC_MONS_MINE,		// 矿石怪物
	enumSC_MONS_FISH,		// 鱼怪物
	enumSC_MONS_DBOAT,		// 沉船怪物

	enumSC_SELF,			// 玩家自己
	enumSC_TEAM,			// 队员
};

enum ESkillEffType
{
	enumSKILL_EFF_BANEFUL	= 0,	// 有害的
	enumSKILL_EFF_HELPFUL	= 1,	// 有益的
};

enum ESkillUpgrade
{
	enumSKILL_UPGRADE_NONE,			// 不能升级
	enumSKILL_UPGRADE_CAN,			// 可升级
	enumSKILL_UPGRADE_MAX,			// 已经升到最大值
};

enum ESkillFightType
{
	enumSKILL_LAND_LIVE		= 0, // 陆地生活技能
	enumSKILL_FIGHT			= 1, // 战斗技能
	enumSKILL_SAIL			= 2, // 航海技能
	enumSKILL_SEE_LIVE		= 3, // 海上生活技能
};

enum ESkillSrcType
{
	enumSKILL_SRC_HUMAN	= 1,	// 人
	enumSKILL_SRC_BOAT	= 2,	// 船
};

enum ESkillTarHabitatType
{
	enumSKILL_TAR_LAND	= 1,	// 陆地
	enumSKILL_TAR_SEA	= 2,	// 海洋
	enumSKILL_TAR_LORS	= 3,	// 两栖
};

enum ESkillType
{
	enumSKILL_INBORN	= 0,	// 天生的技能
	enumSKILL_ACTIVE	= 1,	// 主动的技能
	enumSKILL_PASSIVE	= 2,	// 被动的技能
};

enum ESkillPhaseType
{
	enumSKILL_NOT_MANUAL_ADD   = 6,	// 玩家不允许手动升级
};


class CSkillRecord : public CRawDataInfo
{
public:
	CSkillRecord();

	short	sID;												// 编号
	_TCHAR	szName[defSKILL_NAME_LEN];							// 名称
	char    chFightType;										// 技能类型，生活技能，战斗技能
	char	chJobSelect[defSKILL_JOB_SELECT_NUM][2];			// 可以学习该技能的职业，及对应该职业可以学到的最高等级。
																// 0、怪物 1、剑士 2、狙击手 3、海客 4、航海士 5、商人 6、医生 7、技师 8、所有
	short	sItemNeed[3][defSKILL_ITEM_NEED_NUM][2];			// 左(0)右(1)手，身体(2)必须装备相应的道具才可使用技能。
	short	sConchNeed[defSKILL_ITEM_NEED_NUM][3];				// 贝壳需求（装备部位，类型，编号；装备部位，类型，编号；。。。）
	char	chPhase;											// 技能阶段：1。固有技能，2。基础技能，3。进阶技能，4。终极技能，5。海卵技能，6。技能等级能否手动添加
	char	chType;												// 技能类型：1。主动技能（手动释放），2。被动技能（自动释放）
	short	sLevelDemand;										// 学习该技能需要的等级
	short	sPremissSkill[defSKILL_PRE_SKILL_NUM][2];			// 需要的3个前提技能（技能编号，等级）
	char	chPointExpend;										// 消耗的技能点
	char	chSrcType;											// 施放者形态（人，船）
	char	chTarType;											// 目标类型（陆地生物，海上生物）
	short	sApplyDistance;										// 使用距离。单位：米
	char	chApplyTarget;										// 使用的目标。
	char	chApplyType;										// 作用类型：1。单体，2。范围，3。溅射
	char	chHelpful;											// 技能有益,还是有害
	short	sAngle;												// 角度（0-360），用于“范围“攻击类型。
	short	sRadii;												// 半径，用于“范围“攻击类型。
	char	chRange;											// 技能区域形状编号
	char	szPrepare[defSKILL_RANGE_SET_SCRIPT];				// 准备技能
	char	szUseSP[defSKILL_EFFECT_SCRIPT_LEN];				// 消耗SP公式
	char	szUseEndure[defSKILL_EFFECT_SCRIPT_LEN];			// 消耗“耐久度“公式
	char	szUseEnergy[defSKILL_EFFECT_SCRIPT_LEN];			// 消耗“能量“公式
	char	szSetRange[defSKILL_EFFECT_SCRIPT_LEN];				// 技能区域设定
	char	szRangeState[defSKILL_EFFECT_SCRIPT_LEN];			// 地表状态设定
	char	szUse[defSKILL_EFFECT_SCRIPT_LEN];					// 施放阶段公式
	char	szEffect[defSKILL_EFFECT_SCRIPT_LEN];				// 效果阶段公式
	char	szActive[defSKILL_EFFECT_SCRIPT_LEN];				// 正效果公式
	char	szInactive[defSKILL_EFFECT_SCRIPT_LEN];				// 反效果公式
	int		nStateID;											// 绑定的可手动移除状态编号
	short	sSelfAttr[defEFFECT_SELF_ATTR_NUM];					// 自身属性数值的变化，公式编号。
	short	sSelfEffect[defSELF_EFFECT_NUM];					// 自身效果
	short	sItemExpend[defEXPEND_ITEM_NUM][2];					// 消耗的物品（ID，数量）
	short	sBeingTime;											// 持续时间
	short	sTargetAttr[defEFFECT_TAR_ATTR_NUM];				// 目标属性数值的变化，公式编号。
	short	sSplashPara;										// 溅射参数
	short	sTargetEffect;										// 目标持续效果
	short	sSplashEffect;										// 溅射持续效果
	short	sVariation;											// 变身编号
	short	sSummon;											// 召唤编号
	short	sPreTime;											// 释放时间
	char	szFireSpeed[defSKILL_EFFECT_SCRIPT_LEN];			// 再施放速度公式
	char	chOperate[defSKILL_OPERATE_NUM];					// 特殊操作：0，无。1，上船。2，下船

public:		// 客户端表现用
	short	sActionHarm;										// 动作特效播放伤害表现方式
	char	chActionPlayType;									// 动作播放方式:0-顺序播放,1-随机播放
	short	sActionPose[defSKILL_POSE_NUM];						// 释放该技能主角的动作
	short	sActionKeyFrme;										// 人释放技能动作关键桢
	short	sWhop;												// 攻击音效
	short	sActionDummyLink[defSKILL_ACTION_EFFECT];			// 释放者人物link点
	short	sActionEffect[defSKILL_ACTION_EFFECT];				// 释放该技能主角的特效
	short	sActionEffectType[defSKILL_ACTION_EFFECT];			// 释放该技能的方式:0-无变化,1-能自动缩放
	short	sItemDummyLink;										// 释放者道具link点
	short	sItemEffect1[defSKILL_ITEM_EFFECT];					// 释放者道具特效,0位-道具特效,1位-执行时间点
	short	sItemEffect2[defSKILL_ITEM_EFFECT];					// 
	short	sSkyEffectActionKeyFrame;							// 飞出特效动作关键桢
	short   sSkyEffectActionDummyLink;							// 飞行出发点人物dummy
	short   sSkyEffectItemDummyLink;							// 飞行出发点道具dummy;
	short   sSkyEffect;											// 释放该技能飞行的特效
	short	sSkySpd;											// 飞行速度
	short	sWhoped;											// 受击音效
	short   sTargetDummyLink;									// 受击者link点
	short	sTargetEffectID;									// 受击特效
	char	chTargetEffectTime;									// 受击特效时间点 ,0-动作开始，1－关键帧，2－动作结束(飞行特效伤害无效)
    short   sAgroundEffectID;                                   // 对于范围攻击,地上的出现的特效（包括水）
	short	sWaterEffectID;										// 对于范围攻击,水面上的出现的特效 
	char	szICON[defSKILL_ICON_NAME_LEN];						// 该技能在游戏界面表现的图标
	char	chPlayTime;											// 技能播放次数
	char	szDescribeHint[128];								// 描述,用于hint
	char	szEffectHint[128];									// 效果,用于hint
	char	szExpendHint[128];									// 消耗,用于hint

public:
    bool    IsPlayCyc()         { return chPlayTime==1;     }   // 是否循环播放
    bool    IsAttackArea()		{ return chApplyType==2;    }   // 是否攻击地面
	bool	IsHarmRange()		{ return chApplyType!=1;	}	// 是否范围伤害
    bool    IsActKeyHarm()      { return sActionHarm==1;    }   // 是否动作关键帧伤害
    bool    IsEffectHarm()      { return sActionHarm==2;    }   // 是否特效伤害
    bool    IsNoHarm()          { return sActionHarm==0;    }   // 无伤害技能
	bool	IsShow()			{ return szICON[1]!='\0';	}	// 是否显示出来
	bool	IsPlayRand()		{ return chActionPlayType==1;	}
	int		GetPoseNum()		{ return _nPoseNum;			}

public:
    void    SetSkillGrid( SSkillGridEx& v )   { _Skill = v;   }
    SSkillGridEx&     GetSkillGrid()  { return _Skill;        }

	int     GetSPExpend()       { return _Skill.sUseSP;     }   // SP消耗
    short   GetRange()          { return _Skill.sRange[1];  }   // 攻击范围
    short*  GetParam()          { return &_Skill.sRange[1]; }
    int     GetShape()          { return _Skill.sRange[0];  }   // 技能形状
    int     GetDistance()       { return sApplyDistance;    }   // 使用距离
    int     GetLevel()          { return _Skill.chLv;       }
    int     GetFireSpeed()      { return _Skill.lResumeTime;}   // 攻击速度
    bool    GetIsValid()        { return _Skill.chState!=0; }   // 是否可用

	int     GetUpgrade()		{ return _nUpgrade;			}
	bool    GetIsUpgrade()		{ return _nUpgrade==enumSKILL_UPGRADE_CAN;	}

	bool	GetIsUse()			{ return chType!=2;			}
	bool	GetIsHelpful()		{ return chHelpful==1;		}	// 技能是有益的，可用于安全区

	void	Refresh( int nJob );

	void	SetIsActive( bool v )	{ _IsActive = v;		}
	bool	GetIsActive()			{ return _IsActive;		}

	void	SetAttackTime( DWORD v ){ _dwAttackTime=v;			}
	bool	IsAttackTime( DWORD v )	{ return v>=_dwAttackTime;	}

	int		GetSelectCha()		{ return _eSelectCha;		}

	bool	IsAutoAttack()		{ return !IsPlayCyc() && GetDistance()>0;	}

	void	RefreshPrivateData();				// 刷新内部数据
	
	bool	IsJobAllow( int nJob );
	int		GetJobMax( int nJob );

private:
    SSkillGridEx		_Skill;
	int					_nUpgrade;				// 0-不能升级,1-可升级,2-已经升级到最大值

	bool				_IsActive;				// 用于动画表现,正在激活的画一个动态边框
	DWORD				_dwAttackTime;			// 下次可攻击的时间

	eSelectCha			_eSelectCha;			// 用于场景判断可以选择那些玩家攻击

	int					_nPoseNum;				// 总共的pose个数

	// added by clp
public:
	bool IsReadingSkill()
	{
		return sID == 512;
	}
};

class CSkillRecordSet : public CRawDataSet
{
public:

	static CSkillRecordSet* I() { return _Instance; }

	CSkillRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSkillRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSkillRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSkillRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSkillRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);

};

inline CSkillRecord* GetSkillRecordInfo( int nTypeID )
{
	return (CSkillRecord*)CSkillRecordSet::I()->GetRawDataInfo(nTypeID);
}

inline CSkillRecord* GetSkillRecordInfo( const char* szName )
{
	return (CSkillRecord*)CSkillRecordSet::I()->GetRawDataInfo(szName);
}

inline bool CSkillRecord::IsJobAllow( int nJob )
{
	if( chJobSelect[0][0]==-1 ) return true;

	for (char i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
	{
		if (chJobSelect[i][0] == cchSkillRecordKeyValue)
			return false;
		else if( chJobSelect[i][0]==nJob )
			return true;
	}
	return false;
}

inline int CSkillRecord::GetJobMax( int nJob )
{
	if( chJobSelect[0][0]==-1 ) return chJobSelect[0][1];

	for (int i = 0; i<defSKILL_JOB_SELECT_NUM; i++ )
	{
		if (chJobSelect[i][0] == cchSkillRecordKeyValue)
			return -1;
		else if( chJobSelect[i][0]==nJob )
			return chJobSelect[i][1];
	}

	return -1;
}
#endif //SKILLRECORD_H