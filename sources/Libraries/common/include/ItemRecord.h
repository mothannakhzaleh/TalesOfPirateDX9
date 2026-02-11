//=============================================================================
// FileName: ItemRecord.h
// Creater: ZhangXuedong
// Date: 2004.09.01
// Comment: CItemRecordSet class
//=============================================================================

#ifndef	ITEMRECORD_H
#define ITEMRECORD_H

#include <tchar.h>
#include "util.h"
#include "TableData.h"
#include "CompCommand.h"
#include "JobType.h"

const char cchItemRecordKeyValue = (char)0xfe;	// 道具对职业的需求中占用了-1

#define defITEM_NAME_LEN	80
#define defITEM_MODULE_NUM	5
#define defITEM_MODULE_LEN	19
#define defITEM_ICON_NAME_LEN	17
#define defITEM_ATTREFFECT_NAME_LEN	33

#define defITEM_DESCRIPTOR_NAME_LEN      257
#define defITEM_DESCRIPTOR_SET_LEN      33

#define defITEM_BIND_EFFECT_NUM			8

#define defITEM_BODY 4

enum EItemType
{
	enumItemTypeSword		= 1,	// 剑
	enumItemTypeGlave		= 2,	// 巨剑
	enumItemTypeBow			= 3,	// 弓
	enumItemTypeHarquebus	= 4,	// 火绳枪
	enumItemTypeFalchion	= 5,	// 刀
	enumItemTypeMitten		= 6,	// 拳套
	enumItemTypeStylet		= 7,	// 匕首
	enumItemTypeMoneybag	= 8,	// 钱袋
	enumItemTypeCosh		= 9,	// 短棒
	enumItemTypeSinker		= 10,	// 锤子
	enumItemTypeShield		= 11,	// 盾
	enumItemTypeArrow		= 12,	// 箭
	enumItemTypeAmmo		= 13,	// 弹药
	enumItemTypeHeadpiece	= 19,	// 头盔
	enumItemTypeHair		= 20,	// 头发
	enumItemTypeFace		= 21,	// 脸型
	enumItemTypeClothing	= 22,	// 衣服
	enumItemTypeGlove		= 23,	// 手套
	enumItemTypeBoot		= 24,	// 靴子
	enumItemTypeNecklace	= 25,
	enumItemTypeRing		= 26,
	enumItemTypeTattoo		= 27,
	enumItemTypeConch		= 29,	// 贝壳
	enumItemTypeMedicine	= 31,	// 药品
	enumItemTypeOvum		= 32,	// 海卵
	enumItemTypeUnknownConsumable = 33, // TODO: Proper naming
	enumItemTypeScroll		= 36,	// 回城卷轴
	enumItemTypeGeneral		= 41,	// 一般物品
    enumItemTypeMission     = 42,   // 任务道具
	enumItemTypeBoat		= 43,	// 船长证明
	enumItemTypeWing		= 44,	// 翅膀，客户端用于显示特效用
	enumItemTypeTrade		= 45,	// 贸易证明
	enumItemTypeBravery		= 46,	// 勇者之证
	enumItemTypeHull		= 51,	// 船身
	enumItemTypeEmbolon		= 52,	// 撞角
	enumItemTypeEngine		= 53,	// 发动机
	enumItemTypeArtillery	= 54,	// 火炮
	enumItemTypeAirscrew	= 55,	// 螺旋浆
	enumItemTypeBoatSign	= 56,	// 船标志
	enumItemTypePetFodder	= 57,	// 宠物饲料
	enumItemTypePetSock		= 58,	// 能量零食
	enumItemTypePet			= 59,	// 守护精灵

	enumItemCloak			= 88,
	enumItemMount			= 90,

	// Add by lark.li 20080514 begin
	enumItemTypeNo			= 99,	// 彩票小球
	// End
};

enum EItemPickTo
{
	enumITEM_PICKTO_KITBAG,
	enumITEM_PICKTO_CABIN,
};

