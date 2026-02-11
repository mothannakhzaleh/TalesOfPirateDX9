#pragma once

#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_


//#include "BillThread.h"

#include "tlsindex.h"
#include "BillService.h"
#include "databasectrl.h"

#include "i18n.h"							//Add by lark.li 20080307

// Add by lark.li 20080730 begin
#include "pi_Alloc.h"
#include "pi_Memory.h"
// End

//#define _RELOGIN_MODE_					//新的倒计时重复登陆模式

extern std::string g_strCfgFile;

extern CBillService		g_BillService;
extern dbc::TLSIndex	g_TlsIndex;
extern CDataBaseCtrl	g_MainDBHandle;
extern HWND				g_hMainWnd;
extern DWORD			g_MainThreadID;
//extern BillThread g_BillThread;

#define WM_USER_LOG		WM_USER+100
#define WM_USER_LOG_MAP	WM_USER+101


struct sUserLog
{
	bool bLogin;		//true:login  false:logout
	int nUserID;
	std::string strUserName;
	std::string strPassport;
	std::string strLoginIP;
};


extern HANDLE hConsole;

#define C_PRINT(s, ...) \
	SetConsoleTextAttribute(hConsole, 14); \
	printf(s, __VA_ARGS__); \
	SetConsoleTextAttribute(hConsole, 10);

#define C_TITLE(s) \
	char szPID[32]; \
	_snprintf_s(szPID,sizeof(szPID),_TRUNCATE, "%d", GetCurrentProcessId()); \
	std::string strConsoleT; \
	strConsoleT += "[PID:"; \
	strConsoleT += szPID; \
	strConsoleT += "]"; \
	strConsoleT += s; \
	SetConsoleTitle(strConsoleT.c_str());


#endif