//=============================================================================
// FileName: MgrUnit.cpp
// Creater: ZhangXuedong
// Date: 2005.03.04
// Comment: Map Manage Unit
//=============================================================================

#include "MgrUnitEx.h"
#include "SubMap.h"

//#define defMGRUNIT_LOG

char	szEntiList[2048];

// 实体指针，实体类型（角色，物品），是否在范围内
CEntityListNode* CMgrUnit::AddEntity(Entity *pCEnt, char chType)
{
#ifdef defMGRUNIT_LOG
	if (chType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
		LG("管理单元实体", "[%d, %d]开始增加实体 %s，当前实体数 %d。\n", m_sPosX, m_sPosY, pCEnt->m_CLog.GetLogName(), m_lEntityNum);
#endif
	bool	bAddState = false;
	CEntityListNode	*pNode = g_pGameApp->m_EntityListHeap.Get();
	pNode->m_chEntiType = chType;
	pNode->m_pCEntity = pCEnt;
	if (chType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	{
		bAddState = true;
		if (pNode->m_pCNext = m_pCChaIn)
			m_pCChaIn->m_pCLast = pNode;
		m_pCChaIn = pNode;

		pNode->m_pCEntMgrNode = pCEnt->EnterMgrUnit(this, pNode, true); // 实体记录所在的管理单元
	}
	else if (chType == def_MGRUNIT_ENTITY_TYPE_CHACROSS)
	{
		bAddState = true;
		if (pNode->m_pCNext = m_pCChaCross)
			m_pCChaCross->m_pCLast = pNode;
		m_pCChaCross = pNode;

		pNode->m_pCEntMgrNode = pCEnt->EnterMgrUnit(this, pNode); // 实体记录所在的管理单元
	}
	else if (chType == def_MGRUNIT_ENTITY_TYPE_ITEMIN)
	{
		if (pNode->m_pCNext = m_pCItemIn)
			m_pCItemIn->m_pCLast = pNode;
		m_pCItemIn = pNode;

		pNode->m_pCEntMgrNode = pCEnt->EnterMgrUnit(this, pNode, true); // 实体记录所在的管理单元
	}
	else if (chType == def_MGRUNIT_ENTITY_TYPE_ITEMCROSS)
	{
		if (pNode->m_pCNext = m_pCItemCross)
			m_pCItemCross->m_pCLast = pNode;
		m_pCItemCross = pNode;

		pNode->m_pCEntMgrNode = pCEnt->EnterMgrUnit(this, pNode); // 实体记录所在的管理单元
	}
	else
	{
		pNode->Free();
		return 0;
	}
	m_lEntityNum++;

	if (chType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	{
#ifdef defMGRUNIT_LOG
		LG("管理单元实体", "实体增加完成，当前实体数 %d\n", m_lEntityNum);
#endif
		long	lCount = 0;
		szEntiList[0] = '\0';
		CEntityListNode	*pNode;
		pNode = m_pCChaIn;
		while (pNode)
		{
			strcat(szEntiList, pNode->m_pCEntity->m_CLog.GetLogName());
			strcat(szEntiList, ", ");
			pNode = pNode->m_pCNext;
			lCount++;
			if (lCount > m_lEntityNum)
			{
#ifdef defMGRUNIT_LOG
				//LG("管理单元实体", "msg实体链错误。\n");
#endif
				break;
			}
		}
#ifdef defMGRUNIT_LOG
		LG("管理单元实体", "实体链：%s。\n", szEntiList);
#endif
	}

	// 向角色增加状态
	if (bAddState)
	{
		CCharacter	*pCCha = pCEnt->IsCharacter();
		if (m_CSkillState.m_chStateNum > 0)
		{
			pCCha->m_CChaAttr.ResetChangeFlag();
			pCCha->m_CSkillState.ResetChangeFlag();
			for (char j = 0; j < m_CSkillState.m_chStateNum; j++)
				AddStateToCharacter(j, pCCha, -1, enumSSTATE_ADD_LARGER, false);
			pCCha->SynSkillStateToEyeshot();
			pCCha->SynAttr(enumATTRSYN_SKILL_STATE);
		}
	}

	return pNode;
}

// 实体指针，实体类型（角色，物品），是否在范围内
void CMgrUnit::DelEntity(CEntityListNode *pCEntNode)
{
#ifdef defMGRUNIT_LOG
	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	{
		LG("管理单元实体", "[%d, %d]开始删除实体 %s，当前实体数 %d。\n", m_sPosX, m_sPosY, pCEntNode->m_pCEntity->m_CLog.GetLogName(), m_lEntityNum);
		if (pCEntNode->m_pCLast)
			LG("管理单元实体", "被删实体的上一实体 %s。\n", pCEntNode->m_pCLast->m_pCEntity->m_CLog.GetLogName());
		if (pCEntNode->m_pCNext)
			LG("管理单元实体", "被删实体的下一实体 %s。\n", pCEntNode->m_pCNext->m_pCEntity->m_CLog.GetLogName());
	}
#endif
	bool	bAddState = false;
	if (pCEntNode->m_pCLast)
		pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
	if (pCEntNode->m_pCNext)
		pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	{
		bAddState = true;
		if (m_pCChaIn == pCEntNode)
			m_pCChaIn = pCEntNode->m_pCNext;
	}
	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHACROSS)
	{
		bAddState = true;
		if (m_pCChaCross == pCEntNode)
			m_pCChaCross = pCEntNode->m_pCNext;
	}
	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMIN)
	{
		if (m_pCItemIn == pCEntNode)
			m_pCItemIn = pCEntNode->m_pCNext;
	}
	else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMCROSS)
	{
		if (m_pCItemCross == pCEntNode)
			m_pCItemCross = pCEntNode->m_pCNext;
	}
	else
		return;
	m_lEntityNum--;
	pCEntNode->m_pCEntity->OutMgrUnit(pCEntNode->m_pCEntMgrNode); // 实体记录所在的管理单元

	if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
	{
#ifdef defMGRUNIT_LOG
		LG("管理单元实体", "实体删除完成，当前实体数 %d\n", m_lEntityNum);
#endif
		long	lCount = 0;
		szEntiList[0] = '\0';
		CEntityListNode	*pNode;
		pNode = m_pCChaIn;
		while (pNode)
		{
			strcat(szEntiList, pNode->m_pCEntity->m_CLog.GetLogName());
			strcat(szEntiList, ", ");
			pNode = pNode->m_pCNext;
			lCount++;
			if (lCount > m_lEntityNum)
			{
#ifdef defMGRUNIT_LOG
				//LG("管理单元实体", "msg实体链错误。\n");
#endif
				break;
			}
		}
#ifdef defMGRUNIT_LOG
		LG("管理单元实体", "实体链：%s。\n", szEntiList);
#endif
	}

	// 向角色增加状态
	if (bAddState)
	{
		CCharacter		*pCCha = pCEntNode->m_pCEntity->IsCharacter();
		if (m_CSkillState.m_chStateNum > 0)
		{
			SSkillStateUnit	*pSStateUnit;
			long			lOnTime;
			pCCha->m_CChaAttr.ResetChangeFlag();
			pCCha->m_CSkillState.ResetChangeFlag();
			for (char j = 0; j < m_CSkillState.m_chStateNum; j++)
			{
				pSStateUnit = m_CSkillState.GetSStateByNum(j);
				lOnTime = g_pGameApp->GetSStateTraOnTime(pSStateUnit->chStateID, pSStateUnit->chStateLv);
				AddStateToCharacter(j, pCCha, lOnTime, enumSSTATE_ADD_EQUALORLARGER, false);
			}
			pCCha->SynSkillStateToEyeshot();
			pCCha->SynAttr(enumATTRSYN_SKILL_STATE);
		}
	}

	pCEntNode->Free();
}