class CItemRecord : public CRawDataInfo
{
public:
	enum EItemExtendInfo
	{
		enumItemNormalStart		= 0,		// 普通道具
		enumItemNormalEnd		= 4999,
		enumItemFusionEndure   = 23000,      // 融合道具所要求的最大耐久度//Modify by ning.yan 20080821 （大澎要求融合道具的条件由道具起始、结束栏改为最大耐久度）
		enumItemFusionStart		= 5000,		// 熔合道具起始栏位
		enumItemFusionEnd		= 9800,		// 熔合道具结束栏位
		// Modify by lark.li 20080703
		enumItemMax				= 32768,		// 道具表最大记录数量
		// End

	};

	CItemRecord();

	long	lID;							// 编号
	_TCHAR	szName[defITEM_NAME_LEN];		// 名称
	char    szICON[defITEM_ICON_NAME_LEN];	// ICON
	_TCHAR	chModule[defITEM_MODULE_NUM][defITEM_MODULE_LEN];	// 模型
	short	sShipFlag;						// 船标志
	short	sShipType;						// 船体型号
	short	sType;							// 类型

	char	chForgeLv;						// 精炼等级
	char	chForgeSteady;					// 安定值（精炼物品的稳定值，超过这个值，每精炼一次就递减一定成功率）
	char	chExclusiveID;					// 是否拥有唯一ID
    char    chIsTrade;                      // 可交易
    char    chIsPick;                       // 可拾取
    char    chIsThrow;                      // 可丢弃
	char	chIsDel;						// 可销毁
    long    lPrice;                         // 交易价格
	char	chBody[defITEM_BODY];			// 体形限制
	short	sNeedLv;						// 要求的等级
    char    szWork[MAX_JOB_TYPE];           // 职业

	int		nPileMax;						// 在道具栏一格中可以放置的最多个数
	char	chInstance;						// 是否实例化
	char	szAbleLink[enumEQUIP_NUM];		// 可装备的位置
	char	szNeedLink[enumEQUIP_NUM];		// 需要的位置
	char	chPickTo;						// 0 拾取时进入背包，1 进入船舱

	short	sStrCoef;						// 力量系数加成
	short	sAgiCoef;						// 敏捷系数加成
	short	sDexCoef;						// 专注系数加成
	short	sConCoef;						// 体质系数加成
	short	sStaCoef;						// 精力系数加成
	short	sLukCoef;						// 幸运系数加成	
	short	sASpdCoef;						// 攻击频率系数加成
	short	sADisCoef;						// 攻击距离系数加成
	short	sMnAtkCoef;						// 最小攻击力系数加成
	short	sMxAtkCoef;						// 最大攻击力系数加成
	short	sDefCoef;						// 防御系数加成
	short	sMxHpCoef;						// 最大Hp系数加成
	short	sMxSpCoef;						// 最大Sp系数加成
	short	sFleeCoef;						// 闪避率系数加成
	short	sHitCoef;						// 命中率系数加成
	short	sCrtCoef;						// 爆击率系数加成
	short	sMfCoef;						// 寻宝率系数加成
	short	sHRecCoef;						// hp恢复速度系数加成
	short	sSRecCoef;						// sp恢复速度系数加成
	short	sMSpdCoef;						// 移动速度系数加成
	short	sColCoef;						// 资源采集速度系数加成

	short	sStrValu[2];					// 力量常数加成（最小值，最大值）
	short	sAgiValu[2];					// 敏捷常数加成
	short	sDexValu[2];					// 专注常数加成
	short	sConValu[2];					// 体质常数加成
	short	sStaValu[2];					// 精力常数加成
	short	sLukValu[2];					// 幸运常数加成
	short	sASpdValu[2];					// 攻击频率常数加成
	short	sADisValu[2];					// 攻击距离常数加成
	short	sMnAtkValu[2];					// 最小攻击力常数加成
	short	sMxAtkValu[2];					// 最大攻击力常数加成
	short	sDefValu[2];					// 防御常数加成
	short	sMxHpValu[2];					// 最大Hp常数加成
	short	sMxSpValu[2];					// 最大Sp常数加成
	short	sFleeValu[2];					// 闪避率常数加成
	short	sHitValu[2];					// 命中率常数加成
	short	sCrtValu[2];					// 爆击率常数加成
	short	sMfValu[2];						// 寻宝率常数加成
	short	sHRecValu[2];					// hp恢复速度常数加成
	short	sSRecValu[2];					// sp恢复速度常数加成
	short	sMSpdValu[2];					// 移动速度常数加成
	short	sColValu[2];					// 资源采集速度常数加成

