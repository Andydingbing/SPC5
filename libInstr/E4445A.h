#ifndef __E4445A_H__
#define __E4445A_H__

#include "N9030A.h"

class E4445A : public N9030A
{
public:
	E4445A(void);
	virtual ~E4445A(void);
	virtual char* GetDes();
public:
	virtual bool SetEnablePreamp(bool bEn);
};

#endif