void CMgrUnit::SetEntityIn(CEntityListNode *pCEntNode, bool bIn)
{
	if (bIn)
	{
		if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHACROSS)
		{
			// 删除
			if (pCEntNode->m_pCLast)
				pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
			if (pCEntNode->m_pCNext)
				pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
			if (m_pCChaCross == pCEntNode)
				m_pCChaCross = pCEntNode->m_pCNext;
			// 增加
			if (pCEntNode->m_pCNext = m_pCChaIn)
				m_pCChaIn->m_pCLast = pCEntNode;
			m_pCChaIn = pCEntNode;

			pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_CHAIN;
		}
		else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMCROSS)
		{
			// 删除
			if (pCEntNode->m_pCLast)
				pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
			if (pCEntNode->m_pCNext)
				pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
			if (m_pCItemCross == pCEntNode)
				m_pCItemCross = pCEntNode->m_pCNext;
			// 增加
			if (pCEntNode->m_pCNext = m_pCItemIn)
				m_pCItemIn->m_pCLast = pCEntNode;
			m_pCItemIn = pCEntNode;

			pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_ITEMIN;
		}
		else // 不可能的清况
		{}
		pCEntNode->m_pCEntity->SetCenterMgrNode(pCEntNode->m_pCEntMgrNode);
	}
	else
	{
		if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_CHAIN)
		{
			// 删除
			if (pCEntNode->m_pCLast)
				pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
			if (pCEntNode->m_pCNext)
				pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
			if (m_pCChaIn == pCEntNode)
				m_pCChaIn = pCEntNode->m_pCNext;
			// 增加
			if (pCEntNode->m_pCNext = m_pCChaCross)
				m_pCChaCross->m_pCLast = pCEntNode;
			m_pCChaCross = pCEntNode;

			pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_CHACROSS;
		}
		else if (pCEntNode->m_chEntiType == def_MGRUNIT_ENTITY_TYPE_ITEMIN)
		{
			// 删除
			if (pCEntNode->m_pCLast)
				pCEntNode->m_pCLast->m_pCNext = pCEntNode->m_pCNext;
			if (pCEntNode->m_pCNext)
				pCEntNode->m_pCNext->m_pCLast = pCEntNode->m_pCLast;
			if (m_pCItemIn == pCEntNode)
				m_pCItemIn = pCEntNode->m_pCNext;
			// 增加
			if (pCEntNode->m_pCNext = m_pCItemCross)
				m_pCItemCross->m_pCLast = pCEntNode;
			m_pCItemCross = pCEntNode;

			pCEntNode->m_chEntiType = def_MGRUNIT_ENTITY_TYPE_ITEMCROSS;
		}
		else // 不可能的清况
		{}
	}
}

