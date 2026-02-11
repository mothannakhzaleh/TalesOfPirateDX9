#pragma once
#include "STStateObj.h"
#include "CharacterAction.h"

enum eActorState
{
	enumNormal,		// 正常状态
	enumDied,		// 死亡状态
	enumRemains,	// 残骸
};


// 死亡时执行
class CActorDie
{
public:
	virtual ~CActorDie() {}
	virtual void Exec()	 {}
};

// 怪物掉料
class CSceneItem;
class CMonsterItem : public CActorDie
{
public:
    CMonsterItem();
    virtual ~CMonsterItem();

    void	Exec();

    void    SetCha( CCharacter* pCha )	    { _pCha=pCha;	        }
    void    SetItem( CSceneItem* pItem )    { _pItem = pItem;       }
	CSceneItem*		GetItem()				{ return _pItem;		}

private:
    CSceneItem*         _pItem;
    CCharacter*         _pCha;

};

// 任务提示
struct stNetNpcMission;
class CMissionTrigger : public CActorDie
{
public:
	CMissionTrigger();
	~CMissionTrigger();

	virtual void Exec();

	void	SetData( stNetNpcMission& v );

private:
	stNetNpcMission		*_pData;

};

class CCharacter;
class CActionState;
class CStateSynchro;
class CSkillRecord;
class CServerHarm;

class CActor
{
public:
	CActor(CCharacter *pCha);
	~CActor();

	void			InitState();

	CActionState*	GetCurState()				{ return _pCurState;			}
	CCharacter*		GetCha()					{ return _pCha;					}
	eActorState		GetState()					{ return _eState;				}
	void			SetState( eActorState v );

	bool			IsEmpty()					{ return _statelist.empty();	}

public:
    void            PlayPose( int poseid, bool isKeep=false, bool isSend=false );
	void			SetSleep()					{ _nWaitingTime = -1;			}

public:		// CActionState 使用
	bool	        SwitchState( CActionState* pState );						// 清除现有队列,中断当前操作,执行现有操作
	bool	        InsertState( CActionState* pState, bool IsFront=false );	// 将现有操作压入队列,以后执行
    bool            AddState( CActionState* pState );

    CActionState*   FindStateClass( const type_info& info );            // 寻找同类状态
    void            OverAllState();                                     // 结束所有状态,用于死亡时服务器不发送结束

	void			CancelState();										// 中断当前状态
	void			FrameMove(DWORD dwTimeParam);

	CActionState*   GetServerState();
	CActionState*   GetServerStateByID(int id);
	CActionState*   GetNextState();
	int				GetQueueCount()				{ return (int)_statelist.size();}
	bool			IsEnabled()					{ return GetState()==enumNormal;}

	void			FailedAction();

public:	// 技能
    void	        ActionBegin( DWORD pose_id );
	void			ActionKeyFrame( DWORD pose_id, int key_frame );
	void			ActionEnd( DWORD pose_id );

	void			Stop();
    void	        IdleState();

	void			ExecAllNet();			// 执行掉所有网络的资源包，用于网络删除玩家时

	void			ExecDied();
	bool			AddDieExec( CActorDie* pDieExec );
	
	void			ClearQueueState();

protected:
    //void            _InitIdle( DWORD pose );

    typedef std::list<CStateSynchro*>  synchro;
    void            _ExecSynchro( synchro& s );
    void            _ClearSynchro( synchro& s );

protected:
	CActionState*	_pCurState;				// 当前状态
	typedef std::list<CActionState*> states;
	states			_statelist;

	CCharacter*		_pCha;					// 当前动作所属角色

public:
    CServerHarm*    CreateHarmMgr();
    CServerHarm*    FindHarm( int nFightID );
    CServerHarm*    FindHarm();				// 查找到一个可用的

private:
    typedef std::vector<CServerHarm*>    fights;
    fights          _fights;

	typedef std::vector<CActorDie*>		dies;	// 网络包：怪物掉料
	dies			_dies;

protected:
    int             _nWaitingTime;     // Wait多少时间后,随机一个搞笑Pose
	eActorState		_eState;

};

// 内联函数
inline void	CActor::ActionKeyFrame( DWORD pose_id, int key_frame )	
{ 
	if( _pCurState ) _pCurState->ActionFrame(pose_id, key_frame);	
}

inline void	CActor::ActionBegin( DWORD pose_id )							
{ 
    if ( _pCurState ) _pCurState->ActionBegin(pose_id);				
}

inline void CActor::ActionEnd( DWORD pose_id )
{ 
    if ( _pCurState ) 
    {
        _pCurState->ActionEnd(pose_id);
    }
    else if( _eState==enumNormal && (pose_id == POSE_SHOW || pose_id == POSE_FLY_SHOW ) )
    {
        IdleState();
    }
}

inline CActionState* CActor::GetServerState()
{
    if( _pCurState )
    {
        if( !_pCurState->GetIsOver() )
            return _pCurState;

        for( states::iterator it=_statelist.begin(); it!=_statelist.end(); ++it )
            if( !(*it)->GetIsOver() )
                return *it;
    }
    return NULL;
}

inline CActionState* CActor::GetServerStateByID(int id)
{
    if( _pCurState )
    {
        if( _pCurState->GetServerID()==id )
            return _pCurState;

        for( states::iterator it=_statelist.begin(); it!=_statelist.end(); ++it )
            if( (*it)->GetServerID()==id )
                return *it;
    }
    return NULL;
}

inline void CActor::_ExecSynchro( synchro& s )
{
    for( synchro::iterator it=s.begin(); it!=s.end(); ++it )
    {
        (*it)->Exec();
        // delete *it;
    }
    s.clear();
}

inline void CActor::_ClearSynchro( synchro& s )
{
    for( synchro::iterator it=s.begin(); it!=s.end(); ++it )
    {
        //delete *it;
		SAFE_DELETE(*it);// UI当机处理
    }
    s.clear();
}

inline void CActor::InitState()
{
	ClearQueueState();
	SAFE_DELETE( _pCurState );

	_eState = enumNormal;
    IdleState();
}

inline bool CActor::AddDieExec( CActorDie* pDieExec )
{
	_dies.push_back( pDieExec );
	return true;
}
