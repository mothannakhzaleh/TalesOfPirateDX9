#include "StdAfx.h"
#include "uiboatform.h"
#include "uiform.h"
#include "uiedit.h"
#include "uilabel.h"
#include "tools.h"
#include "uiformmgr.h"
#include "character.h"
#include "uigoodsgrid.h"
#include "uitemplete.h"
#include "uiequipform.h"
#include "uitradeform.h"
#include "gameapp.h"
#include "uistartform.h"
#include "uibourseForm.h"
#include "uiItemCommand.h"

using namespace GUI;

//---------------------------------------------------------------------------
// class CBoat  用户船类
//---------------------------------------------------------------------------
CBoat::CBoat() 
: _chtBoat(0), _frmShipRoom(0)
, _grdHold(0), _nIndex(0) 
{
}

bool CBoat::Init( int n, CForm* ship, GuiDragInGridEvent evt )  // 用户船信息初始化
{
	_frmShipRoom = ship;
	if( !_frmShipRoom ) return false;

	_nIndex = n;

	_grdHold = dynamic_cast<CGoodsGrid*>(_frmShipRoom->Find("grdRoom"));
	if( !_grdHold ) 
	{
		LG("gui", g_oLangRec.GetString(443) );
		return false;
	}

	_grdHold->SetIsHint(false);
	_grdHold->SetParent(ship);
	_frmShipRoom->evtEscClose = _evtEscClose;	// 按 ESC 关闭事件 add by Philip.Wu  2006-06-22

	// 如果没有事件，则为临时船，不能做任何操作
	if( evt )
	{
		_grdHold->evtBeforeAccept = evt;
		_grdHold->evtThrowItem = CEquipMgr::evtThrowItemEvent;
		_grdHold->evtSwapItem = CEquipMgr::evtSwapItemEvent;
		_grdHold->evtRMouseEvent = _evtHoldGridRMouse;
	}
	return true;
}

void CBoat::_evtHoldGridRMouse(CGuiData *pSender,CCommandObj* pItem,int nGridID)
{
	if (!pItem) return;  

	//交易所右键
	if (g_stUIBourse.GetForm()->GetIsShow())
	{
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(pItem);
		if (!pkItemCmd)	return;

		g_stUIBourse.SaleGoods(*pkItemCmd, nGridID);
	}
}

void CBoat::Reset()  //  如果角色不存在就清除用户船信息
{
	_chtBoat = NULL;
	_grdHold->Clear();
}


// 按 ESC 关闭窗体事件，将父窗体也设置为不显示  add by Philip.Wu  2006-06-22
void CBoat::_evtEscClose( CForm* pForm )
{
	if(pForm)
	{
		pForm->SetIsShow(false);

		if(pForm->GetParent())
		{
			pForm->GetParent()->SetIsShow(false);
		}
	}
}


//---------------------------------------------------------------------------
// class CBoatMgr 用户船管理类
//---------------------------------------------------------------------------
bool CBoatMgr::Init() // 用户船管理信息初始化
{
	CForm* frm[defMaxBoat+1] = { 0 }; 
	CForm* frmShipRoom = _FindForm("frmShipRoom"); // 查找船表单
	if(!frmShipRoom) return false;
	frm[0] = frmShipRoom;

	CForm* frmClone = NULL;
	char buf[128] = { 0 };
	for( int i=1; i<defMaxBoat+1; i++ ) // 
	{
		frmClone = dynamic_cast<CForm*>( frmShipRoom->Clone() );
		if( !frmClone ) return false;

		sprintf( buf, "frmShipRoom%d", i );
		frmClone->SetName( buf );

		CFormMgr::s_Mgr.AddForm( frmClone, enumMainForm );
		frmClone->Init();

		frm[i] = frmClone;
	}

	GuiDragInGridEvent evtGrid = _evtDragToGoodsEvent;
	for( int i=0; i<defMaxBoat; i++ )
	{
		if( !_cBoats[i].Init( i, frm[i], evtGrid ) ) return false;
	}

	if( !_cOther.Init( defMaxBoat+1, frm[defMaxBoat], NULL ) )
		return false;

	_pHuman = NULL;
	return true;
}

void CBoatMgr::End()
{

}

bool CBoatMgr::AddBoat( CCharacter* pBoat )
{
	CBoat* p = GetFreeBoat();
	if( !p ) return false; //已经得到最大数量 不能再获得免费的船只

	p->Link( pBoat );
	return true;
}

CBoat*	CBoatMgr::GetFreeBoat()   // 获得免费船只
{ 
	for( int i=0; i<defMaxBoat; i++ ) // 可最大获得船只
		if( !_cBoats[i].GetIsValid() )
			return &_cBoats[i];

	return NULL;
}

CBoat* CBoatMgr::FindBoat( unsigned int ulWorldID )  // 查找用户有的全部船
{
	for( int i=0; i<defMaxBoat; i++ )
	{
		if( _cBoats[i].GetIsValid() && _cBoats[i].GetCha()->getAttachID()==ulWorldID )
		{
			return &_cBoats[i];
		}
	}

	return NULL;
}

CGoodsGrid*	CBoatMgr::FindGoodsGrid( unsigned int ulWorldID ) // 查找船物品
{
	if( _pHuman && _pHuman->getAttachID()==ulWorldID )
		return g_stUIEquip.GetGoodsGrid();

	CBoat* pBoat = FindBoat( ulWorldID );
	if( pBoat ) return pBoat->GetGoodsGrid();

	return NULL;
}

CCharacter*	CBoatMgr::FindCha( unsigned int ulWorldID ) // 查找船只主人
{
	if( _pHuman && _pHuman->getAttachID()==ulWorldID ) return _pHuman;

	CBoat* pBoat = FindBoat( ulWorldID );
	if( pBoat ) return pBoat->GetCha();

	return NULL;
}

CCharacter* CBoatMgr::ChangeMainCha( unsigned int ulWorldID ) // 切换主角
{
	CCharacter* pCha = FindCha( ulWorldID );
	if( pCha ) 
	{
		pCha->SetHide( FALSE );

		CGameScene* pScene = CGameApp::GetCurScene();
		if( pScene )
		{
			pScene->SetMainCha( pCha->getID() );
			g_stUIStart.ShowShipSailForm( pCha->IsBoat() );

			// add by Philip.Wu  2006-07-03  切换地图时关闭交易和装备窗体（BUG处理）
			g_stUITrade.CloseAllForm();
			g_stUIEquip.CloseAllForm();
		}
	}
	else
	{
		LG( "error", g_oLangRec.GetString(444), ulWorldID );
	}
	return pCha;
}

void CBoatMgr::Clear()  //  清除用户所有船信息
{
	_pHuman = NULL;
	for( int i=0; i<defMaxBoat; i++ )
	{
		_cBoats[i].Reset();
	}
}

CCharacter* CBoatMgr::FindCha( CGoodsGrid* pGoods ) // 由装备匹配查找船只的主人
{
	if( pGoods==g_stUIEquip.GetGoodsGrid() )
		return _pHuman;

	for( int i=0; i<defMaxBoat; i++ )
	{
		if( _cBoats[i].GetGoodsGrid()==pGoods )
			return _cBoats[i].GetCha();
	}
	return NULL;
}



