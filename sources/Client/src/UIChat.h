//----------------------------------------------------------------------
// 名称:用于实现聊天界面
// 作者:lh 2005-02-26
// 最后修改日期:2004-10-20
//----------------------------------------------------------------------
#pragma once
#include "UIGlobalVar.h"
#include "netprotocol.h"
class CCharacter2D;

extern int SMALL_ICON_SIZE;
extern int BIG_ICON_SIZE;

namespace GUI
{
class CGuiData;
class CMenu;
class CTreeNodeObj;
class CItemObj;

enum eSendTeamMsg
{
	enumSTM_ADD_GROUP,  // 新增组,参数1:CTeam*
	enumSTM_DEL_GROUP,  // 删除组,参数1:CTeam*
	enumSTM_ADD_MEMBER, // 新增组成员,参数:CMember*
	enumSTM_DEL_MEMBER, // 删除组成员,参数:CMember*
	enumSTM_AFTER_DEL_MEMBER,
	enumSTM_NODE_CHANGE,// 当前节点发生变化,参数:CMember*
	enumSTM_NODE_DATA_CHANGE,   // 节点扩展内容发生变化,用于组队的血,魔法,级别等发生变化时通知,参数:CMember*
};

class CGuiTime;
class CNoteGraph;

class CChat : public CUIInterface
{
public:
    enum 
    {
        MAX_MEMBER = 4,         // 最多成员数
    };

    CChat();

    bool Init();
    void End();

    void RefreshTeam();         // 刷新组队界面

	// 刷新组队成员数据
	void RefreshTeamData( CMember* pCurMember );		

    void ClearTeam();           // 清除组队界面
    int  TeamSend( DWORD dwMsg, void* pData=NULL, DWORD dwParam=0 );

	void SortOnlineFrnd();

	// Add by lark.li 20080806 begin
	void SortOnlineFrnd(CTreeGridNode* pNode);
	// End

	void MasterAsk(const char* szName, DWORD dwCharID);
	void PrenticeAsk(const char* szName, DWORD dwCharID);

public:
	CForm*			GetQQFrom()		{ return _frmQQ;		}
    CTreeView*      GetTeamView()   { return m_pQQTreeView;   }
	CTreeGridNode*  GetSessionNode(){ return _pSessionNode;	}
	CTreeGridNode*  GetGuildNode(){ return _pGuildNode;	}
    CTeamMgr*       GetTeamMgr()    { return _pTeamMgr;    }
	static bool		_UpdateFrndInfo(CMember *pMember);
	//static void		LoadFilterText(const char *text);
	static bool		_UpdateSelfInfo();
	static DWORD	_dwSelfID;			// 自己的ID
	static DWORD	_dwSelfIcon;			// 自己的头像
	static std::string	_strSelfMottoName;		// 自己的座右铭
	static CMember* _curSelectMember;


public:
	CForm*          frmChatManage;          // Chat界面
	//static CForm*	_frmDetails;
	CForm*       GetDetailsForm()    { return _frmDetails;    }
	//组队界面
	static CForm*          frmTeamMenber[MAX_MEMBER];		// nTag记录HummanID
	static CProgressBar*   proTeamMenberHP[MAX_MEMBER];
	static CProgressBar*   proTeamMenberSP[MAX_MEMBER];
	static CLabelEx*       labMenberName[MAX_MEMBER];
	static CLabelEx*       labLv[MAX_MEMBER];
	static CImage*		   imgWork[MAX_MEMBER];
	static CImage*		   imgLeader[MAX_MEMBER];

	static bool _bForbid;
	static std::vector<char*>	_strFilterTxt;

private:
	static bool _Error(const char* strInfo, const char* strFormName, const char* strCompentName) {
		LG("gui", strInfo, strFormName, strCompentName );
		return false;
	}

    static void _RenderEvent(C3DCompent *pSender, int x, int y);
	static void _MemberMouseDownEvent(CGuiData *pSender, int x, int y, DWORD key);

private:

	static CForm*          _frmQQ;			 // QQ界面

	static CForm*          _frmAddFriend;	 
	static CForm*          _frmChangeSytle;
	static CForm*			_frmEditMotto;
	static CForm*          _frmDetails;
	static CForm*          _frmBlockPlayer;
    static CTeamMgr*       _pTeamMgr;        // 组队管理器
	CTreeView*		m_pQQTreeView;
	CTreeGridNode*  _pFrndNode;              // 好友节点

    CTreeGridNode*  _pGroupNode;             // 队伍节点
	//屏蔽路人聊天

    //CTreeGridNode*  _pRoadNode;              // 路人节点
	static CTreeGridNode*  _pGuildNode;      // 公会节点
	static CTreeGridNode*  _pSessionNode;
	static CTreeGridNode*  _pMasterNode;	// 师傅
	static CTreeGridNode*  _pPrenticeNode;	// 徒弟
	
	static CTreeGridNode*  _pGMNode;	// 徒弟
	static CTreeGridNode*  _pBlockedNode;
	
	static CCharacter2D*   _pCharacter[MAX_MEMBER]; // 组队外观
	static CEdit*	_pEditFrndName;

	static CEdit*	_pEditMotto;
	static CEdit*  _pEditBlockName;
	//static CForm*	_frmMouseRightFrnd;
	//static CForm*	_frmMouseRightGroup;
	//static CForm*	_frmMouseRightRoad;
	static CList*	_lstFrndPlayer;
	static CList*	_lstGroupPlayer;
	static CList*	_lstRoadPlayer;
	static CList*	_lstStyle;

	static CMenu*	_frndMouseRight;
	static CMenu*	_groupMouseRight;
	static CMenu*	_roadMouseRight;
	static CMenu*	_sessMouseRight;
	static CMenu*	_MasterMouseRight;
	static CMenu*	_StudentMouseRight;
	static CMenu*	_BlockedMouseRight;
	static CMenu*	_styMenu;

	static CLabelEx*		_labNameOfMottoFrm;
	static CImage*			_imgFaceOfMottoFrm;
	static CCheckBox*		_chkForbidOfMottoFrm;

	static void _MainMouseClick(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseFrndMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseGroupMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseRoadMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseSessMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseMasterMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseStudentMenu(CGuiData *pSender, int x, int y, DWORD key);
	static void _OnMouseBlockedMenu(CGuiData *pSender, int x, int y, DWORD key);

	static void _evtQQMainShow(CGuiData *pSender);
	static void _evtQQMainAddFrnd(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtQQMainChangeStyle(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtAddFrnd(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtChangeStyle(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtQQMainEditMotto(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtChangeMotto(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtRefreshInfo(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtQQMainBlockPlayer(CGuiData *pSender, int x, int y, DWORD key);
	static void _evtBlockPlayer(CGuiData *pSender, int x, int y, DWORD key);

	static void _OnTimerFlash(CGuiTime *pSender);

	static void _OnDragEnd(CForm* pTargetForm, CTreeView *pTree, CTreeNodeObj* pNode, CItemObj* pItem, int x, int y, DWORD key );

	static void _OnFrndDeleteConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnMasterDeleteConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnStudentDeleteConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnStudentAskConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
	static void _OnMasterAskConfirm(CCompent *pSender, int nMsgType, int x, int y, DWORD dwKey);
};

}

extern CChat            g_stUIChat;
