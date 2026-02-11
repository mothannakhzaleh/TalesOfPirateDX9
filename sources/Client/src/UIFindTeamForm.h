

#pragma once

#include "uiglobalvar.h"
#include "uiform.h"
#include "uiformmgr.h"
#include "uilabel.h"
#include "uitextbutton.h"
#include "uifastcommand.h"
#include <string>


namespace GUI
{

	class CFindTeamMgr : public CUIInterface
	{
	public:
		CFindTeamMgr();
		~CFindTeamMgr();

		void ShowFindTeamForm(bool bShow = true);
		bool IsShowFom(){return frmFindTeam->GetIsShow();}	//	Add by alfred.shi 20080902
		void SetFindTeamPage(int nCurPage, int nPageNum);
		void AddFindTeamInfo(int nSeq, const char* szName, long nLevel, long nJob, const char* szPlace);
		void RemoveTeamInfo();
		void SetOwnFindTeamState(bool bState);
		void FindTeamAsk(const char* szName);

		static const int FINDTEAM_PAGE_SIZE = 10;	// 寻求组队一页显示条数
		static const int FINDTEAM_INTERVAL  = 60;	// 两次申请间隔时间：60秒

	protected:
		virtual bool Init();
		virtual void CloseForm();

	private:

		//
		// 寻找组队
		//
		CForm*		frmFindTeam;

		CLabelEx*		labName[FINDTEAM_PAGE_SIZE];	// 角色姓名
		CLabelEx*		labLevel[FINDTEAM_PAGE_SIZE];	// 角色等级
		CLabelEx*		labJob[FINDTEAM_PAGE_SIZE];		// 角色职业
		CLabelEx*		labPlace[FINDTEAM_PAGE_SIZE];	// 角色所在地
		CTextButton*	btnSubmit[FINDTEAM_PAGE_SIZE];	// 提交按钮

		CTextButton*	btnAddme;
		CTextButton*	btnDelme;
		CLabelEx*		labListPage;		// 显示页数

		int				m_nCurPage;			// 当前页
		int				m_nPageNum;			// 总页数
		DWORD			m_dwLastTick;		// 上一次申请组队的时间
		std::string		m_strTeamLeader;	// 申请的队长

		static void _evtFindTeamMouseButton(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);// 鼠标点击按钮事件
		static void	_evtFindTeamCheckEvent(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey); // 组队申请

	};

}

