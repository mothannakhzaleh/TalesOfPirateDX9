#pragma once

#include "packetqueue.h"
#include "BTIService.h"
#include "PlayerMgr.h"


class CBillService	//只允许创建一个全局对象,不支持多线程
{
public:
	CBillService(void);
	~CBillService(void);
	bool CreateBillingSystem(HWND hwnd);
	const char *GetServerIP(int n);

	bool BeginBilling(std::string strUserName, std::string strPassport);
	bool EndBilling(std::string strUserName);

	void CALLBACK VerifyBillingCode(long nTaskID, LPARAM nErr);
    void CALLBACK AdjustExpScale(long hour, long num);

    bool IsEnablePassport();    //  add by jampe
    bool IsEnableKickUser();    //  add by jampe

private:
	CBTI m_BillingService;
	bool m_bEnableBilling;
    bool m_bEnablePassport;     //  add by jampe
    bool m_bEnableKickUser;     //  add by jampe
	static bool m_bBillingInitialized;
	std::string m_strBillService1;
	std::string m_strBillService2;

private:
	typedef std::map<long, std::string> IntMap;
	IntMap m_mapTaskToUsername;
};
