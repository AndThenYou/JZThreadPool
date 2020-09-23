#ifndef _MSWINAPI_H_
#define _MSWINAPI_H_

#include "JZTypes.h"


#ifdef MSWINDOWS_OS

#include <process.h>
#include <Windows.h>

typedef VOID (WINAPI* sInitCondVar)
(PCONDITION_VARIABLE condtion_variable);

typedef BOOL(WINAPI* sSleepCondVar)
(PCONDITION_VARIABLE condtion_variable,
PCRITICAL_SECTION critcal_section,
DWORD dwMillsecond);

typedef BOOL(WINAPI* sSleepCondVarSRW)
(PCONDITION_VARIABLE condtion_variable,
PSRWLOCK SRWLock,
DWORD dwMillsecond,
ULONG flags);

typedef VOID(WINAPI* sWakeAllCondVar)
(PCONDITION_VARIABLE condtion_variable);

typedef VOID (WINAPI* sWakeCondVar)
(PCONDITION_VARIABLE condtion_variable);


extern sInitCondVar pInitCondVar;
extern sSleepCondVar pSleepCondVar;
extern sSleepCondVarSRW pSleepCondVarSRW;
extern sWakeAllCondVar pWakeAllCondVar;
extern sWakeCondVar pWakeCondVar;

extern void _MSWinInstanceSysFunc(void);
extern volatile LONG g_CntInstanceSysFunc;

inline static bool IsHasCondVarApi()
{
	if (g_CntInstanceSysFunc)
	{
		return (pInitCondVar != NULL);
	}
	_MSWinInstanceSysFunc();
	return (pInitCondVar != NULL);
}


#endif


#endif