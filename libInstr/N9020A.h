#ifndef __N9020A_H__
#define __N9020A_H__

#include "N9030A.h"

class N9020A : public N9030A
{
public:
	N9020A(void);
	virtual ~N9020A(void);
	virtual char* GetDes();
};

#endif