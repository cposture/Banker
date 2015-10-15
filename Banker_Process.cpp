#ifndef BANKER_PROCESS
#define BANKER_PROCESS

#include "Banker_Process.h"
#include "Banker_System.h"
#include <Windows.h>

void Process::setMaxNeed(int max)
{
	maxNeed = max > 0 ? max:0;
}

void  Process::setOwnNeed(int own)
{
	ownNeed = own > 0 ? own:0;
}

int  Process::getMaxNeed(void) const
{
	return maxNeed;
}

int  Process::getOwnNeed(void) const
{
	return ownNeed;
}

bool  Process::requestResource(int num, System &s)
{
	bool rtn = false;
	HANDLE hMutex;

	if(num < getMaxNeed() - getOwnNeed())
	{
		hMutex = CreateSemaphore(NULL, 1, 1, NULL);
		rtn = s.attainResource(num, &hMutex);
		WaitForSingleObject(hMutex, INFINITE);

		CloseHandle(hMutex);
		
		if(rtn)
		{
			setOwnNeed(getOwnNeed()+num);
		}
	}
	return rtn;
}
#endif