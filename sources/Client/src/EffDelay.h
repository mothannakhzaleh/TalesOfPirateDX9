#pragma once
#include "SkillRecord.h"

#define ACTION_BEGIN_HIT	-1
#define ACTION_END_HIT		-2

class CCharacter;
class CEffectObj;
class CActor;
class CSkillRecord;
class CServerHarm;

// 一次性的受击特效,如地面特效,受击音效,人物受击特效等
class CHitRepresent 
{
public:
	CHitRepresent() : _pTarget(NULL), _pSkill(NULL), _nAttackX(0), _nAttackY(0), _pAttack(NULL) {}

	void	SetAttack( CCharacter*	p )		{ _pAttack=p;				}
	void	SetTarget( CCharacter*	p )		{ _pTarget=p;				}
	void	SetSkill( CSkillRecord* p )		{ _pSkill=p;				}
	void	SetAttackPoint( int x, int y )	{ _nAttackX=x; _nAttackY=y;	}

	void	ActionExec( CServerHarm* pHarm, int nActionKey );
	void    EffectExec( CServerHarm* pHarm );

private:
	CSkillRecord*	_pSkill;
	CCharacter*		_pTarget;				// 攻击单人时的目标
	int				_nAttackX,  _nAttackY;	// 攻击时攻击中心点
	CCharacter*		_pAttack;				// 攻击方, 用于显示受击的方向,可能为空

private:
	void	Exec( CServerHarm* pHarm );

};

// 用于飞行特效失效后播放延迟的受击特效,延迟特效,网络伤害包等
class CEffDelay
{
public:
    enum ePlayStyle
    {
        enumNone,       // 什么事都不做 
        enumPos,        // 在某地点播放
        enumHitEffect,  // 受击特效
    };

public:
    CEffDelay(CEffectObj* pOwn);
    ~CEffDelay();

    void Reset()        { _eStyle=enumNone; }
    void Exec();

    void SetPosEffect( int nEffID, const D3DXVECTOR3& pos );
    void SetServerHarm( CHitRepresent& Hit, CServerHarm* pHarm );

private:
    CEffectObj* _pOwn;
    ePlayStyle  _eStyle;
    int         _nEffectID;

    // enumPos
    D3DXVECTOR3 _Pos;

    // enumActor
	CHitRepresent	_cHit;
    CServerHarm*    _pHarm;

};

// 内联函数
inline void CHitRepresent::EffectExec( CServerHarm* pHarm )
{
	if( !_pSkill->IsEffectHarm() ) return;

	Exec( pHarm );
}

inline void CHitRepresent::ActionExec( CServerHarm* pHarm, int nActionKey )
{
	if( !_pSkill->IsActKeyHarm() ) return;

	if( _pSkill->chTargetEffectTime!=nActionKey ) return;

	Exec( pHarm );
}

inline void CEffDelay::SetServerHarm( CHitRepresent& Hit, CServerHarm* pHarm )
{
    if( _eStyle!=enumNone ) 
    {
        LG( "CEffDelay", "msgCEffDelay::SetServerHarm() Style[%d] is Valid", _eStyle );
    }

    _eStyle = enumHitEffect;

	_cHit = Hit;
	_pHarm = pHarm;
}

inline void CEffDelay::SetPosEffect( int nEffID, const D3DXVECTOR3& pos )
{
    if( _eStyle!=enumNone ) 
    {
        LG( "CEffDelay", "msgCEffDelay::SetPosEffect(nEffID[%d],pos[%f,%f,%f]) Style[%d] is Valid", nEffID, pos.x, pos.y, pos.z, _eStyle );
    }
    _eStyle = enumPos;

    _nEffectID = nEffID;
    _Pos = pos;
}
