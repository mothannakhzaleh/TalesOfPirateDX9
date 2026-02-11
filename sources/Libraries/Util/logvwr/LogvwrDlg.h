// LogvwrDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLogvwrDlg 对话框
class CLogPacket;
class CLogTypeData;
class CGplTypeData;
class CGplViewDlg;
class CLogvwrDlg : public CDialog
    {
    // 构造
public:
    CLogvwrDlg(CWnd* pParent = NULL);   // 标准构造函数
	~CLogvwrDlg();

    // 对话框数据
    enum {IDD = IDD_LOGVWR_DIALOG};

	// 主菜单对象指针
	CMenu* m_pMainMenu;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // 自添加消息映射函数
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

private:
	// 右键弹出菜单对象
	CMenu m_PopMenu;

    // 放置所有显示元素的容器
    CStatic m_Container;

	// 当前激活的窗口
	CWnd* m_pActiveWnd;

    // 显示所有LOG的列表
    CListCtrl m_LogList;

	// 存放各种类别LOG的ListCtrl对象及类别标识串
	std::vector<CLogTypeData*> m_LGData;

	// 显示所有GPL的对话框
	CGplViewDlg* m_pGplDlg;

	// 存放各种类别GPL的类别对象
	std::vector<CGplTypeData*> m_GPLData;

	// 测试、清除按钮
    CButton m_btnTestLog;
    CButton m_btnClearLog;

    // 显示与清除及防止重入的同步对象
    CCriticalSection m_CS;

    // LOG包对象
    CLogPacket* m_pLogPacket;

    // 接收所有包计数
    DWORD m_dwPacketCnt;

public:
    afx_msg void OnBnClickedClearlog();
    afx_msg void OnBnClickedTestlog();
	afx_msg void OnFileTest();
	afx_msg void OnViewAll();
	afx_msg void OnNMRclickLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnViewGpl();
	BOOL HaveThisLGType(const char* szTypeName, CLogTypeData** ppltd);
	BOOL HaveThisGPLType(const char* szTypeName, CGplTypeData** ppgtd);
	BOOL SwitchView(CWnd& ActiveWnd);
	BOOL SwitchView(CString& strActiveWnd);
    void ToggleTopMost();
	afx_msg void OnViewTest();
	afx_msg void OnViewClear();
	afx_msg void OnViewPlaymp3();
	afx_msg void OnViewStopmp3();
	afx_msg void OnViewPlaywave();
	afx_msg void OnTimer(UINT nIDEvent);
	};