bool CMgrUnit::AddState(unsigned char uchFightID, unsigned long ulSrcWorldID, long lSrcHandle, char chEffType,
						char chStateID, char chStateLv, unsigned long ulStartTick, long lOnTick, char chType)
{
	// 向地表增加状态
	if (!m_CSkillState.Add(uchFightID, ulSrcWorldID, lSrcHandle, chEffType, chStateID, chStateLv, ulStartTick, lOnTick, chType))
		return false;

	// 向角色增加状态
	char	chStateNo = m_CSkillState.GetReverseID(chStateID);
	CEntityListNode	*pNode;
	pNode = m_pCChaIn;
	while (pNode)
	{
		AddStateToCharacter(chStateNo, pNode->m_pCEntity->IsCharacter(), -1, enumSSTATE_ADD_LARGER);
		pNode = pNode->m_pCNext;
	}
	pNode = m_pCChaCross;
	while (pNode)
	{
		AddStateToCharacter(chStateNo, pNode->m_pCEntity->IsCharacter(), -1, enumSSTATE_ADD_LARGER);
		pNode = pNode->m_pCNext;
	}

	return true;
}

void CMgrUnit::DelState(char chStateID)
{
}

bool CMgrUnit::AddStateToCharacter(char chStateNo, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice)
{
	if (!pCCha->IsLiveing())
		return false;
	CCharacter		*pCSrcCha;
	Entity			*pCSrcEnt;
	SSkillStateUnit	*pSStateUnit;
	pSStateUnit = m_CSkillState.GetSStateByNum(chStateNo);
	if (!pSStateUnit)
		return false;
	pCSrcEnt = g_pGameApp->IsLiveingEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt)
	{
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCSrcCha->IsObjRight(pSStateUnit->chEffectType, pCCha))
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle, pSStateUnit->chEffectType,
					pSStateUnit->chStateID, pSStateUnit->chStateLv, lOnTime, chAddType, bNotice);
	}

	return false;
}

