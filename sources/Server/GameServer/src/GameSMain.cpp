// GameServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"                           

#include "GameAppNet.h"
#include "GameApp.h"
#include "SystemDialog.h"
#include "Config.h"
#include "GameDB.h"


// #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址 

extern BOOL GameServer_Begin();
extern void GameServer_End();
BOOL       g_bGameEnd = FALSE;
CGameApp*  g_pGameApp = NULL;
std::string g_strLogName = "GameServerLog";
HANDLE     hGameT;
HANDLE hConsole = NULL;

void DisableCloseButton();
void AppExit(void);


//#pragma init_seg(user)
#pragma init_seg( lib )
pi_LeakReporter pi_leakReporter("gamememleak.log");
CResourceBundleManage g_ResourceBundleManage("GameServer.loc"); //Add by lark.li 20080304

#include <signal.h>

void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	C_PRINT("Notify Logout to GameServer...\n");
	g_pGameApp->m_CTimerReset.Begin(1000);
	g_pGameApp->m_ulLeftSec = 3;
	fflush(stdout);
}

int main(int argc, char* argv[])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	C_TITLE("GameServer.exe")
	C_PRINT("Loading %s\n", szConfigFileN);

	DisableCloseButton();

	SEHTranslator translator;

	T_B
	if (argc >= 2)
	{
		strncpy(szConfigFileN, argv[1], defCONFIG_FILE_NAME_LEN - 1);
		szConfigFileN[defCONFIG_FILE_NAME_LEN - 1] = '\0';
	}
	if (!g_Config.Load(szConfigFileN))
	{
		LG("init", "config init...Fail!\n");
		return FALSE;		
	}
	if (!g_Command.Load("cmd.cfg"))
	{
		g_Command.SetDefault();
	}

#ifdef __CATCH	
    LG("init", "Define __CATCH\n");
#endif

	atexit(AppExit);
	if(!GameServer_Begin()) 
	{
		return 0;
	}
	signal(SIGINT, sigintHandler);

	MSG msg;
	while(!g_bGameEnd)
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			if(msg.message==WM_QUIT)
			{
				if (!g_bGameEnd)
					g_pGameApp->SaveAllPlayer();
				g_bGameEnd = TRUE;
				break;
			}
		}
		else
		{
			SystemReport(GetTickCount());
			Sleep(50);
		}
	}
	GameServer_End();
	LG("init", "game map server succeed to exit\n"); 
	T_FINAL
	return 0;
}
 



//_DBC_USING

#ifdef USE_IOCP

#else
ThreadPool	*l_proc = NULL;
ThreadPool	*l_comm = NULL;
#endif


extern DWORD WINAPI g_GameLogicProcess(LPVOID lpParameter);
BOOL GameServer_Begin()
{T_B
	_setmaxstdio(2048);

	//LG("init", "游戏地图服务器[%s]启动...\n", g_Config.m_szName);
LG("init", "game map server [%s] startup...\n", g_Config.m_szName);

	g_pGameApp = new CGameApp();
	if(!g_pGameApp->Init())
	{
		//LG("init", "GameApp 初始化失败, 退出!\n");
		LG("init", "GameApp initialization failed, exit!\n");
		return FALSE;
	}
	
#ifdef USE_IOCP
    cfl_iocpapp::startup();

    g_mygmsvr = new myiocpclt();
    g_mygmsvr->init_gtconn(&g_Config);

#else
    TcpCommApp::WSAStartup();
    //l_proc = ThreadPool::CreatePool(1, 60, 512);
    l_comm = ThreadPool::CreatePool(8, 8, 512);//,THREAD_PRIORITY_ABOVE_NORMAL);

	g_gmsvr	= new GameServerApp(0, l_comm);

    // 启动 GateServer 连接线程
   // LG("init", "启动Gate服务器连接线程...\n");
	 LG("init", "startup Gate server connect thread...\n");
    l_comm->AddTask(new ConnectGateServer(g_gmsvr));
#endif

    //连接并监听InfoServer
	//LG("init", "启动信息服务器连接线程...\n");
	//LG("init", "startup information server connect thread...\n");
    //l_comm->AddTask(new ToInfoServer(g_gmsvr));
	
	// 创建游戏线程
	//LG("init", "启动游戏线程...\n");
	LG("init", "startup game thread...\n");
	DWORD	dwThreadID;
	hGameT = CreateThread(NULL, 0, g_GameLogicProcess, 0, 0, &dwThreadID);
	LG("init", "Game Thread ID = %d\n", dwThreadID);
	//

	//LG("init",  "开始创建Win32 控制对话框\n");
	LG("init",  "start create Win32 control dialog box\n");
	HINSTANCE hInst = GetModuleHandle(0);
	CreateMainDialog(hInst, NULL);

	//Log("重启", "GameServer重启", g_Config.m_szMapList[0], "", "", "");
	Log("restart", "GameServer restart", g_Config.m_szMapList[0], "", "", "");
	
	return TRUE;
T_E}


void GameServer_End()
{T_B
	//LG("init", "开始结束游戏地图服务器\n");
	LG("init", "start to exit game map server\n");
	CloseHandle(hGameT);

	HWND hConsole = GetConsoleWindow();
	if(hConsole)
	{
		SendMessage(hConsole, WM_CLOSE, 0, 0);
	}
	
	Sleep(400);

	SAFE_DELETE(g_pGameApp);

#ifdef USE_IOCP
    if (g_mygmsvr != NULL)
    {
        g_mygmsvr->exit();
        Sleep(3000);
        delete g_mygmsvr;
    }

    cfl_iocpapp::cleanup();
#else
	delete g_gmsvr;

	// l_comm->DestroyPool();
	// l_proc->DestroyPool();

	TcpCommApp::WSACleanup();
#endif

T_E}

typedef HWND (*LPGETCONSOLEWINDOW)(void);
void DisableCloseButton()
{
	HMODULE hMod = LoadLibrary("kernel32.dll");

	LPGETCONSOLEWINDOW lpfnGetConsoleWindow =
		(LPGETCONSOLEWINDOW)GetProcAddress(hMod, "GetConsoleWindow");

	HWND hWnd = lpfnGetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	if (hMenu != NULL)
	{
		EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}

	FreeLibrary(hMod);
}

void AppExit(void)
{
	//int	*p = NULL;
	// *p = 1;
}

/*
 GameServer设计

 GameServer负责游戏逻辑的处理

 包含的主要模块有 
 
[GameData]
子数据类型有 
Map        地图 
MgrUnit    地图管理单元
Player     玩家
Character  角色
Item       道具
Skill      技能
SkillState 技能状态
Mission    任务

 
GameData应该单纯的做为数据容器

[GameControl]
App       应用程序框架 
TimerMgr  定时器管理
AI        AI控制

[EventHandler] 事件处理器

GameServer的运作方式为 

由GameControl启动应用程序, 启动AI定时器, 启动碰撞检测定时器

GameControl 向 EventHandler 产生Event, 比如AI事件, 与跳转点碰撞
客户端 向 EventHandler 产生Event, 比如请求行走, 请求使用技能, 请求使用道具

EventHandler对Event的处理为应答式, 即当时返回结果, 无任何中间状态

EventHandler在对Event的处理过程中, 产生Modify GameData的操作

此时EventHandler对于服务器内部逻辑和来自客户端的请求， 表现为一个双向插头, 并产生
唯一的结果Modify GameData


Control -> Event 
 





*/


