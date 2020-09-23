#ifndef _JZTYPES_H_
#define _JZTYPES_H_

#if defined(_MSC_VER) || defined(WIN32) ||defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#define MSWINDOWS_OS
#include <Windows.h>
#include <process.h>
#else
#define LINUX_OS
#include <unistd.h>
#include <stdio.h>
#endif



#endif