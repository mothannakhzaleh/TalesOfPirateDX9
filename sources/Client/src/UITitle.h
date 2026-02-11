//----------------------------------------------------------------------
// 名称:游戏标题
// 作者:lh 2005-05-23
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI
{

class CTitle : public CCompent
{
	enum ePicPos
	{
		enumLeft,
		enumCenter,
		enumRight,
		enumMax,
	};
public:
	CTitle(CForm& frmOwn);
	CTitle( const CTitle& rhs );
    CTitle& operator=(const CTitle& rhs);
	virtual ~CTitle(void);
	GUI_CLONE(CTitle)

	virtual void	Init();
	virtual void	Render();
	virtual void	SetCaption( const char * str );
	const char*		GetCaption()					{ return _strCaption.c_str();	}
	virtual CGuiPic*	GetImage()					{ return _pImage;				}
	virtual void		SetTextColor( DWORD color ) { _dwColor = color;				}

	void			SetFont( unsigned int nFont )	{ _nFontIndex=nFont;			}
	void			SetFontH( int v )				{ _nFontH=v;			}
	void			SetShowTime( DWORD v )			{ _dwShowTime=v;				}

private:
	void	_Copy( const CTitle& rhs );

protected:
	CGuiPic*		_pImage;

	unsigned int	_nFontIndex;
	std::string			_strCaption;		// 显示在表面的名称
	DWORD			_dwColor;			// 字体颜色
	DWORD			_dwShowTime;
	int				_nFontH;

private:
	BYTE			_alpha;
	DWORD			_endtime;
	int				_posx[enumMax];		// 坐标
	int				_posy;
	int				_posfonty;

};

}
