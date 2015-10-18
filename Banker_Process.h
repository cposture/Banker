#include <Windows.h>
#include "Banker_System.h"

class Process{
public:
	Process(unsigned int max=0,unsigned int own=0)
	{
		setMaxNeed(max);
		setOwnNeed(own);
	}
	void setMaxNeed(unsigned int max);
	void setOwnNeed(unsigned int own);
	unsigned int getMaxNeed(void) const;
	unsigned int getOwnNeed(void) const;
	bool requestResource(unsigned int num, System &s);
	bool freeSource(unsigned int num, System &s);
private:
	unsigned int maxNeed;
	unsigned int ownNeed;
};

