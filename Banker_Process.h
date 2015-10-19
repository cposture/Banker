#include <Windows.h>
#include "Banker_System.h"

class Process{
public:
	Process(unsigned int max=0,unsigned int own=0)
	{
		setMaxNeed(max);
		setOwnNeed(own);
		setFinish(false);
	}
	void setMaxNeed(unsigned int max);
	void setOwnNeed(unsigned int own);
	unsigned int getMaxNeed(void) const;
	unsigned int getOwnNeed(void) const;
	bool requestResource(unsigned int num, System &s);
	bool freeSource(System &s);
	unsigned int getRequestNeed(void);
	bool setRequestNeed(unsigned int r);
	bool isFinish(void);
	void setFinish(bool f);


private:
	unsigned int maxNeed;
	unsigned int ownNeed;
	unsigned int requestNeed;
	bool finish;
};

