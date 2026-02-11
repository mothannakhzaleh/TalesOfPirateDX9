//----------------------------------------------------------------------
// 名称:固定的图象式列表
// 作者:lh 2004-10-09
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "Tools.h"

namespace GUI
{

// 固定大小的List,实际上仅是一个背景+选择框,选择时移动选择框上下位置
class CFixList : public CCompent
{
public:
	CFixList(CForm& frmOwn);
	CFixList( const CFixList& rhs );
	CFixList& operator=(const CFixList& rhs);
	~CFixList();
	GUI_CLONE(CFixList)

	virtual void		Render();
	virtual void		Refresh();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll );
	virtual bool		SetMaxNum( int n );
	virtual bool		IsHandleMouse()				{ return true;					}

	void				SetMargin( int left, int top, int right, int bottom );
	void				SetString( int n, const char* str );
	const char*			GetString( int n );
	int					GetSelIndex()				{ return _lmtSel.GetCur();		}

	CGuiPic*			GetImage()					{ return _pImage;				}
	CGuiPic*			GetSelectImage()			{ return _pSelect;				}

	void				SetRowHeight( int v )		{ if( v>0 ) _nRowHeight = v;	}
	void				SetRowSpace( int v )		{ if( v>=0 ) _nRowSpace = v;	}

public:	// 事件
	GuiEvent			evtSelectChange;				// 选择发生了变化
	GuiMouseEvent		evtListMouseDown;			// 鼠标在列表的文字区按下

private:
	void				_SetSelf( const CFixList& rhs );
	void				_RefreshSelY();

protected:
	int					_nLeftMargin;	// 与周围边框的距离
	int					_nTopMargin;
	int					_nRightMargin;
	int					_nBottomMargin;

	int					_nRowHeight;	// 行高
	int					_nRowSpace;		// 行距

	typedef std::vector<std::string> vstring;
	vstring				_vstString;

	CGuiPic*			_pImage;		// 背景图
	CGuiPic*			_pSelect;		// 选择框
	int					_nSelX, _nSelY;
	CLimit<int>			_lmtSel;		// 当前选择

	int					_nHitX1, _nHitY1, _nHitX2, _nHitY2;

};

// 用途:在每一栏上可标识是否已经选择
class CCheckFixList : public CFixList
{
public:
	CCheckFixList(CForm& frmOwn);
	CCheckFixList( const CCheckFixList& rhs );
	CCheckFixList& operator=(const CCheckFixList& rhs);
	~CCheckFixList();
	GUI_CLONE(CCheckFixList)

	virtual bool			SetMaxNum( int n );
	virtual void			Render();

	void					SetCheckMargin( int left, int top )	{ _nCheckLeftMargin=left; _nCheckTopMargin=top; }
	CGuiPic*				GetCheckImage()						{ return _pImgCheck;							}
	CGuiPic*				GetUnCheckImage()					{ return _pImgUnCheck;							}
	void					SetIsCheck( int index, bool v );
	bool					GetIsCheck( int index );

private:
	void					_SetSelf( const CCheckFixList& rhs );

	static void				_OnCheckDown(CGuiData *pSender, int x, int y, DWORD key) {
		((CCheckFixList*)(pSender))->_OnCheckDown();
	}
	void					_OnCheckDown()				{_checks[_lmtSel.GetCur()] = !_checks[_lmtSel.GetCur()];}

protected:
	typedef std::vector<bool>	vbool;
	vbool					_checks;

	int						_nCheckLeftMargin, _nCheckTopMargin;	// 显示Check的左边距和距顶部边距

	CGuiPic*				_pImgCheck;			// 选择
	CGuiPic*				_pImgUnCheck;		// 未选择

};

// 内联函数
inline 	void CFixList::SetMargin( int left, int top, int right, int bottom ) 
{ 
	_nLeftMargin = left;
	_nTopMargin = top;
	_nRightMargin= right;
	_nBottomMargin = bottom; 
}

inline 	void CFixList::SetString( int n, const char* str )
{ 
	if( n>0 && n<=_lmtSel.GetMax() )
	{
		_vstString[n] = str; 
	}
}

inline void	CFixList::_RefreshSelY() 
{ 
	_nSelY = GetSelIndex() * (_nRowHeight+_nRowSpace) + _nHitY1; 
}

inline void CCheckFixList::SetIsCheck( int index, bool v )
{ 
	if(index>=0 && index<=_lmtSel.GetMax()) 
		_checks[index]=v; 
}

inline bool CCheckFixList::GetIsCheck( int index )
{ 
	if(index>=0 && index<=_lmtSel.GetMax()) 
		return _checks[index]; 
	return false;
}

}