	short	sPDef[2];						// 物理抵抗
	short	sLHandValu;						// 道具左手发挥
	short	sEndure[2];						// 耐久度
	short	sEnergy[2];						// 能量
	short	sHole;							// 洞数
	char	szAttrEffect[defITEM_ATTREFFECT_NAME_LEN];	// 使用效果
	short	sDrap;							// drap
	short	sEffect[defITEM_BIND_EFFECT_NUM][2];		// 武器捆绑特效以及对应dummy
	short	sItemEffect[2];					// 道具外观特效
	short	sAreaEffect[2];					// 放在地上时的特效
	short	sUseItemEffect[2];				// 使用时的特效
    _TCHAR  szDescriptor[defITEM_DESCRIPTOR_NAME_LEN];   // 描述

	float		nCooldown;
	//short	sSeteffect[enumEQUIP_NUM][2];
	//_TCHAR  szDescriptor[5][defITEM_DESCRIPTOR_SET_LEN];

public:
    const char* GetIconFile();              // 得到完整的icon文件名称
	static bool	IsVaildFusionID(CItemRecord* pItem); // add by ning.yan  2008-08-21
	
	bool		GetIsPile()							{ return nPileMax>1;	}

	// 检查角色体型是否可以装备
	bool		IsAllowEquip( unsigned int nChaID );

	// 检查是否没有体型限制
	bool		IsAllEquip()						{ return chBody[0]==-1;	}

	void		RefreshData();

	int			GetTypeValue( int nType );

	bool		IsFaceItem()						{ return sItemEffect[0]!=0;	}

	bool		IsSendUseItem()						{ return sUseItemEffect[0]!=0;	}

	bool IsWeapon() const;
	bool IsArmor() const;
	bool IsNecklace() const;
	bool IsRing() const;
	bool IsGlove() const;
	bool IsBoot() const;
	bool IsHair() const;
	bool IsConsumable() const;
	bool IsMission() const;
	bool IsLockable() const;

public:
	short		sEffNum;					// 武器捆绑特效数量

private:
	bool		_IsBody[5];

};

class CItemRecordSet : public CRawDataSet
{
public:

	static CItemRecordSet* I() { return _Instance; }

	CItemRecordSet(int nIDStart, int nIDCnt, int nCol = 128)
		:CRawDataSet(nIDStart, nIDCnt, nCol)
	{
		_Instance = this;
		_Init();
	}

protected:

	static CItemRecordSet* _Instance; // 相当于单键, 把自己记住

	virtual CRawDataInfo* _CreateRawDataArray(int nCnt)
	{
		return new CItemRecord[nCnt];
	}

	virtual void _DeleteRawDataArray()
	{
		delete[] (CItemRecord*)_RawDataArray;
	}

	virtual int _GetRawDataInfoSize()
	{
		return sizeof(CItemRecord);
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
	virtual void _ProcessRawDataInfo(CRawDataInfo *pInfo);
};

inline CItemRecord* GetItemRecordInfo( int nTypeID )
{
	return (CItemRecord*)CItemRecordSet::I()->GetRawDataInfo(nTypeID);
}

inline CItemRecord* GetItemRecordInfo(const char *pszItemName)
{
    return (CItemRecord*)CItemRecordSet::I()->GetRawDataInfo(pszItemName);
}

inline bool CItemRecord::IsAllowEquip( unsigned int nChaID )	
{ 
	if( nChaID<5 ) return _IsBody[nChaID];

	return false;
}

#endif //ITEMRECORD_H