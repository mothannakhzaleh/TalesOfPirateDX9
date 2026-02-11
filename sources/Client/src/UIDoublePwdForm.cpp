
#include "StdAfx.h"
#include "UIDoublePwdForm.h"
#include "UIStoreForm.h"
#include "UIEquipForm.h"

#include "GameApp.h"
#include "PacketCmd.h"
#include "UIItemCommand.h"// ning.yan 2008-11-11

using namespace std;

namespace GUI
{

	CDoublePwdMgr::CDoublePwdMgr()
	{
	}


	// 初始化二次密码所有的界面
	bool CDoublePwdMgr::Init()
	{
		CFormMgr& mgr = CFormMgr::s_Mgr;

		// 二次密码创建表单
		frmDoublePwdCreate = mgr.Find("frmDoublePwdCreate");
		if(! frmDoublePwdCreate) return false;

		edtDoublePwdCreate = dynamic_cast<CEdit*>(frmDoublePwdCreate->Find("edtDoublePwdCreate"));
		if(! edtDoublePwdCreate) return false;

		edtDoublePwdCreateRetry = dynamic_cast<CEdit*>(frmDoublePwdCreate->Find("edtDoublePwdCreateRetry"));
		if(! edtDoublePwdCreateRetry) return false;

		frmDoublePwdCreate->evtEntrustMouseEvent = _evtCreateFromMouseEvent;
		frmDoublePwdCreate->evtClose             = _evtFormClose;
		edtDoublePwdCreate->evtActive            = _evtEditFocus;
		edtDoublePwdCreateRetry->evtActive       = _evtEditFocus;

		edtDoublePwdCreate->SetIsPassWord(true);
		edtDoublePwdCreateRetry->SetIsPassWord(true);


		// 二次密码修改表单
		frmDoublePwdAlter = mgr.Find("frmDoublePwdAlter");
		if(! frmDoublePwdAlter) return false;

		edtDoublePwdAlterOld = dynamic_cast<CEdit*>(frmDoublePwdAlter->Find("edtDoublePwdAlterOld"));
		if(! edtDoublePwdAlterOld) return false;

		edtDoublePwdAlterNew = dynamic_cast<CEdit*>(frmDoublePwdAlter->Find("edtDoublePwdAlterNew"));
		if(! edtDoublePwdAlterNew) return false;

		edtDoublePwdAlterNewRetry = dynamic_cast<CEdit*>(frmDoublePwdAlter->Find("edtDoublePwdAlterNewRetry"));
		if(! edtDoublePwdAlterNewRetry) return false;

		frmDoublePwdAlter->evtEntrustMouseEvent = _evtAlterFromMouseEvent;
		frmDoublePwdAlter->evtClose             = _evtFormClose;
		edtDoublePwdAlterOld->evtActive         = _evtEditFocus;
		edtDoublePwdAlterNew->evtActive         = _evtEditFocus;
		edtDoublePwdAlterNewRetry->evtActive    = _evtEditFocus;

		edtDoublePwdAlterOld->SetIsPassWord(true);
		edtDoublePwdAlterNew->SetIsPassWord(true);
		edtDoublePwdAlterNewRetry->SetIsPassWord(true);


		// 二次密码输入表单
		frmDoublePwd = mgr.Find("frmDoublePwd");
		if(! frmDoublePwd) return false;

		edtDoublePwd = dynamic_cast<CEdit*>(frmDoublePwd->Find("edtDoublePwd"));
		if(! edtDoublePwd) return false;

		frmDoublePwd->evtEntrustMouseEvent = _evtDoublePwdFromMouseEvent;
		frmDoublePwd->evtClose             = _evtFormClose;
		edtDoublePwd->evtActive            = _evtEditFocus;
		edtDoublePwd->SetIsPassWord(true);


		// 二次密码输入软键盘
		frmDoublePwdInput = mgr.Find("frmDoublePwdInput");
		if(! frmDoublePwdInput) return false;

		frmDoublePwdInput->evtEntrustMouseEvent = _evtInputFromMouseEvent;
		frmDoublePwdInput->evtClose             = _evtFormClose;


		// 创建二次密码的提示
		frmDoublePwdInfo = mgr.Find("frmDoublePwdInfo");
		if(! frmDoublePwdInfo) return false;

		return true;
	}


	// 关闭二次密码
	void CDoublePwdMgr::CloseForm()
	{
		CloseAllForm();
	}


