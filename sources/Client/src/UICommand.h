//----------------------------------------------------------------------
// 名称:用于执行快捷命令
// 作者:lh 2004-10-28
//----------------------------------------------------------------------
#pragma once

#include "uicompent.h"

namespace GUI
{
#define defCommandDefaultIndex	-1

class CCommandObj : public CItemObj
{
public:
	CCommandObj() : _nFast(0), nTag(0), _pParent(NULL) ,_bTrade(false), _nIndex(defCommandDefaultIndex) {}
	virtual ~CCommandObj();
    ITEM_CLONE(CCommandObj)

    bool	        Exec();         // 用户选择时执行
	bool	        ExecRightClick();         // 用户选择时执行

	virtual bool	MouseDown(){ return false;	} //鼠标按下
	bool	GetCanDrag();
	virtual	void	SaleRender( int x, int y, int nWidth, int nHeight )	{}
	virtual void	OwnDefRender( int x, int y, int nWidth, int nHeight ) {}
	virtual void	RenderEnergy(int x, int y)	{}

    void            SetParent( CCompent* p )    { _pParent=p;   }
    CCompent*       GetParent()     { return _pParent;          }
    
    static  bool    UserExec();     // 当用户选择后,等待用户输入执行
    static  void    Cancel()        { _pCommand=NULL;           }
	static  CCommandObj* GetReadyCommand()	{ return  _pCommand;}

    virtual bool    UseCommand(bool value = false);   // 使用
	virtual bool	StartCommand()	{ return false;	}

    virtual int     GetTotalNum()   { return 1;     }
    virtual void    SetTotalNum( int num ){}

    virtual bool    IsDragFast()    { return true;  }   // 是否可以拖动为快捷方式

    virtual bool    IsAllowThrow()  { return false; }   // 是否允丢弃
    virtual bool    IsAllowEquip()  { return false; }   // 可装备

	virtual void    SetIsSolid( bool v ) {}
	virtual bool    GetIsSolid()    { return true;  }

	virtual void    SetIsValid( bool v ){}
	virtual bool    GetIsValid()	{ return false; }

    virtual bool    GetIsPile()     { return false; }
    virtual int     GetPrice()      { return 0;     }

    virtual const char* GetName()   { return "unknown";    }
    virtual bool    IsAllowUse()    { return true;  }   // 可以使用

    // hint
    virtual bool    HasHint()       { return true;  }
	virtual void    ReadyForHint( int x, int y, CCompent* pCompent );
    virtual void    RenderHint( int x, int y );

	// 设置自定义字符串
	void			SetOwnDefText(const char* pszText) { _OwnDefText = pszText; }
	const char*		GetOwnDefText()	{ return _OwnDefText.c_str(); }

	static CTextHint& GetHints()	{ return  _hints;}

public:
    void		SetIsFast( bool v );                    // 是否快捷方式
    bool        GetIsFast()             { return _nFast>0;  }

    void        SetIndex( int nIndex )  { _nIndex = nIndex; }
    int         GetIndex()              { return _nIndex;   }

	int			nTag;       // 用于记录Item或Skill的ScriptID

protected:
    virtual bool    IsAtOnce()      { return true;  }   // 可以直接使用
    virtual bool    ReadyUse()      { return false; }   // 准备使用,对应于不能立即使用时
    virtual void    Error(){}

    virtual void    AddHint( int x, int y ) {}

protected:
    static CCommandObj*     _pCommand;      // 当前用户正在操作的技能

private:
    bool    _Exec();

private:
	int	        _nFast;	    // 在快捷栏上的计数,如果已经放在的快捷栏上,则退出时要发出一个通知
    CCompent*   _pParent;   // 所属控件
	bool        _bTrade;
    int         _nIndex;

protected:
    void    PushHint( const char* str, DWORD color=COLOR_WHITE, int height=5, int font=0, int index = -1, bool shadow = false, DWORD scolor = 0); // height:扣除字高后的高度
    void    AddHintHeight( int height=6 );
	void    SetHintIsCenter( bool v );

	static CTextHint _hints;

	std::string	_OwnDefText;

};

// 内联函数
inline void CCommandObj::PushHint( const char* str, DWORD color, int height, int font, int index, bool shadow, DWORD scolor )
{ 
	_hints.PushHint( str, color, height, font, index, shadow, scolor );
}

inline void CCommandObj::AddHintHeight( int height )
{
	_hints.AddHintHeight( height );
}

inline void CCommandObj::SetHintIsCenter( bool v )   
{ 
	_hints.SetHintIsCenter( v );    
}

}
