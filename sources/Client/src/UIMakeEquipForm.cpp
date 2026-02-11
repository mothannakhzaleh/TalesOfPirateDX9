#include "StdAfx.h"

#include "UIMakeEquipForm.h"
#include "UIFormMgr.h"
#include "uiform.h"
#include "UIFastCommand.h"
#include "UIEquipForm.h"
#include "UILabel.h"
#include "UIItemCommand.h"
#include "UIGoodsGrid.h"
#include "UIMemo.h"
#include "PacketCmd.h"
#include "GameApp.h"
#include "UIProgressBar.h"
#include "WorldScene.h"
#include "Character.h"
#include "UIBoxForm.h"
#include "StringLib.h"
#include "ItemRecord.h"

using namespace std;


namespace GUI
{

	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::Init()
	{
		CFormMgr &mgr = CFormMgr::s_Mgr;

		frmMakeEquip = mgr.Find("frmMakeEquip");
		if ( !frmMakeEquip)
		{
			LG("gui", g_oLangRec.GetString(685));
			return false;
		}
		frmMakeEquip->evtEntrustMouseEvent = _MainMouseEvent;
		frmMakeEquip->evtClose = _OnClose;

		cmdRouleau = dynamic_cast<COneCommand*>(frmMakeEquip->Find("cmdRouLeau"));
		if (!cmdRouleau)
		{
			return Error(g_oLangRec.GetString(561),
						 frmMakeEquip->GetName(), 
						 "cmdRouleau");
		}
		cmdRouleau->evtBeforeAccept = _DragEvtRouleau;

		cmdLastEquip = dynamic_cast<COneCommand*>(frmMakeEquip->Find("cmdLastEquip"));
		if (!cmdLastEquip)
		{
			return Error(g_oLangRec.GetString(561),
						 frmMakeEquip->GetName(), 
						 "cmdForgeItem");
		}

		char szBuf[32];
		for (int i(0); i<ITEM_NUM; i++)
		{
			sprintf(szBuf, "cmdItem%d", i);
			cmdItem[i] = dynamic_cast<COneCommand*>(frmMakeEquip->Find(szBuf));
			if (!cmdItem[i]) 
				return Error(g_oLangRec.GetString(561),
							 frmMakeEquip->GetName(), 
							 szBuf);
		}
		cmdItem[0]->evtBeforeAccept = _DragEvtEquipItem0;
		cmdItem[1]->evtBeforeAccept = _DragEvtEquipItem1;
		cmdItem[2]->evtBeforeAccept = _DragEvtEquipItem2;
		cmdItem[3]->evtBeforeAccept = _DragEvtEquipItem3;

		labForgeGold = dynamic_cast<CLabel*>(frmMakeEquip->Find("labForgeGold"));
		if (!labForgeGold)
		{
			return Error(g_oLangRec.GetString(561),
						 frmMakeEquip->GetName(), 
						 "labForgeGold");
		}


		memForgeItemState = dynamic_cast<CMemo*> (frmMakeEquip->Find("memForgeItemState")) ;
		if( !memForgeItemState ) 
			return Error(g_oLangRec.GetString(561), frmMakeEquip->GetName(), "memForgeItemState");	

		btnYes = dynamic_cast<CTextButton*>(frmMakeEquip->Find("btnForgeYes"));
		if( !btnYes ) 
			return Error(g_oLangRec.GetString(561), frmMakeEquip->GetName(), "btnForgeYes");

		return true;
	}

	//-----------------------------------------------------------------------------
	void  CMakeEquipMgr::CloseForm()
	{
	}