	// 二次密码是否合法
	bool CDoublePwdMgr::IsPwdValid(const char* szStr)
	{
		if(! szStr)
			return false;

		for(int i = 0; szStr[i]; ++i)
		{
			if('0' > szStr[i] || szStr[i] > '9')
				return false;
		}

		return true;
	}


	// 显示创建表单
	void CDoublePwdMgr::ShowCreateForm()
	{
		CloseAllForm();

		if(frmDoublePwdCreate && !frmDoublePwdCreate->GetIsShow())
		{
			edtDoublePwdCreate->SetCaption("");
			edtDoublePwdCreateRetry->SetCaption("");

			frmDoublePwdCreate->Show();
		}

		if(frmDoublePwdInfo)
		{
			frmDoublePwdInfo->SetPos(frmDoublePwdCreate->GetLeft(), frmDoublePwdCreate->GetBottom());
			frmDoublePwdInfo->Refresh();

			frmDoublePwdInfo->SetIsShow(true);
		}

		frmDoublePwdInput->SetPos(frmDoublePwdCreate->GetRight(), frmDoublePwdCreate->GetTop());
		frmDoublePwdInput->Refresh();
		ShowDoublePwdKeyboardForm();
	}


	// 显示修改表单
	void CDoublePwdMgr::ShowAlterForm()
	{
		CloseAllForm();

		if(frmDoublePwdAlter && !frmDoublePwdAlter->GetIsShow())
		{
			edtDoublePwdAlterOld->SetCaption("");
			edtDoublePwdAlterNew->SetCaption("");
			edtDoublePwdAlterNewRetry->SetCaption("");

			frmDoublePwdAlter->Show();
		}

		frmDoublePwdInput->SetPos(frmDoublePwdAlter->GetRight(), frmDoublePwdAlter->GetTop());
		frmDoublePwdInput->Refresh();
		ShowDoublePwdKeyboardForm();
	}


	// 显示输入表单
	void CDoublePwdMgr::ShowDoublePwdForm()
	{
		CloseAllForm();

		if(frmDoublePwd && !frmDoublePwd->GetIsShow())
		{
			edtFocusEditBox = edtDoublePwd;

			edtDoublePwd->SetCaption("");
			frmDoublePwd->Show();
		}

		frmDoublePwdInput->SetPos(frmDoublePwd->GetRight(), frmDoublePwd->GetTop());
		frmDoublePwdInput->Refresh();		
		ShowDoublePwdKeyboardForm();
	}


	// 关闭全部表单
	void CDoublePwdMgr::CloseAllForm()
	{
		// 关闭创建表单
		if(frmDoublePwdCreate && frmDoublePwdCreate->GetIsShow())
		{
			frmDoublePwdCreate->Close();
		}

		// 关闭修改表单
		if(frmDoublePwdAlter && frmDoublePwdAlter->GetIsShow())
		{
			frmDoublePwdAlter->Close();
		}

		// 关闭二次密码输入表单
		if(frmDoublePwd && frmDoublePwd->GetIsShow())
		{
			frmDoublePwd->Close();
		}

		// 关闭二次密码软键盘表单
		if(frmDoublePwdInput && frmDoublePwdInput->GetIsShow())
		{
			frmDoublePwdInput->Close();
		}

		// 关闭创建二次密码提示
		if(frmDoublePwdInfo && frmDoublePwdInfo->GetIsShow())
		{
			frmDoublePwdInfo->Close();
		}
	}


	// 显示二次密码软键盘
	void CDoublePwdMgr::ShowDoublePwdKeyboardForm()
	{
		if(! frmDoublePwdInput)
			return;

		RandomInputButton();
		frmDoublePwdInput->Refresh();
		frmDoublePwdInput->Show();
	}


	// 发送删除角色消息
	void CDoublePwdMgr::SendDeleteCharactor()
	{
		CSelectChaScene* pScene = dynamic_cast<CSelectChaScene*>(g_pGameApp->GetCurScene());
		pScene->SendDelChaToServer(g_stUIDoublePwd.edtDoublePwd->GetCaption());
        CGameApp::Waiting();
	}


	// 发送背包解锁消息
	void CDoublePwdMgr::SendPackageUnlock()
	{
		CS_UnlockKitbag(g_stUIDoublePwd.edtDoublePwd->GetCaption());
	}

	 // 发送道具解锁消息 add by ning.yan 2008-11-11 begin
	void CDoublePwdMgr::SendItemUnlock()// 被锁道具的位置和id
	{
		CS_UnlockItem(g_stUIDoublePwd.edtDoublePwd->GetCaption(), lockGridID);
		g_stUIDoublePwd.CloseAllForm();
	} 

