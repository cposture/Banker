#ifndef BANKER_PROCESS
#define BANKER_PROCESS

#include "Banker_Process.h"
#include "Banker_System.h"
#include <Windows.h>
#include <iostream>

using namespace std;

extern HANDLE g_mutex;

void Process::setMaxNeed(unsigned int max)
{
	maxNeed = max > 0 ? max:0;
}

void  Process::setOwnNeed(unsigned int own)
{
	ownNeed = own > 0 ? own:0;
}

unsigned int  Process::getMaxNeed(void) const
{
	return maxNeed;
}

unsigned int  Process::getOwnNeed(void) const
{
	return ownNeed;
}

bool  Process::requestResource(unsigned int num, System &s)
{
	bool rtn = false;
	int res = 0;
	HANDLE hPseudoThread = GetCurrentThread();
	HANDLE hProcess = GetCurrentProcess();
	HANDLE hRealThread=NULL;

	if(num <= getMaxNeed() - getOwnNeed())
	{
		DuplicateHandle(hProcess, hPseudoThread, hProcess, &hRealThread, 0, false, 0);
		res = s.attainResource(num,&hRealThread);
		
		switch (res)
		{
		case 0:
			WaitForSingleObject(g_mutex, INFINITE);
			cout << "Succ" << endl;
			ReleaseSemaphore(g_mutex, 1, NULL);
			setOwnNeed(getOwnNeed()+num);
			rtn = true;
			break;
		case 1:
			WaitForSingleObject(g_mutex, INFINITE);
			cout << "Fail" << endl;
			ReleaseSemaphore(g_mutex, 1, NULL);
			SuspendThread(hPseudoThread);
			rtn = true;
			break;
		}
	}
	return rtn;
}

bool Process::freeSource(unsigned int num, System &s)
{
	if(num <=0)
		return false;
	s.setAvailable(s.getAvailable()+num);
	return true;
}
#endif