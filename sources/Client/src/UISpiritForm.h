#pragma once


#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "uicheckbox.h"
#include "uiimage.h"
#include "uicommand.h"
#include "uifastcommand.h"


namespace GUI
{

	class CSpiritMgr : public CUIInterface
	{
	public:
		CSpiritMgr(void);
		~CSpiritMgr(void);

		static const int NO_USE = -1;
		static const int SPIRIT_MARRY_TYPE = 6;			// 精灵融合类型（与服务器交互用）

		static const int SPIRIT_MARRY_CELL_COUNT = 3;	// 格子个数
		static const int SPIRIT_MARRY_ITEM = 0;			// 恶魔果实
		static const int SPIRIT_MARRY_ONE  = 1;			// 精灵1
		static const int SPIRIT_MARRY_TWO  = 2;			// 精灵2

		static const int ERNIE_IMAGE_COUNT = 5;			// 老虎机图片数
		static const int ERNIE_SPEED       = 50;		// 翻转速度（毫秒）
		static const int ERNIE_COIN_COUNT  = 5;			// 5 个精灵硬币
		static const int ERNIE_EMPTY_COUNT = 5;			// 5 个背包空格

		void ClearAllCommand();
		void ShowMarryForm(bool bShow = true);
		void ShowErnieForm(bool bShow = true);

		void UpdateErnieNumber(short nNum, short nID1, short nID2, short nID3);	// 更新老虎机
		void UpdateErnieString(const char* szText);		// 更新显示内容
		void ShowErnieHighLight();

		int GetType() {  return SPIRIT_MARRY_TYPE;	}	// 类型

	protected:
		virtual bool Init();
		virtual void CloseForm();
		virtual void FrameMove(DWORD dwTime);


	private:

		//
		//  精灵融合
		//
		CForm*		    frmSpiritMarry;
		CLabel*			labMoneyShow;
		CTextButton*	btnForgeYes;
		COneCommand*    cmdSpiritMarry[SPIRIT_MARRY_CELL_COUNT];

		int             m_iSpiritItemPos[SPIRIT_MARRY_CELL_COUNT];

		void		PushItem(int iIndex, CItemCommand& rItem);
		void		PopItem(int iIndex);
		void		SetSpiritUI();

		bool		IsValidSpiritItem(CItemCommand& rItem);	// 是否是合法的恶魔果实
		bool		IsValidSpirit(CItemCommand& rItem);		// 是否是合法的精灵（LV > 20）

		void		SendSpiritMarryProtocol();

		static void _evtDragMarryItem(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// 拖入恶魔果实
		static void _evtDragMarryOne(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// 拖入精灵1
		static void _evtDragMarryTwo(CGuiData *pSender,CCommandObj* pItem,bool& isAccept);	// 拖入精灵2

		static void _evtMainMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// 鼠标点击按钮事件
		static void _evtCloseMarryForm(CForm* pForm, bool& IsClose);	// 关闭窗体事件

	private:

		//
		//  精灵老虎机
		//
		CForm*			frmSpiritErnie;
		CImage*			imgLine[9][ERNIE_IMAGE_COUNT];
		CCheckBox*		chkSetmoney[3];
		CTextButton*	btnStop[3];
		COneCommand*	cmdItem[9];
		CTextButton*	btnStart;

		CLabelEx*		labUsemoney[3];
		CLabelEx*		labLastshow1;
		CLabelEx*		labLastshow2;

		bool			m_bIsRunning[3];

		DWORD			m_dwLastTickCount;
		int				m_nCurrImage;

		void			AddTigerItem(short nNum, short sItemID);
		void			ClearTigerItem(void);
		void			ErnieHightLight(int nNum, bool b = true);

		static void _evtErnieMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// 鼠标点击按钮事件
		static void _evtCloseErnieForm(CForm* pForm, bool& IsClose);	// 关闭窗体事件

	};

}

