#ifndef __N5182A_H__
#define __N5182A_H__

#include "N5182B.h"

class N5182A : public N5182B
{
public:
	N5182A(void);
	virtual ~N5182A(void);
	virtual char* GetDes();
};

#endif