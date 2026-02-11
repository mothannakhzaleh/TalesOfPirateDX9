//----------------------------------------------------------------------
// 名称:列表
// 作者:lh 2004-07-21
// 思想:列表中集合CListItems,CListItems中集合CItemRow
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uiscroll.h"
#include "uiimage.h"
#include "uiitem.h"

namespace GUI
{

class CList;

// 选择条
class CSelectItem
{
public:
	CSelectItem();
	CSelectItem( const CSelectItem& rhs );
	CSelectItem& operator=( const CSelectItem& rhs );

	void		Render( int x, int y );

	void		SetSelect( CItemRow* pItem, DWORD dwIndex );
	void		SetMouseMoveSelect( CItemRow* pItem, DWORD dwIndex );
	void		SetNoSelect();
	int			GetIndex()				{ return _nIndex;			}
	bool		GetIsSelect()			{ return _isSelect;			}

	void		SetSize( int w, int h ) { _pImage->SetScale( w, h );}

	void		SetList( CList* pList ) { _pList = pList;			}

public:
	CItemRow*	GetItem()				{ return _pItem;			}
	void		SetIsEnabled( bool v )	{ _isEnabled=v;				}
	bool		GetIsEnabled()			{ return _isEnabled;		}
	CGuiPic*	GetImage()				{ return _pImage;			}

private:
	void		_Copy( const CSelectItem& rhs );

private:	
	CItemRow*	_pItem;				// 选择的Item
	bool		_isEnabled;			// 是否使用
	bool		_isSelect;
	int			_nIndex;			// 选择的索引，没有选择为-1
	CGuiPic*	_pImage;

private:
	CList*		_pList;

};

class CList;
class CForm;
class CListItems
{
public:
	explicit CListItems();
	explicit CListItems( const CListItems& rhs );
	CListItems& operator=( const CListItems& rhs );
	virtual ~CListItems();
	virtual bool		Clone( CListItems* rhs );

public:
	bool				MouseRun( int x, int y, DWORD key );
	void				Refresh();
	virtual void		Render();
	virtual void		Clear();
	virtual bool		SetColumnWidth( unsigned int nCol, unsigned int width ) { return false; }
	virtual unsigned int GetColumn() { return 1; }
	virtual CItemRow*	Add( const char* str, DWORD c );

	virtual void		SetRect( int x1, int y1, int x2, int y2, int rowheight ); // 设置可以点击的范围
	
	int					GetColumnWidth( unsigned int nCol=0 )	{ return _GetColumnWidth(nCol); }
	void				SetParent( CList* pList );
	CSelectItem*		GetSelect()								{ return _pSelect;				}

	CItemRow*			NewItem();
	bool				Del( CItemRow* item );

	void				SetFirstShowRow( DWORD row );
	CItemRow*			GetItem( int index );
	int					GetIndex( CItemRow *p) ;
	DWORD				GetCount()								{ return _dwCount;				}

	int					GetShowCount()							{ return _nShowCount;			}
	int					GetCurShowCount()						{ return _dwLastShowRow -_dwFirstShowRow;}//Add by zcj 20080801

	int					GetMaxRowNum()							{ return _nMaxRow;				}
	void				SetMaxRowNum( int n );

	void				SetIsMouseFollow( bool v )				{ _IsMouseFollow=v;				}
	bool				GetIsMouseFollow()						{ return _IsMouseFollow;		}	

	void				SetItemMargin( int left, int top );
	void				SetImageMargin( int left, int top );

public:		// 键盘处理	
	void				SelectPrior();			// 选择上一个	
	void				SelectNext();			// 选择下一个
	void				SelectFirst();			// 选择第一个
	void				SelectLast();			// 选择最后一个
	void				SelectAdd( int page );	// 当前选择移动几格
	void				Select( int n );		// 选择第几个

protected:
	void				_CheckMaxRow();			// 检查是否达到最大行数,否则,删除最早的一行
	void				_AddRow( CItemRow* p );

private:
	void				_Copy(const CListItems &rhs);
	virtual int			_GetColumnWidth( unsigned int nCol )	{ return 0;						}

protected:
	typedef		std::deque<CItemRow*>	vitems;
	vitems				_items;
	CSelectItem*		_pSelect;			// 选择的Item

