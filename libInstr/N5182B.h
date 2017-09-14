#ifndef __N5182B_H__
#define __N5182B_H__

#include "ISG.h"

class N5182B : public ISG
{
public:
	N5182B();
	virtual ~N5182B();
	virtual int32_t GetDefaultPriAddr();
	virtual int32_t GetDefaultSecAddr();
	virtual char* GetDes();
public:
	virtual bool Reset();
	virtual bool SetCW(double dFreq);
	virtual bool GetCW(double &dFreq);
	virtual bool SetPL(double dPwr);
	virtual bool GetPL(double &dPwr);
	virtual bool SetOutputEnable(bool bEn);
	virtual bool SetModEnable(bool bEn);
protected:
	double m_dPwr;
};

#endif