#ifndef __ISG_H__
#define __ISG_H__

#include "IGPIBDev.h"

#ifdef _MSC_VER
#pragma comment(lib,"libBusDriver.lib")
#endif

class ISG : public IGPIBDev
{
public:
	ISG(void){}
	virtual ~ISG(void){}
public:
	virtual bool Reset() = 0;
	virtual bool SetCW(double dFreq) = 0;
	virtual bool GetCW(double &dFreq) = 0;
	virtual bool SetPL(double dPwr) = 0;
	virtual bool GetPL(double &dPwr) = 0;
	virtual bool SetOutputEnable(bool bEn) = 0;
	virtual bool SetModEnable(bool bEn) = 0;
};

#endif
