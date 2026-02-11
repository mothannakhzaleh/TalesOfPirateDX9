
#include "stdafx.h"
#include "GameApp.h"
#include "UISpiritForm.h"
#include "UIEquipForm.h"
#include "uigoodsgrid.h"
#include "packetcmd.h"
#include "character.h"
#include "UIBoxform.h"
#include "packetCmd.h"
#include "uiitemcommand.h"
#include "uinpctalkform.h"
#include "PacketCmd.h"


using namespace std;

namespace GUI
{

	CSpiritMgr::CSpiritMgr(void)
	{
	}

	CSpiritMgr::~CSpiritMgr(void)
	{
	}


	bool CSpiritMgr::Init()
	{
		//
		//  精灵融合界面初始化
		//
		frmSpiritMarry = CFormMgr::s_Mgr.Find("frmSpiritMarry");
		if(! frmSpiritMarry)
		{
			LG("gui", "main.clu   frmSpiritMarry not found.\n");
			return false;
		}

		labMoneyShow = dynamic_cast<CLabel*>(frmSpiritMarry->Find("labMoneyShow"));
		if(! labMoneyShow)
		{
			LG("gui", "main.clu   frmSpiritMarry:labMoneyShow not found.\n");
			return false;
		}

		btnForgeYes = dynamic_cast<CTextButton*>(frmSpiritMarry->Find("btnForgeYes"));
		if(! btnForgeYes)
		{
			LG("gui", "main.clu   frmSpiritMarry:btnForgeYes not found.\n");
			return false;
		}

		cmdSpiritMarry[SPIRIT_MARRY_ITEM] = dynamic_cast<COneCommand*>(frmSpiritMarry->Find("cmdSpiritItem"));
		if(! cmdSpiritMarry[SPIRIT_MARRY_ITEM])
		{
			LG("gui", "main.clu   frmSpiritMarry:cmdSpiritItem not found.\n");
			return false;
		}

		cmdSpiritMarry[SPIRIT_MARRY_ONE] = dynamic_cast<COneCommand*>(frmSpiritMarry->Find("cmdSpiritOne"));
		if(! cmdSpiritMarry[SPIRIT_MARRY_ONE])
		{
			LG("gui", "main.clu   frmSpiritMarry:cmdSpiritOne not found.\n");
			return false;
		}

		cmdSpiritMarry[SPIRIT_MARRY_TWO] = dynamic_cast<COneCommand*>(frmSpiritMarry->Find("cmdSpiritTwo"));
		if(! cmdSpiritMarry[SPIRIT_MARRY_TWO])
		{
			LG("gui", "main.clu   frmSpiritMarry:cmdSpiritTwo not found.\n");
			return false;
		}

		frmSpiritMarry->evtClose             = _evtCloseMarryForm;
		frmSpiritMarry->evtEntrustMouseEvent = _evtMainMouseButton;

		cmdSpiritMarry[SPIRIT_MARRY_ITEM]->evtBeforeAccept = _evtDragMarryItem;
		cmdSpiritMarry[SPIRIT_MARRY_ONE]->evtBeforeAccept  = _evtDragMarryOne;
		cmdSpiritMarry[SPIRIT_MARRY_TWO]->evtBeforeAccept  = _evtDragMarryTwo;

		//
		//  精灵老虎机界面初始化
		//
		frmSpiritErnie = CFormMgr::s_Mgr.Find("frmSpiritErnie");
		if(! frmSpiritErnie)
		{
			LG("gui", "main.clu   frmSpiritErnie not found.\n");
			return false;
		}
		frmSpiritErnie->SetIsEscClose(false);
		frmSpiritErnie->evtClose             = _evtCloseErnieForm;
		frmSpiritErnie->evtEntrustMouseEvent = _evtErnieMouseButton;

		btnStart = dynamic_cast<CTextButton*>(frmSpiritErnie->Find("btnStart"));
		if(! btnStart)
		{
			LG("gui", "main.clu   frmSpiritErnie:btnStart not found.\n");
			return false;
		}

		char szName[32] = {0};
		for(int i = 0; i < 9; ++i)
		{
			// 卷动图片
			for(int j = 0; j < ERNIE_IMAGE_COUNT; ++j)
			{
				sprintf(szName, "imgLine%d_%d", i + 1, j + 1);
				imgLine[i][j] = dynamic_cast<CImage*>(frmSpiritErnie->Find(szName));

				if(! imgLine[i][j])
				{
					LG("gui", "main.clu   frmSpiritErnie:%s not found.\n", szName);
					return false;
				}
				imgLine[i][j]->SetIsShow(false);
			}
		}

		for(int i = 0; i < 3; ++i)
		{
			// 显示的格子
			for(int j = 0; j < 3; ++j)
			{
				int nSeq = i * 3 + j;
				sprintf(szName, "cmdItemTiger%d", nSeq + 1);
				cmdItem[nSeq] = dynamic_cast<COneCommand*>(frmSpiritErnie->Find(szName));
				if(! cmdItem[nSeq])
				{
					LG("gui", "main.clu   frmSpiritErnie:%s not found.\n", szName);
					return false;
				}
				//cmdItem[nSeq]->SetIsShow(false);
				cmdItem[nSeq]->SetIsDrag(false);
			}

			// 下注 Check
			sprintf(szName, "chkSetmoney%d", i + 1);
			chkSetmoney[i] = dynamic_cast<CCheckBox*>(frmSpiritErnie->Find(szName));
			if(! chkSetmoney[i])
			{
				LG("gui", "main.clu   frmSpiritErnie:%s not found.\n", szName);
				return false;
			}

			// 下注硬币个数显示
			sprintf(szName, "labUsemoney%d", i + 1);
			labUsemoney[i] = dynamic_cast<CLabelEx*>(frmSpiritErnie->Find(szName));
			if(! labUsemoney[i])
			{
				LG("gui", "main.clu   frmSpiritErnie:%s not found.\n", szName);
				return false;
			}

			// 停止转动按钮
			sprintf(szName, "btnStop%d", i + 1);
			btnStop[i] = dynamic_cast<CTextButton*>(frmSpiritErnie->Find(szName));
			if(! btnStop[i])
			{
				LG("gui", "main.clu   frmSpiritErnie:%s not found.\n", szName);
				return false;
			}
		}

		labLastshow1 = dynamic_cast<CLabelEx*>(frmSpiritErnie->Find("labLastshow1"));
		if(! labLastshow1)
		{
			LG("gui", "main.clu   frmSpiritMarry:labLastshow1 not found.\n");
			return false;
		}

		labLastshow2 = dynamic_cast<CLabelEx*>(frmSpiritErnie->Find("labLastshow2"));
		if(! labLastshow2)
		{
			LG("gui", "main.clu   frmSpiritMarry:labLastshow2 not found.\n");
			return false;
		}

		return true;
	}


