#ifndef BANKER_SYSTEM_H
#define BANKER_SYSTEM_H

#include <Windows.h>
#include <set>

class Process;

class System{
public:
	System(unsigned int max=0){
		setMaxAvailable(max);
		setAvailable(max);
	}
	void setMaxAvailable(unsigned int max);
	void setAvailable(unsigned int num);
	unsigned int getMaxAvailable(void) const;
	unsigned int getAvailable(void) const;
	unsigned int attainResource(Process *process);
	bool freeResource(Process *process);
private:
	unsigned int maxAvailable;
	unsigned int available;
	std::set<Process*> processList;

	bool banker(unsigned int num);
};
#endif