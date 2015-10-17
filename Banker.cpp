#include <Windows.h>
#include <process.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Banker_Process.h"
#include "Banker_System.h"

#define MAX_THREAD 3
#define MAX_AVAILABLE 2000

HANDLE g_mutex;

typedef struct Param
{
	System *system;
	int uid;
}Param;

unsigned int __stdcall myThread(void *param)
{
	int need = 0;
	Param *pa = static_cast<Param*>(param);
 	
 	srand(time(NULL));
 	while(need == 0)
 	{
 		need = rand()%MAX_AVAILABLE;
 	}
 	
	WaitForSingleObject(g_mutex, INFINITE);
	std::cout << "Thread " << pa->uid << "start" << std::endl;
	ReleaseSemaphore(g_mutex, 1, NULL);

	Process pro(need);
	
	while(pro.getOwnNeed() < pro.getMaxNeed())
	{
		bool res = pro.requestResource(rand()%need,*(pa->system));

		WaitForSingleObject(g_mutex, INFINITE);
		if(res == true)
		{
			std::cout << "system" << "\t" << pa->system->getAvailable() << std::endl;
			std::cout << "Thread " << pa->uid  << "\t" << pro.getMaxNeed() << "\t" 
			 	<< pro.getOwnNeed() << std::endl;
		}
		else
		{
			std::cout << "Thread " << pa->uid  << "(" << pro.getMaxNeed() << ")" 
			<< ":" << pro.getOwnNeed() << " Failure" << std::endl;
			
		}
		ReleaseSemaphore(g_mutex, 1, NULL);
	}

	WaitForSingleObject(g_mutex, INFINITE);
	std::cout << "Thread " << pa->uid  << "(" << pro.getMaxNeed() << ")" << "has request success! now free source" << std::endl;
	ReleaseSemaphore(g_mutex, 1, NULL);

	pro.freeSource(pro.getMaxNeed(), *(pa->system));

	return 0;
}

int main(int argc, char const *argv[])
{
	HANDLE hThread[MAX_THREAD];
	System s(MAX_AVAILABLE);
	Param par[MAX_THREAD];

	g_mutex = CreateSemaphore(NULL, 1, 1, NULL);

	WaitForSingleObject(g_mutex, INFINITE);
	std::cout << "Banker start" << std::endl;
	ReleaseSemaphore(g_mutex, 1, NULL);

	for(int i=0; i < MAX_THREAD; ++i)
	{
		par[i].system = &s;
		par[i].uid = i;
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, myThread, par + i, 0, NULL);
	}
	
	WaitForMultipleObjects(MAX_THREAD, hThread, TRUE, INFINITE);

	for(int i = 0; i< MAX_THREAD; ++i)
	{
		CloseHandle(hThread[i]);
	}
	return 0;
}

