#ifndef __SMF100A_H__
#define __SMF100A_H__

#include "N5182B.h"

class SMF100A : public N5182B
{
public:
	SMF100A(void);
	virtual ~SMF100A(void);
	virtual char* GetDes();
	virtual bool SetModEnable(bool bEn);
};

#endif