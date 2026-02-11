// NpcRecord.h Created by knight-gongjian 2004.11.24.
//---------------------------------------------------------
#pragma once

#ifndef _NPCRECORD_H_
#define _NPCRECORD_H_

#include <tchar.h>
#include "util.h"
#include "TableData.h"

//---------------------------------------------------------

#define NPC_MAXSIZE_NAME			128 // npc名称长度
#define NPC_MAXSIZE_MSGPROC			16	// npc消息处理函数名称长度

class CNpcRecord : public CRawDataInfo
{
public:	
	char szName[NPC_MAXSIZE_NAME];		// 地图中显示npc名称
	USHORT sNpcType;					// npc类型
	USHORT sCharID;						// 角色列表编号索引
	BYTE byShowType;					// 客户端显示颜色类型
	DWORD dwxPos0, dwyPos0;				// npc位置信息
	DWORD dwxPos1, dwyPos1;
	USHORT sDir;
	USHORT sParam1, sParam2;
	char szNpc[NPC_MAXSIZE_NAME];		// npc文件名
	char szMsgProc[NPC_MAXSIZE_MSGPROC];// npc消息处理函数
	char szMisProc[NPC_MAXSIZE_MSGPROC];// npc任务状态处理函数
};

class CNpcRecordSet : public CRawDataSet
{
public:
	CNpcRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
	: CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Init();
	}

protected:

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CNpcRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CNpcRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CNpcRecord);
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
//---------------------------------------------------------

#endif // _NPCRECORD_H_