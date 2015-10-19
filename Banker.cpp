#include <Windows.h>
#include <process.h>
#include <iostream>
#include <cstdlib>
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
	unsigned int need = GetCurrentThreadId();
	Param *pa = static_cast<Param*>(param);
 	
 	srand(need);
 	do{
 		need = rand() % MAX_AVAILABLE;
 	}while(need == 0);
 	
	WaitForSingleObject(g_mutex, INFINITE);
	std::cout << "Thread " << pa->uid << "start" << std::endl;
	ReleaseSemaphore(g_mutex, 1, NULL);

	Process pro(need);
	
	while(pro.getOwnNeed() < pro.getMaxNeed())
	{
		unsigned int resource = 0;

		do{
			if(pro.getMaxNeed() - pro.getOwnNeed() > 1)
				resource = rand()% (pro.getMaxNeed() - pro.getOwnNeed());
			else
				resource = 1;
		}while(resource == 0);

		WaitForSingleObject(g_mutex, INFINITE);
		std::cout << "Thread " << pa->uid  << "<<<\t" << resource << std::endl;
		ReleaseSemaphore(g_mutex, 1, NULL);

		bool res = pro.requestResource(resource,*(pa->system));

		WaitForSingleObject(g_mutex, INFINITE);
		if(res == true)
		{
			std::cout << "system" << "\t" << pa->system->getAvailable() << std::endl;
			std::cout << "Thread " << pa->uid  << "\t" << pro.getMaxNeed() << "\t" 
			 	<< pro.getOwnNeed() << "\t" << resource << "\t" << " Success" << std::endl;
		}
		else
		{
			std::cout << "system" << "\t" << pa->system->getAvailable() << std::endl;
			std::cout << "Thread "  << pa->uid  << "\t" << pro.getMaxNeed() << "\t" 
				<< pro.getOwnNeed() << "\t" << resource << "\t" << " Failure" << std::endl;
			
		}
		ReleaseSemaphore(g_mutex, 1, NULL);
	}

	WaitForSingleObject(g_mutex, INFINITE);
	std::cout << "Thread " << pa->uid  << "(" << pro.getMaxNeed() << ")" << "has request success! now free source" << std::endl;
	ReleaseSemaphore(g_mutex, 1, NULL);

	pro.freeSource(*(pa->system));

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

