#include "Banker_System.h"
#include "Banker_Process.h"
//#include <iostream>

//using namespace std;

//extern HANDLE g_mutex;
void System::setMaxAvailable(unsigned int max)
{
	maxAvailable = max > 0 ? max : 0;
}

void System::setAvailable(unsigned int num)
{
	available = num > 0 ? num : 0;
}

unsigned int System::getMaxAvailable(void) const
{
	return maxAvailable;
}

unsigned int System::getAvailable(void) const
{
	return available;
}

unsigned int System::attainResource(Process *process)
{
	int rtn = 0;
	unsigned int requestNum = process->getRequestNeed();
	unsigned int available_bk = getAvailable();
	unsigned int ownNeed_bk = process->getOwnNeed();


	if(requestNum <= getAvailable())
	{
		setAvailable(getAvailable() - requestNum);
		process->setOwnNeed(process->getOwnNeed() + requestNum);

		if(banker(requestNum))
		{
			processList.insert(process);
			rtn = 0;
		}
		else
		{
			setAvailable(available_bk);
			process->setOwnNeed(ownNeed_bk);
			rtn = 1;
		}
	}
	else
	{
		rtn = 1;
	}
	return rtn;
}

bool System::banker(unsigned int num)
{
	bool rtn;
	unsigned int available_bk = getAvailable();

	for(auto i = processList.begin(); i != processList.end(); )
	{
		//WaitForSingleObject(g_mutex, INFINITE);
		//cout << "--=== " << boolalpha << (*i)->isFinish() <<endl;
		//ReleaseSemaphore(g_mutex, 1, NULL);
		if(false == (*i)->isFinish() && (*i)->getMaxNeed() - (*i)->getOwnNeed() <= getAvailable())
		{
			//cout << "++==  " << (*i)->getMaxNeed() << endl;
			(*i)->setFinish(true);
			setAvailable(getAvailable()+(*i)->getOwnNeed());
			i = processList.begin();
		}
		else
		{
			++i;
		}
	}
	rtn = true;
	for(auto i = processList.begin(); i != processList.end(); ++i)
	{
		if((*i)->isFinish() == false)
			rtn = false;
		(*i)->setFinish(false);
	}
	setAvailable(available_bk);
	return rtn;
}


bool System::freeResource(Process *process)
{
	if(processList.find(process) == processList.end())
		return false;
	setAvailable(getAvailable() + process->getOwnNeed());
	processList.erase(process);
	return true;
}