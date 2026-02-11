#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "AccountServer2.h"
#include "inifile.h"
#include "NetCommand.h"
#include "BillThread.h"
#include "GlobalVariable.h"


bool BillThread::m_bBillingInitialized=false;

BillThread::BillThread() : PKQueue(false)
{
	IniFile inf(g_strCfgFile.c_str());
	IniSection& is = inf["bill"];
	int enable = atoi(is["enable_bill"]);
	m_bEnableBilling=(enable>0);
	if (m_bEnableBilling)
	{
		printf("Enable Billing System ... ");
		m_strBillService1=is["bill_server1"];
		m_strBillService2=is["bill_server2"];
		if (m_strBillService1.empty() || m_strBillService2.empty())
		{
			m_bEnableBilling=false;
			printf("Failure! Billing Server IP is empty!\n");
		}
		else
		{
			printf("Success!!\n");
		}
	}
}

BillThread::~BillThread()
{
	if (m_bEnableBilling)
	{
		if (m_BillingService.Close())
		{
			printf("Billing System Close Connection ... Success!\n");
		}
		else
		{
			printf("Billing System Close Connection ... Failure!\n");
		}
		if (m_bBillingInitialized)
		{
			if (ipBTI_Terminate())
			{
				printf("Billing System Terminate ... Success!\n");
				m_bBillingInitialized=false;
			}
			else
			{
				printf("Billing System Terminate ... Failure!\n");
			}
		}
	}
}

bool BillThread::CreateBillingSystem(HWND hwnd)
{
	//m_PlayerMgr.Initial();
	if (!m_bEnableBilling || m_bBillingInitialized) return true;

	LONG lDllVer=ipBTI_Initial((LONG)hwnd);
	if (lDllVer==0)
	{
		printf("Billing System Initial ... Failure!\n");
		return false;
	}
	printf("Billing System Initial ... Success! (BTI Dll Version %d)\n",lDllVer);
	m_bBillingInitialized=true;
	if (m_BillingService.Open(m_strBillService1.c_str(), m_strBillService2.c_str())==0)
	{
		printf("Billing System Open Connection ... Failure!\n");
		return false;
	}
	printf("Billing System Open Connection ... Success!\n");
	return true;
}

const char *BillThread::GetServerIP(int n)
{
	if (n==0)
	{
		return m_strBillService1.c_str();
	}
	else if (n==1)
	{
		return m_strBillService2.c_str();
	}
	return "";
}

void BillThread::AddPK(DataSocket * datasock,RPacket &pk)
{
	if (!m_bEnableBilling || !m_bBillingInitialized) return;
	PKQueue::AddPK(datasock,pk);
}

int BillThread::Run()
{
	while (!GetExitFlag() || (GetPkTotal() != 0))
	{
		PeekPacket(1000);		// 给于1秒的时间来采集队列中的网络包
	}

	ExitThread();
	return 0;
}

void BillThread::ProcessData(DataSocket* datasock, RPacket& rpkt)
{
	bool bRetry = true;				//看不懂有什么用途，保留(2005-11-30,Arcol). 补充:可能是复制AuthQueue::ProcessData(DataSocket* datasock, RPacket& rpkt)的处理,此参数用于重连接数据库,但在此无意义
	char const* lpszName = NULL;
	unsigned short usCmd = rpkt.ReadCmd();

	while (bRetry) {
		try {
			switch (usCmd) {
			case CMD_PA_USER_BILLBGN:
				{
					lpszName = rpkt.ReadString();
					//m_PlayerMgr.PlayerLogin(lpszName);

					if (m_bEnableBilling && m_bBillingInitialized)
					{
						char const* pszPassport = rpkt.ReadString();
						//printf("--->BEGIN [%s], PASSPORT [%s]\n", lpszName, pszPassport);
						if (strcmp(pszPassport, "nobill") == 0)
						{
							;
						}
						else
						{
							m_BillingService.Start(lpszName, pszPassport);
						}
					}
				}
				break;

			case CMD_PA_USER_BILLEND:
				{
					lpszName = rpkt.ReadString();
					//m_PlayerMgr.PlayerLogout(lpszName);

					if (m_bEnableBilling && m_bBillingInitialized)
					{
						//printf("<---END [%s]\n", lpszName);
						m_BillingService.Stop(lpszName);
					}
				}
				break;

			case CMD_PA_GROUP_BILLEND_AND_LOGOUT:
				break;

			default:
				LG("BillServiceWarning", "Unknown usCmd=[%d], Skipped...\n", usCmd);
				break;
			}
			bRetry = false;
		}
		catch (CSQLException* se)
		{
			LG("BillProcessDataExcp", "SQL Exception: %s\n", se->m_strError.c_str());
			bRetry = false; // 测试中，放过
		}
		catch (...)
		{
			LG("BillProcessDataExcp", "Unknown exception raised from BillService::ProcessData()\n");
			bRetry = false; // 非数据库造成的异常放过
		}
	}
}