	void CSpiritMgr::CloseForm()
	{
		// 不作任何处理
	}


	// 清除界面上所有的格子
	void CSpiritMgr::ClearAllCommand()
	{
		PopItem(SPIRIT_MARRY_ITEM);
		PopItem(SPIRIT_MARRY_ONE);
		PopItem(SPIRIT_MARRY_TWO);
	}


	// 显示精灵融合界面
	void CSpiritMgr::ShowMarryForm(bool bShow)
	{
		if(frmSpiritMarry)
		{
			if(bShow)
			{
				btnForgeYes->SetIsEnabled(false);

				labMoneyShow->SetCaption("");
				labLastshow1->SetCaption("");
				labLastshow2->SetCaption("");

				frmSpiritMarry->SetPos(100, 100);
				frmSpiritMarry->Refresh();
				frmSpiritMarry->SetIsShow(true);

				int nLeft, nTop;
				nLeft = frmSpiritMarry->GetX2();
				nTop  = frmSpiritMarry->GetY();

				g_stUIEquip.GetItemForm()->SetPos(nLeft, nTop);
				g_stUIEquip.GetItemForm()->Refresh();
				g_stUIEquip.GetItemForm()->Show();
			}
			else
			{
				frmSpiritMarry->SetIsShow(false);
			}
		}
	}