	void CDoublePwdMgr::SendUnlockSelectedItems()
	{
		auto grid = g_stUIEquip.GetGoodsGrid();
		if (!grid)
		{
			return;
		}

		for (auto i = 0, n = grid->GetMaxNum(); i < n; ++i)
		{
			if (!grid->IsItemSelected(i))
			{
				continue;
			}

			auto item = static_cast<CItemCommand*>(grid->GetItem(i));
			if (!item || !item->GetIsValid())
			{
				continue;
			}

			if (!item->IsLocked())
			{
				continue;
			}

			CS_UnlockItem(g_stUIDoublePwd.edtDoublePwd->GetCaption(), i);
		}

		grid->ResetItemSelections();
	}
	// end

	// 发送商城打开消息
	void CDoublePwdMgr::SendPackageStoreOpen()
	{
		if(! g_stUIStore.ResetLastOperate())
		{
			CloseAllForm();
			return;
		}

		g_stUIStore.ShowStoreLoad();
		CS_StoreOpenAsk(g_stUIDoublePwd.edtDoublePwd->GetCaption());
		CloseAllForm();
	}

	void CDoublePwdMgr::SendGameRequest()// 被锁道具的位置和id
	{
		CS_SendGameRequest(g_stUIDoublePwd.edtDoublePwd->GetCaption());
		
		g_stUIDoublePwd.CloseAllForm();
	} 


	// 对软键盘按钮进行随机排列
	void CDoublePwdMgr::RandomInputButton()
	{
		CTextButton* btnNum[10] = {0};

		char szName[32] = {0};
		for(int i = 0; i < 10;++i)
		{
			sprintf(szName, "btnNum%d", i);
			btnNum[i] = dynamic_cast<CTextButton*>(frmDoublePwdInput->Find(szName));

			if(! btnNum[i])
				return;
		}

		const int nRandomCount = 10;
		srand(g_pGameApp->GetCurTick());

		int nOldX, nOldY, nNewX, nNewY, nNum1, nNum2;
		for(int i = 0; i < nRandomCount; )
		{
			nNum1 = rand() % 10;
			nNum2 = rand() % 10;

			if(nNum1 == nNum2)
				continue;

			nOldX = btnNum[nNum1]->GetLeft();
			nOldY = btnNum[nNum1]->GetTop();
			nNewX = btnNum[nNum2]->GetLeft();
			nNewY = btnNum[nNum2]->GetTop();

			btnNum[nNum1]->SetPos(nNewX, nNewY);
			btnNum[nNum2]->SetPos(nOldX, nOldY);

			++i;
		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////

	// 创建二次密码表单按钮事件
	void CDoublePwdMgr::_evtCreateFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();

		if(strName == "btnYes")
		{
			int nPwdLen = (int) strlen(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption());

			if(! IsPwdValid(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption()))
			{
				// 密码只能是数字
				g_pGameApp->MsgBox(g_oLangRec.GetString(797));//"二次密码只能有 0 ~ 9 数字组成，请重新输入"

				g_stUIDoublePwd.edtDoublePwdCreate->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdCreateRetry->SetCaption("");
				return;
			}

			if(6 > nPwdLen || 12 < nPwdLen)
			{
				// 密码长度不在 6 ~ 12 之间
				g_pGameApp->MsgBox(g_oLangRec.GetString(798));//"二次密码长度不在 6 ~ 12，请重新输入"

				g_stUIDoublePwd.edtDoublePwdCreate->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdCreateRetry->SetCaption("");
				return;
			}

			if(0 != strcmp(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption(), 
						   g_stUIDoublePwd.edtDoublePwdCreateRetry->GetCaption()))
			{
				// 两次密码不同
				g_pGameApp->MsgBox(g_oLangRec.GetString(799));//"两次输入的密码不同，请重新输入"

				g_stUIDoublePwd.edtDoublePwdCreate->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdCreateRetry->SetCaption("");
				return;
			}

			// 两次密码相同，向服务器发送创建二次密码消息
			CS_CreatePassword2(g_stUIDoublePwd.edtDoublePwdCreate->GetCaption());
			CCursor::I()->SetCursor(CCursor::stWait);
		}
		else
		{
			// 用户取消
			extern TOM_SERVER g_TomServer;
			if( g_TomServer.bEnable )
			{
				g_pGameApp->SetIsRun( false );
				return;
			}

			// 退出选人场景
			CS_Logout();
			CS_Disconnect(DS_DISCONN);
			g_pGameApp->LoadScriptScene( enumLoginScene );
		}
	}


	// 修改二次密码表单按钮事件
	void CDoublePwdMgr::_evtAlterFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();
		if(strName.size() <= 0) return;

		if(strName == "btnYes")
		{
			if(! IsPwdValid(g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption()))
			{
				// 密码只能是数字
				g_pGameApp->MsgBox(g_oLangRec.GetString(797));//"二次密码只能有 0 ~ 9 数字组成，请重新输入"

				g_stUIDoublePwd.edtDoublePwdAlterOld->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNew->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNewRetry->SetCaption("");
				return;
			}

			int nPwdLen = (int) strlen(g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption());
			if(6 > nPwdLen || 12 < nPwdLen)
			{
				// 密码长度不在 6 ~ 12 之间
				g_pGameApp->MsgBox(g_oLangRec.GetString(798));//"二次密码长度不在 6 ~ 12，请重新输入"

				g_stUIDoublePwd.edtDoublePwdAlterOld->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNew->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNewRetry->SetCaption("");
				return;
			}

			if(0 != strcmp(g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption(), 
						   g_stUIDoublePwd.edtDoublePwdAlterNewRetry->GetCaption()))
			{
				// 两次密码不同
				g_pGameApp->MsgBox(g_oLangRec.GetString(799));//"两次输入的密码不同，请重新输入"

				g_stUIDoublePwd.edtDoublePwdAlterOld->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNew->SetCaption("");
				g_stUIDoublePwd.edtDoublePwdAlterNewRetry->SetCaption("");
				return;
			}

			// 两次密码相同，向服务器发送修改二次密码消息
			CS_UpdatePassword2(g_stUIDoublePwd.edtDoublePwdAlterOld->GetCaption(), g_stUIDoublePwd.edtDoublePwdAlterNew->GetCaption());
		}
		else
		{
			g_stUIDoublePwd.CloseAllForm();
		}
	}


