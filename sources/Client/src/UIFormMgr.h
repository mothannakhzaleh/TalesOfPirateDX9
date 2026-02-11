//--------------------------------------------------------------
// 名称:表单管理类
// 作者:lh 2004-07-08
// 设计思想:管理所有表单
// 用途:显示,隐藏,查找表单,切换表单模板,提供全局钩子回调接口
// 最后修改日期:2004-10-09
//--------------------------------------------------------------
#pragma once
#include "uicompent.h"
#include "uiform.h"

namespace GUI
{
class CFormMgr;
typedef bool (*FormMgrEvent) ( CFormMgr* pSender );

typedef bool (*KeyDownEvent) (int& key);
typedef bool (*KeyCharEvent) (char& key);
typedef bool (*MouseEvent) (int& x, int& y, DWORD& mouse);
typedef bool (*MouseScrollEvent) (int& nScroll);
typedef bool (*HotKeyEvent) ( char& key, int& control );	// 返回true中断


class CFormMgr
{
	friend class CForm;
public:	
	CFormMgr();
	~CFormMgr();

	static void		SetDebugMode(bool v) { _IsDebugMode=v;}		//设置是否启动调试模式（由控制台命令调用）-added by Arcol
	void			ShowDebugInfo();							//显示调试信息 -added by Arcol
	static void		SetDrawFrameInDebugMode(bool v)	{_IsDrawFrameInDebugMode=v;}	//设置在调试模式下是否显示窗口边框 -added by Arcol
	static void		SetDrawBackGroundInDebugMode(bool v) {_IsDrawBackgroundInDebugMode=v;}	//置在调试模式下是否显示底色 -added by Arcol

	bool			AddForm( CForm* form, int templete=0 );

	bool			SetFormTempleteMax(int n);	
	unsigned int	GetFormTempleteMax()			{ return (unsigned int)_showforms.size();	}
	int				GetFormTempletetNum()			{ return _nTempleteNo;						}

	bool			SwitchTemplete( int n );		// 切换当前模板,如果为-1,则切换到默认模板

public:
	bool			Init(HWND hWnd);
	void			Clear();

    void            FrameMove( int x, int y, DWORD dwMouseKey, DWORD dwTime );
	void			Render();
	void            RenderHint( int x, int y );
	bool			HandleWindowMsg(DWORD dwMsg, DWORD dwParam1, DWORD dwParam2);

	bool			OnKeyDown(int key);
	bool			OnKeyChar(char key);
	bool			OnHotKey( char key, int control );

	bool			MouseScroll( int nScroll );
	void			Refresh();

    void            MouseReset();

	CForm*			GetHitForm( int x, int y );

	CForm*			Find( const char * );			// 在当前模板中，查找Form
	CForm*			Find( const char *str, int no );// 在no模板中，查找Form
	CForm*			FindAll( const char* );			// 在所有表单中，查找Form

	CForm*			FindESCForm();					// 查找可用ESC关闭的Form
	int				ModalFormNum()					{ return (int)_modal.size();		}

	typedef void (*FormFun)(CForm* pSender);
	void			ForEach( FormFun pFun );		// 所有Form都执行一次pFun函数

public:
	void			SetEnabled( bool v )			{ _bEnabled = v;	}
	bool			GetEnabled()					{ return _bEnabled; }
    void            ResetAllForm();
    void            SetScreen();

	void			SetEnableHotKey(int flag, bool v);		// 西门文档修改
	bool			GetEnableHotKey()				{ return _nEnableHotKey == 0xFFFFFFFF; }// 西门文档修改

	static bool		IsMouseInGui()					{ return _eMouseAction==enumMA_Gui;	}
	static eMouseAction	GetMouseAction()			{ return _eMouseAction;	}

public:	// 全局钩子
	bool			AddFormInit( FormMgrEvent pInitFun );	// 读入脚本后,初始化时事件

	bool			AddKeyDownEvent( KeyDownEvent event );
	bool			DelKeyDownEvent( KeyDownEvent event );

	bool			AddKeyCharEvent( KeyCharEvent event );
	bool			DelKeyCharEvent( KeyCharEvent event );

	bool			AddMouseEvent( MouseEvent event );
	bool			DelMouseEvent( MouseEvent event );

	bool			AddMouseScrollEvent( MouseScrollEvent event );
	bool			DelMouseScrollEvent( MouseScrollEvent event );

	bool			AddHotKeyEvent( HotKeyEvent event );
	bool			DelHotKeyEvent( HotKeyEvent event );

public:
	static CFormMgr	s_Mgr;

private:
	bool			_AddMemory( CForm* form );
	bool			_DelMemory( CForm* form );

	bool			_MouseRun( int x, int y, DWORD mouse );

	void			_ShowModal( CForm* form );
    void            _ModalClose( CForm* form );

	void			_SetNewActiveForm();		// 当前激活form丢失后，要寻找一个新的form激活

	void			_InitFormID();

    void            _DelShowForm( CForm* frm );
    void            _AddShowForm( CForm* frm );
    void            _UpdataShowForm( CForm* frm );

    void            _ActiveCompent();

private:
	typedef std::list<CForm*> vfrm;
	vfrm			_allForms;
	vfrm*			_forms;						// 非模态显示表单资源,第一个为当前激活的的表单
	vfrm			_modal;						// 正在显示的模态框
    vfrm            _show;                      // 正在显示的非模态框

	typedef std::vector<vfrm*> frmtemplete;
	frmtemplete		_showforms;
	vfrm			_defaulttemplete;

	bool			_bEnabled;		
	bool			_bInit;
	int				_nEnableHotKey;		// 西门文档修改

	typedef std::vector<KeyDownEvent>	vkeydowns;
	vkeydowns		_OnKeyDown;

	typedef std::vector<KeyCharEvent>	vkeychars;
	vkeychars		_OnKeyChar;

	typedef std::vector<MouseEvent>		vmouses;
	vmouses			_OnMouseRun;
	
	typedef std::vector<MouseScrollEvent> vscrolls;
	vscrolls		_OnMouseScroll;

	typedef std::vector<FormMgrEvent>	vinits;
	vinits			_vinits;

	typedef std::vector<HotKeyEvent>		vhotkey;
	vhotkey			_vhotkey;

	int				_nTempleteNo;				// 当前的模板号

    int             _nMouseHover;               // 鼠标在不做任何操作已经多少时间
    
    CGuiData*       _pHintGui;

	static eMouseAction		_eMouseAction;
	static bool		_IsDebugMode;				// 是否在调试模式 -added by Arcol
	static bool		_IsDrawFrameInDebugMode;	// 仅用于调试模式 -added by Arcol
	static bool		_IsDrawBackgroundInDebugMode;	// 仅用于调试模式 -added by Arcol

private:
	void			_DelForm( vfrm& list, CForm* frm );

};

inline void CFormMgr::MouseReset()                    
{ 
    _nMouseHover=0;     
}

}

