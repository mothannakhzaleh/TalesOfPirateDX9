//  [10/14/2005]	从AccountServer2文件里进行模块拆分,分离出此模块 - by Arcol
#pragma once

#ifndef _BILLTHREAD_H_
#define _BILLTHREAD_H_

#include "commrpc.h"
#include "packetqueue.h"
#include "BTIService.h"
//#include "Player.h"
//#include "PlayerMgr.h"

using namespace dbc;

class BillThread : public MyThread, public PKQueue
{
public:
	BillThread();
	virtual ~BillThread();
	bool CreateBillingSystem(HWND hwnd);
	void AddPK(DataSocket * datasock,RPacket &pk);
	const char *GetServerIP(int n);

protected:
	virtual int Run();
	virtual void ProcessData(DataSocket* datasock, RPacket& rpkt);

private:
	//CPlayerMgr m_PlayerMgr;
	CBTI m_BillingService;
	bool m_bEnableBilling;
	static bool m_bBillingInitialized;
	std::string m_strBillService1;
	std::string m_strBillService2;
};

#endif