#define _CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>

#include "windows.h"
#include "BTIService.h"
#include "BTI.h"

CBTI::CBTI() : m_openflag(false)
{
}

CBTI::~CBTI()
{
}

bool CBTI::Open(char const* ip1, char const* ip2)
{
	if (m_openflag) return false;

	wchar_t temp[100];

	mbstowcs(temp, ip1, 99);
	BSTR bstrIp1 = ::SysAllocString(temp);

	mbstowcs(temp, ip2, 99);
	BSTR bstrIp2 = ::SysAllocString(temp);

	LONG lRet=ipBTI_Open(&bstrIp1, &bstrIp2);

	::SysFreeString(bstrIp2);
	::SysFreeString(bstrIp1);

	if (lRet<=0) return false;
	m_openflag=true;
	return true;
}

bool CBTI::Close()
{
	if (m_openflag)
	{
		LONG lRet=ipBTI_Close();
		if (lRet>0)
		{
			m_openflag = false;
			return true;
		}
	}
	return false;
}

long CBTI::Start(char const* name, char const* passport)
{
	long lRet=0;
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	mbstowcs(temp, passport, 99);
	BSTR bstrPassport = ::SysAllocString(temp);

	lRet=ipBTI_GAME(&bstrName, &bstrPassport);

	::SysFreeString(bstrName);
	::SysFreeString(bstrPassport);

	return lRet;
}

void CBTI::Stop(char const* name)
{
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	ipBTI_STOP(&bstrName, 0);

	::SysFreeString(bstrName);
}

long CBTI::GetData(char const* name, long v)
{
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	//LONG lret = ipBTI_DATA(&bstrName, v);

	::SysFreeString(bstrName);
	return 0;//lret;
}

long CBTI::Buys(char const* name, long credit, char const* obj)
{
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	mbstowcs(temp, obj, 99);
	BSTR bstrObj = ::SysAllocString(temp);

	//LONG lret = ipBTI_BUYS(&bstrName, credit, &bstrObj);

	::SysFreeString(bstrObj);
	::SysFreeString(bstrName);
	return 0;//lret;
}

long CBTI::Sale(char const* name, long credit, char const* obj)
{
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	mbstowcs(temp, obj, 99);
	BSTR bstrObj = ::SysAllocString(temp);

	//LONG lret = ipBTI_SALE(&bstrName, credit, &bstrObj);

	::SysFreeString(bstrObj);
	::SysFreeString(bstrName);
	return 0;//lret;
}

long CBTI::Deal(char const* p_name, char const* m_name, long credit, char const* obj)
{
	wchar_t temp[100];

	mbstowcs(temp, p_name, 99);
	BSTR bstrPName = ::SysAllocString(temp);

	mbstowcs(temp, m_name, 99);
	BSTR bstrMName = ::SysAllocString(temp);

	mbstowcs(temp, obj, 99);
	BSTR bstrObj = ::SysAllocString(temp);

	//LONG lret = ipBTI_DEAL(&bstrPName, &bstrMName, credit, &bstrObj);

	::SysFreeString(bstrObj);
	::SysFreeString(bstrMName);
	::SysFreeString(bstrPName);
	return 0;//lret;
}

long CBTI::Give(char const* name, long credit, char const* svc)
{
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	mbstowcs(temp, svc, 99);
	BSTR bstrObj = ::SysAllocString(temp);

	//LONG lret = ipBTI_GIVE(&bstrName, credit, &bstrObj);

	::SysFreeString(bstrObj);
	::SysFreeString(bstrName);
	return 0;//lret;
}

long CBTI::Bill(char const* name, long credit, char const* svc)
{
	wchar_t temp[100];

	mbstowcs(temp, name, 99);
	BSTR bstrName = ::SysAllocString(temp);

	mbstowcs(temp, svc, 99);
	BSTR bstrObj = ::SysAllocString(temp);

	//LONG lret = ipBTI_BILL(&bstrName, credit, &bstrObj);

	::SysFreeString(bstrObj);
	::SysFreeString(bstrName);
	return 0;//lret;
}