	void CSpiritMgr::ShowErnieForm(bool bShow)
	{
		if(bShow)
		{
			m_nCurrImage      = 0;

			m_bIsRunning[0] = false;
			m_bIsRunning[1] = false;
			m_bIsRunning[2] = false;

			btnStart->SetIsEnabled(true);

			chkSetmoney[0]->SetIsChecked(false);
			chkSetmoney[1]->SetIsChecked(false);
			chkSetmoney[2]->SetIsChecked(false);
			chkSetmoney[0]->SetIsEnabled(true);;
			chkSetmoney[1]->SetIsEnabled(true);;
			chkSetmoney[2]->SetIsEnabled(true);;

			btnStop[0]->SetIsEnabled(false);
			btnStop[1]->SetIsEnabled(false);
			btnStop[2]->SetIsEnabled(false);

			imgLine[0][0]->SetIsShow(true);
			imgLine[1][0]->SetIsShow(true);
			imgLine[2][0]->SetIsShow(true);
			imgLine[3][0]->SetIsShow(true);
			imgLine[4][0]->SetIsShow(true);
			imgLine[5][0]->SetIsShow(true);
			imgLine[6][0]->SetIsShow(true);
			imgLine[7][0]->SetIsShow(true);
			imgLine[8][0]->SetIsShow(true);

			labLastshow1->SetCaption("");
			labLastshow2->SetCaption("");

			frmSpiritErnie->SetIsShow(true);
		}
		else
		{
			frmSpiritErnie->SetIsShow(false);
		}
	}


	// 更新老虎机
	void CSpiritMgr::UpdateErnieNumber(short nNum, short nID1, short nID2, short nID3)
	{
		switch(nNum)
		{
		case 1:
			{
				btnStop[1]->SetIsEnabled(true);

				for(int i = 0; i < ERNIE_IMAGE_COUNT; ++i)
				{
					imgLine[0][i]->SetIsShow(false);
					imgLine[1][i]->SetIsShow(false);
					imgLine[2][i]->SetIsShow(false);
				}

				m_bIsRunning[0] = false;
				AddTigerItem(0, nID1);
				AddTigerItem(1, nID2);
				AddTigerItem(2, nID3);
			}
			break;

		case 2:
			{
				btnStop[2]->SetIsEnabled(true);
				for(int i = 0; i < ERNIE_IMAGE_COUNT; ++i)
				{
					imgLine[3][i]->SetIsShow(false);
					imgLine[4][i]->SetIsShow(false);
					imgLine[5][i]->SetIsShow(false);
				}

				m_bIsRunning[1] = false;
				AddTigerItem(3, nID1);
				AddTigerItem(4, nID2);
				AddTigerItem(5, nID3);
			}
			break;

		case 3:
			{
				for(int i = 0; i < ERNIE_IMAGE_COUNT; ++i)
				{
					imgLine[6][i]->SetIsShow(false);
					imgLine[7][i]->SetIsShow(false);
					imgLine[8][i]->SetIsShow(false);
				}

				m_bIsRunning[2] = false;
				AddTigerItem(6, nID1);
				AddTigerItem(7, nID2);
				AddTigerItem(8, nID3);

				btnStart->SetIsEnabled(true);
				chkSetmoney[0]->SetIsEnabled(true);
				chkSetmoney[1]->SetIsEnabled(true);
				chkSetmoney[2]->SetIsEnabled(true);
			}
			break;

		default:
			break;
		}
	}


	void CSpiritMgr::PushItem(int iIndex, CItemCommand& rItem)
	{
		// 查看原来的Cmd中是否已经有Item了，如果有则移出
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdSpiritMarry[iIndex]->GetCommand());
		if (pItemCommand)
		{
			PopItem(iIndex);
		}

		// 记录Item在物品栏中的位置
		m_iSpiritItemPos[iIndex] = g_stUIEquip.GetGoodsGrid()->GetDragIndex();
		// 将Item相应的物品栏灰调
		rItem.SetIsValid(false);

		// 将创建的Item放入Cmd中，这里用new将会在PopItem()中删除
		CItemCommand* pItemCmd = new CItemCommand(rItem);
		pItemCmd->SetIsValid(true);
		cmdSpiritMarry[iIndex]->AddCommand(pItemCmd);

