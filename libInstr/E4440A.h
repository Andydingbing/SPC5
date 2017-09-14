#ifndef __E4440A_H__
#define __E4440A_H__

#include "N9030A.h"

class E4440A : public N9030A
{
public:
	E4440A(void);
	virtual ~E4440A(void);
	virtual char* GetDes();
};

#endif