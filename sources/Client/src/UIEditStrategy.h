//----------------------------------------------------------------------
// 描述:Edit的数据结构解析算法
// 作者:lh 2004-07-12
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditArticle;
class CEditChar;
class CEditRow;
class CEditSentence;
class CEditObj;

// 一行,仅为一个容器
class CEditRow
{
public:
	CEditRow();

	void	SetPos( int x, int y );
	void	PushUnit( CEditObj* pObj, CEditObj* pAtom );
	void	Clear();
	void	Render();
	DWORD	GetWordCount()					{ return _dwWordCount;				}

	CEditObj*	GetObj( DWORD dwIndex )		{ if( dwIndex<_units.size() ) return _units[dwIndex]; return NULL;	}
	DWORD		GetObjNum()					{ return (DWORD)_units.size();		}

	DWORD		GetHeight()					{ return _dwHeight;					}
	DWORD		GetWidth()					{ return _dwWidth;					}

private:
	typedef std::vector<CEditObj*> units;
	units			_units;				// 格式后的数据
	DWORD			_dwWordCount;
	units			_atoms;				// 原始数据指针，不管理内存，用于计算光标

	DWORD			_dwHeight;
	DWORD			_dwWidth;

};

class CEditTextObj;
class CEditControl;
class CEditStrategy;

class CEditParse
{
public:
	CEditParse()	{	_items.push_back( new CEditRow );	}

	// 在位置处插入对象,成功返回插入后的索引，失败返回-1
	int		Insert( DWORD nIndex, CEditObj* pObj )			{ return -1;			}

	// 删除起始到终止的块
	bool	Del( DWORD nStart, DWORD nEnd );

	// 得到行列处的光标信息,内部是返回最后一个文本的字体以及颜色
	bool	GetPos( DWORD nIndex, int& x, int& y, DWORD& dwFont, DWORD& dwColor );

	// 得到选择后的区域
	bool	GetSelectRect( DWORD dwStart, DWORD dwEnd, std::vector<POINT>& pt );

public:	
	// 插入单元
	bool	InsertText( CEditTextObj* pText );
	bool	InsertControl( CEditControl* pControl );
	bool	InsertObj( CEditObj* pObj );

	// 计算光标
	int		CursorText( CEditTextObj* pText );
	int		CursorControl( CEditControl* pControl );
	int		CursorObj( CEditControl* pControl );

private:
	typedef std::vector<CEditRow*> items;
	items		_items;					// 逻辑内存，用于显示，计算逻辑等

	DWORD		_dwRowLimit;			// 行数限制
	DWORD		_dwMaxWordLimit;		// 最大字数限制

};

// 算法，用于跟据原始内存生存可显示的文章
class CEditStrategy
{
public:
	CEditStrategy( CEditArticle* pActicle );
	~CEditStrategy();

	void	Init();
	bool	Append( CEditObj *pChar );
	void	Clear();
	void	Render();
	void	RefreshPos( int x, int y );

public:		// vistor模式，用于文本，控制，图元三种类型的解析
	void	ParseText( CEditTextObj* pText );
	void	ParseControl( CEditControl* pControl );
	void	ParseObj( CEditObj* pObj );

private:
	CEditRow*	_AppendToBackRow( CEditObj *pChar );

private:
	CEditArticle*	_pActicle;	

	typedef std::vector<CEditRow*> items;
	items			_items;			// 逻辑内存，用于显示，计算逻辑等

};

inline void CEditStrategy::ParseObj( CEditObj* pObj )
{
	// 不可合并的图元
	CEditRow* pRow = _AppendToBackRow( pObj );
	pRow->PushUnit( pObj, pObj );
}

inline void CEditStrategy::ParseControl( CEditControl* pControl )
{
	_items.push_back( new CEditRow );	
}

}
