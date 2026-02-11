#pragma once
#include "UIGlobalVar.h"
#include "uiform.h"
#include "uigoodsgrid.h"

class CCharacter;

namespace GUI
{
#define defMaxBoat 3 // 可获得最大船只数

class CBoat // 船信息类
{
public:
	CBoat();
	bool Init( int n, CForm* ship, GuiDragInGridEvent evt );

	void		Reset(); // 重新设置船
	void		Link( CCharacter* pBoat )	{ _chtBoat = pBoat;			}  // 船人物和角色人物对应
	bool		GetIsValid()				{ return _chtBoat!=NULL;	}  // 获得角色信息
	void		UnLink()					{ _chtBoat=NULL;			}  

	CCharacter* GetCha()			{ return _chtBoat;		}  // 找人物
	CGoodsGrid*	GetGoodsGrid()		{ return _grdHold;		}  // 获得角色道具
	CForm*		GetForm()			{ return _frmShipRoom;	}  // 找船道具栏信息

private:
	static void _evtHoldGridRMouse(CGuiData *pSender,CCommandObj* pItem,int nGridID);
	static void _evtEscClose( CForm* pForm );	// 按 ESC 关闭窗体事件  add by Philip.Wu  2006-06-22

private:
	int				_nIndex;

	CCharacter*		_chtBoat;
	CForm*			_frmShipRoom;
	CGoodsGrid*		_grdHold;

};

class CBoatMgr : public CUIInterface
{
public:
	void	Clear();

	bool	AddBoat( CCharacter* pBoat );  // 用户添加船只
	CBoat*	GetBoat( unsigned int n )		{ return &_cBoats[n];	} // 获取船只
	CBoat*	FindBoat( unsigned int ulWorldID );  // 找到用户全部船只

	void	SetHuman( CCharacter* p )		{ _pHuman = p;			} 
	CCharacter*		GetHuman()				{ return _pHuman;		} // 查找当前角色

	CGoodsGrid*		FindGoodsGrid( unsigned int ulWorldID );	// 根据ID查找对应的道具栏
	CCharacter*		FindCha( unsigned int ulWorldID );         // 查找船主人
	CCharacter*		FindCha( CGoodsGrid* pGoods );   // 由装备匹配查找船只的主人

	CCharacter*		ChangeMainCha( unsigned int ulWorldID );	// 切换主角

	CBoat*	GetOtherBoat()					{ return &_cOther;		}  // 查看别人船

protected:
	virtual bool Init(); //用户船信息初始化
    virtual void End(); 
	virtual void SwitchMap()				{	Clear();			} // 跳转地图时清除船信息

private:
	CBoat*		GetFreeBoat(); // 获得免费船只

private:
	CCharacter*	_pHuman;
	CBoat		_cBoats[defMaxBoat];        // 当前角色可拥有的船数量
	CBoat		_cOther;					// 一个用于查看别人的船舱，货物时的临时变量　

};

}