bool CMgrUnit::AddStateToCharacter(SSkillStateUnit	*pSStateUnit, CCharacter *pCCha, long lOnTime, char chAddType, bool bNotice)
{
	if (!pCCha->IsLiveing())
		return false;
	CCharacter		*pCSrcCha;
	Entity			*pCSrcEnt;
	pCSrcEnt = g_pGameApp->IsLiveingEntity(pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle);
	if (pCSrcEnt)
	{
		pCSrcCha = pCSrcEnt->IsCharacter();
		if (pCSrcCha->IsObjRight(pSStateUnit->chEffectType, pCCha))
			return pCCha->AddSkillState(pSStateUnit->uchFightID, pSStateUnit->ulSrcWorldID, pSStateUnit->lSrcHandle, pSStateUnit->chEffectType,
					pSStateUnit->chStateID, pSStateUnit->chStateLv, lOnTime, chAddType, bNotice);
	}

	return false;
}

void CMgrUnit::StateRun(unsigned long ulCurTick, SubMap *pCMap)
{
	char	chCount = 0;
	char	chStateNum = m_CSkillState.m_chStateNum;
	SSkillStateUnit	*pSStateUnit;
	long			lOnTime;
	CEntityListNode	*pNode;
	for (char j = 0; j < chStateNum; j++)
	{
		pSStateUnit = m_CSkillState.GetSStateByNum(chCount);
		if (pSStateUnit->lOnTick > 0)
		{
			if (ulCurTick - pSStateUnit->ulStartTick >= (unsigned long)pSStateUnit->lOnTick * 1000) // 状态计时完成
			{
				lOnTime = g_pGameApp->GetSStateTraOnTime(pSStateUnit->chStateID, pSStateUnit->chStateLv);
				pNode = m_pCChaIn;
				while (pNode)
				{
					AddStateToCharacter(pSStateUnit, pNode->m_pCEntity->IsCharacter(), lOnTime, enumSSTATE_ADD_EQUALORLARGER);
					pNode = pNode->m_pCNext;
				}
				pNode = m_pCChaCross;
				while (pNode)
				{
					AddStateToCharacter(pSStateUnit, pNode->m_pCEntity->IsCharacter(), lOnTime, enumSSTATE_ADD_EQUALORLARGER);
					pNode = pNode->m_pCNext;
				}
				m_CSkillState.Del(pSStateUnit->chStateID);
			}
			else
			{
				// 向角色增加状态
				CEntityListNode	*pNode;
				pNode = m_pCChaIn;
				while (pNode)
				{
					if (!pNode->m_pCEntity->IsCharacter()->m_CSkillState.HasState(pSStateUnit->chStateID, pSStateUnit->chStateLv))
						AddStateToCharacter(pSStateUnit, pNode->m_pCEntity->IsCharacter(), -1, enumSSTATE_ADD_LARGER);
					pNode = pNode->m_pCNext;
				}
				pNode = m_pCChaCross;
				while (pNode)
				{
					if (!pNode->m_pCEntity->IsCharacter()->m_CSkillState.HasState(pSStateUnit->chStateID, pSStateUnit->chStateLv))
						AddStateToCharacter(pSStateUnit, pNode->m_pCEntity->IsCharacter(), -1, enumSSTATE_ADD_LARGER);
					pNode = pNode->m_pCNext;
				}
				chCount++;
			}
		}
		else
			chCount++;
	}
	if (m_CSkillState.m_chStateNum != chStateNum)
	{
		pCMap->NotiMgrUnitStateToEyeshot(m_sPosX, m_sPosY);

		if (m_CSkillState.m_chStateNum <= 0) // 管理单元状态消失
		{
			pCMap->InactiveMgrUnit(m_sPosX, m_sPosY);
			LG("地表表状态", "管理单元[%d, %d]，的状态消失，当前激活数 %d。\n", m_sPosX, m_sPosY, m_lActiveNum);
		}
	}
}
