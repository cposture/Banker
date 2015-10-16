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
	int res = 0;
	HANDLE hThread = GetCurrentThread();

	if(num <= getMaxNeed() - getOwnNeed())
	{
		res = s.attainResource(num,&hThread);
		
		switch (res)
		{
		case 0:
			setOwnNeed(getOwnNeed()+num);
			rtn = true;
			break;
		case 1:
			SuspendThread(hThread);
			rtn = true;
			break;
		case 2:
			rtn = false;
			break;
		}
	}
	return rtn;
}

bool Process::freeSource(int num, System &s)
{
	if(num <=0)
		return false;
	s.setAvailable(s.getAvailable()+num);
	return true;
}
#endif