	// 输入二次密码表单按钮事件
	void CDoublePwdMgr::_evtDoublePwdFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		string strName = pSender->GetName();
		if(strName.size() <= 0) return;

		if(strName == "btnYes")
		{
			switch(g_stUIDoublePwd.m_nType)
			{
			case DELETE_CHARACTOR:
				g_stUIDoublePwd.SendDeleteCharactor();
				break;

			case PACKAGE_UNLOCK:
				g_stUIDoublePwd.SendPackageUnlock();
				break;

			case STORE_OPEN_ASK:
				g_stUIDoublePwd.SendPackageStoreOpen();
				break;

			// add by ning.yan 2008-11-11 道具解锁 begin
			case ITEM_UNLOCK:
				g_stUIDoublePwd.SendItemUnlock();
				break;// end

			case MULTI_ITEM_UNLOCK:
				g_stUIDoublePwd.SendUnlockSelectedItems();

			case MC_REQUEST:
				g_stUIDoublePwd.SendGameRequest();
				break;// end
			}
		}
		else
		{
			g_stUIDoublePwd.RandomInputButton();
			g_stUIDoublePwd.CloseAllForm();
		}
	}


	// 二次密码输入软键盘表单按钮事件
	void CDoublePwdMgr::_evtInputFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey)
	{
		if(! g_stUIDoublePwd.edtFocusEditBox)
			return;

		string strName = pSender->GetName();
		if(strName.empty()) return;

		char cNumber = strName[strName.size() - 1];
		string strPwd  = g_stUIDoublePwd.edtFocusEditBox->GetCaption();

		if(strName == "btnClear")
		{
			// 清空按钮
			g_stUIDoublePwd.edtFocusEditBox->SetCaption("");
		}
		else if('0' <= cNumber && cNumber <= '9' && strPwd.size() < 12)
		{ 
			// 数字键
			strPwd += cNumber;
			g_stUIDoublePwd.edtFocusEditBox->SetCaption(strPwd.c_str());
		}
	}


	// 编辑框激活事件（保存下激活的编辑框）
	void CDoublePwdMgr::_evtEditFocus(CGuiData* pSender)
	{
		CEdit* edtTemp = dynamic_cast<CEdit*>(pSender);
		if(edtTemp)
		{
			g_stUIDoublePwd.SetFocusEditBox(edtTemp);
		}
	}


	// 关闭窗体事件
	void CDoublePwdMgr::_evtFormClose(CForm* pForm, bool& IsClose)
	{
		//g_stUIDoublePwd.CloseAllForm();
	}

}

