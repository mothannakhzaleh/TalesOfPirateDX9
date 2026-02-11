#pragma once

#ifndef UI_BOURSE_FORM_H
#define	UI_BOURSE_FORM_H

#include "UIGlobalVar.h"
#include "uiform.h"
#include "uiGoodsGrid.h"
#include <map>
#include "NetProtocol.h"

struct BOAT_BERTH_DATA;

#define GOOD_DISTINGUISH_PILE 0

namespace GUI
{

struct stTradeBox;

class CMemo;
class COneCommand;

// 货舱交易所
class CBourseMgr : public CUIInterface
{
public:
	CBourseMgr();
	void ShowBourse(const NET_TRADEINFO& TradeInfo, BYTE byCmd, 
					DWORD dwNpcID, DWORD dwParam);

	void ShowNPCSelectShip(BYTE byNumBoat, const BOAT_BERTH_DATA& Data, 
						   BYTE byType);

	bool IsTradeCmd(COneCommand* pkCommand);

	CGoodsGrid* GetBuyGoodsGrid();

	CGoodsGrid* GetSaleGoodsGrid();

	CGoodsGrid* GetShipRoomGoodsGrid();

	void BuyItem(CGoodsGrid& rkToGoodsGrid, CCommandObj& rkItem, int nGridID);

	CForm* GetForm() { return frmSeaTrade; }

	DWORD			GetSalePriceById(USHORT sId);

	bool			IsShow() { return frmSeaTrade->GetIsShow(); }

	DWORD			GetNpcId() { return m_dwNpcID; }

	DWORD			GetBoatId(){ return m_dwBoatID;}

	void			UpdateOneGood(BYTE byPage, BYTE byIndex, USHORT sItemID, USHORT sCount, DWORD dwPrice);

	bool			SaleGoods(CItemCommand& rkBuy, int iGridIndex);
	void			BuyGoods(CItemCommand& rkBuy, int nFreeCnt);

protected:
	virtual bool Init();
    virtual void End();
	virtual void CloseForm();

	//~ 回掉函数
	static void _MainMouseSeaTradeEvent(CCompent *pSender, int nMsgType, 
										int x, int y, DWORD dwKey);
	static void __gui_event_left_rotate(CGuiData *sender,
										int x, int y, DWORD key );
	static void __gui_event_right_rotate(CGuiData *sender,
										 int x, int y, DWORD key );
	static void __gui_event_left_continue_rotate(CGuiData *sender );
	static void __gui_event_right_continue_rotate(CGuiData *sender );

	static void __gui_event_drag_before(CGuiData *pSender, CCommandObj* pItem, 
		int nGridID, bool& isAccept);
	static void __gui_event_sale_drag_before(CGuiData *pSender, CCommandObj* pItem, 
		int nGridID, bool& isAccept);
	static void __gui_event_drag_before_com(CGuiData *pSender, CCommandObj* pItem, 
		bool& isAccept);

	static void _BuyGoodsEvent(CCompent *pSender, int nMsgType, 
							   int x, int y, DWORD dwKey);
	static void _SaleGoodsEvent(CCompent *pSender, int nMsgType, 
								int x, int y, DWORD dwKey);
	static void _BuyAGoodEvent(CCompent *pSender, int nMsgType, 
							   int x, int y, DWORD dwKey);
	static void _SaleAGoodEvent(CCompent *pSender, int nMsgType, 
							    int x, int y, DWORD dwKey);



private:
	enum eDirectType { LEFT = -1, RIGHT = 1,};
	static const int MAX_BOAT_NUM = 3;

	struct ItemInfo_T
	{
		USHORT		sId;		//物品ID号
		DWORD		dwPrice;	//物品价格
		WORD		wNum;		//物品数量
		BYTE		byLevel;


		ItemInfo_T() : sId(0), dwPrice(0), wNum(0), byLevel(0) {}
	};
	typedef std::vector<ItemInfo_T*> ItemList;
	typedef ItemList::iterator ItemListIter;


	// 逻辑函数
	void            ChangeItem(eDirectType enumDirect = LEFT);

	void			SetItems();

	bool			ShowBoat(unsigned int iBoatIndex = 0);

	int				GetGoodsIndex(CItemRecord* pkGoodRecord);
	void			Refresh();

	void			ClearItemList(ItemList& itemMap);

	BYTE			GetTradeLevel();


private:

	//~ 界面 =================================================================
	CForm*			frmSeaTrade;
	CForm*			frmBoatRoom;

	CTextButton*	btnItemLeft;
	CTextButton*	btnItemRight;

	//买卖货物的表格
	CGoodsGrid*		grdItemSale;
	CGoodsGrid*		grdItemBuy; 

	CGoodsGrid*		grdShipRoom;

	//NPC界面显示选择船的界面
	CForm*			frmNPCchat;
	CMemo*			memCtrl;

	//~ 逻辑 ==================================================================
	static const BYTE ITEM_TYPE;			//物品类型,这里约定为0
	static const float SALE_RATE;			//物品卖出的折减数目前约定为50%
	static const int BUY_PAGE_INDEX;		//买入物品的页索引号
	static const int SALE_PAGE_INDEX;		//卖出物品的页索引号

	DWORD			 m_dwNpcID;
	DWORD			 m_dwBoatID;
	BYTE			 m_byGoodIndex;
	BYTE			 m_byDragIndex;
	stTradeBox*		 m_pkTradeBox;


	//物品的个数
	int				 m_iItemSelIndex;

	ItemList		 m_BuyList;
	ItemList		 m_SaleList;
};



////////////////////////////////////////////////////////////////////////////////
//
//  黑市商人
//
class CBlackTradeMgr : public CUIInterface
{
public:
	CGoodsGrid* GetBuyGoodsGrid()	{ return grdItemBuy;	}
	CGoodsGrid* GetSaleGoodsGrid()	{ return grdItemSale;	}

	DWORD GetNpcID()				{ return m_dwNpcID;		}
	void  SetNpcID(DWORD dwNpcID);

	void ClearItemData()			{ m_vecBlackTrade.clear();	}
	void SetDefaultSaleGrid()		{ grdItemSale->SetContent(14, 3); }
	void SetItem(stBlackTrade* pBlackTrade);
	void ShowBlackTradeForm(bool b);
	void ExchangeAnswerProc(bool bSuccess, stBlackTrade* pBlackTrade);

	bool GetIsShow()	{ return frmBlackTrade->GetIsShow(); }

	bool SailToBuy(CGoodsGrid& rkDrag, CGoodsGrid& rkSelf, int nGridID, CCommandObj& rkItem);

protected:
	virtual bool Init();
	virtual void CloseForm();

private:
	CForm*			frmBlackTrade;
	CGoodsGrid*		grdItemSale;
	CGoodsGrid*		grdItemBuy;

	int				m_nDragIndex;
	DWORD			m_dwNpcID;

	typedef std::vector<stBlackTrade>    BlackTradeVec;
	typedef BlackTradeVec::iterator BlackTradeVecIter;

	BlackTradeVec	m_vecBlackTrade;

	// 刷新购买条件
	void RefreshSaleGrid();

	// 兑换确认
	static void		_TradeExchangeEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	// 关闭窗体事件
	static void		_evtCloseForm(CForm* pForm, bool& IsClose);
};


}

#endif // end of UI_BOURSE_FORM_H

