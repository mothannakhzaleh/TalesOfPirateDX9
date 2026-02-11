//=============================================================================
// FileName: LevelRecord.cpp
// Creater: ZhangXuedong
// Date: 2004.12.10
// Comment: CLevelRecord class
//=============================================================================

#include "LevelRecord.h"
using namespace std;

CLevelRecordSet * CLevelRecordSet::_Instance = NULL;

BOOL CLevelRecordSet::_ReadRawDataInfo(CRawDataInfo *pRawDataInfo, vector<string> &ParamList)
{   
	if(ParamList.size()==0) return FALSE;

	CLevelRecord *pInfo = (CLevelRecord*)pRawDataInfo;

	int m = 0, n = 0;
    string strList[80];
	string strLine;

	// 编号
	pInfo->lID = pInfo->nID;
	// 等级
	pInfo->sLevel = Str2Int(pInfo->szDataName);
	// 经验值
	pInfo->ulExp = _atoi64(ParamList[m++].c_str()); //Str2Int(ParamList[m++]);

	return TRUE;
}
