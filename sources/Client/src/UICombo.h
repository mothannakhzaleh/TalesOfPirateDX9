//----------------------------------------------------------------------
// 名称:组合框
// 作者:lh 2004-07-08
// 设计思想:由CEdit,CTextButton,CList组合
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "UIEdit.h"
#include "uilist.h"

namespace GUI
{

class CCombo : public CCompent
{
public:
	enum eListStyle
	{
		lsUp,		// List在上面
		lsDown,		// List在下面
	};

	CCombo(CForm& frmOwn);
	CCombo( const CCombo& rhs );
	CCombo& operator=(const CCombo& rhs);
	virtual ~CCombo(void);
	GUI_CLONE(CCombo)

	virtual void	Init();
	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual bool	MouseScroll( int nScroll );
	virtual void	SetAlpha( BYTE alpha );
	virtual bool	IsHandleMouse() { return true;		}

public:
	virtual bool	OnKeyDown( BYTE key );
	virtual bool	OnChar( BYTE c );
	virtual void	OnActive();
	virtual void	OnLost();

public:
	CEdit*			GetEdit()	{ return _pEdit;	}
	CList*			GetList()	{ return _pList;	}
	CTextButton*	GetButton() { return _pButton;	}

	void			SetListStyle( eListStyle v );
	eListStyle		GetListStyle()			{ return _eListStyle;			}

	const char*		GetText()				{ return _pEdit->GetCaption();	}

public:
	GuiEvent		evtSelectChange;	// 选择发生了变化
	GuiMouseEvent	evtMouseClick;		// 鼠标点击按钮事件

protected:
	static void		_OnButtonClick(CGuiData *pSender, int x, int y, DWORD key) {
		((CCombo*)(pSender)->GetParent())->_OnButtonClick( x, y, key );
	}
	void			_OnButtonClick(int x, int y, DWORD key);

	static void		_OnListDown(CGuiData *pSender, int x, int y, DWORD key) {
		((CCombo*)(pSender)->GetParent())->_OnListDown();
	}
	void			_OnListDown()	{ _pList->SetIsShow( !_pList->GetIsShow() );	}

	static void		_OnListSelectChange(CGuiData *pSender) {
		((CCombo*)(pSender)->GetParent())->_OnListSelectChange();
	}
	void			_OnListSelectChange();

	void			_RefreshList();
	void			_SetSelf();

protected:
	CEdit*			_pEdit;
	CList*			_pList;
	CTextButton*	_pButton;

	eListStyle		_eListStyle;

};

}
