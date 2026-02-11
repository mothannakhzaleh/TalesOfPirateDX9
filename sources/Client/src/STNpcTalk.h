#pragma once
#include "STStateObj.h"

class CCharacter;
class CNpcState : public CActionState
{
public:
	CNpcState(CActor* p);

    virtual const char* GetExplain(){ return "CNpcState";       }

    void SetNpc( CCharacter* p )    { _pNpc=p;                  }

protected:
	virtual bool _Start();

protected:
    CCharacter*	_pNpc;

};

class CShopState : public CActionState
{
public:
	CShopState(CActor* p);

    virtual const char* GetExplain(){ return "CShopState";       }

    void SetShop( CCharacter* p )   { _pShop=p;                  }

protected:
	virtual bool _Start();

protected:
    CCharacter*	_pShop;

};

class CSceneItem;
class CPickState : public CActionState		// 捡道具
{
public:
	CPickState(CActor* p);
	~CPickState();
    virtual const char* GetExplain()    { return "CPickState";      }

    void SetItem( CSceneItem* p )       { _pItem = p;               }

protected:
	virtual bool _Start();

protected:
    CSceneItem*     _pItem;
	
};

class CSceneNode;
class CEvent;
class CEventState : public CActionState
{
public:
	CEventState(CActor* p);
    virtual const char* GetExplain()    { return "CEventState";     }

    void SetNode( CSceneNode* p )       { _pNode = p;               }
	void SetEvent( CEvent* p )			{ _pEvent = p;				}

protected:
	virtual bool _Start();

private:
	CSceneNode*		_pNode;
	CEvent*			_pEvent;

};

// 修正状态,服务器返回时产生,开始时改变鼠标状态,点右键时取消本状态
class CRepairState : public CActionState
{
public:
	CRepairState(CActor* p);
    virtual const char* GetExplain()    { return "CRepairState";    }
	virtual void Cancel(){}
	virtual void MouseRightDown()		{ PopState();				}

protected:
	virtual bool _Start();
	virtual void End();

	bool	_IsBeforeShow;

};

// 给宠物喂食
class CFeedState : public CActionState
{
public:
	CFeedState(CActor* p);
    virtual const char* GetExplain()    { return "CFeedState";		}
	virtual void Cancel(){}
	virtual void MouseRightDown()		{ PopState();				}

	void	SetFeedGridID( int n )		{ _nFeedGridID = n;			}
	int		GetFeedGridID()				{ return _nFeedGridID;		}

protected:
	virtual bool _Start();
	virtual void End();

	bool	_IsBeforeShow;
	int		_nFeedGridID;

};


// 祭祀状态    add by Philip.Wu  2006-06-20  祭祀状态
class CFeteState : public CActionState
{
public:
	CFeteState(CActor* p);
    virtual const char* GetExplain()    { return "CFeteState";		}
	virtual void Cancel()				{}
	virtual void MouseRightDown()		{ PopState();				}

	void	SetFeteGridID( int n )		{ _nFeteGridID = n;			}
	int		GetFeteGridID()				{ return _nFeteGridID;		}

protected:
	virtual bool _Start();
	virtual void End();

	bool	_IsBeforeShow;
	int		_nFeteGridID;
};

