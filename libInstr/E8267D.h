#ifndef __E8267D_H__
#define __E8267D_H__

#include "N5182B.h"

class E8267D : public N5182B
{
public:
	E8267D(void);
	virtual ~E8267D(void);
	virtual char* GetDes();
};

#endif