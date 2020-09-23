#include "MSWinApi.h"

#ifdef MSWINDOWS_OS

sInitCondVar pInitCondVar = NULL;
sSleepCondVar pSleepCondVar = NULL;
sSleepCondVarSRW pSleepCondVarSRW = NULL;
sWakeAllCondVar pWakeAllCondVar = NULL;
sWakeCondVar pWakeCondVar = NULL;

volatile LONG g_CntInstanceSysFunc = 0;

void _MSWinInstanceSysFunc(void)
{
	HMODULE kernel32_module = NULL;
	kernel32_module = GetModuleHandleA("kernel32.dll");
	if (kernel32_module == NULL)
	{
		InterlockedIncrement(&g_CntInstanceSysFunc);
		return;
	}

	pSleepCondVar = (sSleepCondVar)GetProcAddress(kernel32_module, "SleepConditionVariableCS");

	if (pSleepCondVar)
		pSleepCondVarSRW = (sSleepCondVarSRW)GetProcAddress(kernel32_module, "SleepConditionVariableSRW");

	if (pSleepCondVarSRW)
		pWakeAllCondVar = (sWakeAllCondVar)GetProcAddress(kernel32_module, "WakeAllConditionVariable");

	if (pWakeAllCondVar)
		pWakeCondVar = (sWakeCondVar)GetProcAddress(kernel32_module, "WakeConditionVariable");

	if (pWakeCondVar)
		pInitCondVar = (sInitCondVar)GetProcAddress(kernel32_module, "InitializeConditionVariable");

	InterlockedIncrement(&g_CntInstanceSysFunc);
}

#endif