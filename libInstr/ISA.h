#ifndef __ISA_H__
#define __ISA_H__

#include "IGPIBDev.h"

class ISA : public IGPIBDev
{
public:
	ISA(void);
	virtual	~ISA();
public:
	enum DetectorTypes {
		DetAUTO,
		DetNormal,
		DetAverage,
		DetPeak,
		DetSample,
		DetNegPeak
	};
	enum PeakSearchTypes {
		PeakHighest,
		PeakNextHigh,
		PeakNextRight,
		PeakNextLeft
	};
	enum MarkerTypes {
		MarkerNormal,
		MarkerDelta,
		MarkerOff
	};
	enum MarkerMoveTypes {
		MarkerToCF,
		MarkerToRefLvl
	};
	enum CalibrationTypes {
		AUTO_ON,
		AUTO_OFF,
		CAL_NOW
	};
public:
	virtual bool Reset() = 0;
	virtual bool SetCF(double dFreq) = 0;
	virtual bool SetSpan(double dFreq) = 0;
	virtual bool SetRBW(double dFreq, bool bAuto) = 0;
	virtual bool SetVBW(double dFreq, bool bAuto) = 0;
	virtual bool SetDectector(DetectorTypes Type) = 0;
	virtual bool SetPeakSearch(PeakSearchTypes Type) = 0;
	virtual bool SetMarker(MarkerTypes Type) = 0;
	virtual bool SetCalibration(CalibrationTypes Type) = 0;
	virtual bool SetAverTrace(bool bEn,int32_t iCnt) = 0;
	virtual bool SetRefLevel(double dRef) = 0;
	virtual bool GetRefLevel(double &dRef) = 0;
	virtual bool GetMarkerPwr(double &dPwr) = 0;
	virtual bool SetEnablePreamp(bool bEn) = 0;
	virtual bool GetMarkerFrequency(double &dFreq) = 0;
	virtual bool WaitForContinue() = 0;
	virtual bool SweepOnce() = 0;
	virtual bool MarkerMove(MarkerMoveTypes Type) = 0;
	virtual bool MarkerMove(double dFreq) = 0;
	virtual bool SetAverTraceAndGetData(int32_t iAvgCnt,int32_t iPtCnt,double *pData) = 0;
	virtual bool SetMechAtten(bool bAuto,int32_t iAtten) = 0;
public:
	virtual bool SetRefAuto();
};

#endif
