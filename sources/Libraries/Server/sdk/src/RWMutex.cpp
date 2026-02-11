//================================================================
// It must be permitted by Dabo.Zhang that this program is used for
// any purpose in any situation.
// Copyright (C) Dabo.Zhang 2000-2003
// All rights reserved by ZhangDabo.
// This program is written(created) by Zhang.Dabo in 2000.3
// This program is modified recently by Zhang.Dabo in 2003.7
//=================================================================

#include "RWMutex.h"
#include "TLSIndex.h"

_DBC_USING

//==============================================================================
class AccThread:public PreAllocStru{
public:
	AccThread(uLong size):PreAllocStru(size),m_rwsync(0),m_RDCount(0),m_WRCount(0),nextthread(0){};
	~AccThread(){if(nextthread)delete nextthread;}
private:
	void Initially(){m_rwsync =0;m_RDCount =m_WRCount =0;}
	void Finally(){nextthread =0;}

	friend class	RWMutex;
	RWMutex		*	m_rwsync;
	volatile int	m_RDCount;				//读计数
	volatile int	m_WRCount;				//写计数
	AccThread	*	nextthread;				//链式结构的下一个项目
};
typedef AccThread * PAccThread;
static TLSIndex st_tls;
static PreAllocHeapPtr<AccThread> st_FreeAccThread(0,20);
//==============================================================================
void RWMutex::BeginRead()
{
	AccThread	*	l_AccThread,*l_bakAccThread;

	l_bakAccThread	=l_AccThread	=(PAccThread)st_tls.GetPointer();
	while(l_AccThread){
		if(l_AccThread->m_rwsync	==this){break;}
		l_AccThread	=l_AccThread->nextthread;
	}
	if(!l_AccThread){
		l_AccThread	=st_FreeAccThread.Get();
		l_AccThread->m_rwsync	=this;
		l_AccThread->nextthread		=l_bakAccThread;
		st_tls.SetPointer(l_AccThread);
	}

	m_mtxRWCount.lock();
	try{
		while(m_WRCount&&(!l_AccThread->m_WRCount)){	//有写线程并且不是当前线程在写,就阻塞在读锁上
			m_mtxRWCount.unlock();
			try{
				m_semRead.lock();	//阻塞读线程
			}catch(...){}
			m_mtxRWCount.lock();
		}
		m_semRead.unlock();//释放让其他读线程并发操作

		l_AccThread->m_RDCount++;
		m_RDCount++;
	}catch(...){}
	m_mtxRWCount.unlock();
};
//==============================================================================
void RWMutex::BeginWrite(){
	AccThread	*	l_AccThread,*l_bakAccThread;

	l_bakAccThread	=l_AccThread	=(PAccThread)st_tls.GetPointer();
	while(l_AccThread){
		if(l_AccThread->m_rwsync	==this){break;}
		l_AccThread	=l_AccThread->nextthread;
	}
	if(!l_AccThread){
		l_AccThread	=st_FreeAccThread.Get();
		l_AccThread->m_rwsync	=this;
		l_AccThread->nextthread		=l_bakAccThread;
		st_tls.SetPointer(l_AccThread);
	}

	m_mtxRWCount.lock();
	try{
		while((m_WRCount &&(!l_AccThread->m_WRCount))||				//有写线程并且不是当前线程在写或者
				(m_RDCount &&(m_RDCount !=l_AccThread->m_RDCount))){	//有读线程并且不只当前线程在读,就阻塞在写锁上
			m_mtxRWCount.unlock();
			try{
				m_semWrite.lock();										//阻塞写线程
			}catch(...){}
			m_mtxRWCount.lock();
		}

		l_AccThread->m_WRCount++;
		m_WRCount++;
	}catch(...){}
	m_mtxRWCount.unlock();
};
//==============================================================================
void RWMutex::EndRead()
{
	AccThread	*	l_AccThread,*l_bakAccThread;

	l_bakAccThread =0;
	l_AccThread	=(PAccThread)st_tls.GetPointer();
	while(l_AccThread){
		if(l_AccThread->m_rwsync==this){break;}
		l_bakAccThread =l_AccThread;
		l_AccThread	=l_AccThread->nextthread;
	}
	m_mtxRWCount.lock();
	try{
		m_RDCount --;
		l_AccThread->m_RDCount --;
		m_semWrite.unlock();		//读线程,释放写锁,让写锁检查

		if(!l_AccThread->m_RDCount && !l_AccThread->m_WRCount){ //最后一次操作
			m_mtxRWCount.unlock();

			if(l_bakAccThread){
				l_bakAccThread->nextthread =l_AccThread->nextthread;
			}else{
				st_tls.SetPointer(l_AccThread->nextthread);
			}
			l_AccThread->Free();
		}else{
			m_mtxRWCount.unlock();
		}
	}catch(...){
		m_mtxRWCount.unlock();
	}
};
//==============================================================================
void RWMutex::EndWrite(){
	AccThread	*	l_AccThread,*l_bakAccThread;

	l_bakAccThread =0;
	l_AccThread	=(PAccThread)st_tls.GetPointer();
	while(l_AccThread){
		if(l_AccThread->m_rwsync	==this){break;}
		l_bakAccThread =l_AccThread;
		l_AccThread	=l_AccThread->nextthread;
	}
	m_mtxRWCount.lock();
	try{
		m_WRCount --;
		l_AccThread->m_WRCount --;
		m_semWrite.unlock();//释放写锁
		m_semRead.unlock();//释放读锁
		if(!l_AccThread->m_WRCount && !l_AccThread->m_RDCount){ //最后一次操作
			m_mtxRWCount.unlock();
			if(l_bakAccThread){
				l_bakAccThread->nextthread =l_AccThread->nextthread;
			}else{
				st_tls.SetPointer(l_AccThread->nextthread);
			}
			l_AccThread->Free();
		}else{
			m_mtxRWCount.unlock();
		}
	}catch(...){
		m_mtxRWCount.unlock();
	}
};
