#ifndef BANKER_SYSTEM_H
#define BANKER_SYSTEM_H

#include <Windows.h>

class Process;

typedef struct ProcessList{
	int need;
	HANDLE *hMutex;
	bool finish;
	ProcessList *next;
}ProcessList;

class System{
public:
	System(int max=0, ProcessList *head=NULL, ProcessList *tail = NULL){
		setMaxAvailable(max);
		setAvailable(max);
		list_head = head;
		list_tail = tail;
	}
	void setMaxAvailable(int max);
	void setAvailable(int num);
	int getMaxAvailable(void) const;
	int getAvailable(void) const;
	int attainResource(int num,HANDLE *hThread);
private:
	int maxAvailable;
	int available;
	ProcessList *list_head;
	ProcessList *list_tail;

	bool assignResource(int num);
	bool banker(int num);
	bool addList(int num, HANDLE *hMutex);
	ProcessList *getProcessListHead(void) const;
	ProcessList *getProcessListTail(void) const;
	ProcessList *getProcessListNext(ProcessList *cur);
	void setProcessListHead(ProcessList *head);
	void setProcessListTail(ProcessList *tail);
};
#endif