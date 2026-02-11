//=============================================================================
// FileName: SkillStateRecord.cpp
// Creater: ZhangXuedong
// Date: 2005.02.04
// Comment: Skill State Record
//=============================================================================

#include "SkillStateRecord.h"
using namespace std;
//---------------------------------------------------------------------------
// class CSkillStateRecord
//---------------------------------------------------------------------------
void CSkillStateRecord::RefreshPrivateData()
{
	_nActNum = 0;
	for (int i = 0; i < defSKILLSTATE_ACT_NUM; i++)
	{		
		if( nActBehave[i]!=0 )
		{
			_nActNum++;
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------
// class CSkillStateRecordSet
//---------------------------------------------------------------------------
CSkillStateRecordSet * CSkillStateRecordSet::_Instance = NULL;

BOOL CSkillStateRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CSkillStateRecord *pInfo = (CSkillStateRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 编号
	pInfo->chID = (char)pInfo->nID;
	// 名称
	strncpy(pInfo->szName, pInfo->szDataName, defSKILLSTATE_NAME_LEN);
	pInfo->szName[defSKILLSTATE_NAME_LEN - 1] = '\0';
	// 触发的时间间隔（单位：秒）
	pInfo->sFrequency = Str2Int(ParamList[m++]);
	// 状态到传递目标上的作用时间脚本
	strncpy(pInfo->szOnTransfer, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szOnTransfer[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// 使用效果脚本
	strncpy(pInfo->szAddState, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szAddState[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// 移除效果脚本
	strncpy(pInfo->szSubState, ParamList[m++].c_str(), defSKILLSTATE_SCRIPT_NAME);
	pInfo->szSubState[defSKILLSTATE_SCRIPT_NAME - 1] = '\0';
	// 替换策略
	pInfo->chAddType = Str2Int(ParamList[m++]);
	// 是否可由终端取消
	pInfo->bCanCancel = Str2Int(ParamList[m++]) != 0 ? true : false;
	// 是否能够移动
	pInfo->bCanMove = Str2Int(ParamList[m++]) ? true : false;
	// 是否能够使用魔法技能
	pInfo->bCanMSkill = Str2Int(ParamList[m++]) ? true : false;
	// 是否能够使用普通技能
	pInfo->bCanGSkill = Str2Int(ParamList[m++]) ? true : false;
	// 是否能够交易
	pInfo->bCanTrade = Str2Int(ParamList[m++]) ? true : false;
	// 是否能够使用物品
	pInfo->bCanItem = Str2Int(ParamList[m++]) ? true : false;
	// 是否无敌
	pInfo->bCanUnbeatable = Str2Int(ParamList[m++]) ? true : false;
	// 是否能够被使用物品
	pInfo->bCanItemmed = Str2Int(ParamList[m++]) ? true : false;
	// 是否能够被使用技能
	pInfo->bCanSkilled = Str2Int(ParamList[m++]) ? true : false;
	// 是否隐身
	pInfo->bNoHide = Str2Int(ParamList[m++]) ? true : false;
	// 是否被强制显形
	pInfo->bNoShow = Str2Int(ParamList[m++]) ? true : false;
	// 是否可操作道具
	pInfo->bOptItem = Str2Int(ParamList[m++]) ? true : false;
	// 是否可操作道具
	pInfo->bTalkToNPC = Str2Int(ParamList[m++]) ? true : false;
	// 解除的效果编号
	pInfo->bFreeStateID = Str2Int(ParamList[m++]);

	// 屏幕效果
	pInfo->chScreen = Str2Int(ParamList[m++]);
	// 出生表现
	memset( pInfo->nActBehave, 0, sizeof(pInfo->nActBehave) );
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defSKILLSTATE_ACT_NUM ? defSKILLSTATE_ACT_NUM : n;	
	for (int i = 0; i < n; i++)
	{		
		pInfo->nActBehave[i] = Str2Int(strList[i]);
	}

	//客户端表现标识
    pInfo->sChargeLink = Str2Int(ParamList[m++]);

    // 地面持续特效
    pInfo->sAreaEffect = Str2Int(ParamList[m++]);
	// 地面状态特效,是否仅显示中心点
    pInfo->IsShowCenter = Str2Int(ParamList[m++]) ? true : false;
	// 挂效果方POSE
	pInfo->IsDizzy = Str2Int(ParamList[m++]) ? true : false;
	// 挂效果方特效表现
	pInfo->sEffect = Str2Int(ParamList[m++]);
	// 挂特效玩家的播放dummy点
	pInfo->sDummy1 = Str2Int(ParamList[m++]);
	// 该状态下受击特效表现
	pInfo->sBitEffect = Str2Int(ParamList[m++]);
	// 特效播放dummy点
	pInfo->sDummy2 = Str2Int(ParamList[m++]);
	// 中该效果挂在身上的ICON表现
	pInfo->sIcon = Str2Int(ParamList[m++]);

	memset(pInfo->szIcon, 0, sizeof(pInfo->szIcon));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 10, ',');
	n = n > 10 ? 10 : n;
	for (int i = 0; i < n; i++)
	{
		strcpy(pInfo->szIcon[i], strList[i].c_str());
		pInfo->szIcon[defSKILLSTATE_NAME_LEN - 1][i] = _TEXT('\0');
	}

	strncpy(pInfo->szDesc, ParamList[m++].c_str(), defSKILLSTATE_DESC_NAME_LEN);
	pInfo->szDesc[defSKILLSTATE_DESC_NAME_LEN - 1] = '\0';

	pInfo->lColour = Str2Int(ParamList[m++]);
	
	return TRUE;
}

void CSkillStateRecordSet::_ProcessRawDataInfo(CRawDataInfo *pInfo)
{
	CSkillStateRecord *pState = (CSkillStateRecord*)pInfo;

    // 更新动态属性
	pState->RefreshPrivateData();	
}