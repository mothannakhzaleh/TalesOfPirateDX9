#pragma once

class CSynchroManage;
class CServerHarm;
class CCharacter;

// 用于客户端同步与服务器的显示
class CStateSynchro
{
    friend class CSynchroManage;
public:
    CStateSynchro();
    virtual ~CStateSynchro();

    // 将当前包按参数比例分隔出(创建)另一个包
    virtual CStateSynchro*   Gouge( float fRate )    { return NULL;  }

    void    Exec();
    void    Exec( DWORD time );

    bool    GetIsExec()							{ return _isExec;       }

	void	SetServerHarm( CServerHarm* pHarm )	{ _pServerHarm=pHarm; 	}
	CServerHarm*	GetServerHarm()				{ return _pServerHarm;	}

	void	Reset()								{ _isExec=false;		}

	virtual CCharacter*	GetHarmCha() = 0;
	virtual const char* GetClassName()			{ return "CStateSynchro";}

protected:
    virtual void _Exec(){}

    bool			_isExec;

	CServerHarm*	_pServerHarm;

private:
    int				_nID;				// CSynchroManage的索引
	DWORD			_dwExecTime;		// 执行时间
	DWORD			_dwCreateTime;

};

// 用于管理同步包，使其按一定顺序执行
class CSynchroManage
{
    friend class CStateSynchro;
public:
    CSynchroManage();
    ~CSynchroManage();

    void	FrameMove( DWORD dwTime );
	void	Reset();

public: 
    static CSynchroManage* I() { return _pInstance;     }

private:
    void    _CollectEmpty();        // 由于头尾已经相接，但实际仍有空间，收集剩余空间

private:
    int     _AddState( CStateSynchro* pState );
    bool    _DelState( CStateSynchro* pState );

private:
    enum 
    {
        MAX_SIZE = 4096,
        ERROR_ID = -1,
    };

    CStateSynchro*  _All[MAX_SIZE];
    DWORD           _dwHead, _dwTail;       // 头尾有效区
    DWORD           _nSynchroNum;           // 有效状态个数

private:
    static CSynchroManage* _pInstance;

};

// 内联函数
inline void CStateSynchro::Exec()
{ 
    _isExec = true; 
}

inline void CStateSynchro::Exec( DWORD time )
{
	if( time<_dwExecTime )
	{
		_dwExecTime = time;
	}
}