	CGuiPic*			_pItemPic;			// 指向父类的图片，用于显示在项目条背景中
	CList*				_pList;

	int					_nX1, _nX2, _nY1, _nY2;
	int					_nHeight;
	int					_nShowCount;

	DWORD				_dwFirstShowRow;	// 第一行可以显示的行数
	DWORD				_dwLastShowRow;		// 最后一行的显示
	DWORD				_dwCount;			// 总共的行数
	int					_nMaxRow;			// 允许的最大行数

	// 项目条距离边框的距离
	int					_nItemLeft, _nItemTop;
	int					_nItemX;

	int					_nPicLeft,  _nPicTop;
	int					_nPicX;
	bool				_IsMouseFollow;		// 鼠标是否自动跟随

protected:
	static int			_nTmpY;

};

class CListItemsEx : public CListItems
{
public:
	explicit CListItemsEx(unsigned int nCol );
	explicit CListItemsEx( const CListItemsEx& rhs );
	CListItemsEx& operator=( const CListItemsEx& rhs );
	~CListItemsEx();

	virtual bool		Clone( CListItems* rhs );
	virtual void		Clear();
	virtual void		SetRect( int x1, int y1, int x2, int y2, int rowheight );
	virtual void		Render();

public:
	virtual CItemRow*	Add( const char* str, DWORD c );
	virtual bool		SetColumnWidth( unsigned int nCol, unsigned int width );
	virtual unsigned int GetColumn() { return _nCol; }

private:
	virtual int			_GetColumnWidth( unsigned int nCol );

	void				_SetSelf(const CListItemsEx &rhs);
	void				_RefreshColX();

protected:
	unsigned int		_nCol;			// 列数
	unsigned int*		_nColWidth;		// 每一列的宽度
	int*				_nColX;			// 每一列的位置

}; 

class CList : public CCompent
{
public:
	CList(CForm& frmOwn);
	CList(CForm& frmOwn, int nCol);
	CList( const CList& rhs );
	CList& operator=(const CList& rhs);
	virtual ~CList(void);
	GUI_CLONE(CList)

	virtual void		Init();
	virtual void		Render();
	virtual void		Refresh();
	virtual bool		MouseRun( int x, int y, DWORD key );
	virtual bool		MouseScroll( int nScroll );

	virtual bool		IsHandleMouse()				{ return true;	}
	virtual void		SetAlpha( BYTE alpha );

public:		// 键盘处理
	virtual void		OnActive();
	virtual void		OnLost();
	virtual bool		OnKeyDown( int key );

	virtual bool		SetShowRow( int n );			// 设置显示时的行高,会改变List总高度

public:
	CItemRow*			GetSelectItem()						{ return  _pItems->GetSelect()->GetItem();	}

	CScroll*			GetScroll()							{ return _pScroll;							}
	CGuiPic*			GetImage()							{ return _pImage;							}
	CGuiPic*			GetItemImage()						{ return _pItemImage;						}
	CListItems*			GetItems()							{ return _pItems;							}

	void				SetRowHeight( int v )				{ if( v>0) _nRowHeight = v;					}
	int					GetRowHeight()						{ return _nRowHeight;						}
	void				SetSpaceBottom( int n )				{ if(n>=0) _nSpaceBottom=n;					}

	void				SetMargin( int left, int top, int right, int bottom );

	void                SetFrameSelectAtrribute(int width ,int height ,int x, int y);

	void                ResetScrollChange();

public:
	CItemRow*			Add( const char* str );
	bool				Del( CItemRow* item );
	void				Clear();
	CItemRow*			NewItem();

	void				SetFontColor( DWORD c )		{ _dwFontColor = c;			}
	DWORD				GetFontColor()				{ return _dwFontColor;		}

