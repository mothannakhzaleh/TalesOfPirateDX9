//----------------------------------------------------------------------
// 名称:3D控件
// 作者:lh 2004-09-13
// 用途:在GUI系统中提供外部显示接口
//		有二类接口:	一,外部继承C3DRenderObj实现Render函数
//					二,注册回调函数显示:GUIRenderEvent,GUIRenderMsgEvent
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI
{

class C3DCompent;
typedef void(*GUIRenderEvent)(C3DCompent *pSender, int x, int y);

class C3DCompent : public CCompent
{
public:
	C3DCompent(CForm& frmOwn);
	C3DCompent( const C3DCompent& rhs );
	C3DCompent& operator=(const C3DCompent& rhs);
	virtual ~C3DCompent();

	GUI_CLONE(C3DCompent)

	virtual void	Render();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual bool	IsHandleMouse()		{ return true;			}		// 是否可以响应鼠标
	virtual void	Refresh();

	void			SetRenderEvent( GUIRenderEvent p )			{ _pRenderEvent=p;			}
	GUIRenderEvent	GetRenderEvent()	{ return _pRenderEvent;	}

	int				GetCenterX()	{ return _nCenterX;		}
	int				GetCenterY()	{ return _nCenterY;		}

    void            SetPointer( void * p )  { _pPointer = p;    }
    void*           GetPointer()            { return _pPointer; }

public:
	GuiMouseEvent	evtMouseDown;		// 鼠标按下事件
	GuiMouseEvent   evtMouseLDDown ;    //鼠标双击事件

protected:
	void			_Copy( const C3DCompent& rhs );

protected:
	GUIRenderEvent		_pRenderEvent;
	int					_nCenterX, _nCenterY;

    void*            _pPointer;

};

}
