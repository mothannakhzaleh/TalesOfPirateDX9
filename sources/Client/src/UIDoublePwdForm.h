
#pragma once


#include "UIFormMgr.h"
#include "UIForm.h"
#include "UIEdit.h"
#include "UILabel.h"
#include "UITextButton.h"

#include "UIGlobalVar.h"
#include "NetProtocol.h"

#include "LoginScene.h"
#include "SelectChaScene.h"


namespace GUI
{
	class CForm;
	class CEdit;
	class CTextButton;

	// 二次密码界面
	class CDoublePwdMgr : public CUIInterface	
	{
	public:

		CDoublePwdMgr();

		// 显示创建表单
		void ShowCreateForm();

		// 显示修改表单
		void ShowAlterForm();

		// 显示输入表单
		void ShowDoublePwdForm();

		// 关闭全部表单
		void CloseAllForm();

		// 获得表单是否已显示
		bool GetIsShowCreateForm()    { return frmDoublePwdCreate->GetIsShow(); }
		bool GetIsShowAlterForm()     { return frmDoublePwdAlter->GetIsShow();  }
		bool GetIsShowDoublePwdForm() { return frmDoublePwd->GetIsShow();       }

		// 设置输入二次密码后执行类型
		void SetType(int nType) { m_nType = nType; }
		int  GetType()          { return  m_nType; }

		void SetLockGridID(int id) { lockGridID = id; }

		static const int MC_REQUEST	= -1;
		static const int DELETE_CHARACTOR	= 1;	// 删除当前角色
		static const int PACKAGE_UNLOCK		= 2;	// 背包解锁
		static const int STORE_OPEN_ASK		= 3;	// 商城打开
		static const int ITEM_UNLOCK		= 4;	// 道具解锁  ning.yan 2008-11-10
		static const int MULTI_ITEM_UNLOCK = 5;
		static const int SHOW_EXCHANGEFORM	= 5;

	protected:

		virtual bool Init();
		virtual void CloseForm();

		// 显示二次密码软键盘
		void ShowDoublePwdKeyboardForm();

		// 设置聚焦的密码编辑框
		void SetFocusEditBox(CEdit* edt) { edtFocusEditBox = edt; }

		// 发送删除角色消息
		void SendDeleteCharactor();

		// 发送背包解锁消息
		void SendPackageUnlock();

		// 发送道具解锁消息
		void SendItemUnlock(); //add by ning.yan 2008-11-11

		void SendUnlockSelectedItems();
		// 发送商城打开消息
		void SendPackageStoreOpen();

		void SendGameRequest();

		// 对软键盘按钮进行随机排列
		void RandomInputButton();

	private:

		// 创建二次密码表单
		CForm*      frmDoublePwdCreate;
		CEdit*		edtDoublePwdCreate;
		CEdit*      edtDoublePwdCreateRetry;

		// 修改二次密码表单
		CForm*      frmDoublePwdAlter;
		CEdit*		edtDoublePwdAlterOld;
		CEdit*		edtDoublePwdAlterNew;
		CEdit*		edtDoublePwdAlterNewRetry;

		// 输入二次密码表单
		CForm*      frmDoublePwd;
		CEdit*      edtDoublePwd;

		// 二次密码输入软键盘表单
		CForm*      frmDoublePwdInput;

		// 创建二次密码时的提示
		CForm*		frmDoublePwdInfo;

		// 焦点密码编辑框
		CEdit*		edtFocusEditBox;

		// 类型
		int         m_nType;

		int lockGridID; //TODO: No default value

	private:

		// 创建二次密码表单按钮事件
		static void _evtCreateFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 修改二次密码表单按钮事件
		static void _evtAlterFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 输入二次密码表单按钮事件
		static void _evtDoublePwdFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 二次密码输入软键盘表单按钮事件
		static void _evtInputFromMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

		// 编辑框激活事件（保存下激活的编辑框）
		static void _evtEditFocus(CGuiData* pSender);

		// 关闭窗体事件
		static void _evtFormClose(CForm* pForm, bool& IsClose );

		// 二次密码是否合法
		static bool IsPwdValid(const char* szStr);

	}; // end of class CDoublePwdMgr

} // end of namespace GUI