	void				SetSelectColor( DWORD c )	{ _dwSelectColor = c;		}
	DWORD				GetSelectColor()			{ return _dwSelectColor;	}		

	void				SetIsChangeColor( bool v )	{ _IsChangeColor = v;		}
	bool				GetIsChangeColor()			{ return _IsChangeColor;	}
	
public:
	GuiEvent			evtSelectChange;			// 选择发生了变化
	GuiMouseEvent		evtListMouseDown;			// 鼠标在列表的文字区按下
	GuiMouseEvent		evtListMouseDB;

protected:
	static void			_ScrollChange(CGuiData *pSender) {
		((CList*)(pSender->GetParent()))->_OnScrollChange();
	}
	void				_OnScrollChange();
	void				_ResetPageNum();

protected:
	void				_SetScrollRange();
	void				_SetSelf();
	void				_Copy( const CList& rhs );

protected:
	CScroll*			_pScroll;	
	CListItems*			_pItems;
	CGuiPic*			_pImage;		// 背景图
	CGuiPic*			_pItemImage;	

	int					_nLeftMargin;	// 与周围边框的距离
	int					_nTopMargin;	// 与周围边框的距离
	int					_nRightMargin;	// 与周围边框的距离
	int					_nBottomMargin;	// 与周围边框的距离

	int					_nRowHeight;	// 行高
	int					_nSpaceBottom;	// 距底部空闲行数

	DWORD				_dwFontColor;
	DWORD				_dwSelectColor;
	bool				_IsChangeColor;	// 不改变颜色


};

// 内联函数
inline 	void CSelectItem::SetSelect( CItemRow* pItem, DWORD dwIndex ) 
{
	if( _pList->GetIsChangeColor() )
	{
		if ( _pItem )
		{
			_pItem->SetColor( _pList->GetFontColor() );
			_pItem=NULL; 
		}

		_pItem = pItem;
		if (_pItem)
		{
			_pItem->SetColor( _pList->GetSelectColor() );
		}
	}
	else
	{
		_pItem = pItem;
	}

	_isSelect = true;
	_nIndex = dwIndex;	
}

inline 	void CSelectItem::SetNoSelect()	
{ 
	if( _pItem )
	{
		_pItem->SetColor( _pList->GetFontColor() );
	}

	_isSelect=false;
	_pItem=NULL; 
	_nIndex=-1; 
}

inline 	void CListItems::SetFirstShowRow( DWORD row ) 
{
	if( row <= _dwCount ) 
	{
		_dwFirstShowRow = row;
		_dwLastShowRow = _dwFirstShowRow + _nShowCount;
		_dwLastShowRow = _dwLastShowRow > _dwCount ? _dwCount : _dwLastShowRow;
	}
}

inline void	CListItems::SetItemMargin( int left, int top )
{
	_nItemLeft = left;
	_nItemTop = top;
}

inline void	CListItems::SetImageMargin( int left, int top )
{
	_nPicLeft = left;
	_nPicTop = top;
}

inline 	void CList::_ResetPageNum() 
{ 
	_pScroll->SetPageNum( ( GetHeight() - _nTopMargin - _nBottomMargin ) / _nRowHeight - 1 );
}

inline void CListItems::SelectPrior()
{	
	if( _pSelect->GetIsSelect() ) Select( _pSelect->GetIndex() - 1 ); 
	else Select( (int)_items.size() );
}

inline void CListItems::SelectNext() 
{	
	if( _pSelect->GetIsSelect() ) Select( _pSelect->GetIndex() + 1 ); 
	else Select( 0 );
}

inline void CListItems::SelectFirst()
{
	Select( 0 );
}

inline void CListItems::SelectLast()
{
	Select( _dwCount - 1 );
}

inline void CListItems::SelectAdd( int page )
{
	if( _pSelect->GetIsSelect() ) 
	{
		Select( _pSelect->GetIndex() + page ); 
	}
}

}