		// 属性
		SItemGrid& oItemGridSrc  = rItem.GetData();
		SItemGrid& oItemGridDest = pItemCmd->GetData();
		for(int i = 0; i < defITEM_INSTANCE_ATTR_NUM; ++i)
		{
			oItemGridDest.sInstAttr[i][0] = oItemGridSrc.sInstAttr[i][0];
			oItemGridDest.sInstAttr[i][1] = oItemGridSrc.sInstAttr[i][1];
		}
	}


	void CSpiritMgr::PopItem(int iIndex)
	{
		// 删除Cmd中的Item，该Item会在PushItem()中由new生成
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdSpiritMarry[iIndex]->GetCommand());
		if (! pItemCommand) return;

		cmdSpiritMarry[iIndex]->DelCommand();	// 该函数将删除delete Item

		// 将Item相应的物品栏恢复成可用
		CCommandObj* pItem = 
					g_stUIEquip.GetGoodsGrid()->GetItem(m_iSpiritItemPos[iIndex]);
		if (pItem)
		{
			pItem->SetIsValid(true);
		}

		// 记录Item在物品栏中的位置
		m_iSpiritItemPos[iIndex] = NO_USE;
	}


	// 是否是合法的恶魔果实
	bool CSpiritMgr::IsValidSpiritItem(CItemCommand& rItem)
	{
		CItemRecord* pItem = rItem.GetItemInfo();
		if( pItem      != NULL && pItem->lID == 3918 || pItem->lID == 3919 || pItem->lID == 3920 || 
			pItem->lID == 3921 || pItem->lID == 3922 || pItem->lID == 3924 || pItem->lID == 3925)
		{
			return true;
		}

		return false;
	}


	// 是否是合法的精灵（LV > 20）
	bool CSpiritMgr::IsValidSpirit(CItemCommand& rItem)
	{
		static CItemRecord* pInfo = NULL;
		pInfo = rItem.GetItemInfo();

		static SItemHint s_item;
		memset( &s_item, 0, sizeof(SItemHint) );
		s_item.Convert( rItem.GetData(), pInfo );

		// 更新宠物等级,头像
		int nLevel  = s_item.sInstAttr[ITEMATTR_VAL_STR]
					+ s_item.sInstAttr[ITEMATTR_VAL_AGI] 
					+ s_item.sInstAttr[ITEMATTR_VAL_DEX] 
					+ s_item.sInstAttr[ITEMATTR_VAL_CON] 
					+ s_item.sInstAttr[ITEMATTR_VAL_STA];

		if (pInfo && pInfo->sType == 59 && nLevel >= 20)
		{
			return true;
		}

		return false;
	}


	// 设置界面
	void CSpiritMgr::SetSpiritUI()
	{
		CItemCommand* pItemCommand =  
			dynamic_cast<CItemCommand*>(cmdSpiritMarry[SPIRIT_MARRY_ONE]->GetCommand());
		if(! pItemCommand) return;

		pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(m_iSpiritItemPos[SPIRIT_MARRY_ONE]));
		if(! pItemCommand) return;

		int nLevel1 = pItemCommand->GetData().sInstAttr[0][1] + 
					  pItemCommand->GetData().sInstAttr[1][1] + 
					  pItemCommand->GetData().sInstAttr[2][1] + 
					  pItemCommand->GetData().sInstAttr[3][1] + 
					  pItemCommand->GetData().sInstAttr[4][1];

		pItemCommand = dynamic_cast<CItemCommand*>(cmdSpiritMarry[SPIRIT_MARRY_TWO]->GetCommand());
		if(! pItemCommand) return;

		pItemCommand = dynamic_cast<CItemCommand*>(g_stUIEquip.GetGoodsGrid()->GetItem(m_iSpiritItemPos[SPIRIT_MARRY_TWO]));
		if(! pItemCommand) return;

		int nLevel2 = pItemCommand->GetData().sInstAttr[0][1] + 
					  pItemCommand->GetData().sInstAttr[1][1] + 
					  pItemCommand->GetData().sInstAttr[2][1] + 
					  pItemCommand->GetData().sInstAttr[3][1] + 
					  pItemCommand->GetData().sInstAttr[4][1];

		int nMoney = 0;

		// 计算价格（策划文档）
		if(nLevel1 < 60 && nLevel2 < 60)
		{
			nMoney = (60 - nLevel1) * (60 - nLevel2) * 100;
		}

		char szBuffer[64] = {0};
		sprintf(szBuffer, "%d", nMoney);
		labMoneyShow->SetCaption(szBuffer);

		pItemCommand = dynamic_cast<CItemCommand*>(cmdSpiritMarry[SPIRIT_MARRY_ITEM]->GetCommand());
		if(! pItemCommand) return;

		// 三个栏位都已经放入，允许点击“确定”按钮
		btnForgeYes->SetIsEnabled(true);
	}


	// 提交信息给服务器
	void CSpiritMgr::SendSpiritMarryProtocol()
	{
		CS_ItemForgeAsk(true, GetType(), m_iSpiritItemPos, SPIRIT_MARRY_CELL_COUNT);
	}


