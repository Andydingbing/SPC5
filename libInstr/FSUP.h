#ifndef __FSUP_H__
#define __FSUP_H__

#include "ISA.h"

class FSUP : public ISA
{
public:
	FSUP();
	virtual ~FSUP();
	virtual int32_t	GetDefaultPriAddr();
	virtual int32_t	GetDefaultSecAddr();
	virtual char* GetDes();
public:
	virtual bool Reset();
	virtual bool SetCF(double dFreq);
	virtual bool SetSpan(double dFreq);
	virtual bool SetRBW(double dFreq, bool bAuto);
	virtual bool SetVBW(double dFreq, bool bAuto);
	virtual bool SetDectector(DetectorTypes Type);
	virtual bool SetPeakSearch(PeakSearchTypes Type);
	virtual bool SetMarker(MarkerTypes Type);
	virtual bool SetCalibration(CalibrationTypes Type);
	virtual bool SetAverTrace(bool bEn,int32_t iCnt);
	virtual bool SetRefLevel(double dRef);
	virtual bool GetRefLevel(double &dRef);
	virtual bool GetMarkerPwr(double &dPwr);
	virtual bool SetEnablePreamp(bool bEn);
	virtual bool GetMarkerFrequency(double &dFreq);
	virtual bool WaitForContinue();
	virtual bool SweepOnce();
	virtual bool MarkerMove(MarkerMoveTypes Type);
	virtual bool MarkerMove(double dFreq);
	virtual bool SetAverTraceAndGetData(int32_t iAvgCnt,int32_t iPtCnt,double *pData);
	virtual bool SetMechAtten(bool bAuto,int32_t iAtten);
};

#endif
