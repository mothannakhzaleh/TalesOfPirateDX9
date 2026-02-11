#pragma once
#include "UIGlobalVar.h"
#include "gamecommon.h"

struct NET_CHARTRADE_BOATDATA;
namespace GUI
{
// 与玩家交易
class CTradeMgr : public CUIInterface
{
public:
	void	ShowCharTradeRequest( BYTE byType, DWORD dwRequestID );				// 显示一个邀请框
	void	ShowCharTrade( BYTE byType, DWORD dwAcceptID,DWORD dwRequestID );	// 显示交易界面
	void	CancelCharTrade(  DWORD dwCharID );						// 取消交易
	void	ShowCharTradeMoney(DWORD dwCharID, DWORD dwMoney);   // 显示交易的金额
	void	ShowCharTradeIMP(DWORD dwCharID, DWORD dwMoney);   // 显示交易的金额

	void	ValidateTradeData( DWORD dwCharID );
	void	ValidateTrade(  DWORD dwCharID );
	void	ShowTradeSuccess(); // 显示交易成功
	void	ShowTradeFailed();  // 显示交易失败

	void    CloseAllForm();		// 关闭窗体

	// 拖动到交易面板中
	void	DragTradeToItem(DWORD dwCharID,BYTE byIndex,BYTE byItemIndex );

	//sItem: 交易的道具在道具表中的序列， byIndex : 在交易表单中的索引 byCount：交易的数量
	//byItemIndex: ：给道具在道具栏的索引
	void	DragItemToTrade(DWORD dwCharID,USHORT sItemID,BYTE byIndex,BYTE byCount,BYTE byItemIndex, SItemGrid* pGrid, const NET_CHARTRADE_BOATDATA* const pBoat );

	CGoodsGrid*     GetPlayertradeSaleGrid()    { return  grdSale;		} 
	CGoodsGrid*		GetRequestGrid()			{ return  grdRequest;	} //

	void			LocalSaleItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem );  // 交易的道具
	void			LocalCancelItem( CGoodsGrid* pSaleGrid, CGoodsGrid* pSelfGrid, int nGridID, CCommandObj* pItem );// 取消交易的道具

	// 是否正在交易中
	bool			IsTrading();

	CForm*			GetForm()	{ return frmPlayertrade; } //

protected:
	bool Init();
    void End();
    void FrameMove(DWORD dwTime);

	void Clear();

protected:
	static void _evtSelectYesNoEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
    static void _evtThrowItemEvent(CGuiData *pSender,int id,bool& isThrow);     // 从道具栏中扔出道具

	static void _evtIMPFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtGoldFormEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _MainMousePlayerTradeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _evtSelfRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);		// 交易界面点查看自己的物品道具
	static void _evtOtherRMouseGridEvent(CGuiData *pSender,CCommandObj* pItem,int nGridID);		// 交易界面点查看对方的物品道具

private:
	CGoodsGrid* GetPlayertradeBuyGrid()     { return  grdBuy;	}
	static void _evtLocalSaleEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

private:
	CForm*		frmPlayertrade;
	CForm*		frmRequest;				//交易左边的道具表单(或船舱)
	CGoodsGrid* grdRequest;
	CGoodsGrid*	grdBuy;
	CGoodsGrid*	grdSale;
	CLabelEx*	labOtherGold;
	CLabelEx*	labSelfGold;

	CLabelEx*	labOtherIMP;
	CLabelEx*	labSelfIMP;
	
	CTextButton*	btnTrade;
	CTextButton*	btnYes;
	CTextButton*	btnGold;
	CTextButton*	btnIMP;

	CCheckBox*		chkTrade;
	CCheckBox*		chkYes;

	DWORD			m_dwAcceptID;         // 交易接受方
	DWORD			m_dwRequestID;        // 交易申请方
	BYTE			m_bTradeType;		  // 交易类型,参看RoleCommand.h TRADE_CHAR_TYPE

	DWORD			m_dwMainID;

private:
	struct stSale
	{
		stSale() : dwSaleID(0), nGridID(0), nDragID(0) {}

		DWORD   dwSaleID;
		int		nGridID;
		int		nDragID;
	};
	stSale		_sSale;

};

}

