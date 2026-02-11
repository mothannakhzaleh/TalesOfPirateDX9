
/* * * * * * * * * * * * * * * * * * * *

    线程封装
    Jampe
    2006/3/27

 * * * * * * * * * * * * * * * * * * * */


#if !defined __THREAD_H__
#define __THREAD_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    virtual bool Begin(int flag = 0);  //  开始线程
    virtual bool Resume();  //  恢复线程
    virtual bool Suspend(); //  挂起线程
    virtual bool Terminate();   //  强制终止线程
    virtual bool SetPriority(int priority);     //  设置优先级
    virtual int GetPriority();  //  获取优先级
    virtual int Wait(int time = INFINITE);  //  等待时间
    virtual unsigned Run() = 0;

public:
    HANDLE          m_thread;
    unsigned        m_threadid;
};


#endif
