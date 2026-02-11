#pragma once

#include "HMSynchroObj.h"

class CStateSynchro;
class CSkillRecord;
class CActor;
class CCharater;
class CAttackEffect;
class CCharacter;
class CAttackRepSynchro;

const int ERROR_FIGHT_ID = INT_MAX;

class CServerHarm
{
    typedef std::list<CAttackEffect*> synchros;
    friend CActor;
public:
    CServerHarm( CActor* pActor );
    ~CServerHarm();

	void	InitMemory();

    void    Exec();             
    void    Gouge( int nGouge );                // 将一个包分隔成为几次执行

    bool    AddHarm( CAttackEffect* s, CSkillRecord *pSkill );        // 加入网络传来的伤害包,在伤害时执行
    bool    AddRep( CAttackRepSynchro* s );         // 表现包,在开始执行时执行
    bool    IsAllowStateOver();                 // 是否允许状态结束

    void    ReadyExec()             { _nReadyExec++;            }
	void    SetIsOuter( bool v );

	void	ExecAll(int nCount);
	void	ExecAll();

	void	SetFightID( int v )		{ _nFightID = v;			}
	void	SetSkill( CSkillRecord* p )	{ _pSkill = p;			}
	
	int		GetCount()				{ return (int)_harm.size();	}

	void	DelHarm( CAttackEffect* p )	{ _harm.remove( p );	}

	void 	HitRepresent( CSkillRecord* pSkill, int nAngle );

public:
    CSkillRecord*   GetSkill()      { return _pSkill;           }

    int     GetFightID()            { return _nFightID;         }
	bool	GetIsExecEnd()		    { return _nFightID==ERROR_FIGHT_ID; }

private:

private:
    int         _nFightID;
    CActor*     _pActor;            // 所属Actor
	CCharacter* _pCha;

    synchros    _harm;              // 伤害包,在关键帧播放
    synchros    _rep;               // 表现包,在开始时播放

    CSkillRecord*   _pSkill;

    int         _nReadyExec;        // 客户端正在准备执行的次数

	bool		_IsOuter;			// 是否有外部调用

};

// 内联函数
