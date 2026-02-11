#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>

#include "AccountServer2.h"
#include "inifile.h"
#include "NetCommand.h"
#include ".\billservice.h"
#include "GlobalVariable.h"


bool CBillService::m_bBillingInitialized=false;

CBillService::CBillService(void)
{
	IniFile inf(g_strCfgFile.c_str());
	IniSection& is = inf["bill"];
	int enable = std::stoi(is["enable_bill"]);
    int passport = std::stoi(is["enable_passport"]);
    int kickuser = std::stoi(is["enable_kickuser"]);
    m_bEnablePassport = (passport > 0);     //  add by jampe
    m_bEnableKickUser = (kickuser > 0);     //  add by jampe
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

CBillService::~CBillService(void)
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

bool CBillService::CreateBillingSystem(HWND hwnd)
{
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

const char *CBillService::GetServerIP(int n)
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

void CALLBACK CBillService::VerifyBillingCode(long nTaskID, LPARAM nErr)
{
    if(!m_bEnableKickUser)
    {
        return;
    }
	IntMap::iterator iter=m_mapTaskToUsername.find(nTaskID);
	if (iter==m_mapTaskToUsername.end())
	{
		LG("AccountServer", "CBillService::VerifyBillingCode: Can't found the task ID=%d and code = %d\r\n", nTaskID, (DWORD)nErr );
		return;
	}
	if (nErr)
	{
		if (!g_MainDBHandle.KickUser(iter->second))
		{
			LG("AccountServer", "CBillService::VerifyBillingCode: KickUser failed! UserName=%s and code = %d\r\n", iter->second.c_str(), (DWORD)nErr);
		}
		else
		{
			LG("AccountServer", "CBillService::VerifyBillingCode: KickUser success! UserName=%s and code = %d\r\n", iter->second.c_str(), (DWORD)nErr);
		}
		m_mapTaskToUsername.erase( iter );
	}
}

void CALLBACK CBillService::AdjustExpScale(long hour, long num)
{
    LG("ExpScale", "billing adjust exp scale. hour: %li, count: %li\n", hour, num);
    char seps[] = ",";
    //BSTR list[128][64] = {0};
    long array = num;
    BSTR** list;
    list = new BSTR*[array];
    for(int j = 0; j < array; j++)
    {
        list[j] = new BSTR[64];
        memset(list[j], 0, sizeof(BSTR) * 64);
    }
    long ret = 0;

    while(num)
    {
        ret = ipBTI_TIME(list, hour, array);
        LG("ExpScale", "call ipBTI_TIME. total return: %li\n", ret);
        num -= ret;
        for(long i = 0; i < ret; i++)
        {
            LG("nameList_log","name: %s \n", (const char*)list[i]);
            g_MainDBHandle.SetExpScale((const char*)list[i], hour);
        }
    }
    for(int k = 0; k < array; k++)
    {
        delete [] list[k];
    }
    delete [] list;
}

bool CBillService::BeginBilling(std::string strUserName, std::string strPassport)
{
	if (m_bEnableBilling && m_bBillingInitialized)
	{
		if (!strUserName.c_str() || strUserName=="")
		{
			LG("AccountServer", "CBillService::BeginBilling: parameter strUserName is empty or null\n");
			return false;
		}
		//LG("AccountServer", "CBillService::BeginBilling: UserName=[%s] begin billing\n", strUserName.c_str());

		if (strcmp(strPassport.c_str(), "nobill") == 0)
		{
		}
		else
		{
			long nTaskID=m_BillingService.Start(strUserName.c_str(), strPassport.c_str());
			if (nTaskID != 0)
			{
				LG("AccountServer", "CBillService::BeginBilling: UserName=[%s] begin billing start! ID = %d\n", strUserName.c_str(), nTaskID );
				m_mapTaskToUsername[nTaskID]=strUserName;
			}
			else
			{
				LG("AccountServer", "CBillService::BeginBilling: UserName=[%s] begin billing start failed and kick user! ID = %d\n", strUserName.c_str(), nTaskID );
				return g_MainDBHandle.KickUser(strUserName);
			}
		}
	}
	return true;
}

bool CBillService::EndBilling(std::string strUserName)
{
	if (m_bEnableBilling && m_bBillingInitialized)
	{
		LG("AccountServer", "CBillService::BeginBilling: UserName=[%s] begin billing end!\n", strUserName.c_str() );
		m_BillingService.Stop(strUserName.c_str());
	}
	return true;
}


//  add by jampe
bool CBillService::IsEnablePassport()
{
    return m_bEnablePassport;
}

bool CBillService::IsEnableKickUser()
{
    return m_bEnableKickUser;
}
//  end add

