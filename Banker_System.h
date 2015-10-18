#ifndef BANKER_SYSTEM_H
#define BANKER_SYSTEM_H

#include <Windows.h>

class Process;

typedef struct ProcessList{
	unsigned int need;
	HANDLE *hMutex;
	bool finish;
	ProcessList *next;
}ProcessList;

class System{
public:
	System(unsigned int max=0, ProcessList *head=NULL, ProcessList *tail = NULL){
		setMaxAvailable(max);
		setAvailable(max);
		list_head = head;
		list_tail = tail;
	}
	void setMaxAvailable(unsigned int max);
	void setAvailable(unsigned int num);
	unsigned int getMaxAvailable(void) const;
	unsigned int getAvailable(void) const;
	unsigned int attainResource(unsigned int num,HANDLE *hThread);
private:
	unsigned int maxAvailable;
	unsigned int available;
	ProcessList *list_head;
	ProcessList *list_tail;

	bool assignResource(unsigned int num);
	bool banker(unsigned int num);
	bool addList(unsigned int num, HANDLE *hMutex);
	ProcessList *getProcessListHead(void) const;
	ProcessList *getProcessListTail(void) const;
	ProcessList *getProcessListNext(ProcessList *cur);
	void setProcessListHead(ProcessList *head);
	void setProcessListTail(ProcessList *tail);
};
#endif