// JZThreadPool.cpp
//
#include "JZThreadPool.h"
//#include "MSWinApi.h"
#include <assert.h>


CJZThreadPool::CJZThreadPool(int nThreadNum)
{
	m_bRunning = true;
	m_nThreadNums = nThreadNum;
	MyCreateThread();
}

CJZThreadPool::~CJZThreadPool()
{

}

int CJZThreadPool::MyCreateThread(void)
{
	m_vecThread.clear();
#ifdef MSWINDOWS_OS
	InitializeCriticalSection(&m_mutex);
	InitializeConditionVariable(&m_condition);
	//pInitCondVar(&m_condition);
	for (int i = 0; i < m_nThreadNums; i++)
	{
		unsigned int id;
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, EntryThreadFunc, this, 0, (unsigned int*)&id);
		if (hThread == NULL)
		{
			m_bRunning = false;
			assert(hThread);
			return false;
		}
		m_vecThread.push_back(hThread);
	}
#else
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condition, NULL);
	for (int i = 0; i < m_nThreadNums; i++)
	{
		pthread_t *pThread = (pthread_t*)malloc(sizeof(pthread_t));
		int nRet = pthread_create(pThread, NULL, EntryThreadFunc, this);
		if (nRet != 0)
		{
			free(pThread);
			assert(0);
			return false;
		}
		m_vecThread.push_back(pThread);
	}
#endif
	return 0;
}

#ifdef MSWINDOWS_OS
unsigned CJZThreadPool::EntryThreadFunc(HANDLE threadData)
{
	DWORD tid = GetCurrentThreadId();
	CJZThreadPool* pool = static_cast<CJZThreadPool*>(threadData);
	while (pool->m_bRunning)
	{
		Task* task = pool->Take();
		if (!task)
		{
			printf("thread %lu will exit\n", tid);
			break;
		}

		assert(task);
		task->run();
	}
	return 0;
}
#else
void * CJZThreadPool::EntryThreadFunc(void* threadData)
{

	pthread_t tid = pthread_self();
	CJZThreadPool* pool = static_cast<CJZThreadPool*>(threadData);
	while (pool->m_bRunning)
	{
		Task* task = pool->Take();
		if (!task)
		{
			break;
		}

		assert(task);
		task->run();
	}
	printf("thread %lu will exit\n", tid);
	return 0;
}
#endif

Task* CJZThreadPool::Take(void)
{
	Task *pTask = NULL;
	while (!pTask)
	{
#ifdef MSWINDOWS_OS
		EnterCriticalSection(&m_mutex);
		while(m_bRunning && m_queTask.empty())
		{
			SleepConditionVariableCS(&m_condition, &m_mutex, INFINITE);
		}

		if (!m_bRunning)
		{
			LeaveCriticalSection(&m_mutex);
			break;
		}
		else if (m_queTask.empty())
		{
			LeaveCriticalSection(&m_mutex);
			continue;
		}

		assert(!m_queTask.empty());
		pTask = m_queTask.front();
		m_queTask.pop_front();
		LeaveCriticalSection(&m_mutex);
#else
		pthread_mutex_lock(&m_mutex);
		while (m_queTask.empty() && m_bRunning)
		{
			pthread_cond_wait(&m_condition, &m_mutex);
		}

		if (!m_bRunning)
		{
			pthread_mutex_unlock(&m_mutex);

			break;
		}
		else if (m_queTask.empty())
		{
			pthread_mutex_unlock(&m_mutex);
			continue;
		}

		assert(!m_queTask.empty());
		pTask = m_queTask.front();
		m_queTask.pop_front();
		pthread_mutex_unlock(&m_mutex);
#endif
	}
	return pTask;
}

bool CJZThreadPool::AddTask(Task* pTask)
{
#ifdef MSWINDOWS_OS
	EnterCriticalSection(&m_mutex);
	m_queTask.push_back(pTask);
	LeaveCriticalSection(&m_mutex);
	WakeConditionVariable(&m_condition);
	return true;
#else
	pthread_mutex_lock(&m_mutex);
	m_queTask.push_back(pTask);
	pthread_mutex_unlock(&m_mutex);
	pthread_cond_signal(&m_condition);
	return true;
#endif
}

void CJZThreadPool::StopAll(void)
{
	if (!m_bRunning)
	{
		return;
	}
	m_bRunning = false;
#ifdef MSWINDOWS_OS
	WakeAllConditionVariable(&m_condition);
	for (int i = 0; i < m_nThreadNums; i++)
	{
		WaitForSingleObject(m_vecThread[i], INFINITE);
		CloseHandle(m_vecThread[i]);
	}
	DeleteCriticalSection(&m_mutex);

#else
	pthread_cond_broadcast(&m_condition);

	for (int i = 0; i < m_nThreadNums; i++)
	{
		pthread_join(*m_vecThread[i], NULL);
		free(m_vecThread[i]);
	}
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_condition);
#endif
}

int CJZThreadPool::Size(void)
{
	if (!m_bRunning)
		return 0;
	int nSize = 0;
#ifdef MSWINDOWS_OS
	EnterCriticalSection(&m_mutex);
	nSize = m_queTask.size();
	LeaveCriticalSection(&m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
	nSize = m_queTask.size();
	pthread_mutex_unlock(&m_mutex);
#endif
	return nSize;
}