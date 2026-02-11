#pragma once
#include "STStateObj.h"
#include <queue>
#include "NetProtocol.h"

class CSceneItem;

class CWaitMoveState : public CActionState
{
public:
	CWaitMoveState(CActor* p);
	~CWaitMoveState();

	virtual void FrameMove();
	virtual void PushPoint( int x, int y );
    virtual const char* GetExplain()            { return "CWaitMoveState";  }
	virtual void Cancel(){}

	CMoveList*		GetMoveList()				{ return &_cMoveList;		}

    void            Clear()                     { _cMoveList.Clear();		}
    void			ChaRun();

	static void		SetPreMoveTime( long time );
	static int		GetPreMoveTime()				{ return _ulPreMoveTime;	}
	static float	GetMoveRate()					{ return _fMoveRate;		}
	static DWORD	GetPreMoveDis()					{ return _dwPreMoveDis;		}

	static void		RenderPing();
	static void		RenderCameraInfo();

protected:
	virtual bool _Start();
    virtual bool _IsAllowCancel()				{ return false;				}

	bool		IsAllowMove();

protected:
	CMoveList	_cMoveList;
	CCharacter*	_pCha;

	static float			_fMoveRate;
	static	unsigned long	_ulPreMoveTime;		// 预移动时间
	static DWORD			_dwPreMoveDis;

private:		// 统计的Ping值
	enum  
	{
		LAST_NUM = 3,
	};
	static int MaxPing;
	static int MinPing;
	static int LastPing[LAST_NUM];
	static int LastPingCnt;
	static int LastPingShow[LAST_NUM];			// 用于显示最近的ping值
	static int nTotalPing;
	static int nTotalPingCnt;
	static float fAveragePing;
};

// 自主移动
class CMoveState : public CWaitMoveState
{
public:
	CMoveState(CActor* p);

	virtual void MoveEnd(int x, int y, int nState);
    virtual void PushPoint( int x, int y );
    virtual void FrameMove();
    virtual void Cancel();
    virtual const char* GetExplain()        { return "CMoveState";      }

	void			SetWalkLine( bool v )		{ _IsWalkLine = v;			}
	void			SetMoveTo( int x, int y )	{ _nTargetX=x; _nTargetY=y; }

    stNetMoveInfo&  GetPathInfo()           { return _stPathInfo;       }

    static bool     IsSameServer( CCharacter* pCha );

protected:
	virtual bool _Start();
	virtual void _End();
    virtual bool _IsAllowCancel();

protected:
    void     WriteInfo( S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info );
    void     _SynchroServer();

protected:
	int			_nTargetX,		_nTargetY;

    stNetMoveInfo   _stPathInfo;    // 用于服务器端的寻路列表
	bool	        _IsWalkLine;	// 走直线

};


// 听从服务器的移动
class CServerMoveState : public CMoveState
{
public:
    CServerMoveState(CActor* p) : CMoveState(p), _IsClientMove(true) {}

    virtual const char* GetExplain()        { return "CServerMoveState";		}

    virtual void Cancel();
    virtual void FrameMove();
    virtual void MoveEnd(int x, int y, int nState);
    virtual void PushPoint( int x, int y );

	void	PreMove();
	static  DWORD			GetLastTime()	{ return _dwLastMoveTime;			}

protected:
    virtual bool _Start();
    virtual void _End();

	int			_nLocalTargetX, _nLocalTargetY;

	static DWORD	_dwLastMoveTime;
	static DWORD	_dwLastEndTime;	// 用于判断是否为静止状态

	bool		_IsClientMove;

};

// 向后走，不改变方向,仅用于单机测试　
class CBackMoveState : public CMoveState
{
public:
	CBackMoveState(CActor* p) : CMoveState(p) {}
	virtual void FrameMove();

protected:
    virtual bool _Start();

};

// 一体化的移动
class COneMoveState : public CWaitMoveState
{
public: 
	COneMoveState(CActor* p);

    virtual void Cancel();
	virtual void FrameMove();
	virtual void PushPoint( int x, int y );
    virtual void MoveEnd(int x, int y, int nState);
    virtual const char* GetExplain()            { return "COneMoveState";	}

	void	SetMoveTo( int x, int y )			{ _nStartX=x; _nStartY=y;	}
	void	SetWalkLine( bool v )				{ _IsStartWalkLine = v;		}
	bool	ContinueMove( int nTargetX, int nTargetY, bool isWalkLine, bool IsCheckTime=false );
	bool	AddPath( int x, int y, bool IsWalkLine );
	bool	IsSameServerPos( int x, int y );
	void    SetRate(float r) { _fRate = r; }
	float   GetRate() { return _fRate; }
	// 对寻路路径分段
	// 参数分别为:输入列表,输入路径,分段长度(小于零不分段)
	static void	CompartMoveList( CMoveList& outlist, S_BVECTOR<D3DXVECTOR3>& path, int length );

protected:
    virtual bool _Start();
    virtual void _End();

    void    WriteInfo( S_BVECTOR<D3DXVECTOR3>& path, stNetMoveInfo& info );
	bool	SendInfo();
	bool	StartMove( int nTargetX, int nTargetY, bool isWalkLine );
	void    SynchroPos( int x, int y );

	float	RefreshRate( int nLocalLen, int nServerLen );
	int		GetSyschroDistance();

private:
	int			_nStartX,	_nStartY;		// 开始移动时的目标点  
	bool		_IsStartWalkLine;			// 开始移动时的参数

	bool		_IsWalkLine;
	int			_nTargetX,	_nTargetY;

	CMoveList	_cLocalList;	// 本地移动序列
	int			_nLocalDis;		// 本地距离

	int			_nServerDis;		// 服务器需要行走的总距离
	int			_nServerStepDis;	// 服务器已经行走的距离

	int			_nMoveCount;		// 请求移动次数
	int			_nStartCount;		// 移动请求次数
	int			_nSendCount;
	int			_nEndCount;			// 服务器返回的结束次数	
	bool		_IsSendCancel;

	DWORD		_dwSendTime;
	DWORD		_dwEndTime;			// 结束时的时间

	float		_fRate;

private:
	struct NeedPath
	{
		NeedPath( int nX, int nY, bool bLine ) : x(nX), y(nY), IsLine(bLine) {}

		int		x, y;
		bool	IsLine;
	};
	typedef std::list<NeedPath> path;
	path		_cNeedList;			// 需要移动的点

	static CMoveList	_List;

};

