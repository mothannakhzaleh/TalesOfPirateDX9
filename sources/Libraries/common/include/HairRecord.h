//======================================================================================================================
// FileName: HairRecord.h
// Creater: Jerry li
// Date: 2005.08.29
// Comment: CHairRecordSet class
//======================================================================================================================

#ifndef	HAIRRECORD_H
#define	HAIRRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"

#define defHAIR_MAX_ITEM		4
#define defHAIR_MAX_FAIL_ITEM	3

class CHairRecord : public CRawDataInfo
{
public:
	CHairRecord();
	char	szColor[10];

	DWORD	dwNeedItem[defHAIR_MAX_ITEM][2];		// 需要的道具ID,以及所需要的数量
	DWORD	dwMoney;								// 需要的金钱
	DWORD	dwItemID;								// 对应的(发型部位)道具ID
	DWORD	dwFailItemID[defHAIR_MAX_FAIL_ITEM];	// 失败时的发型
	bool	IsChaUse[4];							// 4个主角是否可用
	
	int		GetFailItemNum()		{ return _nFailNum;		}
	void	RefreshPrivateData();

private:
	int		_nFailNum;
	
};

class CHairRecordSet : public CRawDataSet
{
public:
	static CHairRecordSet* I() { return _Instance; }

	CHairRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:
	static CHairRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CHairRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CHairRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CHairRecord);
	}

	virtual void*	_CreateNewRawData(CRawDataInfo *pInfo)
	{
		return NULL;
	}

	virtual void  _DeleteRawData(CRawDataInfo *pInfo)
	{
		SAFE_DELETE(pInfo->pData);
	}
    virtual void  _AfterLoad();

	virtual BOOL _ReadRawDataInfo(CRawDataInfo *pRawDataInfo, std::vector<std::string> &ParamList);
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);

};

inline CHairRecord* GetHairRecordInfo( int nTypeID )
{
	return (CHairRecord*)CHairRecordSet::I()->GetRawDataInfo(nTypeID);
}

#endif //HAIRRECORD_H