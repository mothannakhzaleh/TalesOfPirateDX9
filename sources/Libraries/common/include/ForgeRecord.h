// ForgeRecord.h Created by knight-gongjian 2005.1.24.
//---------------------------------------------------------
#pragma once

#ifndef _FORGERECORD_H_
#define _FORGERECORD_H_

#include <tchar.h>
#include "util.h"
#include "TableData.h"

//---------------------------------------------------------
#define FORGE_MAXNUM_ITEM				6 // 精练需求最大物品数

class CForgeRecord : public CRawDataInfo
{
public:
	BYTE byLevel;	// 精练等级
	BYTE byFailure; // 失败后物品退回等级
	BYTE byRate;	// 精练武器的成功率	
	BYTE byParam;	// 保留参数
	DWORD dwMoney;  // 精练需求金钱

	// 精练物品需求数据
	struct FORGE_ITEM
	{
		USHORT sItem;	// 需求物品ID
		BYTE   byNum;	// 需求物品的数量
		BYTE   byParam; // 保留参数
	};	
	FORGE_ITEM ForgeItem[FORGE_MAXNUM_ITEM];
};

class CForgeRecordSet : public CRawDataSet
{
public:

	static CForgeRecordSet* I() { return _Instance; }

	CForgeRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CForgeRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CForgeRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CForgeRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CForgeRecord);
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

};


inline CForgeRecord* GetForgeRecordInfo(const char *pszName)
{
    return (CForgeRecord*)CForgeRecordSet::I()->GetRawDataInfo(pszName);
}

inline CForgeRecord* GetForgeRecordInfo(int nIndex)
{
    return (CForgeRecord*)CForgeRecordSet::I()->GetRawDataInfo(nIndex);
}
//---------------------------------------------------------
#endif // _FORGERECORD_H_