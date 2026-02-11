#include "stdafx.h"
#include "TradeLogDB.h"
#include "Config.h"

BOOL CTradeLogDB::Init()
{T_B
	m_bInitOK = FALSE;

	if(g_Config.m_bTradeLogIsConfig)
	{
		const char* buf = g_Config.m_szTradeLogDBPass;
		if(strcmp(buf,"\"\"") == 0 || strcmp(buf,"''") == 0 || strcmp(buf,"22222") == 0)
		{
			LG("trade log db", "Database  Password Error!");
			return FALSE;
		}

		_connect.enable_errinfo();

		printf("Connectting database [%s : %s]......\n", g_Config.m_szTradeLogDBIP, g_Config.m_szTradeLogDBName);


		std::string err_info;
		//if(!pswd.c_str() || pswd.length() == 0)
		//{
		//	LG("gamedb", "Database  Password Error!");
		//	return FALSE;
		//}
		bool r = _connect.connect(g_Config.m_szTradeLogDBIP, g_Config.m_szTradeLogDBName, g_Config.m_szTradeLogDBUsr, g_Config.m_szTradeLogDBPass, err_info);
		if(!r)
		{
			LG("trade log db", "msgDatabase [%s] Connect Failed!, ERROR REPORT[%d]", g_Config.m_szTradeLogDBName, err_info.c_str() );
			return FALSE;
		}

		printf("Database Connected!\n");

		_tab_log   = new CTradeTableLog(&_connect);

		if (!_tab_log )
			return FALSE;
	}

	m_bInitOK = TRUE;

	return TRUE;
T_E}


CTradeLogDB tradeLog_db;
