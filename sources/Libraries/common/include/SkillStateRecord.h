//=============================================================================
// FileName: SkillStateRecord.h
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#ifndef SKILLSTATERECORD_H
#define SKILLSTATERECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchSkillStateRecordKeyValue = (char)0xff;

#define defSKILLSTATE_NAME_LEN		17
#define defSKILLSTATE_SCRIPT_NAME	32
#define defSKILLSTATE_ACT_NUM		3
#define defSKILLSTATE_DESC_NAME_LEN	255

class CSkillStateRecord : public CRawDataInfo
{
public:
	// CSkillStateRecord();

	char	chID;									// 编号
	char	szName[defSKILLSTATE_NAME_LEN];			// 名称
	short	sFrequency;								// 触发的时间间隔（单位：秒）
	char	szOnTransfer[defSKILLSTATE_SCRIPT_NAME];// 状态到传递目标上的作用时间脚本
	char	szAddState[defSKILLSTATE_SCRIPT_NAME];	// 使用效果脚本
	char	szSubState[defSKILLSTATE_SCRIPT_NAME];	// 移除效果脚本
	char	chAddType;								// 替换策略
	bool	bCanCancel;								// 是否可由终端取消
	bool	bCanMove;								// 是否能够移动
	bool	bCanMSkill;								// 是否能够使用魔法技能
	bool	bCanGSkill;								// 是否能够使用普通技能
	bool	bCanTrade;								// 是否能够交易
	bool	bCanItem;								// 是否能够使用物品
	bool	bCanUnbeatable;							// 是否无敌
	bool	bCanItemmed;							// 是否能够被使用物品
	bool	bCanSkilled;							// 是否能够被使用技能
	bool	bNoHide;								// 是否不隐身
	bool	bNoShow;								// 是否不显形
	bool	bOptItem;								// 是否可操作道具
	bool	bTalkToNPC;								// 是否可和NPC对话
	char	bFreeStateID;							// 解除的效果编号

	// 客户端表现
	char	chScreen;								// 屏幕效果
	char    nActBehave[defSKILLSTATE_ACT_NUM];		// 动作表现
	short	sChargeLink;							// 客户端表现标识,用于表现充电贝壳
    short   sAreaEffect;                            // 地面持续特效
	bool	IsShowCenter;							// 地面状态特效,是否仅显示中心点
	bool	IsDizzy;								// 是否显示眩晕效果
	short	sEffect;								// 挂效果方特效表现
	short	sDummy1;								// 挂特效玩家的播放dummy点
	short	sBitEffect;								// 该状态下受击特效表现
	short	sDummy2;								// 特效播放dummy点
	short	sIcon;									// 中该效果挂在身上的ICON表现
	char	szIcon[defSKILLSTATE_NAME_LEN][10];		// icons for pots per level 
	char	szDesc[defSKILLSTATE_DESC_NAME_LEN];
	int		lColour;
public:
	void	RefreshPrivateData();

	int		GetActNum()			{ return _nActNum;		}

public:
	int		_nActNum;								// 动作表现个数

};

class CSkillStateRecordSet : public CRawDataSet
{
public:

	static CSkillStateRecordSet* I() { return _Instance; }

	CSkillStateRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSkillStateRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSkillStateRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSkillStateRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSkillStateRecord);
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

inline CSkillStateRecord* GetCSkillStateRecordInfo( int nTypeID )
{
	return (CSkillStateRecord*)CSkillStateRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // SKILLSTATERECORD_H