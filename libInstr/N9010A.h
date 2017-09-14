#ifndef __N9010A_H__
#define __N9010A_H__

#include "N9030A.h"

class N9010A : public N9030A
{
public:
	N9010A(void);
	virtual ~N9010A(void);
	virtual char* GetDes();
};

#endif