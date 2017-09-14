#ifndef __RSNRP_H__
#define __RSNRP_H__

#include <math.h>
#include "IPM.h"
#include "rsnrpz.h"

class RSNRP : public IPM
{
public:
	RSNRP();
	virtual ~RSNRP();
	virtual char* GetDes();
	virtual bool Init(ViRsrc strDev);
public:
	virtual bool Reset();
	virtual bool SetFrequency(double dFreq);
	virtual bool GetPwr(double &dPwr);
	virtual bool GetPwr(double dFreq,double &dPwr);
	virtual bool Close();
};

#endif
