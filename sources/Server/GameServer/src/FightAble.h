//=============================================================================
// FileName: FightAble.h
// Creater: ZhangXuedong
// Date: 2004.09.15
// Comment: CFightAble class
//=============================================================================

#ifndef FIGHTABLE_H
#define FIGHTABLE_H

#include "Attachable.h"
#include "ChaAttr.h"
#include "CharacterRecord.h"
#include "SkillRecord.h"
#include "GameCommon.h"
#include "SkillState.h"
#include "SkillBag.h"
#include "TryUtil.h"
#include "Timer.h"
#include "SkillTemp.h"

enum EItemInstance // 此处需要跟脚本统一
{
	enumITEM_INST_BUY	= 0,		// 商店买卖
	enumITEM_INST_MONS	= 1,		// 怪物掉落
	enumITEM_INST_COMP	= 2,		// 合成
	enumITEM_INST_TASK	= 3,		// 任务获得
};

enum EFightChaType
{
	enumFIGHT_CHA_SRC		= 0,	// 攻击者
	enumFIGHT_CHA_TAR		= 1,	// 受击者
	enumFIGHT_CHA_SPLASH	= 2,	// 溅射者
};

struct SFireUnit
{
#ifdef defPROTOCOL_HAVE_PACKETID
	dbc::uLong	ulPacketID;	// 包的ID
#endif
	dbc::uChar	uchFightID;

	CFightAble	*pCFightSrc;
	dbc::uLong	ulID;
	Point		SSrcPos;
	dbc::Long	lTarInfo1;
	dbc::Long	lTarInfo2;

	dbc::Short		sExecTime;	// 执行次数
	CSkillRecord	*pCSkillRecord;
	CSkillTempData	*pCSkillTData;
};

struct SFightInit
{
	CSkillRecord	*pCSkillRecord;
	SSkillGrid		*pSSkillGrid;
	CSkillTempData	*pCSkillTData;
	// lInfo1,lInfo2 若目标是实体,则分别是WorldID,Handle 否则分别是坐标的x,y
	struct
	{
		dbc::Char		chTarType;	// 0，无目标。1，目标是实体。2，目标是坐标
		dbc::Long		lTarInfo1;
		dbc::Long		lTarInfo2;
	};

	dbc::Short		sStopState;		// 技能停止后的状态（enumEXISTS_WAITING, enumEXISTS_SLEEPING）
};

/*
*	可战斗实体
*	lark.li
*/
class	CFightAble : public CAttachable
{
public:
	struct SFightProc
	{
		enum class Request
		{
			None,
			StopAttack,
			StartAttack
		};

		dbc::Short	sState;
		Request	sRequestState;

		bool		bCrt;			// 爆击
		bool		bMiss;			// Miss

		long		lERangeBParam[defSKILL_RANGE_BASEP_NUM];	// 区域基本参数（中心坐标，方向）
	};

	dbc::Short	GetFightState(void) {return m_SFightProc.sState;}
	dbc::Short	GetFightStopState(void) {return m_SFightInit.sStopState;}

	bool	DesireFightBegin(SFightInit *);
	void	DesireFightEnd(void) {EndFight();}
	void	OnFight(dbc::uLong ulCurTick);

	void	RangeEffect(SFireUnit *pSFireSrc, SubMap *pCMap, dbc::Long *plRangeBParam);
	void	SkillTarEffect(SFireUnit *pSFire);
	void	NotiSkillSrcToEyeshot(dbc::Short sExecTime = 0);
	void	NotiSkillSrcToSelf(dbc::Short sExecTime = 0);
	void	NotiSkillTarToEyeshot(SFireUnit *pSFireSrc);
	void	NotiChangeMainCha(dbc::uLong ulTargetID);
	void	SynAttr(dbc::Short sType);
	void	SynAttrToSelf(dbc::Short sType);
	void	SynAttrToEyeshot(dbc::Short sType);
	void	SynAttrToUnit(CFightAble *pCObj, dbc::Short sType);
	void	SynAttrToUnit(CFightAble *pCObj, dbc::Short sStartAttr, dbc::Short sEndAttr, dbc::Short sType);
	void	SynSkillStateToSelf(void);
	void	SynSkillStateToEyeshot(void);
	void	SynSkillStateToUnit(CFightAble *pCObj);
	void	SynLookEnergy(void);
	// 数据报组织
	void	WriteSkillState(WPACKET &pk);
	void	WriteAttr(WPACKET &pk, dbc::Short sSynType);
	void	WriteMonsAttr(WPACKET &pk, dbc::Short sSynType);
	void	WriteAttr(WPACKET &pk, dbc::Short sStartAttr, dbc::Short sEndAttr, dbc::Short sSynType);
	void	WriteLookEnergy(WPACKET &pk);

