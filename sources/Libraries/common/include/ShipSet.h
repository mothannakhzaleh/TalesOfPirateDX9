//
#pragma once

#include <tchar.h>
#include "util.h"
#include "TableData.h"

// 船体更换命令按钮值
enum ID_BTN_BOAT
{
    ID_BTN_HEADLEFT =      0,
    ID_BTN_HEADRIGHT,
    ID_BTN_POWERLEFT,
    ID_BTN_POWERRIGHT,
    ID_BTN_WEAPONLEFT,
    ID_BTN_WEAPONRIGHT,

    ID_BTN_ITEM_NUM,
};
#define BOAT_MAXSIZE_MINNAME	2   // 船只名称最少字符长度
#define BOAT_MAXSIZE_BOATNAME	17	// 船只的名称最大长度
#define BOAT_MAXSIZE_NAME		64	// 船只部件名称最大长度
#define BOAT_MAXSIZE_DESP		128 // 船只部件信息最大描述字节数
#define BOAT_MAXSIZE_PART		3   // 船只最大部件数量
#define BOAT_MAXNUM_PARTITEM	16  // 船只每个可换部件最大的选择数量
#define BOAT_MAXNUM_MOTOR		4	// 每个引擎最大可以有最大马达数量
#define BOAT_MAXNUM_MODEL		8	// 每个船只最大模型数量
#define MAX_CHAR_BOAT			3	// 每个角色最大可以拥有的船只数量

// 船只选择列表数据
struct BOAT_BERTH_DATA
{
	BYTE byID[MAX_CHAR_BOAT];
	BYTE byState[MAX_CHAR_BOAT];
	char szName[MAX_CHAR_BOAT][BOAT_MAXSIZE_BOATNAME + BOAT_MAXSIZE_DESP];
};

enum BOAT_STATE
{
	BS_GOOD		= 0,				// 状态良好
	BS_NOSP		= 1,				// 需要补充SP
	BS_NOHP		= 2,				// 需要修理
	BS_DEAD		= 3,				// 船只沉没
};

// 角色建造、设置拥有的船只
struct BOAT_DATA
{
	char szName[BOAT_MAXSIZE_BOATNAME];		// 船只名称
	USHORT sBoat;				// 船只信息ID
	USHORT sHeader;				// 船头
	USHORT sBody;				// 船身
	USHORT sEngine;				// 船只马达
	USHORT sCannon;				// 船只火炮
	USHORT sEquipment;			// 船只装备
	USHORT sBerth;				// 船只停泊港口
	USHORT sCapacity;			// 货舱容量
	DWORD  dwOwnerID;			// 船只拥有者ID
};

// 角色装载船只属性
struct BOAT_LOAD_INFO
{
	DWORD  dwID;				// 船只数据库ID
	char   szName[BOAT_MAXSIZE_BOATNAME];			// 船只名称
	USHORT sHeader;				// 船头
	USHORT sBody;				// 船身
	USHORT sEngine;				// 船只马达
	USHORT sCannon;				// 船只火炮
	USHORT sEquipment;			// 船只装备
	USHORT sBerth;				// 船只停泊港口
};

struct xShipBuildInfo
{
	char szName[BOAT_MAXSIZE_NAME];			// 船名
	char szDesp[BOAT_MAXSIZE_DESP];			// 描述（道具等级）
	char szBerth[BOAT_MAXSIZE_NAME];		// 船只停泊港口名称
	char szHeader[BOAT_MAXSIZE_NAME];		// 船头名称
	char szBody[BOAT_MAXSIZE_NAME];			// 船身名称
	char szEngine[BOAT_MAXSIZE_NAME];		// 船马达名称
	char szCannon[BOAT_MAXSIZE_NAME];		// 船装载火炮名称
	char szEquipment[BOAT_MAXSIZE_NAME];	// 船配件名称

	// 船只组成模型ID	
	USHORT sPosID;				// 模型动作信息ID
	union
	{
		struct
		{
			DWORD dwBody;		// 船身		
			DWORD dwHeader;		// 船头	
			DWORD dwEngine;		// 船只动力
			DWORD dwFlag;		// 船标志
			DWORD dwMotor[BOAT_MAXNUM_MOTOR];
		};

