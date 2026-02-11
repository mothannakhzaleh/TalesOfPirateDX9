//----------------------------------------------------------------------
// 描述:Edit的数据结构
// 作者:lh 2004-07-12
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once

namespace GUI
{

class CEditParse;
class CEditStrategy;
class CRichEdit;

// 最小单元
class CEditObj
{
public:
	virtual			~CEditObj()	{ Clear();		 }
	virtual void	SetPos( int x, int y )		{}
	virtual void	GetSize( int& w, int& h )   {}
	virtual void	Render()					{}
	virtual void	Clear()						{}

	virtual const char* GetCaption()			{ return "";		}
	virtual DWORD		GetWordCount()			{ return 0;			}

	// 子对象的操作
	virtual void	PushUnit( CEditObj* pObj )	{}
	virtual void	ClearChilds()				{}
	virtual	bool	IsEmpty()					{ return true;		}

	virtual void	Parse( CEditStrategy* pParse );

};

// 文字类从祖先
class CEditTextObj : public CEditObj
{
public:
	CEditTextObj();

	virtual void		SetCaption( const char* ){}
	virtual void		AddCaption( const char* ){}
	virtual void		Parse( CEditStrategy* pParse );

public:
	void		SetFont( DWORD dwFont )			{ _dwFont=dwFont;			}
	DWORD		GetFont()						{ return _dwFont;			}

	void		SetColor( DWORD dwColor )		{ _dwColor=dwColor;			}
	DWORD		GetColor()						{ return _dwColor;			}

	// 用于判断是否可以合并文字
	bool		IsSameType( CEditTextObj* pObj ){ return pObj->GetColor()==GetColor() && pObj->GetFont()==GetFont();	}

private:
	DWORD			_dwFont;			// 所用的字体
	DWORD			_dwColor;			// 颜色

};


// 单个字符,包括汉字
class CEditChar : public CEditTextObj
{
public:
	CEditChar( char c1, char c2=0 ) { _szChar[0]=c1; _szChar[1]=c2; _szChar[2]=0;	}

	const char*		GetCaption()				{ return _szChar;					}
	DWORD			GetWordCount()				{ return _szChar[1]==0 ? 1 : 2;		}
	void			SetWord( char c1,char c2=0 );

	const char*		GetWord()					{ return _szChar;					}

	int				GetWidth()					{ return _nWidth;					}

	// 用于显示被选择时
	void			InvertRedner( int x, int y );

protected:
	char			_szChar[3];
	int				_nWidth;

};

// 控制符
class CEditControl : public CEditObj
{
public:
	CEditControl( char c ) : _cChar( c )		{}

	virtual void	Parse( CEditStrategy* pParse );
	virtual DWORD		GetWordCount()			{ return 1;					}

	char			GetControl()				{ return _cChar;			}

private:
	char			_cChar;

};

// 句子，同类字符的集合，用于快速显示
class CEditSentence : public CEditTextObj
{
public:
	CEditSentence();
	virtual void	Render();
	virtual void	SetPos( int x, int y )		{ _nX = x; _nY = y;			}
	virtual void	GetSize( int& w, int& h );

	void		SetCaption( const char* str )	{ _szString=str;			}
	const char* GetCaption()					{ return _szString.c_str();	}

	void		AddCaption( const char* str )	{ _szString+=str;			}

	bool		AddChar( DWORD dwIndex, CEditChar* pChar );

private:
	typedef std::vector<CEditChar*> chars;
	chars			_chars;

	std::string			_szString;			// 合并后的字串
	int				_nX,	_nY;

};

// 文章，包含了所有内容以及一个用换行的算法
class CEditArticle : public CEditObj
{
public:
	CEditArticle();
	~CEditArticle();

	virtual void	Init();
	virtual void	Render();
	virtual void	Clear();

	void	SetEdit( CRichEdit* pEdit )		{ _pEdit=pEdit;		}

	bool	AddControl( char c );
	bool	AddChar( char c1, char c2=0 );
	bool	DelChar( unsigned int nIndex );

	void	SetFont( DWORD dwFont )			{ _dwFontIndex=dwFont;		}
	DWORD	GetFont()						{ return _dwFontIndex;		}

	void	SetColor( DWORD dwColor )		{ _dwFontColor=dwColor;		}
	DWORD	GetColor()						{ return _dwFontColor;		}

public:		// 键盘事件
	void	OnKeyDown( int key, bool IsShiftPress );
	void	DelSelect();

private:
	void	RefreshText();

private:
	void	_ToSelectMode();
	bool	_AddObj( CEditObj* pObj );

private:
	enum eRunType
	{
		enumNormal,							// 正常模式
		enumSelect,							// 用户正在选择，此时不显示光标
	};

	CRichEdit*		_pEdit;
	CEditStrategy*	_pStrategy;

	typedef std::vector<CEditObj*> memorys;
	memorys			_memorys;				// 原始内存，用于管理内存,也用于计算光标

	eRunType		_eRunType;

	unsigned int	_nCursor;				// 光标所在位置
	unsigned int	_nSelStart;				// 用户选择的区域
	unsigned int	_nSelEnd;

	unsigned int	_dwWordMax;				// 可输入字数的最大值,为0不限制
	unsigned int	_dwWordNum;				// 目前文字个数

	DWORD			_dwFontColor;			// 当前字体颜色
	int				_dwFontIndex;			// 当前字体

};

inline void	CEditArticle::_ToSelectMode()
{
	if( _eRunType==enumNormal )
	{
		_eRunType = enumSelect;
		_nSelStart = _nSelEnd = _nCursor;
	}
}

inline bool CEditArticle::AddControl( char c )
{
	CEditControl* pChar = new CEditControl(c);
	if( !_AddObj( pChar ) )
	{
		//delete pChar;
		SAFE_DELETE(pChar); // UI当机处理
		return false;
	}
	return true;
}

inline bool CEditArticle::AddChar( char c1, char c2 )
{	
	CEditChar* pChar = new CEditChar(c1, c2);
	pChar->SetColor( _dwFontColor );
	pChar->SetFont( _dwFontIndex );
	if( !_AddObj( pChar ) )
	{
		//delete pChar;
		SAFE_DELETE(pChar); // UI当机处理
		return true;
	}
	return false;
}

inline bool CEditSentence::AddChar( DWORD dwIndex, CEditChar* pChar )
{
	if( !IsSameType( pChar ) ) return false;

	if( dwIndex>=_chars.size() )
	{
		_szString+=pChar->GetWord();
		_chars.push_back( pChar );
	}
	else
	{
		chars::iterator where =_chars.begin() + dwIndex;
		_chars.insert( where, pChar );
		_szString = "";
		for( chars::iterator it=_chars.begin(); it!=_chars.end(); it++ )
			_szString = (*it)->GetWord();
	}
	return true;
}

}