	//-----------------------------------------------------------------------------
	void  CMakeEquipMgr::SwitchMap()
	{
		this->Clear();
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::ShowMakeEquipForm(bool bShow)
	{
		this->Clear();

		if (bShow)
		{

			frmMakeEquip->SetPos(150, 150);
			frmMakeEquip->Reset();
			frmMakeEquip->Refresh();
			frmMakeEquip->Show();

			//同时打开玩家的装备栏
			int x = frmMakeEquip->GetX() + frmMakeEquip->GetWidth();
			int y = frmMakeEquip->GetY();
			g_stUIEquip.GetItemForm()->SetPos(x, y);
			g_stUIEquip.GetItemForm()->Refresh();

			if (!(m_isOldEquipFormShow = g_stUIEquip.GetItemForm()->GetIsShow()))
			{
				g_stUIEquip.GetItemForm()->Show();
			}
		}
		else
		{
			frmMakeEquip->Close();
			g_stUIEquip.GetItemForm()->SetIsShow(m_isOldEquipFormShow);
		}

		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::ShowConfirmDialog(long lMoney)
	{
		char szBuf[255] = { 0 };
		sprintf( szBuf, g_oLangRec.GetString(568), lMoney );
		GUI::stSelectBox * pBox = g_stUIBox.ShowSelectBox( _evtConfirmEvent, szBuf, true );
		pBox->frmDialog->evtEscClose = _evtConfirmCancelEvent;
	}

	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsRouleauCommand(COneCommand* oneCommand)
	{
		return (oneCommand == cmdRouleau);
	}

	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsAllCommand(COneCommand* oneCommand)
	{
		if (oneCommand == cmdRouleau)
			return true;
		else if (oneCommand == cmdRouleau)
			return true;
		else
			for (int i(0); i<ITEM_NUM; i++)
				if (cmdItem[i] == oneCommand)
					return true;

		return false;
	}

	int CMakeEquipMgr::GetItemComIndex(COneCommand* oneCommand)
	{
		for (int i(0); i<ITEM_NUM; i++)
			if (cmdItem[i] == oneCommand)
				return i;
		return -1;
	}

	void CMakeEquipMgr::PopGemItem(int iIndex)
	{
		PopItem(iIndex);
	}


	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::DragRouleauToEquipGrid()
	{
		this->PopRouleau();
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::DragItemToEquipGrid(int iIndex)
	{
		switch(m_iType) 
		{
		case MAKE_EQUIP_TYPE:
			if (this->IsMakeGem())
			{
				PopGemItem(iIndex);
			}
			else
			{
				PopEquipItem(iIndex);
			}
			break;
		case EQUIP_FUSION_TYPE:
			if (iIndex == 0)
			{	// 移去外观，同时移去装备
				PopItem(iIndex);

				CItemCommand* pEquipItemCommand = 
					dynamic_cast<CItemCommand*>(cmdItem[1]->GetCommand());
				if (pEquipItemCommand)
				{
					PopItem(1);
				}
			}
			else if (iIndex == 1)
			{	// 移去装备
				PopItem(iIndex);
			}
			else if (iIndex == 2)
			{	// 移去催化剂
				PopItem(iIndex);
			}
			break;
		case EQUIP_UPGRADE_TYPE:
			PopItem(iIndex);
			break;
		}
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::MakeEquipSuccess(long lChaID)
	{
		if( !CGameApp::GetCurScene() ) return;

		CCharacter* pCha = CGameApp::GetCurScene()->SearchByID( lChaID );
		if( !pCha ) return;

		pCha->SelfEffect(FORGE_SUCCESS_EFF_ID);

		if( pCha->IsMainCha() )
			this->ShowMakeEquipForm(false);

	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::MakeEquipFailed(long lChaID)
	{
		if( !CGameApp::GetCurScene() ) return;

		CCharacter* pCha = CGameApp::GetCurScene()->SearchByID( lChaID );
		if( !pCha ) return;

		pCha->SelfEffect(FORGE_FAILED_EFF_ID);

		if( pCha->IsMainCha() )
			this->ShowMakeEquipForm(false);

	}


	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::MakeEquipOther(long lChaID)
	{
			this->ShowMakeEquipForm(false);
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushEquipItem(int iIndex, CItemCommand& rItem)
	{
		CItemCommand* pOldItemCommand = 
			dynamic_cast<CItemCommand*>(cmdItem[iIndex]->GetCommand());
		if (!pOldItemCommand)
			return;

		int iNum(0), iPos(-1);
		// 更新相应的COneCommand的数据
		if (pOldItemCommand->GetIsPile())
		{
		}
		else
		{
			iNum = 1;
		}

		//int iPos = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
		//// 更新相应的EquipList
		//EquipInfo* pEquipInfo = new EquipInfo();
		//pEquipInfo->iPos = iPos;
		//pEquipInfo->iNum = iNum;
		//equipItems[iIndex].push_back(pEquipInfo);

		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PopEquipItem(int iIndex)
	{
		// 清除相应COneCommand的数据

		// 清除相应的EquipList
		ClearEquipList(iIndex);
	}
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushEquipFusionItem(int iIndex, CItemCommand& rItem)
	{
		if (iIndex == 0)
		{
			if (IsAppearanceEquip(rItem))
			{
				PushItem(iIndex, rItem, 1);
			}
			else 
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(686));
				return;
			}
		}
		else if (iIndex == 1)
		{
			CItemCommand* pItemCommand =  
				dynamic_cast<CItemCommand*>(cmdItem[0]->GetCommand());
			if (!pItemCommand)
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(687));
				return;
			}

			// modify by Philip.Wu  2006-06-11
			// 熔合时，外观装备类型为 27，并且属性装备类型为 22 时，不作匹配判断
			if ((pItemCommand->GetItemInfo()->sType == 27 && rItem.GetItemInfo()->sType == 22) ||
				(IsSameAppearEquip(rItem, *pItemCommand)))
			{
				PushItem(iIndex, rItem, 1);
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(688));
				return;
			}
		}
		else if (iIndex == 2)
		{
			if (IsEquipFusionCatalyzer(rItem))
			{
				PushItem(iIndex, rItem, 1);
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(689));
				return;
			}
		}
	}
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PopEquipFusionItem(int iIndex)
	{
		PopItem(iIndex);
	}
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushEquipUpgradeItem(int iIndex, CItemCommand& rItem)
	{
		if (iIndex == 0)
		{
			if (IsFusionEquip(rItem))
			{
				PushItem(iIndex, rItem, 1);
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(690));
				return;
			}
		}
		else if (iIndex == 1)
		{
			if (IsEquipUpgradeSpar(rItem))
			{
				PushItem(iIndex, rItem, 1);
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(691));
				return;
			}
		}
	}
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PopEquipUpgradeItem(int iIndex)
	{
		PopItem(iIndex);
	}
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushRouleau(CItemCommand& rItem)
	{
		// 查看是否已经有卷轴了，如果有则移出该卷轴
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdRouleau->GetCommand());
		if (pItemCommand)
		{
			if (pItemCommand->GetItemInfo()->lID == rItem.GetItemInfo()->lID)
			{
				return;
			}
			else
			{
				PopRouleau();
			}
		}

		// 记录卷轴在物品栏中的位置
		m_iRouleauPos = g_stUIEquip.GetGoodsGrid()->GetDragIndex();

		// 将卷轴相应的物品栏灰调
		rItem.SetIsValid(false);
		// 将创建的Command放入
		CItemCommand* pItemCmd = new CItemCommand(rItem);
		cmdRouleau->AddCommand(pItemCmd);
		pItemCmd->SetIsValid(true);
		
		// 更新相应配方的COneCommand的数据,这里分成两个流程，道具精炼和宝石合成
		if (rItem.GetItemInfo()->sType == GEM_ROULEAU_TYPE)
		{	//宝石合成
			PushNewGems();
		}
		else
		{	//道具精炼
			PushNewEquips(*(rItem.GetItemInfo()));
		}

		this->SetMakeEquipUI();

		return;
	}

	void CMakeEquipMgr::PushNewGems()
	{
	}


	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PopRouleau()
	{
		// 清除相应COneCommand的数据
		if (m_iRouleauPos == -1)
			return;
		
		CCommandObj* pCmdObj = g_stUIEquip.GetGoodsGrid()->GetItem(m_iRouleauPos);
		if (pCmdObj)
			pCmdObj->SetIsValid(true);

		// 删除其中的Command (DelCommand()中调用delete删除)
		cmdRouleau->DelCommand();

		// 清除三个配方COneCommand相应的数据
		for (int i(0); i<ITEM_NUM; ++i)
			PopItem(i);

		// 清除最后合成的物品
		PopLastEquip();

		this->SetMakeEquipUI();

	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushLastEquip(CItemCommand& rItem)
	{
		
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PopLastEquip()
	{
		CCommandObj* pCom = cmdLastEquip->GetCommand();
		if (pCom)
		{
			cmdLastEquip->DelCommand();
		}

	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::ClearEquipList(int iIndex)
	{
		EquipListIter iter = equipItems[iIndex].begin();
		EquipListIter end  = equipItems[iIndex].end(); 
		for (; iter != end; ++iter)
		{
			//delete (*iter);
			SAFE_DELETE(*iter); // UI当机处理
		}
		equipItems[iIndex].clear();

	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::Clear()
	{
		// 清除UI表单数据
		labForgeGold->SetCaption("");
		btnYes->SetIsEnabled(false);


		// 清除卷轴（同时会清除其他Item）
		PopRouleau();
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::ClearEquips()
	{
		// 清除精炼对应的装备栏物品
		EquipListIter iter, end;
		for (int i(0); i<ITEM_NUM; ++i)
		{
			ClearEquipList(i);
		}
		
		return;
	}

	bool CMakeEquipMgr::CanPushEquip(int iIndex, CItemCommand& rItem)
	{
		return true;
	}

	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::CanPushStone(int iIndex, CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();
		if (!pItemRecord)
			return false;

		//  不是宝石和精炼石直接返回false
		if (pItemRecord->sType != GEN_STONE_TYPE && pItemRecord->sType != FORGE_STONE_TYPE)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(692));
			return false;
		}

		// 检测另一个位置是否有宝石
		int iOtherIndex = iIndex == 0 ? 1 : 0;
		CItemCommand* pOtherItem = dynamic_cast<CItemCommand* >(cmdItem[iOtherIndex]->GetCommand());
		if (pOtherItem)
		{	//另一个位置有宝石，则仅当ID号相同才返回true
			CItemRecord* pOtherItemRecord = pOtherItem->GetItemInfo();
			if (pItemRecord->lID == pOtherItemRecord->lID)
			{
					return true;
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(693));
				return false;
			}
		}
		else //另一个位置没有宝石，直接返回true
		{
			return true;
		}
		return false;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushItem(int iIndex, CItemCommand& rItem, int iItemNum)
	{
		// 查看原来的Cmd中是否已经有Item了，如果有则移出
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdItem[iIndex]->GetCommand());
		if (pItemCommand)
		{
			PopItem(iIndex);
		}

		if (iItemNum == 1)
		{
			// 记录Item在物品栏中的位置
			EquipInfo* pEquipInfo = new EquipInfo();
			pEquipInfo->iPos = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
			pEquipInfo->iNum = iItemNum;
			equipItems[iIndex].resize(1);
			equipItems[iIndex][0] = pEquipInfo;

			// 将Item相应的物品栏灰调
			rItem.SetIsValid(false);

			// 将创建的Item放入Cmd中，这里用new将会在PopItem()中删除
			CItemCommand* pItemCmd = new CItemCommand(rItem);
			pItemCmd->SetIsValid(true);
			cmdItem[iIndex]->AddCommand(pItemCmd);
		}

	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushNewEquips(CItemRecord& rRouleauRecord)
	{
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PopItem(int iIndex)
	{
		// 删除Cmd中的Item，该Item会在PushItem()中由new生成
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdItem[iIndex]->GetCommand());
		if (pItemCommand)
			cmdItem[iIndex]->DelCommand();	// 该函数将删除delete Item

		// 将Item相应的物品栏灰调
		CCommandObj* pItem(0);
		EquipListIter iter = equipItems[iIndex].begin();
		EquipListIter end  = equipItems[iIndex].end();
		for (; iter != end; ++iter)
		{
			pItem = g_stUIEquip.GetGoodsGrid()->GetItem((*iter)->iPos);
			if (pItem)
			{
				pItem->SetIsValid(true);
			}

		}

		ClearEquipList(iIndex);

		this->SetMakeEquipUI();

		return;
	}


	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::PushGemItem(int iIndex, CItemCommand& rItem)
	{
		if (iIndex < STONE_ITEM_NUM)
		{
			if (this->CanPushStone(iIndex, rItem))
			{
				// 查看原来的Cmd中是否已经有Item了，如果有则移出
				CItemCommand* pItemCommand =  
					dynamic_cast<CItemCommand*>(cmdItem[iIndex]->GetCommand());
				if (pItemCommand)
				{
					PopItem(iIndex);
				}

				// 记录Item在物品栏中的位置
				EquipInfo* pEquipInfo = new EquipInfo();
				pEquipInfo->iPos = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
				pEquipInfo->iNum = 1;
				equipItems[iIndex].push_back(pEquipInfo);

				// 将Item相应的物品栏灰调
				rItem.SetIsValid(false);

				// 将创建的Item放入Cmd中，这里用new将会在PopItem()中删除
				CItemCommand* pItemCmd = new CItemCommand(rItem);
				pItemCmd->SetIsValid(true);
				cmdItem[iIndex]->AddCommand(pItemCmd);
			}
		}
		this->SetMakeEquipUI();
	}
	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsEquipMakeRouleau(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		if (pItemRecord && pItemRecord->sType == GEM_ROULEAU_TYPE || pItemRecord->sType == EQUIP_ROULEAU_TYPE)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsEquipFusionRouleau(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		if (pItemRecord && pItemRecord->sType == EQUIP_FUSION_ROULEAU_TYPE)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsEquipUpgradeRouleau(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		if (pItemRecord && pItemRecord->sType == EQUIP_UPGRADE_ROULEAU_TYPE)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsAppearanceEquip(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		return CItemRecord::IsVaildFusionID(pItemRecord);
		
		//if (pItemRecord && pItemRecord->lID > APPEAR_EQUIP_BASE_ID)
		//if (pItemRecord && 
		//	(pItemRecord->lID >= CItemRecord::enumItemFusionStart && pItemRecord->lID < CItemRecord::enumItemFusionEnd) /*&&
		//	rItem.GetData().GetFusionItemID() > 0*/ )
		//{
		//	return true;
		//}
		//return false;

	}
	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsEquipFusionCatalyzer(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		if (pItemRecord && pItemRecord->sType == EQUIP_FUSION_CATALYZER_TYPE)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsSameAppearEquip(CItemCommand& rEquipItem, CItemCommand& rAppearItem)
	{
		CItemRecord* pEquipRecord = rEquipItem.GetItemInfo();
		CItemRecord* pAppearRecord = rAppearItem.GetItemInfo();

		// comment by Philip.Wu  2006-08-15  允许外观装备也是熔合过的装备
		//if (IsAppearanceEquip(rEquipItem))
		//{
		//	return false;
		//}

		if (pEquipRecord && pAppearRecord&& pEquipRecord->sType == pAppearRecord->sType)
		{
			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	bool CMakeEquipMgr::IsEquipUpgradeSpar(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		if (pItemRecord && pItemRecord->sType == EQUIP_UPGRADE_SPAR)
		{
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------
	bool CMakeEquipMgr::IsFusionEquip(CItemCommand& rItem)
	{
		//CItemRecord* pItemRecord = rItem.GetItemInfo();
		//SItemGrid& rItemData =rItem.GetData();
		//if ((pItemRecord->lID >= CItemRecord::enumItemFusionStart && pItemRecord->lID < CItemRecord::enumItemFusionEnd)
		//	&& rItemData.GetFusionItemID() > 0)
		//{
		//	return true;
		//}
		//return false;
		CItemRecord* pItemRecord = rItem.GetItemInfo();

		if (pItemRecord) {
			short sType = pItemRecord->sType;
			//	Close by alfred.shi 20080912 帽子也可以打磨
			
			switch(sType){
				case enumItemTypeSword		:
				case enumItemTypeGlave		:
				case enumItemTypeBow		:	
				case enumItemTypeHarquebus	:
				case enumItemTypeStylet		:
				case enumItemTypeCosh		:
				case enumItemTypeShield		:
				case enumItemTypeHair		:
				case enumItemTypeClothing	:
				case enumItemTypeGlove		:
				case enumItemTypeBoot		:
				case 25:
				case 26:
				case 27:
				case 81:
				case 82:
				case 83:
					return true;
				default:
					return false;
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------------
	// 回调函数
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_MainMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string name = pSender->GetName();
		if( name=="btnClose"  || name == "btnForgeNo" )  
		{ ///关闭表单

			g_stUIMakeEquip.ShowMakeEquipForm(false);
			return;
		}
		else if( name == "btnForgeYes") 
		{
			if (g_stUIMakeEquip.m_iType == EQUIP_FUSION_TYPE)
			{
				if (!g_stUIMakeEquip.cmdItem[2]->GetCommand())
				{
					g_stUIBox.ShowSelectBox( _evtFusionNoCatalyzerConfirmEvent, 
						g_oLangRec.GetString(694), 
						true );
				}
				else
				{
					g_stUIMakeEquip.SendMakeEquipProtocol();
				}
			}
			else
			{
				g_stUIMakeEquip.SendMakeEquipProtocol();
			}
		}

		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_DragEvtRouleau(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		isAccept = false;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood!=g_stUIEquip.GetGoodsGrid() ) return;

		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand ) return;
		if (!(pItemCommand->GetIsValid())) return;


		switch(g_stUIMakeEquip.m_iType)
		{
		case MAKE_EQUIP_TYPE:
			if (g_stUIMakeEquip.IsEquipMakeRouleau(*pItemCommand))	
			{
				g_stUIMakeEquip.PushRouleau(*pItemCommand);
				g_stUIMakeEquip.SetMakeEquipUI();
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(695));
			}
			break;
		case EQUIP_FUSION_TYPE:
			if (g_stUIMakeEquip.IsEquipFusionRouleau(*pItemCommand))	
			{
				g_stUIMakeEquip.PushRouleau(*pItemCommand);
				g_stUIMakeEquip.SetMakeEquipUI();
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(696));
			}
			break;
		case EQUIP_UPGRADE_TYPE:
			if (g_stUIMakeEquip.IsEquipUpgradeRouleau(*pItemCommand))	
			{
				g_stUIMakeEquip.PushRouleau(*pItemCommand);
				g_stUIMakeEquip.SetMakeEquipUI();
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(697));
			}
			break;
		case ELF_SHIFT_TYPE:	// 精灵二转
			if (g_stUIMakeEquip.IsElfShiftStone(*pItemCommand))
			{
				g_stUIMakeEquip.PushRouleau(*pItemCommand);
				g_stUIMakeEquip.SetMakeEquipUI();
			}
			else
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(698));
			}
		}

		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_DragEvtEquipItem0(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		g_stUIMakeEquip.DragEvtEquipItem(0, pSender, pItem, isAccept);
		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_DragEvtEquipItem1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		g_stUIMakeEquip.DragEvtEquipItem(1, pSender, pItem, isAccept);
		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_DragEvtEquipItem2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		g_stUIMakeEquip.DragEvtEquipItem(2, pSender, pItem, isAccept);
		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_DragEvtEquipItem3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		g_stUIMakeEquip.DragEvtEquipItem(3, pSender, pItem, isAccept);
		return;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_OnClose(CForm* pForm, bool& IsClose)
	{
		g_stUIMakeEquip.Clear();
		CS_ItemForgeAsk(false);

	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_evtConfirmEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		CS_ItemForgeAnswer( nMsgType==CForm::mrYes );
	}
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::_evtConfirmCancelEvent(CForm* pForm)
	{
		CS_ItemForgeAnswer( false );
		pForm->SetIsShow(false);
	}
	//---------------------------------------------------------------------
	void CMakeEquipMgr::_evtFusionNoCatalyzerConfirmEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if (nMsgType==CForm::mrYes)
		{
			g_stUIMakeEquip.SendMakeEquipProtocol();
		}
	}



	//-----------------------------------------------------------------------------
	// 私有函数
	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::DragEvtEquipItem(int index, CGuiData *pSender, CCommandObj* pItem, bool& isAccept)
	{
		isAccept = false;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood!=g_stUIEquip.GetGoodsGrid() ) return;

		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand ) return;
		
		//if (!(pItemCommand->GetIsValid())) return; //removed this line, so you can drag 2x of the same gem in.
		//howver this could lead to issue with apparel, so make sure to check type.
		if (!pItemCommand->GetIsValid()){
			SItemGrid item = pItemCommand->GetData() ;
			int sType = pItemCommand->GetItemInfo()->sType;
			if ((sType != GEN_STONE_TYPE &&  sType !=FORGE_STONE_TYPE ) || item.sNum == 1){
				return; 
			}
		}

		if (!cmdRouleau->GetCommand())
		{
			// by Philip.Wu  修改原先判定逻辑，当顶格关键道具示未放入时，下面的道具不允许拖入
			switch(this->m_iType)
			{
			case MAKE_EQUIP_TYPE:
			case EQUIP_FUSION_TYPE:
			case EQUIP_UPGRADE_TYPE:
				g_pGameApp->MsgBox(g_oLangRec.GetString(699));
				break;

			case ELF_SHIFT_TYPE:
				g_pGameApp->MsgBox(g_oLangRec.GetString(700));
				break;

			default:
				g_pGameApp->MsgBox(g_oLangRec.GetString(701));
				break;
			}

			return;

			// 以下是原先的处理，暂时保留
			//if (IsEquipMakeRouleau(*pItemCommand) || 
			//	IsEquipFusionRouleau(*pItemCommand) || 
			//	IsEquipUpgradeRouleau(*pItemCommand))
			//{
			//	g_pGameApp->MsgBox("请将卷轴放入顶格");
			//	return;
			//}
			//else
			//{
			//	g_pGameApp->MsgBox("请先放入卷轴");
			//	return;
			//}
		}

		switch(g_stUIMakeEquip.m_iType)
		{
		case MAKE_EQUIP_TYPE:
			if (this->IsMakeGem())
			{
				PushGemItem(index, *pItemCommand);
			}
			else
			{
				PushEquipItem(index, *pItemCommand);
			}
			break;

		case EQUIP_FUSION_TYPE:
			PushEquipFusionItem(index, *pItemCommand);
			break;

		case EQUIP_UPGRADE_TYPE:
			PushEquipUpgradeItem(index, *pItemCommand);
			break;

		case ELF_SHIFT_TYPE:
			PushElfShiftItem(index, *pItemCommand);
			break;
		}

		this->SetMakeEquipUI();

		return;
	}

	//-----------------------------------------------------------------------------
	bool CMakeEquipMgr::IsMakeGem()
	{
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdRouleau->GetCommand());
		if (pItemCommand && pItemCommand->GetItemInfo()->sType == GEM_ROULEAU_TYPE)
		{
			return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::SetMakeEquipUI()
	{
		//memForgeItemState->SetCaption("这里写具体的内容");
		//memForgeItemState->ProcessCaption();
		switch(m_iType) 
		{
		case MAKE_EQUIP_TYPE:
			if (IsMakeGem() && cmdItem[0]->GetCommand() && cmdItem[1]->GetCommand())
			{
				labForgeGold->SetCaption(StringSplitNum(CalMakeEquipMoney()));
				btnYes->SetIsEnabled(true);
			}
			else
			{
				labForgeGold->SetCaption("");
				btnYes->SetIsEnabled(false);
			}
			break;
		case EQUIP_FUSION_TYPE:
			if (cmdRouleau->GetCommand() && cmdItem[0]->GetCommand() && cmdItem[1]->GetCommand())
			{
				labForgeGold->SetCaption(StringSplitNum(CalMakeEquipMoney()));
				btnYes->SetIsEnabled(true);
			}
			else
			{
				labForgeGold->SetCaption("");
				btnYes->SetIsEnabled(false);
			}
			break;
		case EQUIP_UPGRADE_TYPE:
			if (cmdRouleau->GetCommand() && cmdItem[0]->GetCommand() && cmdItem[1]->GetCommand())
			{
				labForgeGold->SetCaption(StringSplitNum(CalMakeEquipMoney()));
				btnYes->SetIsEnabled(true);
			}
			else
			{
				labForgeGold->SetCaption("");
				btnYes->SetIsEnabled(false);
			}
		case ELF_SHIFT_TYPE:
			if (cmdRouleau->GetCommand() && cmdItem[0]->GetCommand() && cmdItem[1]->GetCommand())
			{
				labForgeGold->SetCaption(StringSplitNum(CalMakeEquipMoney()));
				btnYes->SetIsEnabled(true);
			}
			else
			{
				labForgeGold->SetCaption("0");
				btnYes->SetIsEnabled(false);
			}

			break;
		}
	}

	//-----------------------------------------------------------------------------
	void CMakeEquipMgr::SendMakeEquipProtocol()
	{
		stNetItemForgeAsk kNetItemForgeAsk;
		kNetItemForgeAsk.chType = char(m_iType);	// 合成

		if (m_iType == MAKE_EQUIP_TYPE)
		{
			if (IsMakeGem())
			{
				// 卷轴0
				kNetItemForgeAsk.SGroup[0].sCellNum = 1;
				kNetItemForgeAsk.SGroup[0].pCell = new SForgeCell::SCell[1];
				kNetItemForgeAsk.SGroup[0].pCell[0].sNum = 1;
				kNetItemForgeAsk.SGroup[0].pCell[0].sPosID = m_iRouleauPos;

				// 宝石1～2
				for (int i(1); i<=STONE_ITEM_NUM; ++i)
				{
					kNetItemForgeAsk.SGroup[i].sCellNum = 1;		// 合成始终是1
					kNetItemForgeAsk.SGroup[i].pCell = new SForgeCell::SCell[1];
					kNetItemForgeAsk.SGroup[i].pCell[0].sNum = equipItems[i-1][0]->iNum;
					kNetItemForgeAsk.SGroup[i].pCell[0].sPosID = equipItems[i-1][0]->iPos;
				}
			}
			else
			{
				for (int i(0); i<ITEM_NUM; ++i)
				{
					kNetItemForgeAsk.SGroup[i].sCellNum = 1;		// 合成始终是1
					kNetItemForgeAsk.SGroup[i].pCell = new SForgeCell::SCell[1];
					kNetItemForgeAsk.SGroup[i].pCell[1].sNum = 1;
					//kNetItemForgeAsk.SGroup[i].pCell[1].sPosID = m_iForgeItemPos[i];
				}

			}
		}
		else
		{	// 溶合和升级
			// 卷轴0
			kNetItemForgeAsk.SGroup[0].sCellNum = 1;
			kNetItemForgeAsk.SGroup[0].pCell = new SForgeCell::SCell[1];
			kNetItemForgeAsk.SGroup[0].pCell[0].sNum = 1;
			kNetItemForgeAsk.SGroup[0].pCell[0].sPosID = m_iRouleauPos;

			int iNum(0);
			if (m_iType == EQUIP_FUSION_TYPE)
			{
				if (!cmdItem[2]->GetCommand())
				{
					iNum = FUSION_NUM-1;
				}
				else
				{
					iNum = FUSION_NUM;
				}
			}
			else if (m_iType == EQUIP_UPGRADE_TYPE)
			{
				iNum = UPGRADE_NUM;
			}
			else if (m_iType == ELF_SHIFT_TYPE)
			{
				iNum = SHIFT_NUM;
			}

			for (int i(1); i<=iNum; ++i)
			{
				kNetItemForgeAsk.SGroup[i].sCellNum = 1;		// 合成始终是1
				kNetItemForgeAsk.SGroup[i].pCell = new SForgeCell::SCell[1];
				kNetItemForgeAsk.SGroup[i].pCell[0].sNum = equipItems[i-1][0]->iNum;
				kNetItemForgeAsk.SGroup[i].pCell[0].sPosID = equipItems[i-1][0]->iPos;
			}

		}

		CS_ItemForgeAsk(true, &kNetItemForgeAsk);

		ShowMakeEquipForm(false);

		return;
	}

	//-----------------------------------------------------------------------------
	long CMakeEquipMgr::CalMakeEquipMoney()
	{
		CItemCommand* pItemCommand(0);
		long iLevelPlusOne = 0, nLevel1 = 0, nLevel2 = 0;		
		switch(m_iType) 
		{
		case MAKE_EQUIP_TYPE:
			return MAKE_EQUIP_MONEY;
			break;
		case EQUIP_FUSION_TYPE:
			pItemCommand = dynamic_cast<CItemCommand*>(cmdItem[1]->GetCommand());	
			return EQUIP_FUSION_MONEY * pItemCommand->GetItemInfo()->sNeedLv;
			break;
		case EQUIP_UPGRADE_TYPE:
			// 装备等级+1的平方*1W(策划文档)
			pItemCommand = dynamic_cast<CItemCommand*>(cmdItem[0]->GetCommand());	
			iLevelPlusOne = pItemCommand->GetData().GetItemLevel() + 1;
			return EQUIP_UPGRADE_MONEY * iLevelPlusOne * iLevelPlusOne;
			break;
		case ELF_SHIFT_TYPE:
			// 精灵二转（策划文档）
			pItemCommand = dynamic_cast<CItemCommand*>(cmdItem[0]->GetCommand());
			nLevel1  = pItemCommand->GetData().GetItemLevel();
			pItemCommand = dynamic_cast<CItemCommand*>(cmdItem[1]->GetCommand());
			nLevel2  = pItemCommand->GetData().GetItemLevel();
			return (nLevel1 >= 60 || nLevel2 >= 60) ? 0 : (60 - nLevel1) * (60 - nLevel2) * 10000; 
			break;
		}
		return 0;
	}


	// 是否是精灵二转所需的恶魔果实
	bool CMakeEquipMgr::IsElfShiftStone(CItemCommand& rItem)
	{
		CItemRecord* pItem = rItem.GetItemInfo();
		if( pItem      != NULL && pItem->lID == 3918 || pItem->lID == 3919 || pItem->lID == 3920 || 
			pItem->lID == 3921 || pItem->lID == 3922 || pItem->lID == 3924 || pItem->lID == 3925)
		{
			return true;
		}

		return false;
	}


	// 是否是精灵
	bool CMakeEquipMgr::IsElfShiftItem(CItemCommand& rItem)
	{
		CItemRecord* pItemRecord = rItem.GetItemInfo();
		if(pItemRecord && pItemRecord->sType == 59)
		{
			return true;
		}

		return false;
	}


	// 放入精灵
	void CMakeEquipMgr::PushElfShiftItem(int iIndex, CItemCommand& rItem)
	{
		CItemCommand* pItemCommand = NULL;
		SItemHint sItemHint;
		memset(&sItemHint, 0, sizeof(SItemHint));
		sItemHint.Convert(rItem.GetData(), rItem.GetItemInfo());

		// 计算精灵等级
		int nLevel = sItemHint.sInstAttr[ITEMATTR_VAL_STR] +
					 sItemHint.sInstAttr[ITEMATTR_VAL_AGI] +
					 sItemHint.sInstAttr[ITEMATTR_VAL_DEX] +
					 sItemHint.sInstAttr[ITEMATTR_VAL_CON] +
					 sItemHint.sInstAttr[ITEMATTR_VAL_STA];

		if(20 > nLevel)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(702));
			return;
		}

		if (iIndex == 0)
		{
			pItemCommand = dynamic_cast<CItemCommand*>(cmdItem[1]->GetCommand());

			// 判断是否是精灵，并且不能与第二格的精灵ID相同
			if (IsElfShiftItem(rItem) && 
				(NULL == pItemCommand || (rItem.GetItemInfo()->lID != pItemCommand->GetItemInfo()->lID)))
			{
				PushItem(iIndex, rItem, 1);
			}
			else 
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(703));
				return;
			}
		}
		else if (iIndex == 1)
		{
			pItemCommand = dynamic_cast<CItemCommand*>(cmdItem[0]->GetCommand());

			// 判断是否是精灵，并且不能与第一格的精灵ID相同
			if (IsElfShiftItem(rItem) && 
				(NULL == pItemCommand || (rItem.GetItemInfo()->lID != pItemCommand->GetItemInfo()->lID)))
			{
				PushItem(iIndex, rItem, 1);
			}
			else 
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(703));
				return;
			}
		}
	}

	void CMakeEquipMgr::PopElfShiftItem(int iIndex)
	{
		PopItem(iIndex);
	}

} // end of namespace GUI