///////////////////////////////////////////////////////////////////////////
//
//	界面消息处理
//

	// 拖入恶魔果实
	void CSpiritMgr::_evtDragMarryItem(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand) return;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood != g_stUIEquip.GetGoodsGrid() ) return;

		if(g_stUISpirit.IsValidSpiritItem(*pItemCommand))
		{
			g_stUISpirit.PushItem(SPIRIT_MARRY_ITEM, *pItemCommand);
			g_stUISpirit.SetSpiritUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(698));	// "您放入的不是恶魔果实，请重新选择恶魔果实"
		}
	}


	// 拖入精灵1
	void CSpiritMgr::_evtDragMarryOne(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand) return;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood != g_stUIEquip.GetGoodsGrid() ) return;

		CItemCommand* pStoneItem = dynamic_cast<CItemCommand*>(g_stUISpirit.cmdSpiritMarry[SPIRIT_MARRY_ITEM]->GetCommand());
		if(NULL == pStoneItem)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(826));//请先放入恶魔果实
			return;
		}

		if(! pItemCommand->GetIsValid())
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(899));//您放入的精灵不合法，请重新选择精灵
			return;
		}

		if(g_stUISpirit.IsValidSpirit(*pItemCommand))
		{
			g_stUISpirit.PushItem(SPIRIT_MARRY_ONE, *pItemCommand);
			g_stUISpirit.SetSpiritUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(827));//您放入的不是精灵或者是低于20级的精灵，请重新选择精灵
		}
	}


	// 拖入精灵2
	void CSpiritMgr::_evtDragMarryTwo(CGuiData *pSender,CCommandObj* pItem,bool& isAccept)
	{
		CItemCommand* pItemCommand =  dynamic_cast<CItemCommand*>(pItem);
		if( !pItemCommand) return;

		CGoodsGrid* pGood = dynamic_cast<CGoodsGrid*>(CDrag::GetParent());
		if( pGood != g_stUIEquip.GetGoodsGrid() ) return;

		CItemCommand* pStoneItem = dynamic_cast<CItemCommand*>(g_stUISpirit.cmdSpiritMarry[SPIRIT_MARRY_ITEM]->GetCommand());
		if(NULL == pStoneItem)
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(826));
			return;
		}

		if(! pItemCommand->GetIsValid())
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(899));//您放入的精灵不合法，请重新选择精灵
			return;
		}

		if(g_stUISpirit.IsValidSpirit(*pItemCommand))
		{
			g_stUISpirit.PushItem(SPIRIT_MARRY_TWO, *pItemCommand);
			g_stUISpirit.SetSpiritUI();
		}
		else
		{
			g_pGameApp->MsgBox(g_oLangRec.GetString(827));//您放入的不是精灵或者是低于20级的精灵，请重新选择精灵
		}
	}


	// 鼠标点击按钮事件
	void CSpiritMgr::_evtMainMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnForgeYes")
		{
			g_stUISpirit.SendSpiritMarryProtocol();
			g_stUISpirit.ShowMarryForm(false);
		}
	}


	// 关闭窗体事件
	void CSpiritMgr::_evtCloseMarryForm(CForm* pForm, bool& IsClose)
	{
		g_stUISpirit.ClearAllCommand();
		CS_ItemForgeAsk(false);
	}

