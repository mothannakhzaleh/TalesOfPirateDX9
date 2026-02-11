#pragma once
#include "UIGlobalVar.h"
#include "uicloneform.h"
#include "uiform.h"

namespace GUI
{
struct stTradeBox
{
	stTradeBox();
	void Refresh();
	int GetTradeNum();

	CForm*  frmDialog;
	CLabel*	labPrice;
	CEdit*	edtNumber;
	CLabel* labName;

	// 用户数据
	float	fUnitMoney;
	int		nTotalNum;

};

struct stSelectBox
{
	stSelectBox();

	CForm*  frmDialog;
	CLabel* labInfo;

	// 用户数据
	DWORD	dwTag;
	DWORD	dwParam;
	void*	pointer;
};

struct stNumBox
{
	stNumBox();
	void Refresh();
	int GetNumber();
	void GetString(const char*& out);

	CForm*  frmDialog;
	CEdit*	edtNumber;
	CLabel* labInfo;

	// 用户数据
	int		nTotalNum;
	void*	pointer;
};

struct stPasswordBox
{
	stPasswordBox():frmDialog(NULL),edtPassword(NULL),labInfo(NULL){}
	CForm*  frmDialog;
	CEdit*	edtPassword;
	CLabel* labInfo;
};

struct stMsgBox
{
	CForm*  frmDialog;
	CLabel* labInfo;
	
	// 用户数据
};

struct stMsgTimeBox
{
	CForm*  frmDialog;
	CLabel* labInfo;

	// 用户数据
	short teamID;
};

// 一些通用对话框
class CBoxMgr : public CUIInterface
{
public:
	// 交易用的对话框
	static stTradeBox*		ShowTradeBox( FormMouseEvent evtForm=NULL, float fUnitMoney=0, int nTotalNum=-1, const char* szName=NULL );

	// 选择确定或取消的对话框
	static stSelectBox*		ShowSelectBox( FormMouseEvent evtForm=NULL, const char* szTitle=NULL, bool bModal=false );

	// 输入数量对话框
	static stNumBox*		ShowNumberBox( FormMouseEvent evtForm=NULL, int nTotalNum=-1, const char* szTitle=NULL, bool IsMax=true, bool bModal=true );

	// 输入密码对话框
	static stPasswordBox*	ShowPasswordBox( FormMouseEvent evtForm=NULL, const char* szTitle=NULL, const char* szPassword=NULL, bool bModal=true );

	// 显示一个对话框
	static stMsgBox*		ShowMsgBox( FormMouseEvent evtForm=NULL, const char* szTitle=NULL, bool bModal=true );

	static stMsgTimeBox*	ShowMsgTime(  FormMouseEvent evtForm=NULL, const char* szTitle=NULL, int iSeconds = 60 );

	static void				CloseAllBox();

protected:
	virtual bool Init();
    virtual void End();
	virtual void FrameMove(DWORD dwTime);

private:
	static void _ClearBox( CHideForm& list );

	CForm*		_FindForm(const char * frmName);

private:
	// 交易数量对话框:数量,总价
	static CHideForm	_cTrade;	

	// 选择是,或者取消
	static CHideForm	_cSelect;	

	// 输入数量对话框
	static CHideForm	_cNumber;

	// 输入密码对话框
	static CHideForm	_cPassword;

	// 显示一个对话框
	static CHideForm	_cMsg;

	// 计时用Tick
	static CForm*	_cfrmError;
	static DWORD    _cTick;
	static int		_cSeconds;
	static bool		_cStart;
	
};

}
