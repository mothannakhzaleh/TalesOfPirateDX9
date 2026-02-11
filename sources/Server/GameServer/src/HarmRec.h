#pragma once

// 仇恨度系统 - 伤害值管理
class CCharacter;

#define MAX_HARM_REC    5
#define MAX_VALID_CNT   8


extern BOOL g_bLogHarmRec;

struct SHarmRec // 伤害记录
{
	CCharacter *pAtk;		// 攻击者指针
	DWORD		sHarm;		// 累加伤害值
	DWORD       sHate;      // 仇恨度
	BYTE		btValid;	// 是否有效
	DWORD		dwID;
	DWORD		dwTime;		// 第一次攻击的时间

	SHarmRec(): pAtk(0), sHarm(0), sHate(0), btValid(0), dwID(0), dwTime(0)
	{
	
	}
	
	bool IsChaValid() // 返回一个角色是否还有效
	{
		if (!pAtk)
		{
			return false;
		}
		if (!g_pGameApp->IsLiveingEntity(dwID, pAtk->GetHandle()) )
		{
			return false;
		}
		return true;
	}
};


class CHateMgr
{
public:

	CHateMgr()
	:_dwLastDecValid(0),
	 _dwLastSortTick(0)
	{
	}

	CCharacter*	GetCurTarget();
	void		AddHarm(CCharacter *pAtk, short sHarm, DWORD dwID);
	void		AddHate(CCharacter *pAtk, short sHate, DWORD dwID);
	void		UpdateHarmRec(CCharacter *pSelf);
	void		ClearHarmRec();
	void		ClearHarmRecByCha(CCharacter *pAtk);
	void		DebugNotice(CCharacter *pSelf);
	SHarmRec*   GetHarmRec(int nNo) { return &_HarmRec[nNo]; }

protected:

	SHarmRec	_HarmRec[MAX_HARM_REC];
	DWORD		_dwLastDecValid;
	DWORD		_dwLastSortTick;
};

inline CCharacter* CHateMgr::GetCurTarget()
{
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid > 0 && pHarm->IsChaValid())
		{
			return pHarm->pAtk;
		}
	}
	return NULL;
}

inline void CHateMgr::ClearHarmRec()
{
	// 已有角色累加伤害
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		pHarm->btValid = 0;
		pHarm->dwID    = 0;
		pHarm->sHarm   = 0;
		pHarm->sHate   = 0;
		pHarm->pAtk    = NULL;
		pHarm->dwTime  = 0;
	}
}

inline void CHateMgr::ClearHarmRecByCha(CCharacter *pAtk)
{
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pAtk && pHarm->pAtk==pAtk)
		{
			pHarm->btValid = 0;
			pHarm->dwID    = 0;
			pHarm->sHarm   = 0;
			pHarm->sHate   = 0;
			pHarm->pAtk    = NULL;
			pHarm->dwTime  = 0;
		}
	}
}


inline void CHateMgr::AddHarm(CCharacter *pAtk, short sHarm, DWORD dwID)
{
	if(g_bLogHarmRec)
	{
		//LG("harm", "开始添加伤害, 攻击者[%s], 伤害%d\n", pAtk->GetName(), sHarm);
		LG("harm", "begin to add harm, attacker[%s], harm%d\n", pAtk->GetName(), sHarm);
	}
	// 已有角色累加伤害
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->pAtk==pAtk && pHarm->pAtk->GetID()==dwID)
		{
			pHarm->sHarm+=sHarm;
			pHarm->sHate+=sHarm; // 普通伤害时, 伤害和仇恨同步增加
			if(pHarm->btValid < MAX_VALID_CNT)
			{
				pHarm->btValid++;
				if(g_bLogHarmRec)
				{
					//LG("harm", "攻击者[%s], 累计伤害=%d，valid=%d\n", pAtk->GetName(), pHarm->sHarm, pHarm->btValid);
					LG("harm", "attacker[%s], accunulative harm=%d，valid=%d\n", pAtk->GetName(), pHarm->sHarm, pHarm->btValid);
				}
			}
			return;
		}
	}

	// 添加新的伤害
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid==0)
		{
			pHarm->pAtk    = pAtk;
			pHarm->sHarm   = sHarm;
			pHarm->sHate   = sHarm;
			pHarm->btValid = 3;
			pHarm->dwID    = dwID;
			pHarm->dwTime  = g_pGameApp->m_dwRunCnt;
			if(g_bLogHarmRec)
			{
				//LG("harm", "添加新的攻击者[%s], 伤害 = %d\n", pAtk->GetName(), pHarm->sHarm);
				LG("harm", "add new attacker[%s], harm = %d\n", pAtk->GetName(), pHarm->sHarm);
			}
			break;
		}
	}
}

