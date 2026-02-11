//=============================================================================
// FileName: LifeLvRecord.h
// Creater: ZhangXuedong
// Date: 2005.05.28
// Comment: CLifeLvRecord class
//=============================================================================

#ifndef LIFELVRECORD_H
#define LIFELVRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

const char cchLifeLvRecordKeyValue = (char)0xff;

class CLifeLvRecord : public CRawDataInfo
{
public:
	//CLifeLvRecord();

	long	lID;			// 编号
	short	sLevel;			// 等级
	unsigned long	ulExp;	// 经验值
};

class CLifeLvRecordSet : public CRawDataSet
{
public:

	static CLifeLvRecordSet* I() { return _Instance; }

	CLifeLvRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CLifeLvRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CLifeLvRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CLifeLvRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CLifeLvRecord);
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

inline CLifeLvRecord* GetLifeLvRecordInfo( int nTypeID )
{
	return (CLifeLvRecord*)CLifeLvRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // LIFELVRECORD_H