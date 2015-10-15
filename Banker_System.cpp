#include "Banker_System.h"
#include <iostream>

using namespace std;
void System::setMaxAvailable(int max)
{
	maxAvailable = max > 0 ? max : 0;
}

void System::setAvailable(int num)
{
	available = num > 0 ? num : 0;
}

int System::getMaxAvailable(void) const
{
	return maxAvailable;
}

int System::getAvailable(void) const
{
	return available;
}

bool System::attainResource(int num,HANDLE *hMutex)
{
	bool rtn = false;

	if(num <= getMaxAvailable())
	{
		rtn = true;
		if(banker(num))
		{
			setAvailable(getAvailable() - num);
			ReleaseSemaphore(*hMutex, 1, NULL);
			cout << "banker success" << endl;
		}
		else
		{
			cout << "banker failure" << endl;
			addList(num, hMutex);
		}
	}
	else
	{
		cout << "banker failure" << endl;
		ReleaseSemaphore(*hMutex, 1, NULL);
		rtn = false;
	}
	return rtn;
}

bool System::assignResource(int num)
{
	bool rtn = false;
	if(num <= getAvailable())
	{
		rtn = true;
		setAvailable(getAvailable() - num);
	}
	return rtn;
}

bool System::banker(int num)
{
	bool rtn;
	int available_bk = getAvailable();
	ProcessList *list_head = getProcessListHead();
	ProcessList *list_tail = getProcessListTail();
	ProcessList *cur = list_head;

	setAvailable(getAvailable() - num);
	while(cur != NULL)
	{
		if(cur->finish = false && cur->need <= getAvailable())
		{
			cur->finish = true;
			setAvailable(getAvailable()+cur->need);
			cur = list_head;
			continue;
		}

		if(cur == list_tail)
		{
			if(cur->finish == false)
			{
				rtn = false;
				break;
			}
			else
			{
				rtn = true;
				break;
			}
			
		}
		cur = getProcessListNext(cur);
	}

	cur = list_head;
	while(cur != NULL)
	{
		cur->finish = false;
		cur = getProcessListNext(cur);
	}	
	setAvailable(available_bk);
	return rtn;
}

bool System::addList(int num, HANDLE *hMutex)
{
	struct ProcessList *node = new struct ProcessList();
	node->need = num;
	node->hMutex = hMutex;
	node->finish = false;
	node->next = NULL;

	ProcessList *head = getProcessListHead();
	if(head == NULL)
	{
		setProcessListHead(node);
		setProcessListTail(node);
	}
	else
	{
		setProcessListHead(node);
		node->next = head;
	}

	return true;
}

ProcessList *System::getProcessListHead(void) const
{
	return list_head;
}

ProcessList *System::getProcessListTail(void) const
{
	return list_tail;
}

ProcessList *System::getProcessListNext(ProcessList *cur)
{
	if (cur == NULL)
		return NULL;
	return cur->next;
}

void System::setProcessListHead(ProcessList *head)
{
	list_head = head;
}

void System::setProcessListTail(ProcessList *tail)
{
	list_tail = tail;
}