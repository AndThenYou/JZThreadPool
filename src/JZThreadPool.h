#ifndef _JZTHREADPOOL_H_
#define _JZTHREADPOOL_H_

#include "JZTypes.h"
#include <stdlib.h>
#include <queue>
#include <string>
#include <vector>
#ifdef LINUX_OS
#include <pthread.h>
#else 
#include <Windows.h>
#endif

using std::vector;
using std::deque;
using std::string;

class Task
{
public:
	Task(void * arg = NULL)
		:m_pUserData(arg)
	{

	}
	
	virtual ~Task(){};

	void SetUserData(void *arg)
	{
		m_pUserData = arg;
	}

	virtual int run() = 0;

protected:
	void *m_pUserData;
};


class CJZThreadPool
{
public:
	CJZThreadPool(int nThreadNum = 10);

	~CJZThreadPool();

public:
	bool AddTask(Task* pTask);
	void StopAll(void);
	int Size(void);

private:
	int MyCreateThread(void);
#ifdef MSWINDOWS_OS
	static unsigned __stdcall EntryThreadFunc(HANDLE threadData);
#else
	static void * EntryThreadFunc(void* threadData);
#endif
	Task* Take(void);

private:
	CJZThreadPool& operator= (const CJZThreadPool&);
	CJZThreadPool(const CJZThreadPool&);

private:
	volatile bool		m_bRunning;
	int					m_nThreadNums;
	deque<Task*>		m_queTask;
#ifdef MSWINDOWS_OS
	vector<HANDLE>		m_vecThread;
	CRITICAL_SECTION	m_mutex;
	CONDITION_VARIABLE	m_condition;
#else
	vector<pthread_t*>	m_vecThread;
	pthread_mutex_t		m_mutex;
	pthread_cond_t		m_condition;
#endif
};



#endif