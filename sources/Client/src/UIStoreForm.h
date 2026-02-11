
#pragma once

#include "uiglobalvar.h"
#include "uiguidata.h"
#include "uiform.h"
#include "uilabel.h"
#include "uimemo.h"
#include "uiboxform.h"
#include "uifastcommand.h"
#include "uitreeview.h"
#include "uiprogressbar.h"
#include "UI3DCompent.h"
#include "NetProtocol.h"

#include <map>


namespace GUI
{

	// 单个商品里单个道具信息
	struct stItemInfo
	{
		short       itemID;         //	道具编号
		int         itemNum;        //	道具个数
		int         itemFlute;      //	凹槽数
		short       itemAttrID[5];  //	五个属性编号
		short       itemAttrVal[5]; //	五个属性值

		void Clear() { memset(this, 0, sizeof(stItemInfo)); }
	};

	// 单个商品信息
	struct stStoreInfo
	{
		long        comID;          //	商品编号
		char        comName[21];    //	商品名称
		long        comClass;       //	商品分类
		long        comTime;        //	商品上架时间
		long        comPrice;       //	价格
		char        comRemark[129]; //	商品备注
		int         comNumber;      //  商品剩余个数。-1不限制，0下架
		time_t      comExpire;      //  商品到期时间。
		int         itemNum;        //  道具个数
		int         isHot;          //  是否热卖
		stItemInfo  comItemInfo[6]; //  最多六个道具

		stStoreInfo() { memset(this, 0, sizeof(stStoreInfo)); }
		void Clear()  { memset(this, 0, sizeof(stStoreInfo)); }
	};

	//  单个商品GUI
	struct stStoreGui
	{
		CLabelEx*		labName;			// 商品名
		CLabelEx*		labPrice;			// 商品价格
		CLabelEx*		labLeftTime;		// 商品剩余时间
		CLabelEx*		labLeftNum;			// 商品剩余个数
		CLabelEx*		labRemark;			// 商品描述
		CLabelEx*		labRightClickView;	// 右键查看祥细
		CImage*			imgCutLine;			// 分割线
		CImage*			imgHot;				// 热卖图
		CImage*			imgNew;				// 新品图
		CImage*			imgBlue;			// 选中商品高亮
		CImage*			imgSquare;			// 道具格子
		COneCommand*	cmdStore;			// 商品表格
		CTextButton*	btnBlue;			// 高亮按钮
		
		CImage*			imgWhite;

		void SetIsShow(bool v)
		{
			labName->SetIsShow(v);
			labPrice->SetIsShow(v);
			labRemark->SetIsShow(v);
			labLeftTime->SetIsShow(v);
			labLeftNum->SetIsShow(v);
			labRightClickView->SetIsShow(v);
			imgCutLine->SetIsShow(v);
			imgHot->SetIsShow(v);
			imgNew->SetIsShow(v);
			imgBlue->SetIsShow(v);
			imgSquare->SetIsShow(v);
			cmdStore->SetIsShow(v);
			btnBlue->SetIsShow(v);

			if(!v) cmdStore->DelCommand();
		}
	};


	class CStoreMgr : public CUIInterface
	{
	public:
		CStoreMgr();

		void ShowTempKitbag(bool bShow = true);
		void ShowStoreForm(bool bShow = true);
		void ShowStoreLoad(bool bShow = true);
		void ShowViewAllForm(bool bShow = true);
		void ShowTryonForm(bool bShow = true);
		void ShowAlphaMatteForm(bool bShow = true);
		void ShowStoreWebPage();

		CGoodsGrid* GetTempKitbagGrid()		{  return grdTempBag;	}
		CForm*		GetStoreForm()			{  return frmStore;		}
		static int	GetPageSize()			{  return STORE_PAGE_SIZE;  }

		// 场景变黑
		void DarkScene(bool bDark);

		// 从临时背包取出
		bool PopFromTempKitbag(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);
		
		// 打开商城询问
		void OpenStoreAsk();

		// 添加商城树结点（父结点为零表示根结点）
		void AddStoreTreeNode(long nParentID, long nID, const char* szCaption);

		// 添加商城商品信息
		void AddStoreItemInfo(long nSeq, long nID, const char* szName, long nPrice, const char* szRemark, bool isHot, long nTime, long nRemainNum, long nRemainTime);

		// 添加商城商品细节信息
		void AddStoreItemDetail(long nSeq, long nSubSeq, short sItemID, short sItemNum, short sFlute, short pItemAttrID[], short pItemAttrVal[]);

		// 添加用户设置结点
		void AddStoreUserTreeNode(void);

		// 设置商城列表页
		void SetStoreItemPage(long nCurPage, long nMaxPage);

		// 设置魔豆数量
		void SetStoreMoney(long nMoBean, long nRplMoney);

		// 设置VIP值
		void SetStoreVip(long nVip);

		// 添加商城公告
		//void AddStoreCommunique(long nID, const char* szTitle, 

		// 刷新商城树
		void StoreTreeRefresh();

		// 清除全部的商城树结点
		void ClearStoreTreeNode();

		// 清除全部商城物品列表
		void ClearStoreItemList();

		// 设置商城按钮是否可用
		void SetStoreBuyButtonEnable(bool b);

		// 获得当前选中ID
		int GetCurSelItemID();

		// 重置最后操作时间，10 秒间隔处理
		bool ResetLastOperate(bool bSilent = false);

