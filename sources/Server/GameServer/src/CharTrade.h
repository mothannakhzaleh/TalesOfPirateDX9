// CharTrade.h Created by knight-gongjian 2004.12.7.
//---------------------------------------------------------
#pragma once

#ifndef _CHARTRADE_H_
#define _CHARTRADE_H_


#include "Character.h"
//---------------------------------------------------------
_DBC_USING

namespace mission
{
	typedef struct _TRADE_DATA
	{
		struct TRADE_ITEM_DATA
		{
			BYTE byType : 2;
			BYTE byIndex: 6;
			BYTE byCount;
			USHORT sItemID;			
		};
		TRADE_ITEM_DATA ItemArray[ROLE_MAXNUM_TRADEDATA];
		BYTE  byItemCount;						// 物品计数
		DWORD dwMoney;							// 交易钱数
		DWORD dwIMP;

	} TRADE_DATA, *PTRADE_DATA;

	class CTradeData : public dbc::PreAllocStru
	{
	public:
		CTradeData(dbc::uLong lSize);
		virtual ~CTradeData();

		void Clear()
		{
			pRequest	= NULL;
			pAccept		= NULL;
			byValue		= 0;

			memset( &ReqTradeData, 0, sizeof(TRADE_DATA) );
			memset( &AcpTradeData, 0, sizeof(TRADE_DATA) );
		}

		CCharacter *pRequest, *pAccept;
		union
		{
			struct
			{
				BYTE  bTradeStart : 1;		// 交易是否开始
				BYTE  bReqTrade : 1;		// 请求方交易数据确认
				BYTE  bAcpTrade : 1;		// 接受方交易数据确认
				BYTE  bReqOk : 1;			// 请求方交易操作确认
				BYTE  bAcpOk : 1;			// 接受方交易操作确认
				BYTE  byParam : 3;			// 保留
			};
			BYTE byValue;
		};
		//USHORT sxPos, syPos;		// 交易地点信息
		TRADE_DATA ReqTradeData;	// 请求方交易数据信息
		TRADE_DATA AcpTradeData;	// 接受方交易数据信息
		
		DWORD dwTradeTime;			// 交易时间计时(交易超过规定操作时间，系统取消交易操作，回收缓冲区)
	};

	class CTradeSystem
	{
	public:
		CTradeSystem();
		~CTradeSystem();

		// 交易操作
		BOOL Request( BYTE byType, CCharacter& character, DWORD dwAcceptID );
		BOOL Accept( BYTE byType, CCharacter& character,  DWORD dwRequestID );
		BOOL Cancel( BYTE byType, CCharacter& character,  DWORD dwCharID );
		
		// 角色离线清除交易信息
		BOOL Clear( BYTE byType, CCharacter& character );

		// 确认交易信息和操作
		BOOL ValidateItemData( BYTE byType, CCharacter& character, DWORD dwCharID );
		BOOL ValidateTrade( BYTE byType, CCharacter& character, DWORD dwCharID );

		// 放置或者取走物品到交易栏
		BOOL AddItem( BYTE byType, CCharacter& character, DWORD dwCharID, BYTE byOpType, BYTE byIndex, BYTE byItemIndex, BYTE byCount );
		BOOL AddMoney( BYTE byType, CCharacter& charactar, DWORD dwCharID, BYTE byOpType, DWORD dwMoney );
		BOOL AddIMP(BYTE byType, CCharacter& charactar, DWORD dwCharID, BYTE byOpType, DWORD dwMoney);
		BOOL IsTradeDist( CCharacter& Char1, CCharacter& Char2, DWORD dwDist );
	private:
		void ResetItemState( CCharacter& character, CTradeData& TradeData );

	};

#pragma pack( push, before_InfoNet )
#pragma pack( 8 )

    //商城订单信息
    struct SOrderData
    {        
        long long	lOrderID;  //订单号
        long		lComID;    //商品ID
		long		lNum;	   //数量
        long		ChaID;	   //角色数据库ID
		long		lRecDBID;
		char		ChaName[defENTITY_NAME_LEN];
		DWORD		dwTickCount;
    };

	//商品信息
	struct SItemData
	{
		SItemData():pItemArray(NULL)
		{
		}

		SItemData(const SItemData &rItem)
		{
			memcpy(&store_head, &rItem.store_head, sizeof(StoreInfo));
			int nItemNum = store_head.itemNum;
			if(nItemNum > 0)
			{
				pItemArray = new ItemInfo[nItemNum];
				memcpy(pItemArray, rItem.pItemArray, nItemNum * sizeof(ItemInfo));
			}
			else
				pItemArray = NULL;
		}

