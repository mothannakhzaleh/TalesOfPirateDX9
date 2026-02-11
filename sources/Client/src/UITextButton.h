//----------------------------------------------------------------------
// 名称:按钮
// 作者:lh 2004-07-19
// 用途:可显示文字
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "UIForm.h"

namespace GUI
{

class CTextButton : public CCompent
{
public:
	enum eButtonState	// 按钮状态：正常，鼠标浮动在上面，已经按下，禁用
	{
		csNormal = 0,
		csHover,
		csDown,
		csDisable,
		csEnd,
	};

public:
	CTextButton(CForm& frmOwn);
	CTextButton(const CTextButton& rhs);
	CTextButton& operator=(const CTextButton& rhs);
	~CTextButton(void);
	GUI_CLONE(CTextButton)

	virtual void		Render();
	virtual void		Refresh();
	virtual void		SetAlpha( BYTE alpha )  { _pImage->SetAlpha(alpha); }

	virtual bool		IsHandleMouse()         { return true;              }
	virtual bool		MouseRun( int x, int y, DWORD key );
    virtual void		FrameMove( DWORD dwTime );
	virtual bool		IsFrameMove()			{ return true;				}

	virtual CGuiPic*	GetImage()              { return _pImage;           }

	const char*			GetCaption()            {return _strCaption.c_str();}
	void				SetCaption( const char * str ) { _strCaption = str; }

	virtual	void		SetIsEnabled( bool v );

	// 文件中存储格式：依次按csNormal,csHover,csDown,csDisable排列，w，h为单元宽高
	bool				LoadImage( const char* file, int w=32, int h=32, int tx=0, int ty=0, bool isHorizontal=true );

	DWORD				GetTextColor()						{ return _textColor;		}
	void				SetTextColor(DWORD color )			{ _textColor=color;			}
	bool                GetState()                          { return _isDown;           }

    void				SetFormModal( CForm::eModalResult v );
	CForm::eModalResult GetFormModal()						{ return _eFormModal;		}

	void				DoClick(eMouseState state = Mouse_LUp);		// 外部模拟点击事件

	// 设置闪烁周期（毫秒），设 0 不闪烁
	void				SetFlashCycle( DWORD dwCycle = 1000 )	{ _dwFlashCycle = dwCycle;  }
	DWORD				GetFlashCycle()			{ return _dwFlashCycle;		}

public:	// 事件
	GuiMouseEvent		evtMouseClick;			// 鼠标左键点击事件
	GuiMouseEvent		evtMouseRClick;			// 鼠标右键点击事件  add by Philip.Wu 2006/01/23
	GuiMouseEvent		evtMouseDBClick;		// 鼠标左键双击事件  add by Philip.Wu 2006/01/23
	GuiEvent			evtMouseDownContinue;	// 鼠标持续按在按钮

protected:
	void				_SetState( eButtonState v );
	void				_ClearOldState();

protected:
	std::string				_strCaption;	// 显示在表面的名称
	DWORD				_textColor;		// 字体颜色

	CGuiPic*			_pImage;		// 背景图,共四帧，分别对应：bsNormal,bsHover,bsDown,bsDisable
	bool				_isDown;		// 以前已经在按钮上按下，等行松开执行evtMouseClick事件

    CForm::eModalResult _eFormModal;    // 对于所属表单为模态显示时，赋于表单的返回值

	DWORD				_dwFlashCycle;	// 闪烁周期，=0 不闪烁
	DWORD				_dwLastClick;	// 上一次点击的时间

	static CTextButton*	m_pCurButton;
};

inline void CTextButton::SetFormModal( CForm::eModalResult v )              
{ 
    if( v>=CForm::mrNone && v<CForm::mrEnd )
        _eFormModal=v;            
}

}
