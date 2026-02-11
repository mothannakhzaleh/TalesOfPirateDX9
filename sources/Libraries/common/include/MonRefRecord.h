//======================================================================================================================
// FileName: MonRefRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.05
// Comment: CMonRefRecordSet class
//======================================================================================================================

#ifndef	MONSTERREFRESH_H
#define	MONSTERREFRESH_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"
using	namespace	GAME;

const char cchMonRefRecordKeyValue = (char)0xff;

#define defMAX_REGION_MONSTER_TYPE	15	// 每个区域的最多怪物类型

class CMonRefRecord : public CRawDataInfo
{
public:
	//CMonRefRecord();

	long	lID;		// 区域编号
	Point	SRegion[2];	// 区域的左上角和右下角
	short	sAngle;		// 出生方向
	long	lMonster[defMAX_REGION_MONSTER_TYPE][4]; // ID，数量，刷新概率，刷新间隔
};

class CMonRefRecordSet : public CRawDataSet
{
public:

	static CMonRefRecordSet* I() { return _Instance; }

	CMonRefRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CMonRefRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CMonRefRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CMonRefRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CMonRefRecord);
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

inline CMonRefRecord* GetMonRefRecordInfo( int nTypeID )
{
	return (CMonRefRecord*)CMonRefRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif //MONSTERREFRESH_H