		DWORD dwBuf[BOAT_MAXNUM_MODEL];
	};

	BYTE  byHeader;	   // 船头的选择索引记录
	BYTE  byEngine;	   // 船动力的选择索引记录
	BYTE  byCannon;	   // 船火炮的选择索引记录
	BYTE  byEquipment; // 船配件的选择索引记录
	BYTE  byIsUpdate;  // 是否可以更新船只部件
	DWORD dwMoney;	   // 价值
    DWORD dwMinAttack; // 最小攻击
    DWORD dwMaxAttack; // 最大攻击
    DWORD dwCurEndure; // 当前耐久
    DWORD dwMaxEndure; // 最大耐久
    DWORD dwSpeed;	   // 移动速度
    DWORD dwDistance;  // 最大攻击距离
    DWORD dwDefence;   // 防御
	DWORD dwCurSupply; // 当前补给
	DWORD dwMaxSupply; // 最大补给
	DWORD dwConsume;   // 船只补给消耗
    DWORD dwAttackTime;// 攻击间隔时间
	USHORT	sCapacity; // 货舱容量
};

struct xShipAttrInfo
{
	// 船只建造基本属性
	DWORD dwMoney;	   // 价值
    DWORD dwMinAttack; // 最小攻击
    DWORD dwMaxAttack; // 最大攻击
    DWORD dwCurEndure; // 当前耐久
    DWORD dwMaxEndure; // 最大耐久
    DWORD dwSpeed;	   // 移动速度
    DWORD dwDistance;  // 最大攻击距离
    DWORD dwDefence;   // 防御
	DWORD dwCurSupply; // 当前补给
	DWORD dwMaxSupply; // 最大补给
    DWORD dwAttackTime;// 攻击间隔时间
	USHORT	sCapacity; // 货舱容量

	// 其他属性	
	DWORD dwResume;			// 船耐久恢复
	DWORD dwResist;	        // 船只抗力
	DWORD dwScope;			// 爆炸范围
	DWORD dwConsume;		// 船只补给消耗
	DWORD dwCannonSpeed;	// 炮弹飞行速度

	// 船只属性系数
};

struct xShipInfo: public CRawDataInfo
{
	char szName[BOAT_MAXSIZE_NAME];	// 船只名称
	char szDesp[BOAT_MAXSIZE_DESP];	// 描述（道具等级）
	USHORT sItemID;					// 船长证明道具ID
	USHORT sCharID;					// 船只角色属性ID
	USHORT sPosID;					// 模型动作信息ID
	BYTE byIsUpdate;				// 是否可以更换船各部件	
	USHORT sNumHeader;
	USHORT sNumEngine;
	USHORT sNumCannon;
	USHORT sNumEquipment;
	USHORT sHeader[BOAT_MAXNUM_PARTITEM];		// 船头	
	USHORT sEngine[BOAT_MAXNUM_PARTITEM];		// 动力
	USHORT sCannon[BOAT_MAXNUM_PARTITEM];		// 火炮
	USHORT sEquipment[BOAT_MAXNUM_PARTITEM];	// 配件
	USHORT sBody;			// 船身	
	USHORT sLvLimit;		// 等级限制
	USHORT sNumPfLimit;
	USHORT sPfLimit[BOAT_MAXNUM_PARTITEM];		// 职业限制

	USHORT sEndure;			// 船只耐久
	USHORT sResume;			// 船耐久恢复
	USHORT sDefence;		// 船只防御力
	USHORT sResist;			// 船只抗力
    USHORT sMinAttack;		// 最小攻击
    USHORT sMaxAttack;		// 最大攻击
	USHORT sDistance;		// 攻击距离
	USHORT sTime;			// 添弹时间
	USHORT sScope;			// 爆炸范围
	USHORT sCapacity;		// 货舱容量
	USHORT sSupply;			// 船只补给
	USHORT sConsume;		// 船只补给消耗
	USHORT sCannonSpeed;	// 炮弹飞行速度
	USHORT sSpeed;			// 船移动速度
	USHORT sParam;			// 备注

