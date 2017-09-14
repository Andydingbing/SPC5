#include "N9030A.h"
#include <stdlib.h>
#include <string.h>

N9030A::N9030A() : ISA()
{
}

N9030A::~N9030A()
{
}

int32_t N9030A::GetDefaultPriAddr()
{
	return -1;
}

int32_t N9030A::GetDefaultSecAddr()
{
	return -1;
}

char* N9030A::GetDes()
{
    return const_cast<char *>("N9030A");
}

bool N9030A::Reset()
{
	return Write("*RST;*WAI");
}

bool N9030A::SetCF(double dFreq)
{
	return Write("FREQ:CENT %.0f;*WAI",dFreq);
}

bool N9030A::SetSpan(double dFreq)
{
	return Write("FREQ:SPAN %.0f;*WAI",dFreq);
}

bool N9030A::SetRBW(double dFreq, bool bAuto)
{
	if (bAuto)
		return Write("BAND:AUTO ON;*WAI");

	return Write("BAND %.0f Hz;*WAI",dFreq);
}

bool N9030A::SetVBW(double dFreq, bool bAuto)
{
	if (bAuto)
		return Write("BAND:VID:AUTO ON;*WAI");

	return Write("BAND:VID %.0f Hz;*WAI",dFreq);
}

bool N9030A::SetDectector(DetectorTypes Type)
{
	switch (Type) {
		case DetAverage : return Write("DET RMS;*WAI");
		case DetNormal :  return Write("DET NORM;*WAI");
		case DetPeak :	  return Write("DET POS;*WAI");
		case DetAUTO :	  return Write("DET:AUTO ON;*WAI");
		case DetSample :  return Write("DET SAMP;*WAI");
		case DetNegPeak : return Write("DET NEG;*WAI");
		default:return false;
	}
	return false;
}

bool N9030A::SetPeakSearch(PeakSearchTypes Type)
{
	switch (Type) {
		case PeakHighest:   return Write("CALC:MARK1:MAX;*WAI");
		case PeakNextHigh:  return Write("CALC:MARK1:MAX:NEXT;*WAI");
		case PeakNextRight: return Write("CALC:MARK1:MAX:RIGH;*WAI");
		case PeakNextLeft:  return Write("CALC:MARK1:MAX:LEFT;*WAI");
		default:return false;
	}
	return false;
}

bool N9030A::SetRefLevel(double dRef)
{
	return Write("DISP:WIND:TRAC:Y:RLEV %f dbm;*WAI",dRef);
}

bool N9030A::GetRefLevel(double &dRef)
{
	char szBuf[256] = {0};
	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
	BOOL_CHECK(Read(szBuf,256));
	dRef = atof(szBuf);
	return true;
}

bool N9030A::SetMarker(MarkerTypes Type)
{
	switch (Type) {
		case MarkerOff:    return Write("CALC:MARK1:STAT OFF;*WAI");
		case MarkerNormal: return Write("CALC:MARK1:MODE POS;*WAI");
		case MarkerDelta:  return Write("CALC:MARK1:MODE DELT;*WAI");
		default:return false;
	}
	return false;
}

bool N9030A::SetEnablePreamp(bool bEn)
{
	if (bEn) {
		BOOL_CHECK(Write(":POW:GAIN:STAT ON;:POW:GAIN:BAND FULL;*WAI"));
	} else {
		BOOL_CHECK(Write("POW:GAIN:STAT OFF;*WAI"));
	}
	return true;
}

bool N9030A::GetMarkerPwr(double &dPwr)
{
	char szBuf[256] = {0};
	double dFreq = 0.0;

	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("CALC:MARK1:Y?;*WAI"));
	BOOL_CHECK(Read(szBuf,256));
	BOOL_CHECK(GetMarkerFrequency(dFreq));

	dPwr= atof(szBuf);
	return true;
}

bool N9030A::GetMarkerFrequency(double &dFreq)
{
	char szBuf[256] = {0};
	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("CALC:MARK1:X?;*WAI"));
	BOOL_CHECK(Read(szBuf,256));
	dFreq = atof(szBuf);
	return true;
}

bool N9030A::WaitForContinue()
{
	return Write("*WAI");
}

bool N9030A::SweepOnce()
{
	char szBuf[256] = {0};
	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("INIT:IMM;*WAI"));
	BOOL_CHECK(Write("*OPC?"));
	BOOL_CHECK(Read(szBuf,256,60000));
	return true;
}

bool N9030A::MarkerMove(MarkerMoveTypes Type)
{
	switch (Type) {
		case MarkerToCF :     return Write("CALC:MARK1:CENT;*WAI");
		case MarkerToRefLvl : return Write("CALC:MARK1:RLEV;*WAI");
		default:return false;
	}
	return false;
}

bool N9030A::MarkerMove(double dFreq)
{
	return Write("CALC:MARK1:X %f HZ;*WAI",dFreq);
}

bool N9030A::SetCalibration(CalibrationTypes Type)
{
	switch (Type) {
		case AUTO_OFF : return Write("CAL:AUTO OFF;*WAI");
		case AUTO_ON  : return Write("CAL:AUTO ON;*WAI");
		case CAL_NOW  : return Write("CAL:ALL;*WAI");
		default: return false;
	}
	return false;
}

bool N9030A::SetAverTrace(bool bEn,int32_t iCnt)
{
	if (bEn) {
		BOOL_CHECK(Write("AVER:COUN %d;*WAI",iCnt));
		BOOL_CHECK(Write("AVER ON;*WAI"));
	} else {
		BOOL_CHECK(Write("AVER OFF;*WAI"));
	}
	return true;
}

bool N9030A::SetAverTraceAndGetData(int32_t iAvgCnt,int32_t iPtCnt,double *pData)
{
	BOOL_CHECK(Write("SWE:POIN %d;*WAI",iPtCnt));
	BOOL_CHECK(SetAverTrace(true,iAvgCnt));
	BOOL_CHECK(SweepOnce());
	BOOL_CHECK(Write("FORM ASCii;*WAI"));
	BOOL_CHECK(Write("TRAC? TRACE1;*WAI"));

	int iBufSize = 18 * iPtCnt;
	char *pBuf = new char[iBufSize];
	BOOL_CHECK_S(Read(pBuf,iBufSize),delete []pBuf);

	int iDataIdx = 0, iLastIdx = 0;
	for (int i = 0;i < iBufSize;i ++) {
		if (pBuf[i] == ',' || i == iBufSize - 1) {
			pBuf[i] = 0;
			pData[iDataIdx++] = atof(&pBuf[iLastIdx]);
			iLastIdx = i + 1;
			i++;
		}
	}
	delete []pBuf;
	return true;
}

bool N9030A::SetMechAtten(bool bAuto,int32_t iAtten)
{
	if (bAuto) {
		BOOL_CHECK(Write("POW:ATT:AUTO ON"));
	} else {
		BOOL_CHECK(Write("POW:ATT:AUTO OFF"));
		BOOL_CHECK(Write("POW:ATT %d",iAtten));
	}
	return true;
}