		// 用户信息结点
		static const long USER_NODEID = 0x7FFFFFFE;
		static const long HELP_NODEID = 0x7FFFFFFF;

	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void SwitchMap();
		virtual void FrameMove(DWORD dwTime);

	private:

		// 临时背包
		CForm*			frmTempBag;			// 临时背包界面
		CGoodsGrid*		grdTempBag;			// 临时背包表格
		
		
		stNumBox*		m_pkNumberBox;		// 询问个数
		stNetTempKitbag m_NetTempKitbag;	// 临时背包网络协议数据

		static void _evtDragItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _MoveItemsEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:
		// 商城
		CForm*			frmStore;			// 商城界面
		CTreeView*		trvStore;			// 树列表
		CLabelEx*		labMoneyLeft;		// 金钱剩余
		CLabelEx*		labBeanLeft;		// 代币剩余
		CLabelEx*		labMemberStyle;		// 会员类型

		CLabelEx*		labNameTitle;
		CLabelEx*		labPriceTitle;
		CLabelEx*		labLeftTimeTitle;
		CLabelEx*		labLeftNumTitle;

		CLabelEx*		labListPage;		// 页标签
		CTextButton*	btnLeftPage;		// 左按钮
		CTextButton*	btnRightPage;		// 右按钮
		CTextButton*	btnTrade;			// 交易按钮
		CTextButton*	btnTryon;			// 试穿按钮
		CTextButton*	btnViewAll;			// 查看全部商品按钮
		CLabelEx*		labTrade;			// 交易按钮字
		CLabelEx*		labTryon;			// 试穿按钮字
		CLabelEx*		labViewAll;			// 查看全部商品按钮字
		CMemo*			memStoreHelp;		// 个人信息帮助
		CImage*			imgBackGround10;	// 白色挡板（商品购买时显示，自定义信息时不显示）
		stNumBox*		m_pkExchangeNum;	// 兑换代币数输入框

		static const int STORE_PAGE_SIZE = 9;		  // 商品列表一页最多 9 条
		stStoreGui		m_stStoreGui [STORE_PAGE_SIZE];
		stStoreInfo		m_stStoreInfo[STORE_PAGE_SIZE];

		long			m_nCurClass;		// 当前类型
		long			m_nCurSel;			// 当前选择
		long			m_nCurPage;			// 当前页
		long			m_nMaxPage;			// 最大页
		long			m_nExchangeNum;		// 兑换数
		long			m_nVip;				// 是否是VIP


		typedef std::map<long, std::string> MapNode;
		typedef std::map<long, std::string>::iterator MapNodeIter;
		MapNode			m_mapNode;		// 树结点

		void _SetIsShowUserInfo(bool bShow);
		void _SetIsShowHelpInfo(bool bShow);
		void _SetIsShowCozeForm(bool bShow);
		void _RefreshStoreListHighLight();
		void _ShowTradeSelectBox();
		void _LoadStoreHelpInfo();
		void _ChangeChaPart(stNetLookInfo& SLookInfo);
		bool _IsCurSelVipNode(void);

		static void	_evtStoreTreeMouseClick(CGuiData *pSender, int x, int y, DWORD key);
		static void	_evtStoreOpenCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtStoreToVipEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtExchangeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtTradeCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtStoreFormClose(CForm* pForm, bool& IsClose);

		static void _evtStoreFormMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void	_evtStoreListMouseRClick(CGuiData *pSender, int x, int y, DWORD key);
		static void	_evtStoreListMouseDBClick(CGuiData *pSender, int x, int y, DWORD key);

	private:

		// 商城读取界面
		CForm*			frmStoreLoad;
		CProgressBar*	proStoreLoad;
		static void _evtStoreLoadFormClose(CForm* pForm, bool& IsClose);
		static void _evtProTimeArriveEvt(CGuiData *pSender);
		DWORD			_dwDarkScene;

		static const int STORE_OPEN_TIMEOUT = 15;	// 商城打开超时（单位：秒）

	private:

		// 查看祥细
		static const int STORE_ITEM_COUNT = 6;	// 商品最多六个道具
		CForm*			frmViewAll;
		CMemo*			memViewAll;
		CImage*			imgSquareViewAll[STORE_ITEM_COUNT];
		COneCommand*	cmdSquareIcon[STORE_ITEM_COUNT];

		static void _evtStoreViewAllMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtStoreViewAllLostEvent(CGuiData *pSender);

	private:

		// 试穿(Avata)
		CForm*			frmTryon;		// 试穿界面
		C3DCompent*		ui3dplayer;		// 3D试穿

		int				m_nChaRotate;	// 角色旋转
		CCharacter*		m_pCurrMainCha;
		bool			m_isFight;
		stNetLookInfo	m_sLookInfo;
		stNetLookInfo	m_sCurLookInfo;

		void			ChaEquipClearAll();
		void			ChaLeftRotate();
		void			ChaRightRotate();
		void			RenderChaTryon(int x, int y);	// 绘制 3D 角色

		static void	_evtChaTryonRenderEvent(C3DCompent *pSender, int x, int y);
		static void _evtChaEquipClearAll(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaLeftRotate(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaRightRotate(CGuiData *sender, int x, int y, DWORD key);
		static void _evtChaLeftContinueRotate(CGuiData *sender);
		static void _evtChaRightContinueRotate(CGuiData *sender);
		static void _evtTryonFormClose(CForm* pForm, bool& IsClose);

	};

}