		~SItemData()
		{
			if(pItemArray)
			{
				delete [] pItemArray;
				pItemArray = NULL;
			}
		}

		SItemData& operator=(const SItemData &rItem)
		{
			memcpy(&store_head, &rItem.store_head, sizeof(StoreInfo));
			int nItemNum = store_head.itemNum;
			if(nItemNum > 0)
			{
				if(pItemArray)
				{
					delete [] pItemArray;
				}
				pItemArray = new ItemInfo[nItemNum];
				memcpy(pItemArray, rItem.pItemArray, nItemNum * sizeof(ItemInfo));
			}
			else
				pItemArray = NULL;
			return *this;
		}

		StoreInfo   store_head;       //  商品信息头
		ItemInfo	*pItemArray;      //  道具信息体
	};

    //商城系统
    class CStoreSystem
    {
    public:
        CStoreSystem();
        ~CStoreSystem();

        //用户购买道具
        BOOL Request( CCharacter *pCha, long lComID );
		BOOL Accept( long long lOrderID, RoleInfo *ChaInfo );
		BOOL Accept( CCharacter *pCha, long lComID );
		BOOL Cancel( long long lOrderID );

		BOOL RequestVIP(CCharacter *pCha, short sVipID, short sMonth);
		BOOL AcceptVIP(long long lOrderID, RoleInfo *ChaInfo, DWORD dwVipParam);
		BOOL CancelVIP(long long lOrderID);

		//用户获取商品列表
		BOOL RequestItemList(CCharacter *pCha, long lClsID, short sPage, short sNum);

		//用户请求兑换代币
		BOOL RequestChange(CCharacter *pCha, long lNum);
		BOOL AcceptChange(long long lOrderID, RoleInfo *ChaInfo);
		BOOL CancelChange(long long lOrderID);

		//查询角色信息
		BOOL RequestRoleInfo(CCharacter *pCha);
		BOOL AcceptRoleInfo(long long lOrderID, RoleInfo *ChaInfo);
		BOOL CancelRoleInfo(long long lOrderID);

		//查询交易记录
		BOOL RequestRecord(CCharacter *pCha, long lNum);
		BOOL AcceptRecord(long long lOrderID, HistoryInfo *pRecord);
		BOOL CancelRecord(long long lOrderID);

		BOOL RequestGMSend(CCharacter *pCha, cChar *szTitle, cChar *szContent);
		BOOL AcceptGMSend(long long lOrderID, long lMailID);
		BOOL CancelGMSend(long long lOrderID);

		BOOL RequestGMRecv(CCharacter *pCha);
		BOOL AcceptGMRecv(long long lOrderID, MailInfo *pMi);
		BOOL CancelGMRecv(long long lOrderID);

		//用户开启商城
		BOOL Open(CCharacter *pCha, long vip);

		//商城列表
		BOOL GetItemList();
		BOOL GetAfficheList();

		BOOL SetItemList(void *pItemList, long lNum);
		BOOL ClearItemList();

		BOOL SetItemClass(ClassInfo *pClassList, long lNum);
		BOOL ClearItemClass();

		BOOL SetAfficheList(AfficheInfo *pAfficheList, long lNum);
		BOOL ClearAfficheList();

		void InValid() { m_bValid = false; }
		void SetValid() { m_bValid = true; }
		BOOL IsValid() { return m_bValid; }

        void Run( DWORD dwCurTime, DWORD dwIntervalTime, DWORD dwOrderTime );    //处理订单列表
        
    private:
		//查询商品
		long		GetClassId(long lComID);
		cChar		*GetClassName(long lClsID);
		SItemData	*GetItemData(long lComID);
		BOOL		DelItemData(long lComID);

		//订单操作
		BOOL		PushOrder(CCharacter *pCha, long long lOrderID, long lComID, long lNum);
		SOrderData	PopOrder(long long lOrderID);
		BOOL		HasOrder(long long lOrderID);

		//map<long long, SOrderData>		m_OrderList;		//订单列表
		std::vector<SOrderData>				m_OrderList;
		std::map<long, long>					m_ItemSearchList;	//商品ID和分类的映射关系
		std::map< long, std::vector<SItemData> >	m_ItemList;			//商品列表
		std::vector<ClassInfo>				m_ItemClass;		//商品分类
		std::vector<AfficheInfo>				m_AfficheList;		//公告列表
		BOOL							m_bValid;
        
    };

#pragma pack( pop, before_InfoNet )

}

extern mission::CTradeSystem g_TradeSystem;
extern mission::CStoreSystem g_StoreSystem;

//---------------------------------------------------------

#endif // _CHARTRADE_H_