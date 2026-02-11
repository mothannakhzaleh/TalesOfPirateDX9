#pragma once
#include "excp.h"

_DBC_BEGIN

//==============================================================================
struct TLSIndex
{
	TLSIndex():index(TlsAlloc())
	{
		if(index ==TLS_OUT_OF_INDEXES)
		{
			THROW_EXCP(excpThrd,"分配线程本地存储索引超过最大数");
		}
	}
	~TLSIndex()
	{
		TlsFree(index);
	}
	LPVOID	GetPointer(){return TlsGetValue(index);}
	BOOL	SetPointer(LPVOID p){return TlsSetValue(index,p);}
private:
	DWORD		index;
};

_DBC_END