	xShipInfo()
	{
		memset( szName, 0, BOAT_MAXSIZE_NAME );
		memset( szDesp, 0, BOAT_MAXSIZE_DESP );
		sItemID = 0;
		sCharID = 0;
		sPosID = 0;
		byIsUpdate = 0;
		sNumHeader = 0;
		sNumEngine = 0;
		sNumCannon = 0;
		sNumEquipment = 0;
		memset( sHeader, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sEngine, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sCannon, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		memset( sEquipment, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );
		sBody = 0;
		sLvLimit = 0;		// 等级限制
		sNumPfLimit = 0;
		memset( sPfLimit, 0, sizeof(USHORT)*BOAT_MAXNUM_PARTITEM );	


		sEndure = 0;
		sResume = 0;
		sDefence = 0;
		sResist = 0;
		sMinAttack = 0;
		sMaxAttack = 0;
		sDistance = 0;
		sTime = 0;
		sScope = 0;
		sCapacity = 0;
		sSupply = 0;			
		sConsume = 0;		
		sCannonSpeed = 0;	
		sSpeed = 0;
		sParam = 0;
	}
};

struct xShipPartInfo: public CRawDataInfo
{
	char szName[BOAT_MAXSIZE_NAME];	// 船只部件名称
	char szDesp[BOAT_MAXSIZE_DESP];	// 描述（道具等级）
    DWORD dwModel;			// 部件模型ID
	USHORT sMotor[BOAT_MAXNUM_MOTOR];	// 船只引擎马达
	DWORD  dwPrice;			// 价格

	USHORT sEndure;			// 船只耐久
	USHORT sResume;			// 船耐久恢复
	USHORT sDefence;		// 船只防御力
	USHORT sResist;			// 船只抗力
    USHORT sMinAttack;		// 最小攻击
    USHORT sMaxAttack;		// 最大攻击
	USHORT sDistance;		// 攻击距离
	USHORT sTime;			// 添弹时间
	USHORT sScope;			// 爆炸范围
	USHORT sCapacity;		// 货舱容量
	USHORT sSupply;			// 船只补给
	USHORT sConsume;		// 船只补给消耗
	USHORT sCannonSpeed;	// 炮弹飞行速度
	USHORT sSpeed;			// 船移动速度
	USHORT sParam;			// 备注
	
	xShipPartInfo()
	{
		memset( szName, 0, BOAT_MAXSIZE_NAME );
		memset( szDesp, 0, BOAT_MAXSIZE_DESP );
		dwModel = 0;
		memset( sMotor, 0, sizeof(USHORT)*BOAT_MAXNUM_MOTOR );
		dwPrice = 0;
		sEndure = 0;
		sResume = 0;
		sDefence = 0;
		sResist = 0;
		sMinAttack = 0;
		sMaxAttack = 0;
		sDistance = 0;
		sTime = 0;
		sScope = 0;
		sCapacity = 0;
		sSupply = 0;			
		sConsume = 0;		
		sCannonSpeed = 0;	
		sSpeed = 0;
		sParam = 0;
	}
};

// 船只建造属性表
class xShipSet : public CRawDataSet
{
public:
	xShipSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}
	
	static xShipSet* I() { return _Instance; }

protected:

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return (CRawDataInfo*)new xShipInfo[nCnt];
	}
	
	static xShipSet* _Instance; // 相当于单键, 把自己记住

	virtual void _DeleteRawDataArray()
	{
		delete[] (xShipInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(xShipInfo);
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

// 船只部件基本属性表
class xShipPartSet : public CRawDataSet
{
public:
	
	static xShipPartSet* I() { return _Instance; }

	
	xShipPartSet(int nIDStart, int nIDCnt)
		:CRawDataSet(nIDStart, nIDCnt)
	{
		_Instance = this;
		_Init();
	}

protected:

	static xShipPartSet* _Instance;
	
	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return (CRawDataInfo*)new xShipPartInfo[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (xShipPartInfo*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(xShipPartInfo);
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

inline xShipInfo* GetShipInfo( int nTypeID )
{
	return (xShipInfo*)xShipSet::I()->GetRawDataInfo(nTypeID);
}

inline xShipPartInfo* GetShipPartInfo( int nTypeID )
{
	return (xShipPartInfo*)xShipPartSet::I()->GetRawDataInfo(nTypeID);
}
