//----------------------------------------------------------------------
// 名称:检查框
// 作者:lh 2004-07-27
// 用途:响应是与否的状态 
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------

#pragma once
#include "uicompent.h"

namespace GUI
{

// 单个检查框,也可用于选择框
class CCheckBox : public CCompent
{
public:
	enum eStyle
	{
		stUnChecked,
		stChecked,
		stEnd,
	};

public:
	CCheckBox(CForm& frmOwn);
	CCheckBox( const CCheckBox& rhs );
	CCheckBox& operator=(const CCheckBox& rhs);
	~CCheckBox() { SAFE_DELETE(_pImage); } // UI当机处理 //delete _pImage; }
	GUI_CLONE(CCheckBox)

	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual void	SetAlpha( BYTE alpha )		{ _pImage->SetAlpha(alpha);		}

	virtual bool	IsHandleMouse()				{ return true;					}
	virtual CGuiPic* GetImage()					{ return _pImage;				}

	bool			GetIsChecked()				{ return _isChecked;			}
	void			SetIsChecked( bool v );
	void			SetTextColor( DWORD color ) { _TextColor = color;			}
	DWORD			GetTextColor()				{ return _TextColor;			}

	const char*		GetCaption()				{ return _strCaption.c_str();	}
	void			SetCaption( const char * str )	{ _strCaption = str;		}

public:
	GuiEvent		evtCheckChange;

	static int		s_nMarginLeft;		// 文字距离左面图片的位置

protected:
	void			_SetSelf();

protected:
	DWORD			_TextColor;
	CGuiPic*		_pImage;			// 背景图,共二帧，分别对应：bsChecked,bsUnChecked
	std::string			_strCaption;

	bool			_isChecked;

private:
	bool			_isDown;
	int				_nTextOffsetX;
	int				_nTextOffsetY;

};

// 将单个检查框分成组
class CCheckGroup : public CContainer
{
public:
	CCheckGroup(CForm& frmOwn);
	CCheckGroup( const CCheckGroup& rhs );
	CCheckGroup& operator=(const CCheckGroup& rhs);
	GUI_CLONE(CCheckGroup)

	virtual void	Init();
	virtual bool	IsHandleMouse()		{ return true;		}

public:
	void			AddCheckBox( CCheckBox* p );
	CCheckBox*		GetActive()			{ return _pActive;	}
	
	void            SetActiveIndex(int n ) ;
	int				GetActiveIndex()	{ return _nActive;	}

public:
	GuiEvent		evtSelectChange;

private:
	void			_UpdataActive( CCheckBox* active );

	static void		_CheckChange(CGuiData *pSender) {
		((CCheckGroup*)(pSender->GetParent()))->_UpdataActive( dynamic_cast<CCheckBox*>(pSender) );
	}

	void			_SetSelf();

private:	
	int				_nSize;
	int				_nActive;

private:
	CCheckBox*		_pActive;

};

// 内联函数
inline void CCheckBox::SetIsChecked( bool v ) 
{
	_isChecked = v;
	_pImage->SetFrame( (int)_isChecked );
}

inline void CCheckGroup::AddCheckBox( CCheckBox* p ) 
{ 
	if( CContainer::AddCompent(p) )	
	{
		_nSize = (int)_items.size();
		p->evtCheckChange = _CheckChange;
	}
}
}
