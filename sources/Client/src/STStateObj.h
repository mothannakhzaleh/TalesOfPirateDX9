#pragma once

#define  _STATE_DEBUG

#include "HMSynchroObj.h"

class CStateSynchro;
class CMoveList;
class CActor;
class CCharacter;

// 动作基类
class CActionState
{
	friend class CActor;
public:
    CActionState(CActor* p);

    void Start();								// 开始执行

	virtual void PushPoint( int x, int y ){}					// 用于可能有移动点的派生类
	virtual void ServerEnd(int nState)      { _IsOver = true;	}	// 设置服务器通告执行完毕
	virtual void MoveEnd(int x, int y, int nState)	{ _IsOver = true;	}	// 对于有移动的Action，仅结束移动
    virtual void SetServerID( int n );
    virtual void Cancel();
	virtual void MouseRightDown(){}

    virtual const char*     GetExplain()    { return "CActionState";}

    int     GetServerID()       { return _nServerID;}

	void	PopState()			{_isExecEnd = true;	}

	CActor* GetActor() 			{ return _pActor;	}

	bool	GetIsOver()			{ return _IsOver;	}			// 执行完剩余动作,动作就完毕了
	bool	GetIsExecEnd()		{ return _isExecEnd;}			// 动作已经全部执行完毕,(服务器已通告执行完毕)
	bool	GetIsCancel()		{ return _IsCancel;	}			// 这个动作已经被玩家取消掉		
	bool	GetIsWait()			{ return _isWait;	}

	// 这个动作为其它动作的子类,注:在CActor中不知道这个动作
    void			SetParent( CActionState* p ){ _pParent=p;		}	
	CActionState*	GetParent()				{ return _pParent;		}

	void			SetIsSend( bool v )		{ _IsSend = v;		    }
    bool            GetIsSend()             { return _IsSend;       }


    bool            GetIsInit()             { return _IsInit;       }

protected:
    virtual		~CActionState();

    virtual void End();												
    virtual void FrameMove();
    virtual void ActionBegin( DWORD pose_id ){}
    virtual void ActionFrame( DWORD pose_id, int key_frame ) {}	
    virtual void ActionEnd( DWORD pose_id )	{}
    virtual void BeforeNewState(){}
    virtual bool IsKeepPose()       { return false;     }       // 动作执行完成后,是否回复到空闲状态
    virtual bool IsAllowUse()       { return true;      }
	virtual void StartFailed()		{}

	virtual bool	_Start()		{ return true;		}
	virtual void	_End()			{}          // 执行结束
    virtual bool    _IsAllowCancel();			// 是否可以被Cancel掉本状态

	bool			_AllowCancel()		{ return !_isWait && !_IsCancel && !_IsOver;	}

private:
	void			SetIsWait( bool v )		    { _isWait=v;		}
    void			_StartFailed();		// 开始时失败时调用

protected:
	bool	_IsCancel;		
	bool	_IsSend;
	bool	_IsOver;
    int     _nServerID;

private:
	CActor			*_pActor;
	CActionState	*_pParent;

	bool	_isWait;			// 为true,放在Actor的队列中,不立即执行,即IsAllowStart()返回false
	bool	_isExecEnd;
	bool	_IsInit;

};

// Actor要移动的列表
class CMoveList
{
public:
	void	Clear(unsigned int count=0);
	void	GotoFront(CCharacter *pCha);
	void	PushPoint( int x, int y );

	bool	IsEmpty()						{ return _path.empty();	    }
	int		GetCount()						{ return (int)_path.size();	}

	void	PopFront()						{ _path.pop_front();		}
	POINT&	GetFront()						{ return _path.front();		}
	POINT&	GetBack()						{ return _path.back();		}

	int		GetListDistance(int x, int y);

private:
	typedef std::list<POINT> path;
	path	_path;

};

// 内联函数
inline void CActionState::SetServerID( int n )
{
    if( _nServerID==INT_MAX )
    {
        _nServerID = n;
    }
    else
    {
        LG( "state", g_oLangRec.GetString(409), GetExplain(), n );
    }
}

