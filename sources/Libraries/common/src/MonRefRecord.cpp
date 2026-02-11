//======================================================================================================================
// FileName: MonRefRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.09.05
// Comment: CMonRefRecordSet class
//======================================================================================================================

#include "MonRefRecord.h"
using namespace std;

CMonRefRecordSet * CMonRefRecordSet::_Instance = NULL;

BOOL CMonRefRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CMonRefRecord *pInfo = (CMonRefRecord*)pRawDataInfo;

	// 区域编号
	pInfo->lID = pInfo->nID;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 区域的左上角，右下角
	memset(pInfo->SRegion, cchMonRefRecordKeyValue, sizeof(pInfo->SRegion));
	strLine = pInfo->szDataName;
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	pInfo->SRegion[0].x = Str2Int(strList[0]);
	pInfo->SRegion[0].y = Str2Int(strList[1]);

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	pInfo->SRegion[1].x = Str2Int(strList[0]);
	pInfo->SRegion[1].y = Str2Int(strList[1]);

	// 出生方向
	pInfo->sAngle = Str2Int(ParamList[m++]);

	// ID，数量，刷新概率，刷新间隔
	memset(pInfo->lMonster, cchMonRefRecordKeyValue, sizeof(pInfo->lMonster));
	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defMAX_REGION_MONSTER_TYPE ? defMAX_REGION_MONSTER_TYPE : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lMonster[i][0] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defMAX_REGION_MONSTER_TYPE ? defMAX_REGION_MONSTER_TYPE : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lMonster[i][1] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defMAX_REGION_MONSTER_TYPE ? defMAX_REGION_MONSTER_TYPE : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lMonster[i][2] = Str2Int(strList[i]);
	}

	strLine = ParamList[m++];
	n = Util_ResolveTextLine(strLine.c_str(), strList, 80, ',');
	n = n > defMAX_REGION_MONSTER_TYPE ? defMAX_REGION_MONSTER_TYPE : n;
	for (int i = 0; i < n; i++)
	{
		pInfo->lMonster[i][3] = Str2Int(strList[i]);
	}

	return TRUE;
}
