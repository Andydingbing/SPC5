#ifndef __IPM_H__
#define __IPM_H__

#include "IGPIBDev.h"

class IPM
{
public:
	IPM(void){}
	virtual ~IPM(void){}
	virtual char* GetDes() = 0;
	virtual bool Init(ViRsrc strDev) = 0;
public:
	virtual bool Reset() = 0;
	virtual bool SetFrequency(double dFreq) = 0;
	virtual bool GetPwr(double &dPwr) = 0;
	virtual bool GetPwr(double dFreq,double &dPwr) = 0;
	virtual bool Close() = 0;
protected:
	ViSession m_viSession;
};

#endif