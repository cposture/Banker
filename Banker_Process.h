#include <Windows.h>
#include "Banker_System.h"

class Process{
public:
	Process(int max=0,int own=0)
	{
		setMaxNeed(max);
		setOwnNeed(own);
	}
	void setMaxNeed(int max);
	void setOwnNeed(int own);
	int getMaxNeed(void) const;
	int getOwnNeed(void) const;
	bool requestResource(int num, System &s);
private:
	int maxNeed;
	int ownNeed;
};

