//=============================================================================
// FileName: SwitchMapRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.23
// Comment:
//=============================================================================

#ifndef SWITCHMAPRECORD_H
#define SWITCHMAPRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"

const char cchSwitchMapRecordKeyValue = (char)0xff;

#define defMAP_NAME_LEN		16

class CSwitchMapRecord : public CRawDataInfo
{
public:
	//CSwitchMapRecord();

	long	lID;							// 编号
	long	lEntityID;						// 被绑定的实体ID
	long	lEventID;						// 事件编号
	Point	SEntityPos;						// 实体的位置（厘米）
	short	sAngle;							// 实体的方向
	_TCHAR	szTarMapName[defMAP_NAME_LEN];	// 目标地图名
	Point	STarPos;						// 目标地图的位置（厘米）
};

class CSwitchMapRecordSet : public CRawDataSet
{
public:

	static CSwitchMapRecordSet* I() { return _Instance; }

	CSwitchMapRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CSwitchMapRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CSwitchMapRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CSwitchMapRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CSwitchMapRecord);
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

inline CSwitchMapRecord* GetSwitchMapRecordInfo( int nTypeID )
{
	return (CSwitchMapRecord*)CSwitchMapRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif