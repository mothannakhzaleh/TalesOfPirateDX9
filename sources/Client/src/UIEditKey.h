//----------------------------------------------------------------------
// 名称:管理Edit的输入，显示光标，显示选中状态等
// 作者:lh 2005-05-23
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditParse;
class CEditObj;
class CEditKey
{
public:		
	CEditKey();
	~CEditKey();

	void	Init();
	bool	SetFont( DWORD dwFont );
	void	SetColor( DWORD dwColor )		{ _dwFontColor=dwColor;		}

public:		// 外部接口
	bool	OnChar( char c );
	bool	OnKeyDown( int key );

	void	Render();	

private:
	void	AddChar( CEditObj* pObj );

private:
	CEditParse*		_pParse;

	char			_szEnter[16];				// 用于计算输入的buf
	int				_nEnterPos;

	bool			_IsReadyOnly;				// 是否只读

	DWORD			_dwFontColor;				// 当前字体颜色
	int				_dwFontIndex;				// 当前字体

private:
	DWORD			_dwCurosrIndex;				// 当前光标位置

	DWORD			_dwCursorHeight;			// 光标高度
	DWORD			_dwCursorColor;				// 光标颜色 
	int				_nCursorX, _nCursorY;		// 光标渲染位置
	bool			_IsShowCursor;
	DWORD			_dwCursorTime;
	DWORD			_dwCursorSpace;

};

}
