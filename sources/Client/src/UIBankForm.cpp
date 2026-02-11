#include "StdAfx.h"

#include "UIBankForm.h"
#include "uiform.h"
#include "uilabel.h"
#include "uiformmgr.h"
#include "uigoodsgrid.h"
#include "NetProtocol.h"
#include "uiboxform.h"
#include "uiEquipForm.h"
#include "UIGoodsGrid.h"
#include "uiItemCommand.h"
#include "uiform.h"
#include "uiBoatForm.h"
#include "packetcmd.h"
#include "Character.h"
#include "GameApp.h"

namespace GUI
{
	//=======================================================================
	//	CBankMgr 's Members
	//=======================================================================

	bool CBankMgr::Init()  //用户银行信息初始化
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;

		frmBank = mgr.Find("frmNPCstorage");// 查找NPC银行存储表单 
		if ( !frmBank)
		{
			LG("gui", g_oLangRec.GetString(438));
			return false;
		}
		frmBank->evtClose = _evtOnClose; 

		grdBank = dynamic_cast<CGoodsGrid*>(frmBank->Find("grdNPCstorage"));
		if (!grdBank) 
			return Error(g_oLangRec.GetString(439),
						 frmBank->GetName(), "grdNPCstorage");
		grdBank->evtBeforeAccept = CUIInterface::_evtDragToGoodsEvent;// 消息的处理 有拖入银行就会调用 CUIInterface::_evtDragToGoodsEvent
		grdBank->evtSwapItem = _evtBankToBank;
		labCharName = dynamic_cast<CLabel*>(frmBank->Find("labOwnerName"));
		if (!grdBank) 
			return Error(g_oLangRec.GetString(439),
						 frmBank->GetName(), "labOwnerName");

		return true;
	}

	void CBankMgr::_evtOnClose( CForm* pForm, bool& IsClose )  // 关闭用户银行
	{
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_CLOSE_BANK, NULL); 

		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_BANK, true);		// 西门文档修改
	}


	//-------------------------------------------------------------------------
	void CBankMgr::ShowBank() // 显示物品
	{
		// 保存服务器传来的物品

		if (!g_stUIBoat.GetHuman()) // 找人物 
			return;

		char szBuf[32];
		sprintf(szBuf, "%s%s", g_stUIBoat.GetHuman()->getName(), g_oLangRec.GetString(440));//显示人物名及专用
		labCharName->SetCaption(szBuf);//设置标题名字

		frmBank->Show();

		// 打开玩家的物品栏
		if (!g_stUIEquip.GetItemForm()->GetIsShow())
		{
			int nLeft, nTop;
			nLeft = frmBank->GetX2();
			nTop = frmBank->GetY();

			g_stUIEquip.GetItemForm()->SetPos(nLeft, nTop); //物品放置位置
			g_stUIEquip.GetItemForm()->Refresh(); //更新物品栏		
			g_stUIEquip.GetItemForm()->Show();  //保存在物品栏
		}

		CFormMgr::s_Mgr.SetEnableHotKey(HOTKEY_BANK, false);		// 西门文档修改
	}

	//-------------------------------------------------------------------------
	bool CBankMgr::PushToBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
