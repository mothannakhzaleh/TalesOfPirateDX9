//----------------------------------------------------------------------
// 名称:静态图片
// 作者:lh 2004-07-21
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"

namespace GUI
{

// 一张简单静态图片
class CImage : public CCompent
{
public:
	CImage(CForm& frmOwn, unsigned int max=1 );
	CImage( const CImage& rhs );
	CImage& operator=(const CImage& rhs);
	virtual ~CImage(void);
	GUI_CLONE(CImage)

	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual void	SetAlpha( BYTE alpha ){ _pImage->SetAlpha(alpha); }

	CGuiPic*		GetImage() { return _pImage; }

public:
	GuiMouseEvent	evtMouseDown;		// 鼠标按下事件

public:
	CGuiPic* _pImage;

};

// 带边框的图片控件
class CFrameImage : public CCompent
{
public:
	CFrameImage(CForm& frmOwn );
	CFrameImage( const CFrameImage& rhs );
	CFrameImage& operator=(const CFrameImage& rhs);
	virtual ~CFrameImage(void);
	GUI_CLONE(CFrameImage)

	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual void	SetAlpha( BYTE alpha ){ _pImage->SetAlpha(alpha); }

	CFramePic*		GetFrameImage() { return _pImage; }

public:
	GuiMouseEvent	evtMouseDown;		// 鼠标按下事件

protected:
	CFramePic*		_pImage;

};

//带效果的图片
class CFlashImage: public CCompent
{
public:
	enum eStyle
	{
		enumNone,		// 静止图片
		enumFlash,		// 闪烁
		enumCartoon,	// 动画
		enumRightMove,	// 从左至右移动
	};

	CFlashImage(CForm& frmOwn, unsigned int max=1 );
	CFlashImage( const CFlashImage& rhs );
	CFlashImage& operator=(const CFlashImage& rhs);
	virtual ~CFlashImage(void);
	GUI_CLONE(CFlashImage)

	virtual void	Init();
	virtual void	Render();
	virtual void	Refresh();
	virtual void	SetAlpha( BYTE alpha )	{ _pImage->SetAlpha(alpha); }
	CGuiPic*		GetImage()				{ return _pImage;			}
    virtual void    FrameMove( DWORD dwTime );
	virtual bool	IsFrameMove()			{ return true;				}

	void    SetInterval( DWORD n )			{ _dwInterval = n;			}
	void	SetStyle( eStyle v )			{ _eStyle=v;				}

protected:
	CGuiPic*		_pImage;

	eStyle			_eStyle;
	DWORD			_dwInterval;
	DWORD			_dwLastTime;

	float			_fPos;
	int				_nMoveWidth;
	int				_nRightMoveX;
	int				_nImageStart;

private:
	void		_Copy( const CFlashImage& rhs );

};

}
