//----------------------------------------------------------------------
// 名称:静态文字
// 作者:lh 2004-07-19
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uiguidata.h"

namespace GUI
{

// 图元
class CGraph : public CItemObj
{
public:
	CGraph(const char* file, int w, int h, int sx, int sy, int maxframe) : nTag(0) {
		_pImage = new CGuiPic(NULL, maxframe);
		_pImage->LoadAllImage( file, w, h, sx, sy );
	}
	CGraph( const CGuiPic& rhs ) {
		_pImage = new CGuiPic( rhs );
	}
	CGraph( const CGraph& rhs )	: CItemObj(rhs), _pImage(new CGuiPic(*rhs._pImage)), nTag(rhs.nTag)	{}
	CGraph& operator=(const CGraph& rhs) { CItemObj::operator=(rhs) ;*_pImage=*rhs._pImage; nTag=rhs.nTag; return *this;}
	CGraph() : nTag(0)		{ _pImage = new CGuiPic(NULL, 1);	}
	~CGraph()				{ SAFE_DELETE(_pImage); } // UI当机处理 //delete _pImage;					}
	ITEM_CLONE(CGraph)

	CGraph(unsigned int frame) : nTag(0)	{_pImage = new CGuiPic(NULL, frame);	}

	// Grid接口
	virtual void	Render( int x, int y )	{ _pImage->Render(x, y);	    }

    virtual int     GetWidth()				{ return _pImage->GetWidth();	}
    virtual int     GetHeight()				{ return _pImage->GetHeight();	}

	void		Next()					{ _pImage->Next();			}
	void		Reset()					{ _pImage->SetFrame(0);		}

	// 外部接口
	CGuiPic*	GetImage()				{ return _pImage;			}

	int			nTag;

protected:
	CGuiPic*	_pImage;

};

// 带hint的图元,用于编辑器树中节点
class CHintGraph : public CGraph
{
public:
    CHintGraph( char* file, int w, int h, int sx, int sy, int maxframe );
    CHintGraph(unsigned int maxframe);

    virtual bool    HasHint()           { return !_strHint.empty();     }
    virtual void    RenderHint( int x, int y );

public:
    void    SetHint( const char* str )  { _strHint = str;               }

protected:
    std::string  _strHint;

};

// 用于聊天的头像
class CTextGraph : public CHintGraph
{
public:
    CTextGraph( char* file, int w, int h, int sx, int sy, int maxframe );
    CTextGraph(unsigned int maxframe);
    ~CTextGraph();
	virtual DWORD    GetColor()        { return _color;                        }
	virtual void	Render( int x, int y );
	virtual void	SetString( const char* str ){ _strName=str;				}

	const char*     GetString()					{ return _strName.c_str();	}
	virtual void	SetColor(DWORD color)		{ _color=color;				}

	void			SetPointer( void* v )		{ _pVoid = v;					}
	void*			GetPointer()				{ return _pVoid;				}

protected:
    std::string			_strName;
	DWORD			_color;

	void*			_pVoid;

};

// 用于树型控件的表格节点
class CNoteGraph : public CHintGraph
{
public:
    CNoteGraph( unsigned int maxframe );

	virtual void	Render( int x, int y );

	virtual void	SetString( const char* str ){ _strName=str;				}
	const char*     GetString()					{ return _strName.c_str();	}

	virtual void	SetColor(DWORD color)		{ _color=color;				}

	void			SetTextX( int v )			{ _nTextX = v;				}	
	void			SetTextY( int v )			{ _nTextY = v;				}	

protected:
    std::string			_strName;
	DWORD			_color;
	int				_nTextX, _nTextY;	// 文字的X,Y偏移

};

}
