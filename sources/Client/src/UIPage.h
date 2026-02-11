//----------------------------------------------------------------------
// 名称:换页类
// 作者:lh 2004-08-02
// 设计思想:CPageItem是控件容器,CPage是CPageItem容器,并实现切换CPageItem
// 最后修改日期:2004-10-09
//----------------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uitextbutton.h"
#include "uiimage.h"

namespace GUI
{
class CPage;

// 换页类CPage的页面类
class CPageItem : public CContainer
{
	friend class CPage;
public:
	enum eTitleState
	{
		tsNormal,
		tsActive,
		tsEnd,
	};

public:
	CPageItem(CForm& frmOwn);
	CPageItem( const CPageItem& rhs );
	CPageItem& operator=(const CPageItem& rhs);
	virtual ~CPageItem(void);
	GUI_CLONE(CPageItem)	

	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual void	SetAlpha( BYTE alpha );

    virtual CCompent*   GetHintCompent( int x, int y );

public:
	void			RenderTitle()				{ _pTitle->Render();}

	void			SetIndex( unsigned int n )	{ _nIndex = n;		}
	unsigned int	GetIndex()					{ return _nIndex;	}

	CImage*			GetTitle()					{ return _pTitle;	}
	CImage*			GetBkgImage()				{ return _pImage;	}

protected:
	void			_InActive( bool isActive );

private:
	void			_SetSelf();
	static void		_OnTitleDown(CGuiData *pSender, int x, int y, DWORD key);
	void			_OnTitleDown();

protected:
	CImage			*_pTitle;		// 操作这一页的标题栏,其位置由CPage决定
	CImage			*_pImage;		// 页显示背景图片,充满所有区域
	unsigned int	_nIndex;		// 在Page类里的索引

};

// 换页类
class CPage : public CCompent
{
public:
	enum eButtonPos		// 均为依次排列
	{
		bpLeftUp=0,		// 左上
		bpUpFull,		// 充满上部
        bpCustom,       // 自定义
		bpEnd,
	};

public:
	CPage(CForm& frmOwn);
	CPage( const CPage& rhs );
	CPage& operator=(const CPage& rhs);
	virtual ~CPage(void);
	GUI_CLONE(CPage)

	virtual void	Init();
	virtual void	Render();
	virtual void	Refresh();
	virtual bool	MouseRun( int x, int y, DWORD key );
	virtual bool	IsHandleMouse()								{ return true;					}
	virtual void	SetAlpha( BYTE alpha );

    virtual CCompent*       GetHintCompent( int x, int y );
	virtual CGuiPic*		GetImage()							{ return _pImage;				}
    virtual CCompent*       GetHitCommand( int x, int y );

public:
	void			SetIndex( unsigned int n );		// 激活某一页
	int				GetIndex()									{ return _nIndex;				}
	CPageItem*		GetPage( unsigned int n );

	CPageItem*		NewPage();				// 产生新的一页

	void			SetButtonSize( int w, int h );
	void			SetButtonPutStyle( eButtonPos style )		{ _eButtonPos=style;			}
	void			SetButtonMargin( int n )					{ if(n>0) _nButtonMargin=n;		}

public:
	GuiEvent		evtSelectPage;			// 选择了新的一页

private:
	void			_RefreshButtonPos();

protected:
	unsigned int	_nIndex;				// 当前页		
	unsigned int	_nMaxPage;				// 当前最大页

	CGuiPic*		_pImage;

	typedef std::vector<CPageItem*>	items;
	items			_pages;

	eButtonPos		_eButtonPos;		// 页标题按钮排列方式
	int				_nButtonWidth, _nButtonHeight;		// 页标题按钮宽高
	int				_nButtonMargin;		// 页标题按钮的边距

};

// 内联函数
inline void CPageItem::_OnTitleDown(CGuiData *pSender, int x, int y, DWORD key)
{
	((CPageItem*)(pSender->GetParent()))->_OnTitleDown();
}

inline CPageItem* CPage::GetPage( unsigned int n )
{ 
	if( n<_nMaxPage ) return _pages[n];
	return NULL;
}

inline void	CPage::SetButtonSize( int w, int h ) 
{ 
	if( (w > 0) && (h > 0 ) ) _nButtonWidth=w, _nButtonHeight=h; 
}

}
