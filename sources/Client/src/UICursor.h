#pragma once

namespace GUI
{
class CCursor 
{
public:
	enum eState
	{
		stNormal	= 0,	// 正常状态
		stActive	= 1,	// 带动作状态
		stDrag		= 2,	// 拖动时
		stSize		= 3,	// 改变大小
		stVertical	= 4,	// 垂直
		stLevel		= 5,	// 水平
		stWait		= 6,	// 等待	
		stAttack	= 7,	// 用于鼠标移动时显示的攻击鼠标
		stSkillAttack = 8,	// 单技能攻击
		stUpBank	= 9,	// 上岸
		stUpBoat	= 10,	// 上船
		stStop		= 11,	// 不可操作
		stHover		= 12,	// 悬停物品
		stPick		= 13,	// 拾取物品
        stCamera	= 14,   // 镜头旋转
        stChat		= 15,   // 对话
        stMouse		= 16,   // 一个鼠标
        stButtonClick = 17, // 按钮按下
        stHide		= 18,   // 不显示鼠标
		stSearch	= 19,	// 查询
		stBlock		= 20,	// 不可走
		stRepair	= 21,	// 修理
		stFeed		= 22,	// 喂食
		stEnd,				// 结束符，用于判断数组最大值
	};

public:
	CCursor();
	~CCursor();

	void	Init();
	void	SetCursor( eState v );			// 设置光标状态－状态函数
	eState  GetCursor()			{ return _eActive;				}

	bool	SetFrame( eState v );			// 设置光标状态－每帧的调用函数
    void    Restore()           { _IsShowFrame=false;           }

	int		GetMax()			{ return stEnd;					}
	int		GetIndex()			{ return _eState;				}

    void    Render();

public:
	static	CCursor* I()		{ return &s_Cursor;				}

private:
	void	_ShowCursor();

private:
	static CCursor	s_Cursor;

	bool		_IsInit;
    bool        _IsShowFrame;

	eState		_eFrame;					// 帧函数的光标状态
	eState		_eState;					// 状态函数的光标状态

	eState		_eActive;					// 当前正在使用的状态
	HCURSOR		_hCursor[stEnd];

};

class CWaitCursor
{
public:
	CWaitCursor()					{ CCursor::I()->SetCursor( CCursor::stWait );	}
	CWaitCursor(CCursor::eState c)	{ CCursor::I()->SetCursor( c );					}
	~CWaitCursor()					{ CCursor::I()->SetCursor( CCursor::stNormal );	}
};

// 内联函数
inline void CCursor::SetCursor( eState v )
{
    if( v>=stNormal && v<stEnd )
    {
        _eState = v;
    }
}

inline bool CCursor::SetFrame( eState v )
{
    if( v>=stNormal && v<stEnd )
    {
        _IsShowFrame = true;
        _eFrame = v;
        return true;
    }
    return false;
}

inline void CCursor::Render()
{
    if( _IsShowFrame )
    {
        if( _eActive!=_eFrame )  
        {
            _eActive = _eFrame;
            _ShowCursor();
        }
    }
    else if( _eActive!=_eState )  
    {
        _eActive = _eState;
        _ShowCursor();
    }
}

}
