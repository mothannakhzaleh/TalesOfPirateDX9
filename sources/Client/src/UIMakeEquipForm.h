#ifndef UI_MAKE_EQUIP_FORM_H
#define UI_MAKE_EQUIP_FORM_H

#include "UIGlobalVar.h"
#include <vector>

class CItemRecord;

namespace GUI
{

	class CLabel;

	class CMakeEquipMgr : public CUIInterface	
	{
	public:
		CMakeEquipMgr() { }
		virtual ~CMakeEquipMgr() { }

		void		ShowMakeEquipForm(bool bShow = true);
		void        SendMakeEquipProtocol();
		void		ShowConfirmDialog(long lMoney);

		void		Clear();

		bool		IsRouleauCommand(COneCommand* oneCommand);
		bool		IsAllCommand(COneCommand* oneCommand);
		int			GetItemComIndex(COneCommand* oneCommand);

		void		DragRouleauToEquipGrid();
		void		DragItemToEquipGrid(int iIndex);

		void		MakeEquipSuccess(long lChaID);
		void		MakeEquipFailed(long lChaID);
		void		MakeEquipOther(long lChaID);

		void		SetType(int type) { m_iType = type; }
		int			GetType() const { return m_iType; }

		static const int MAKE_EQUIP_TYPE = 2;	//合成类型（协议中用）
		static const int EQUIP_FUSION_TYPE = 4;	//装备熔合
		static const int EQUIP_UPGRADE_TYPE = 5;//装备升级
		static const int ELF_SHIFT_TYPE     = 6;//精灵二转

		
		void		PushEquipUpgradeItem(int iIndex, CItemCommand& rItem);
		void		PushRouleau(CItemCommand& rItem);
		void		SetMakeEquipUI();
		void		DragEvtEquipItem(int index, CGuiData *pSender, CCommandObj* pItem, bool& isAccept);
		void		PushItem(int iIndex, CItemCommand& rItem, int iItemNum);
		
	protected:	/*@See CUIInterface Methods */
		virtual bool Init();
		virtual void CloseForm();
		virtual void SwitchMap();


	private:	/*Call back Functions*/
		static void _MainMouseEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _DragEvtRouleau(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem0(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem1(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem2(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
		static void _DragEvtEquipItem3(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);
		static void _OnClose(CForm* pForm, bool& IsClose);
		static void _evtConfirmEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
		static void _evtConfirmCancelEvent(CForm* pForm);
		static void _evtFusionNoCatalyzerConfirmEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);

	private:	/*Help Functions:*/

		void		ClearEquips();
		void		ClearEquipList(int iIndex);

		/*
		 *	Help Function:用于统一处理拖动事件
		 */
		//void		DragEvtEquipItem(int index, CGuiData *pSender, CCommandObj* pItem, bool& isAccept);

		/*
		 *	是否宝石合成
		 */
		bool		IsMakeGem();	
		bool		CanPushStone(int iIndex, CItemCommand& rItem);
		bool		CanPushEquip(int iIndex, CItemCommand& rItem);

		//void		SetMakeEquipUI();

		/*
		 *	托放卷轴相关
		 */
		//void		PushRouleau(CItemCommand& rItem);
		void		PopRouleau();
		void		PushNewGems();
		void		PushNewEquips(CItemRecord& rRouleauRecord);

		/*
		 *	托放宝石相关
		 */
		void		PushGemItem(int iIndex, CItemCommand& rItem);
		void		PopGemItem(int iIndex);

		/*
		 *	托放其他合成原料相关
		 */
		void		PushEquipItem(int iIndex, CItemCommand& rItem);
		void		PopEquipItem(int iIndex);

		/*
		 *	拖放熔合物品
		 */
		void		PushEquipFusionItem(int iIndex, CItemCommand& rItem);
		void		PopEquipFusionItem(int iIndex);

		/*
		 *	拖放升级物品
		 */
		//void		PushEquipUpgradeItem(int iIndex, CItemCommand& rItem);
		void		PopEquipUpgradeItem(int iIndex);

		/*
		 *	拖放最后合成物相关
		 */
		void		PushLastEquip(CItemCommand& rItem);
		void		PopLastEquip();

		/*
		 *  拖放精灵二转相关
		 */
		void		PushElfShiftItem(int iIndex, CItemCommand& rItem);
		void		PopElfShiftItem(int iIndex);

		/*
		 *	实际执行托放操作的函数
		 */
		//void		PushItem(int iIndex, CItemCommand& rItem, int iItemNum);
		void		PopItem(int iIndex);

		long		CalMakeEquipMoney();

		bool		IsEquipMakeRouleau(CItemCommand& rItem);

		bool		IsEquipFusionRouleau(CItemCommand& rItem);
		bool		IsAppearanceEquip(CItemCommand& rItem);
		bool		IsEquipFusionCatalyzer(CItemCommand& rItem);
		bool		IsSameAppearEquip(CItemCommand& rEquipItem, CItemCommand& rAppearItem);

		bool		IsEquipUpgradeRouleau(CItemCommand& rItem);
		bool		IsEquipUpgradeSpar(CItemCommand& rItem);
		bool		IsFusionEquip(CItemCommand& rItem);

		bool        IsElfShiftStone(CItemCommand& rItem);	// 是否是精灵二转所需的恶魔果实
		bool        IsElfShiftItem(CItemCommand& rItem);	// 是否是精灵


	private:
		static const int STONE_ITEM_NUM = 2;	//宝石合成数量
		static const int ITEM_NUM = 4;			//其他合成数量
		static const int FUSION_NUM = 3;		//熔合Item数量
		static const int UPGRADE_NUM = 2;		//升级Item数量
		static const int SHIFT_NUM   = 2;       //精灵二转Item数量

		static const int GEM_ROULEAU_TYPE = 47;	//宝石卷轴道具类型号
		static const int EQUIP_ROULEAU_TYPE = 48;//道具合成卷轴D号
		static const int GEN_STONE_TYPE = 49;
		static const int FORGE_STONE_TYPE = 50;
		static const int EQUIP_FUSION_ROULEAU_TYPE = 60;
		static const int EQUIP_FUSION_CATALYZER_TYPE = 61;
		static const int APPEAR_EQUIP_BASE_ID = 5000;
		static const int EQUIP_UPGRADE_ROULEAU_TYPE = 62;
		static const int EQUIP_UPGRADE_SPAR = 63;
		static const int FORGE_SUCCESS_EFF_ID = 345;
		static const int FORGE_FAILED_EFF_ID = 346;


		static const long MAKE_EQUIP_MONEY = 50000;
		static const long EQUIP_FUSION_MONEY = 1000;	// 熔合乘以的钱数
		static const long EQUIP_UPGRADE_MONEY = 10000;

		CForm*			frmMakeEquip;

		COneCommand*	cmdRouleau;
		COneCommand*	cmdLastEquip;
		COneCommand*	cmdItem[ITEM_NUM];
		CLabel*			labForgeGold;
		CMemo*			memForgeItemState;
		CTextButton	*	btnYes;

		struct EquipInfo {
			int iPos;		// 物品栏位的位置索引
			int iNum;		// 物品的数量
		};
		typedef std::vector<EquipInfo*> EquipList;
		typedef EquipList::iterator EquipListIter;
		EquipList equipItems[ITEM_NUM];

		int				m_iRouleauPos;

		bool			m_isOldEquipFormShow;

		int				m_iType;

	}; // end of CMakeEquipMgr class

} // end of GUI namespace

#endif	// end of UI_MAKE_EQUIP_FORM_H
