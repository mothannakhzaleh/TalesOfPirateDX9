//----------------------------------------------------------------------
// 名称:游戏事件
// 作者:lh 2004-05-26
// 用途:用于响应事件
// 实现:事件必须依附于角色或道具存在
//----------------------------------------------------------------------
#pragma once

class CSceneNode;
class CEventRecord;
class CCharacter;
class CGameScene;

class CEvent
{
public:
	CEvent( CGameScene* pScene );
	~CEvent();

	void	Init();

	void	SetIsValid( bool v )	{ _IsValid = v;		}
	bool	GetIsValid()			{ return _IsValid;	}

	void	SetIsEnabled( bool v )	{ _IsEnabled = v;	}
	bool	GetIsEnabled()			{ return _IsEnabled;}

	void	SetName( const char* str )	{ _strName = str;				}

	bool	IsNormal()				{ return _IsValid && _IsEnabled;	}

	void			SetInfo( CEventRecord* pInfo )	{ _pEvent=pInfo;	}
	void			SetNode( CSceneNode* pNode )	{ _pNode=pNode;		}

	CEventRecord*	GetInfo()		{ return _pEvent;	}
	CSceneNode*		GetNode()		{ return _pNode;	}

	bool			DistanceTrigger( int x, int y );
	void			ExecEvent(CCharacter* pCha);

	void			Render();

private:
	CGameScene*		_pScene;
	std::string			_strName;			// 事件显示的名称

	bool			_IsValid;
	bool			_IsEnabled;			

	CEventRecord*   _pEvent;
    CSceneNode*     _pNode;              // 事件所属Node

private: // run time
    bool            _IsActive;           // 是否已经激活
	DWORD			_dwLastTime;		 // 上次激活时间

};

class CEventMgr
{
public:
	CEventMgr( CGameScene* pScene );
	~CEventMgr();

	void		Clear();

	void		DistanceTrigger( CCharacter* pCha );
	CEvent*		CreateEvent( DWORD dwEventID );
	void		Render();

	CEvent*		Search(long	lEntityID);

private:

private:
	CGameScene* _pScene;

	typedef std::vector<CEvent*>	events;
	events		_events;

};

inline void	CEvent::Init()
{
	_IsValid = false;
	_pEvent = NULL;
	_pNode = NULL;
	_IsEnabled = true;

	_IsActive = false;
	_dwLastTime = 0;
}
