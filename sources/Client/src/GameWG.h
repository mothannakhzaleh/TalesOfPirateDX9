
#pragma once

#include <unordered_set>
#include <string>


// 游戏外挂类  add by Philip.Wu  2006-07-06
class CGameWG
{
public:
	CGameWG(void);
	~CGameWG(void);

	// 刷新当前进程里的模块
	bool RefreshModule(void);

	// 是否使用了“海盗天使”外挂
	bool IsUseHdts(void);

	// 启动线程
	void BeginThread(void);

	// 安全终止线程
	void SafeTerminateThread();

private:

	// 存放模块列表
	std::unordered_set<std::string> m_lstModule;

	// 线程句柄
	HANDLE m_hThread;

	// 线程回调
	static UINT CALLBACK Run(void* param);

};

