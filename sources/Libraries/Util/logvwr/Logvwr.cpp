// Logvwr.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Logvwr.h"
#include "LogvwrDlg.h"

#include "LogUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CResourceBundleManage  g_ResourceBundleManage;

// CLogvwrApp

BEGIN_MESSAGE_MAP(CLogvwrApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLogvwrApp 构造

CLogvwrApp::CLogvwrApp()
    {
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
    }


// 唯一的一个 CLogvwrApp 对象

CLogvwrApp theApp;

// CLogvwrApp 初始化

BOOL CLogvwrApp::InitInstance()
    {
    // 保证只运行一个实例
    HANDLE hPrevMutex;
    HANDLE hMutex;
    LPCTSTR pszMutex = "mUtEx Of kOp";

    hPrevMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, pszMutex);
    if (hPrevMutex)
        {
        ::CloseHandle(hPrevMutex);
        HWND hWnd = ::FindWindow(NULL, "kIng Of pIrAtEs lOgvIEwEr");
        if (hWnd)
            ::ShowWindow(hWnd, SW_SHOWMAXIMIZED);
        return FALSE;
        }

    // 创建互斥量
    hMutex = ::CreateMutex(NULL, 0, pszMutex);

    // 如果一个运行在 Windows XP 上的应用程序清单指定要
    // 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
    //则需要 InitCommonControls()。否则，将无法创建窗口。
    InitCommonControls();

    CWinApp::InitInstance();

    if (!AfxSocketInit())
        {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
        }

    AfxEnableControlContainer();

	// Add by lark.li
	if(!g_ResourceBundleManage.Init("E:\\Item\\Source\\VC2003\\Unicode\\Project_Kop\\vss_cs\\client\\bin\\system","zh_Hans_CN"))
	//if(!g_ResourceBundleManage.Init(".","zh_Hans_CN"))
	{
		AfxMessageBox("Cloud't load resource!\n");
		return -1;
	}
	else
	{
		printf(RES_STRING(VW_LOGVWR_CPP_00001));
	}

    // 标准初始化
    // 如果未使用这些功能并希望减小
    // 最终可执行文件的大小，则应移除下列
    // 不需要的特定初始化例程
    // 更改用于存储设置的注册表项
    // TODO: 应适当修改该字符串，
    // 例如修改为公司或组织名
    //SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
    if (!::mus_mgr_init(true))
        {
        AfxMessageBox("mus_mgr_init failed!");
        return FALSE;}

    CLogvwrDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
        {
        // TODO: 在此放置处理何时用“确定”来关闭
        //对话框的代码
        }
    else if (nResponse == IDCANCEL)
        {
        // TODO: 在此放置处理何时用“取消”来关闭
        //对话框的代码
        }

    // 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
    // 而不是启动应用程序的消息泵。
    ::mus_mgr_exit();
    ::CloseHandle(hMutex);
    return FALSE;
    }
