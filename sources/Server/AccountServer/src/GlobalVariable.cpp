#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>

//#include "AccountServer2.h"
#include <string>
#include "MyThread.h"
#include "GlobalVariable.h"



//必须优先定义配置文件字符串
std::string g_strCfgFile="AccountServer.cfg";

CBillService	g_BillService;
dbc::TLSIndex	g_TlsIndex;
CDataBaseCtrl	g_MainDBHandle;
DWORD			g_MainThreadID;
HWND			g_hMainWnd = NULL;

//BillThread g_BillThread;


