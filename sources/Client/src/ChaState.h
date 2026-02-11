//----------------------------------------------------------------------
// ����:������Ҽ���״̬
// ����:lh 2005-05-31
//----------------------------------------------------------------------
#pragma once

#include "boolset.h"
#include "skillstatetype.h"

struct stSkillState;
class CCharacter;
class CEffectObj;
class CSkillStateRecord;

class CChaStateMgr
{
public:
	CChaStateMgr(CCharacter* pCha);

	void		ChaDestroy();									// ��ɫ��Чʱ����
	void		ChaDied()	{ ChaDestroy();	}					// ��ɫ����ʱ����

	CBoolSet&	Synchro(const stSkillState* pState, int nCount );	// ͬ��״̬ʱ����

	int					GetSkillStateNum()					{ return static_cast<int>(_states.size());	}
	CSkillStateRecord*	GetSkillState( unsigned int nID )	{ return _states[nID]->pInfo;	}

	bool		HasSkillState( unsigned int nID );
	int	GetStateLv( unsigned int nID );

	static CSkillStateRecord* GetLastActInfo()				{ return _pLastActInfo;			}
	static int				  GetLastShopLevel()			{ return _nShopLevel;			}

	struct stChaState
	{
		bool				IsDel;

		unsigned char		chStateLv;
		CSkillStateRecord*	pInfo;
		CEffectObj*			pEffect;
		unsigned long lTimeRemaining;
	};
	stChaState GetStateData( unsigned int nID );
private:
	CCharacter*		_pCha;

	

	stChaState		_sChaState[SKILL_STATE_MAXID];
	typedef std::vector<stChaState*>	states;
	states			_states;

	static	bool		_IsInit;
	static stChaState	_sInitState[SKILL_STATE_MAXID];

	static CSkillStateRecord*	_pLastActInfo;		// ����ͬ��ʱ�������һ���ж������ֵ�����
	static int					_nShopLevel;		// ����ʱ���ذ�̯״̬�ȼ�

		

};


inline bool CChaStateMgr::HasSkillState( unsigned int nID )		
{
	return nID<SKILL_STATE_MAXID && _sChaState[nID].chStateLv;
}

inline int CChaStateMgr::GetStateLv( unsigned int nID )		
{
	return HasSkillState( nID )	?_sChaState[nID].chStateLv:0;
}

inline CChaStateMgr::stChaState CChaStateMgr::GetStateData( unsigned int nID )		
{
	return _sChaState[nID];
}