#define EQUIP_TYPE 0
#define BANK_TYPE 1

		// 设置发送拖动物品的服务器信息
		m_kNetBank.chSrcType = EQUIP_TYPE;
		m_kNetBank.sSrcID = rkDrag.GetDragIndex();
		//m_kNetBank.sSrcNum = ; 数量在回调函数中设置
		m_kNetBank.chTarType = BANK_TYPE;
		m_kNetBank.sTarID = nGridID;

		// 判断物品是否是可重叠的物品
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	return false;
		CItemRecord* pkItemRecord = pkItemCmd->GetItemInfo();
		if (!pkItemRecord)	return false;

		//if(pkItemRecord->sType == 59 && m_kNetBank.sSrcID == 1)
		//{
		//	g_pGameApp->MsgBox("您的精灵正在使用中\n请更换到其它位置才可放入仓库");
		//	return false;	// 第二格的精灵不允许拖入银行
		//}

		// if(pkItemRecord->lID == 2520 || pkItemRecord->lID == 2521)
		if( pkItemRecord->lID == 2520 || pkItemRecord->lID == 2521 || pkItemRecord->lID == 6341 || pkItemRecord->lID == 6343
		 || pkItemRecord->lID == 6347 || pkItemRecord->lID == 6359 || pkItemRecord->lID == 6370 || pkItemRecord->lID == 6371
		 || pkItemRecord->lID == 6373 || pkItemRecord->lID >= 6376 && pkItemRecord->lID <= 6378
		 || pkItemRecord->lID >= 6383 && pkItemRecord->lID <= 6385 )// modify by ning.yan 20080820 策划绵羊、李恒等提需求，增加一些道具不准存银行
		{
			//g_pGameApp->MsgBox(g_oLangRec.GetString(958));	// "该道具不允许存入银行！请重新选择"
			g_pGameApp->MsgBox(g_oLangRec.GetString(958));	// "该道具不允许存入银行！请重新选择"
			return false;
		}
		if ( pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1 )
		{	/*存放多个物品*/
			m_pkNumberBox = 
				g_stUIBox.ShowNumberBox(_MoveItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(441), false);

			if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
				return false;
			else
				return true;
		}
		else
		{	/*存放单个物品*/
			g_stUIBank.m_kNetBank.sSrcNum = 1;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
			return true;
			//char buf[256] = { 0 };
			//sprintf(buf, "您确认放入银行\n[%s]?", pkItemCmd->GetName());
			//g_stUIBox.ShowSelectBox(_MoveAItemEvent, buf, true);
			//return true;
		}
	}

	//-------------------------------------------------------------------------
	bool CBankMgr::PopFromBank(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem)
	{
		// 设置发送拖动物品的服务器信息
		m_kNetBank.chSrcType = BANK_TYPE ;
		m_kNetBank.sSrcID = rkDrag.GetDragIndex();
		//m_kNetBank.sSrcNum = ; 数量在回掉函数中设置
		m_kNetBank.chTarType = EQUIP_TYPE;
		m_kNetBank.sTarID = nGridID;


		// 判断物品是否是可重叠的物品
		CItemCommand* pkItemCmd = dynamic_cast<CItemCommand*>(&rkItem);
		if (!pkItemCmd)	return false;

		if ( pkItemCmd->GetItemInfo()->GetIsPile() && pkItemCmd->GetTotalNum() > 1 )
		{	/*取出多个物品*/
			m_pkNumberBox = 
				g_stUIBox.ShowNumberBox( _MoveItemsEvent, pkItemCmd->GetTotalNum(), g_oLangRec.GetString(442), false);

			if (m_pkNumberBox->GetNumber() < pkItemCmd->GetTotalNum())
				return false;
			else
				return true;
		}
		else
		{	/*存放单个物品*/
			g_stUIBank.m_kNetBank.sSrcNum = 1;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
			return true;

			//char buf[256] = { 0 };
			//sprintf(buf, "您确认取出\n[%s]?", pkItemCmd->GetName());
			//g_stUIBox.ShowSelectBox(_MoveAItemEvent, buf, true);
			//return true;
		}
	}

	//-------------------------------------------------------------------------
	void CBankMgr::_MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey) // 多个物品移动
	{
		if(nMsgType != CForm::mrYes)  // 玩家是否同意拖动
			return;


		int num =  g_stUIBank.m_pkNumberBox->GetNumber();// 拖动物品数 
		if ( num > 0 )
		{
			g_stUIBank.m_kNetBank.sSrcNum = num;
			CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
		}
	}

	//-------------------------------------------------------------------------
	void CBankMgr::_MoveAItemEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey) // 单个道具移动
	{
		if(nMsgType != CForm::mrYes) 
			return;

		g_stUIBank.m_kNetBank.sSrcNum = 1;
		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));//更新银行信息
	}
	
	//-------------------------------------------------------------------------
	void CBankMgr::CloseForm()  // 关闭道具栏表单 
	{
		if (frmBank->GetIsShow())
		{
			frmBank->Close();
			g_stUIEquip.GetItemForm()->Close();
		}
	}

	//-------------------------------------------------------------------------
	void CBankMgr::_evtBankToBank(CGuiData *pSender,int nFirst, int nSecond, bool& isSwap) // 用于用户银行表单中道具互换
	{
		isSwap = false;
		if( !g_stUIBoat.GetHuman() ) return;

		g_stUIBank.m_kNetBank.chSrcType = BANK_TYPE ;
		g_stUIBank.m_kNetBank.sSrcID = nSecond;
		g_stUIBank.m_kNetBank.sSrcNum = 0; 
		g_stUIBank.m_kNetBank.chTarType = BANK_TYPE;
		g_stUIBank.m_kNetBank.sTarID = nFirst;

		CS_BeginAction(g_stUIBoat.GetHuman(), enumACTION_BANK, (void*)&(g_stUIBank.m_kNetBank));
	}


}	// end of namespace GUI