/////////////////////////////////////////////////////////////////////////////////////////

	void CSpiritMgr::FrameMove(DWORD dwTime)
	{
		if(frmSpiritErnie && frmSpiritErnie->GetIsShow())
		{
			DWORD dwCurrTickCount = g_pGameApp->GetCurTick();
			if(dwCurrTickCount - m_dwLastTickCount > ERNIE_SPEED)
			{
				//
				// 老虎机翻转
				//
				m_dwLastTickCount = dwCurrTickCount;

				imgLine[0][m_nCurrImage]->SetIsShow(false);
				imgLine[1][m_nCurrImage]->SetIsShow(false);
				imgLine[2][m_nCurrImage]->SetIsShow(false);
				imgLine[3][m_nCurrImage]->SetIsShow(false);
				imgLine[4][m_nCurrImage]->SetIsShow(false);
				imgLine[5][m_nCurrImage]->SetIsShow(false);
				imgLine[6][m_nCurrImage]->SetIsShow(false);
				imgLine[7][m_nCurrImage]->SetIsShow(false);
				imgLine[8][m_nCurrImage]->SetIsShow(false);

				m_nCurrImage = (m_nCurrImage + 1) % ERNIE_IMAGE_COUNT;

				if(m_bIsRunning[0])
				{
					imgLine[0][m_nCurrImage]->SetIsShow(true);
					imgLine[1][m_nCurrImage]->SetIsShow(true);
					imgLine[2][m_nCurrImage]->SetIsShow(true);
				}
				if(m_bIsRunning[1])
				{
					imgLine[3][m_nCurrImage]->SetIsShow(true);
					imgLine[4][m_nCurrImage]->SetIsShow(true);
					imgLine[5][m_nCurrImage]->SetIsShow(true);
				}
				if(m_bIsRunning[2])
				{
					imgLine[6][m_nCurrImage]->SetIsShow(true);
					imgLine[7][m_nCurrImage]->SetIsShow(true);
					imgLine[8][m_nCurrImage]->SetIsShow(true);
				}

				//
				// 显示文字闪
				//
				DWORD dwColorID = (g_pGameApp->GetCurTick() & 1023) >> 8;
				DWORD dwColor = 0;
				switch(dwColorID)
				{
				case 0:
					dwColor = 0xFFFF0000;
					break;
				case 1:
					dwColor = 0xFF800000;
					break;
				case 2:
					dwColor = 0xFF0000FF;
					break;
				case 3:
					dwColor = 0xFF000080;
					break;
				}

				labLastshow1->SetTextColor(dwColor);
				labLastshow2->SetTextColor(dwColor);
			}

			//
			// 下注显示
			//
			char szBuffer[32] = {0};
			sprintf(szBuffer, "%d", ERNIE_COIN_COUNT);
			for(int i = 0; i < 3; ++i)
			{
				labUsemoney[i]->SetCaption(chkSetmoney[i]->GetIsChecked() ? szBuffer : "");
			}
		}
	}


	void CSpiritMgr::ClearTigerItem()
	{
		for(int i = 0; i < 9; ++i)
		{
			cmdItem[i]->DelCommand();
		}
	}


	// 精灵老虎机，鼠标点击事件
	void CSpiritMgr::_evtErnieMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnStart")
		{
			if( ! g_stUISpirit.chkSetmoney[0]->GetIsChecked() &&
				! g_stUISpirit.chkSetmoney[1]->GetIsChecked() &&
				! g_stUISpirit.chkSetmoney[2]->GetIsChecked())
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(844));
				return;
			}

			int nCoinCount = 0;
			for(int i = 0; i < 3; ++i)
			{
				if(g_stUISpirit.chkSetmoney[i]->GetIsChecked())
					nCoinCount += ERNIE_COIN_COUNT;
			}

			if(nCoinCount > g_stUIEquip.GetItemCount(855))	// 是否有足够的精灵硬币
			{
				g_stUISpirit.chkSetmoney[0]->SetIsChecked(false);
				g_stUISpirit.chkSetmoney[1]->SetIsChecked(false);
				g_stUISpirit.chkSetmoney[2]->SetIsChecked(false);

				g_pGameApp->MsgBox(g_oLangRec.GetString(881));
				return;
			}

			int nEmptyCount = g_stUIEquip.GetGoodsGrid()->GetEmptyGridCount();
			if(ERNIE_EMPTY_COUNT > nEmptyCount)	// 背包需要 5 个空格
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(890), ERNIE_EMPTY_COUNT);
				return;
			}

			if(g_stUIEquip.GetIsLock())	// 背包是否锁定
			{
				g_pGameApp->MsgBox(g_oLangRec.GetString(894), ERNIE_EMPTY_COUNT);
				return;
			}

			g_stUISpirit.m_dwLastTickCount = g_pGameApp->GetCurTick();
			g_stUISpirit.m_nCurrImage = 0;

			g_stUISpirit.m_bIsRunning[0] = true;
			g_stUISpirit.m_bIsRunning[1] = true;
			g_stUISpirit.m_bIsRunning[2] = true;

			g_stUISpirit.chkSetmoney[0]->SetIsEnabled(false);
			g_stUISpirit.chkSetmoney[1]->SetIsEnabled(false);
			g_stUISpirit.chkSetmoney[2]->SetIsEnabled(false);

			g_stUISpirit.btnStart->SetIsEnabled(false);
			g_stUISpirit.btnStop[0]->SetIsEnabled(true);

			g_stUISpirit.labLastshow1->SetCaption("");
			g_stUISpirit.labLastshow2->SetCaption("");

			g_stUISpirit.ClearTigerItem();

			// 通知服务器开始
			CS_TigerStart(  g_stUINpcTalk.GetNpcId(),
							g_stUISpirit.chkSetmoney[0]->GetIsChecked(),
							g_stUISpirit.chkSetmoney[1]->GetIsChecked(),
							g_stUISpirit.chkSetmoney[2]->GetIsChecked());
		}
		else if(strName == "btnStop1")
		{
			g_stUISpirit.btnStop[0]->SetIsEnabled(false);
			CS_TigerStop(g_stUINpcTalk.GetNpcId(), 1);
		}
		else if(strName == "btnStop2")
		{
			g_stUISpirit.btnStop[1]->SetIsEnabled(false);
			CS_TigerStop(g_stUINpcTalk.GetNpcId(), 2);
		}
		else if(strName == "btnStop3")
		{
			g_stUISpirit.btnStop[2]->SetIsEnabled(false);
			CS_TigerStop(g_stUINpcTalk.GetNpcId(), 3);
		}
	}


	void CSpiritMgr::AddTigerItem(short nNum, short sItemID)
	{
		if(0 <= nNum && nNum < 9)
		{
			CItemRecord*  pInfo(NULL);
			CItemCommand* pItem(NULL);

			pInfo = GetItemRecordInfo(sItemID);
			if (!pInfo) return;

			pItem = new CItemCommand(pInfo);
			cmdItem[nNum]->AddCommand(pItem);
		}
	}


	// 更新显示内容
	void CSpiritMgr::UpdateErnieString(const char* szText)
	{
		if(0 == strlen(labLastshow1->GetCaption()))
		{
			labLastshow1->SetCaption(szText);
		}
		else if(0 == strlen(labLastshow2->GetCaption()))
		{
			labLastshow2->SetCaption(szText);
		}
		else
		{
			labLastshow1->SetCaption(labLastshow1->GetCaption());
			labLastshow2->SetCaption(szText);
		}
	}


	void CSpiritMgr::ShowErnieHighLight()
	{
		int nID[9] = {0};
		for(int i = 0; i < 9; ++i)
		{
			CItemCommand* pItem = dynamic_cast<CItemCommand*>(cmdItem[i]->GetCommand());
			if(pItem && pItem->GetItemInfo())
			{
				nID[i] = pItem->GetItemInfo()->lID;

				if(nID[i] != 194)	// 单个金钱也有效
				{
					ErnieHightLight(i, false);
				}
			}
		}

		if(nID[0] == nID[3] && nID[0] == nID[6])
		{
			//　■■■　０３６
			//　□□□　１４７
			//　□□□　２５８
			ErnieHightLight(0);
			ErnieHightLight(3);
			ErnieHightLight(6);
		}
		if(nID[1] == nID[4] && nID[1] == nID[7])
		{
			//　□□□
			//　■■■
			//　□□□
			ErnieHightLight(1);
			ErnieHightLight(4);
			ErnieHightLight(7);
		}
		if(nID[2] == nID[5] && nID[2] == nID[8])
		{
			//　□□□
			//　□□□
			//　■■■
			ErnieHightLight(2);
			ErnieHightLight(5);
			ErnieHightLight(8);
		}

		if(nID[3] == nID[4] && nID[3] == nID[5])
		{
			//　□■□
			//　□■□
			//　□■□
			ErnieHightLight(3);
			ErnieHightLight(4);
			ErnieHightLight(5);
		}
		if(nID[2] == nID[4] && nID[2] == nID[6])
		{
			//　□□■
			//　□■□
			//　■□□
			ErnieHightLight(2);
			ErnieHightLight(4);
			ErnieHightLight(6);
		}
		if(nID[0] == nID[4] && nID[0] == nID[8])
		{
			//　■□□
			//　□■□
			//　□□■
			ErnieHightLight(0);
			ErnieHightLight(4);
			ErnieHightLight(8);
		}
		if(nID[1] == nID[3] && nID[1] == nID[5] && nID[1] == nID[7])
		{
			//　□■□
			//　■□■
			//　□■□
			ErnieHightLight(1);
			ErnieHightLight(3);
			ErnieHightLight(5);
			ErnieHightLight(7);
		}
		if(nID[0] == nID[2] && nID[0] == nID[6] && nID[0] == nID[8])
		{
			//　■□■
			//　□□□
			//　■□■
			ErnieHightLight(0);
			ErnieHightLight(2);
			ErnieHightLight(6);
			ErnieHightLight(8);
		}
		if(nID[0] == nID[2] && nID[0] == nID[4] && nID[0] == nID[6] && nID[0] == nID[8])
		{
			//　■□■
			//　□■□
			//　■□■
			ErnieHightLight(0);
			ErnieHightLight(2);
			ErnieHightLight(4);
			ErnieHightLight(6);
			ErnieHightLight(8);
		}
		if(nID[1] == nID[3] && nID[1] == nID[4] && nID[1] == nID[5] && nID[1] == nID[7])
		{
			//　□■□
			//　■■■
			//　□■□
			ErnieHightLight(1);
			ErnieHightLight(3);
			ErnieHightLight(4);
			ErnieHightLight(5);
			ErnieHightLight(7);
		}
		if(nID[0] == nID[1] && nID[0] == nID[2] && nID[0] == nID[3] && nID[0] == nID[5] && nID[0] == nID[6] && nID[0] == nID[7] && nID[0] == nID[8])
		{
			//　■■■
			//　■□■
			//　■■■
			ErnieHightLight(0);
			ErnieHightLight(1);
			ErnieHightLight(2);
			ErnieHightLight(3);
			ErnieHightLight(5);
			ErnieHightLight(6);
			ErnieHightLight(7);
			ErnieHightLight(8);
		}
	}


	void CSpiritMgr::ErnieHightLight(int nNum, bool b)
	{
		if(0 <= nNum && nNum < 9)
		{
			CItemCommand* pItem = dynamic_cast<CItemCommand*>(cmdItem[nNum]->GetCommand());
			if(pItem)
			{
				pItem->SetIsValid(b);
				return;
			}
		}
	}


	void CSpiritMgr::_evtCloseErnieForm(CForm* pForm, bool& IsClose)
	{
		g_stUISpirit.ClearTigerItem();
		CS_TigerStop(g_stUINpcTalk.GetNpcId(), 0);
	}

}


