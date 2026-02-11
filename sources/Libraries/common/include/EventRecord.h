//=============================================================================
// FileName: EventRecord.h
// Creater: ZhangXuedong
// Date: 2004.11.24
// Comment:
//=============================================================================

#ifndef EVENTRECORD_H
#define EVENTRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "point.h"

// 事件触发类型
enum EEventTouchType
{
	enumEVENTT_RANGE	= 1,		// 按半径触发
};

enum EEventExecType
{
	enumEVENTE_SMAP_ENTRY	= 1,	// 地图静态切换点
	enumEVENTE_DMAP_ENTRY	= 2,	// 地图动态切换点
};

enum EEventType
{
	enumEVENT_TYPE_ACTION = 1,	// 
	enumEVENT_TYPE_ENTITY,
};

// 触发类型,决定客户端怎样触发
enum EEventArouseType
{
	enumEVENT_AROUSE_DISTANCE,		// 按半径触发
	enumEVENT_AROUSE_CLICK,			// 点击触发
};

const char cchEventRecordKeyValue = (char)0xff;

#define defEVENT_NAME_LEN	18

class CEventRecord : public CRawDataInfo
{
public:
	//CEventRecord();

	long	lID;						// 编号
	char	szName[defEVENT_NAME_LEN];	// 名称
    short   sEventType;                 // 事件类型,用于区分发送协议方式,参看EEventType
	short	sArouseType;				// 触发类型
	short	sArouseRadius;				// 触发半径
	short	sEffect;					// 触发特效
	short	sMusic;						// 触发音效
	short   sBornEffect;				// 出生时的特效
	short	sCursor;					// 事件显示的光标
	char	chMainChaType;				// 事件点击时什么情况有效(0-都有效,1-人，2-船)

	bool	IsValid( int MainChaType )	{ return chMainChaType==0 || MainChaType==chMainChaType;	}
};

class CEventRecordSet : public CRawDataSet
{
public:

	static CEventRecordSet* I() { return _Instance; }

	CEventRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CEventRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CEventRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CEventRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CEventRecord);
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

inline CEventRecord* GetEventRecordInfo( int nTypeID )
{
	return (CEventRecord*)CEventRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif // EVENTRECORD_H