//----------------------------------------------------------------------
// 名称:技能框
// 作者:lh 2004-12-13
// 用途:双击可以使用技能,升级时显示一个升级按钮,有当前级别,魔法值
//      可拖动到快捷栏,可双击执行,因些有Command系列
//      有动态可显示按钮用于升级,因此要响应鼠标
//      有静态文字,表示技能名称
//      有动态文字,表示当前级别,消耗魔法值
//      有上下换行,换页,到头尾等,要响应键盘
//   注:本来这个类可以直接使用CList,只要派生CListItems即可,但考虑到功能较CList更多更复杂,
//      后期可能有改动,所以新开一个类,避免CList系列可能太过复杂
// 最后修改日期:
//----------------------------------------------------------------------
#pragma once
#include <vector>
#include "uicompent.h"
#include "uiskillcommand.h"
#include "UITextButton.h"
#include "UIScroll.h"

namespace GUI
{

class CSkillList;
typedef void (*GuiSkillListUpgrade) (CSkillList *pSender, CSkillCommand* pSkill);

class CSkillList : public CCompent
{
public:
    CSkillList(CForm& frmOwn);
    CSkillList( const CSkillList& rhs );
    CSkillList& operator=(const CSkillList& rhs);
    virtual ~CSkillList(void);
    GUI_CLONE(CSkillList)

    virtual void	Init();
    virtual void	Render();
    virtual void	Refresh();
    virtual bool	MouseRun( int x, int y, DWORD key );
    virtual bool	MouseScroll( int nScroll );
    virtual void	SetAlpha( BYTE alpha );
    virtual bool	IsHandleMouse()				{ return true;			}
    virtual bool    OnKeyDown( int key );
    virtual void	DragRender();
    virtual CCompent* GetHintCompent( int x, int y );

    virtual void        SetMargin( int left, int top, int right, int bottom );
    virtual CGuiPic*    GetImage()              { return _pImage;       }
    
    CGuiPic*        GetButtonImage()            { return _pButton;      }
    void            Clear();
    void			SetRowHeight( int v )		{ _nRowHeight = v;	    }
    void            SetUnitSize( int w, int h ) { _nUnitHeight=h; _nUnitWidth=w;        }
    void            SetFontLeft( int x )        { _nFontStart = x;      }
    void            SetFontColor( DWORD color ) { _dwFontColor = color; }
    void            SetIsShowUpgrade( bool v )  { _IsShowUpgrade = v;   }

    CSkillCommand*  GetCommand( unsigned int  v );
    bool            AddCommand( CSkillCommand* p );
    CSkillCommand*  FindSkill( int nID );
    bool            DelSkill( int nID );

	int				GetCount()					{ return (int)_skills.size();			}

    CGuiPic*        GetSelect()                 { return _pSelect;      }
    CScroll*        GetScroll()                 { return _pScroll;      }

    int             FindCommand( CSkillCommand* p );

public:
    GuiSkillListUpgrade     evtUpgrade;

private:
    void    _SetSelf();
    void    _Copy( const CSkillList& rhs );
    void    _SetFirstShowRow( DWORD );
	int		_GetHitSkill( int x, int y );

private:
    static void			_ScrollChange(CGuiData *pSender) {
        ((CSkillList*)(pSender->GetParent()))->_OnScrollChange();
    }
    void				_OnScrollChange();
    void	_ResetPageNum();

private:
    static	void	_DragEnd(CGuiData *pSender, int x, int y, DWORD key)	{
        ((CSkillList*)(pSender))->_DragEnd( x, y, key );
    }
    void			_DragEnd( int x, int y, DWORD key );

protected:
    CGuiPic*		_pImage;		// 背景图
    CGuiPic*		_pSelect;	    // 选择条
    CGuiPic*        _pButton;       // 升级按钮
    CScroll*		_pScroll;	    

    typedef std::vector<CSkillCommand*> skills;
    skills          _skills;

    // 与周围边框的距离
    int				_nLeftMargin;	
    int				_nTopMargin;	
    int				_nRightMargin;	
    int				_nBottomMargin;	

    int				_nRowHeight;    // 行高
    int             _nFontStart;    // 字体开始位置
    DWORD           _dwFontColor;

    int             _nUnitHeight, _nUnitWidth;   // 单元图片宽高

    int             _nSelectIndex;  // 选择的行数
    bool            _IsShowUpgrade;

private:
    int             _nShowFirst;    // 显示的第一行
    int             _nShowLast;     // 显示的最后一行
    int				_nShowCount;    // 总共可以显示多少行    

    // 用于显示技能的范围
    int             _nSX1, _nSY1, _nSX2, _nSY2;   

    int             _nButtonX1;     // 按钮显示的X位置
    int             _nButtonOffY;   // 按钮显示的Y轴偏移

    int             _nRowSpace;     // 行距
    int             _nFontYOff;     // 字显示在一行的中部高度

private:
    int             _nDragIndex;
    int             _nDragOffX, _nDragOffY;
    int             _nMouseUpgradeID;

};


// 内联函数 
inline 	void CSkillList::_ResetPageNum() 
{ 
    _pScroll->SetPageNum( ( GetHeight() - _nTopMargin - _nBottomMargin ) / _nRowHeight - 1 );
}

inline CSkillCommand* CSkillList::GetCommand( unsigned int  v )
{
    if( v>=_skills.size() ) return NULL;
    return _skills[v];
}

inline int CSkillList::_GetHitSkill( int x, int y )
{
    if( x>=_nSX1 && x<=_nSX2 && y>=_nSY1 && y<=_nSY2 )
    {
        int h = ( _nShowCount * _nRowHeight + _nSY1 );
        if( y >= h ) return false;

        // 得到行数
        int row = ( y - _nSY1 ) / _nRowHeight + _nShowFirst;
        if( row>=0 && row<(int)_skills.size() ) 
			return row;
	}
	return -1;
}

}
