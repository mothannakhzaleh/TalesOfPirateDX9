

#include "stdafx.h"
#include "ThreadBase.h"
#include <process.h>



unsigned int __stdcall ThreadFunc(void* param)
{
    return ((CThreadBase*)param)->Run();
}


CThreadBase::CThreadBase()
{
}


CThreadBase::~CThreadBase()
{
}


bool CThreadBase::Begin(int flag)
{
    // 创建的时候先挂起
    m_thread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, &m_threadid);

	LG( "threadid", "%d:%s\n", m_threadid, "CThreadBase" );

    if(m_thread)
    {
        // 指定 CPU 运行后再开始运行
        SetThreadAffinityMask(m_thread, 1);
        ResumeThread(m_thread);
    }

    return NULL != m_thread;
}


bool CThreadBase::Resume()
{
    return -1 != ResumeThread((HANDLE)m_thread);
}


bool CThreadBase::Suspend()
{
    return -1 != SuspendThread((HANDLE)m_thread);
}


bool CThreadBase::Terminate()
{
    return TRUE == TerminateThread((HANDLE)m_thread, 0);
}

