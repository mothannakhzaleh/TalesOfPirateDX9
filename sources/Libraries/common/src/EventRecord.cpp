//=============================================================================
// FileName: EventRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.11.24
// Comment:
//=============================================================================

#include "EventRecord.h"
using namespace std;

CEventRecordSet * CEventRecordSet::_Instance = NULL;

BOOL CEventRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CEventRecord *pInfo = (CEventRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 编号
	pInfo->lID = pInfo->nID;
	// 名称
	_tcsncpy(pInfo->szName, pInfo->szDataName, defEVENT_NAME_LEN);
	pInfo->szName[defEVENT_NAME_LEN - 1] = _TEXT('0');

    // 事件类型
    pInfo->sEventType = Str2Int(ParamList[m++]);
	// 触发类型
	pInfo->sArouseType = Str2Int(ParamList[m++]);
	// 触发半径
	pInfo->sArouseRadius = Str2Int(ParamList[m++]);
	// 触发特效
	pInfo->sEffect = Str2Int(ParamList[m++]);
	// 触发音效
	pInfo->sMusic = Str2Int(ParamList[m++]);
	// 出生时的特效
	pInfo->sBornEffect = Str2Int(ParamList[m++]);	
	// 事件显示的光标
	pInfo->sCursor = Str2Int(ParamList[m++]);		
	// 鼠标指针什么情况有效(0-都有效,1-人，2-船)
	pInfo->chMainChaType = Str2Int(ParamList[m++]);		
	return TRUE;
}
