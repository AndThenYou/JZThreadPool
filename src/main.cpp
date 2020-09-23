#include "JZThreadPool.h"

class MyTask : public Task
{
public:
	MyTask(){}

	virtual int run()
	{
#ifdef LINUX_OS
		printf("thread[%lu] : %s\n",pthread_self(),(char*)this->m_pUserData);
#else
		printf("thread[%lu] : %s\n", GetCurrentThreadId(), (char*)this->m_pUserData);
#endif
		return 0;
	}
};

int main()
{

	char szTmp[] = "hello world";

	MyTask taskObj;
	taskObj.SetUserData((void*)szTmp);

	CJZThreadPool threadPool(10);
	for (int i = 0; i < 20; i++)
	{
		threadPool.AddTask(&taskObj);
	}

	while (1)
	{
		printf("there are still %d tasks need to process\n", threadPool.Size());
		if (threadPool.Size() == 0)
		{
			threadPool.StopAll();
			printf("Now I will exit from main\n");
			break;
		}
#ifdef MSWINDOWS_OS
		Sleep(1000);
#else
		sleep(1);
#endif
	}
#ifdef MSWINDOWS_OS
	system("pause");
#endif
	return 0;
}