	bool	IsRightSkill(CSkillRecord *pSkill);
	bool	IsRightSkillSrc(dbc::Char chSkillEffType);
	bool	IsRightSkillTar(CFightAble *pSkillSrc, dbc::Char chSkillObjType, dbc::Char chSkillObjHabitat, dbc::Char chSkillEffType, bool bIncHider = false);
	bool	IsTeammate(CFightAble *pCFighter);
	bool	IsFriend(CFightAble *pCFighter);

	void	ResetFight();
	bool	RectifyAttr();

	dbc::Long	GetLevel(void) {return (long)m_CChaAttr.GetAttr(ATTR_LV);}
	void		AddExp(dbc::uLong);
	bool		AddExpAndNotic(dbc::Long lAddExp, dbc::Short sNotiType = enumATTRSYN_TASK);

	void	CountLevel(void);
	void	CountSailLevel(void);
	void	CountLifeLevel(void);

	// 任务事件处理接口
	virtual void AfterObjDie(CCharacter *pCAtk, CCharacter *pCDead) {}
	virtual void OnLevelUp( USHORT sLevel ) {};
	virtual void OnSailLvUp( USHORT sLevel ) {};
	virtual void OnLifeLvUp( USHORT sLevel ) {};

	// 采集资源，暴物品	
	void	SpawnResource( CCharacter *pCAtk, dbc::Long lSkillLv );
	void	ItemCount(CCharacter *pAtk);
	void	ItemInstance(dbc::Char chType, SItemGrid *pGridContent, BOOL isTradable = 1, LONG expiration = 0);
	bool	GetTrowItemPos(dbc::Long *plPosX, dbc::Long *plPosY);
	bool	SkillExpend(dbc::Short sExecTime = 1);

	dbc::uLong	GetSkillDist(Entity *pTarEnt, CSkillRecord *pRec)
	{
		if (!pRec) return 0;
		if (pTarEnt) return GetRadius() + pTarEnt->GetRadius() + pRec->sApplyDistance;
		else return GetRadius() + pRec->sApplyDistance;
	}
	bool	SkillTarIsEntity(CSkillRecord *pRec)
	{
		if (pRec && (pRec->chApplyType == 1 || pRec->chApplyType == 3)) return true;
		else return false;
	}

	void			BeUseSkill(dbc::Long lPreHp, dbc::Long lNowHp, CCharacter *pCSrcCha, dbc::Char chSkillEffType);
	void			SetMonsterFightObj(dbc::uLong ulObjWorldID, dbc::Long lObjHandle);
	dbc::Long		GetSkillTime(CSkillTempData *pCSkillTData);
	void			EnrichSkillBag(bool bActive = true);
	virtual bool	AddSkillState(dbc::uChar uchFightID, dbc::uLong ulSrcWorldID, dbc::Long lSrcHandle, dbc::Char chObjType, dbc::Char chObjHabitat, dbc::Char chEffType,
					dbc::uChar uchStateID, dbc::uChar uchStateLv, dbc::Long lOnTick, dbc::Char chType = enumSSTATE_ADD_UNDEFINED, bool bNotice = true){return false;}
	virtual bool	DelSkillState(dbc::uChar uchStateID, bool bNotice = true){return false;}
	void			SetItemHostObj(CFightAble *pCObj) {m_pCItemHostObj = pCObj;}