// 仅添加伤害而不添加仇恨
inline void CHateMgr::AddHate(CCharacter *pAtk, short sHate, DWORD dwID)
{
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->pAtk==pAtk && pHarm->pAtk->GetID()==dwID)
		{
			pHarm->sHate+=sHate;
			
			if(sHate > 0)
			{
				if(pHarm->btValid < MAX_VALID_CNT)
				{
					pHarm->btValid++;
				}
			}
			else
			{
				if(pHarm->btValid > 0)
				{
					pHarm->btValid--;
				}
			}

			if(pHarm->sHate < 0) 
			{
				pHarm->sHate = 0;
			}
			return;
		}
	}

	if(sHate > 0)
	{
		// 添加新的hate
		for(int i = 0; i < MAX_HARM_REC; i++)
		{
			SHarmRec *pHarm = &_HarmRec[i];
			if(pHarm->btValid==0)
			{
				pHarm->pAtk    = pAtk;
				pHarm->sHate   = sHate;
				pHarm->btValid = 3;
				pHarm->dwID    = dwID;
				pHarm->dwTime  = g_pGameApp->m_dwRunCnt;
				break;
			}
		}
	}
}

inline int CompareHarm(const void *p1, const void *p2)
{
	SHarmRec *pRec1 = (SHarmRec*)p1;
	SHarmRec *pRec2 = (SHarmRec*)p2;
	
	if(pRec1->sHate < pRec2->sHate)
	{
		return 1;
	}
	return -1;
}

inline void CHateMgr::UpdateHarmRec(CCharacter *pSelf)
{
	DWORD dwCurTick = GetTickCount();
	
	// 重新记录有效的HarmRec
	int nValid = 0;
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid > 0 && pHarm->IsChaValid() )
		{
			memcpy(&_HarmRec[nValid], pHarm, sizeof(SHarmRec));
			nValid++;
		}
	}

	// 剩下的都清除, 保证HarmRec是紧凑的, 没有空位
	for(int j = nValid; j < MAX_HARM_REC; j++)
	{
		SHarmRec *pHarm = &_HarmRec[j];
		pHarm->btValid = 0;
		pHarm->sHarm   = 0;
		pHarm->sHate   = 0;
		pHarm->dwID    = 0;
		pHarm->pAtk    = NULL;
		pHarm->dwTime  = 0;
	}
	
	// 每 2 秒钟,排序一次
	if((dwCurTick - _dwLastSortTick) > 2000)
	{
		_dwLastSortTick = dwCurTick;

		qsort(&_HarmRec, MAX_HARM_REC, sizeof(SHarmRec), CompareHarm);
		if(g_bLogHarmRec)
		{
			DebugNotice(pSelf);
		}
	}

	// 每 5 秒钟 btValid - 1
	if((dwCurTick - _dwLastDecValid) > 5000)
	{
		_dwLastDecValid = dwCurTick;	
		for(int i = 0; i < MAX_HARM_REC; i++)
		{
			SHarmRec *pHarm = &_HarmRec[i];
			if(pHarm->btValid > 0)
			{
				pHarm->btValid--;
				if(pHarm->btValid==0) // 忘掉了
				{
					pHarm->sHarm  = 0;		// 伤害不累计
					pHarm->sHate  = 0;		// 仇恨不累计
					pHarm->dwTime = 0;		// 时间清0
					pHarm->pAtk   = NULL;
					pHarm->dwID   = 0;
				}
				if(g_bLogHarmRec)
				{
					//LG("harm", "攻击者[%s]的valid--, valid = %d\n", pHarm->pAtk->GetName(), pHarm->btValid);
					LG("harm", "attacker[%s]的valid--, valid = %d\n", pHarm->pAtk->GetName(), pHarm->btValid);
				}
			}
		}
	}
}

inline void CHateMgr::DebugNotice(CCharacter *pSelf)
{
	std::string strNotice = pSelf->GetName();
	//strNotice+="目标列表:";
	strNotice+=RES_STRING(GM_HARMREC_H_00001);
	BOOL bSend = FALSE;
	char szHate[64];
	for(int i = 0; i < MAX_HARM_REC; i++)
	{
		SHarmRec *pHarm = &_HarmRec[i];
		if(pHarm->btValid > 0)
		{
			sprintf(szHate, ",%d(time=%d)", pHarm->sHate, pHarm->dwTime);
			strNotice+="[";
			strNotice+=pHarm->pAtk->GetName();
			strNotice+=szHate;
			strNotice+="]";
			bSend = TRUE;
		}
	}

	if(bSend)
	{
		LG("harm", "Notice = [%s]\n", strNotice.c_str());
		g_pGameApp->WorldNotice((char*)(strNotice.c_str()));
	}
}