	dbc::Long		setAttr(int nIdx, LONG32 lValue, int nType = 0);
	dbc::Long		getAttr(int nIdx) {return m_CChaAttr.GetAttr(nIdx);}
	virtual void	AfterAttrChange(int nIdx, dbc::Long lOldVal, dbc::Long lNewVal) {};
	void			SetDie(CCharacter *pCSkillSrcCha);
	virtual void	Die(){};

	CCharacter* SkillPopBoat(dbc::Long lPosX, dbc::Long lPosY, dbc::Short sDir = -1);	// 放船
	bool SkillPopBoat(CCharacter *pCBoat, dbc::Long lPosX, dbc::Long lPosY, dbc::Short sDir = -1);	// 放船
	bool SkillInBoat(CCharacter* pCBoat);	// 上船
	bool SkillOutBoat(dbc::Long lPosX, dbc::Long lPosY, dbc::Short sDir = -1);	// 下船
	bool SkillPushBoat(CCharacter* pCBoat, bool bFree = true);	// 收船

	dbc::uLong	m_ulPacketID;		// 包的ID
	dbc::uChar	m_uchFightID;		// 攻击的编号，只是为了客户端匹配的用途

	SFightInit	m_SFightInit;
	SFightProc	m_SFightProc;
	SFightInit	m_SFightInitCache;

	CChaAttr		m_CChaAttr;
	CChaRecord		*m_pCChaRecord;
	CSkillState		m_CSkillState;
	CSkillBag		m_CSkillBag;
	dbc::Short		m_sDefSkillNo;

	//virtual bool IsBoat(void);

protected:
	CFightAble();
	void	Initially();
	void	Finally();

	CFightAble	*	IsFightAble(){return this;}
	void	WritePK(WPACKET& wpk);			//写入玩家本身及其所有附加结构(如召唤兽等)的所有数据
	void	ReadPK(RPACKET& rpk);			//重构玩家本身及其所有附加结构(如召唤兽等)

	bool	GetFightTargetShape(Square *pSTarShape);

	
	void	OnSkillState(DWORD dwCurTick);
	void	RemoveOtherSkillState();
	void	RemoveAllSkillState();

private:
	virtual void BeginFight();
	virtual void EndFight();
	void OnFightBegin(void) {m_bOnFight = true;}
	void OnFightEnd(void) {m_bOnFight = false;}

	virtual void SubsequenceFight(){};

	virtual void BreakAction(RPACKET pk = NULL) {};
	virtual void EndAction(RPACKET pk = NULL) {}

	bool SkillGeneral(dbc::Long lDistance, dbc::Short sExecTime = 1); // 普通技能

	dbc::uShort	m_usTickInterval;	// 战斗执行的心跳（频率），单位（毫秒）
	dbc::uLong	m_ulLastTick;		// 单位（毫秒）
	bool		m_bOnFight;

	bool		m_bLookAttrChange;	// 外观属性改变
	CFightAble*	m_pCItemHostObj;	// 掉料的属主

};

class	CTimeSkillMgr
{
public:
	struct SMgrUnit
	{
		SFireUnit	SFireSrc;
		dbc::uLong	ulLeftTick;	// 剩余时间
		SubMap		*pCMap;
		Point		STargetPos;	// 目标位置
		long		lERangeBParam[defSKILL_RANGE_BASEP_NUM];	// 区域基本参数（中心坐标，方向）
		SMgrUnit	*pSNext;
	};

	CTimeSkillMgr(unsigned short usFreq = 1000);
	~CTimeSkillMgr();

	void	Add(SFireUnit *pSFireSrc, dbc::uLong ulLeftTick, SubMap *pCMap, Point *pStarget, dbc::Long *lRangeBParam);
	void	Run(unsigned long ulCurTick);
	void	ExecTimeSkill(SMgrUnit *pFireInfo);

private:
	unsigned long	m_ulTick;
	unsigned short	m_usFreq;	// 执行心跳（频率）

	SMgrUnit	*m_pSExecQueue;	// 执行队列
	SMgrUnit	*m_pSFreeQueue;	// 空闲队列

};

extern CTimeSkillMgr	g_CTimeSkillMgr;
extern char	g_chItemFall[defCHA_INIT_ITEM_NUM + 1];

#endif // FIGHTABLE_H