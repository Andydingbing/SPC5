#include "FSUP.h"
#include <stdlib.h>
#include <string.h>

FSUP::FSUP() : ISA()
{
}

FSUP::~FSUP()
{
}

int32_t FSUP::GetDefaultPriAddr()
{
	return -1;
}

int32_t FSUP::GetDefaultSecAddr()
{
	return -1;
}

char* FSUP::GetDes()
{
    return const_cast<char *>("FSUP");
}

bool FSUP::Reset()
{
	return Write("SYSTem:PRESet;*WAI");
}

bool FSUP::SetCF(double dFreq)
{
	return Write("FREQ:CENT %.0f;*WAI",dFreq);
}

bool FSUP::SetSpan(double dFreq)
{
	return Write("FREQ:SPAN %.0f;*WAI",dFreq);
}

bool FSUP::SetRBW(double dFreq, bool bAuto)
{
	if (bAuto)
		return Write("BAND:AUTO ON;*WAI");

	return Write("BAND %.0f Hz;*WAI",dFreq);
}

bool FSUP::SetVBW(double dFreq, bool bAuto)
{
	if (bAuto)
		return Write("BAND:VID:AUTO ON;*WAI");

	return Write("BAND:VID %.0f Hz;*WAI",dFreq);
}

bool FSUP::SetDectector(DetectorTypes Type)
{
	switch (Type) {
		case DetAverage : return Write("DET RMS;*WAI");
		case DetNormal :  return Write("DET APE;*WAI");
		case DetPeak :	  return Write("DET POS;*WAI");
		case DetAUTO :	  return Write("DET:AUTO ON;*WAI");
		case DetSample :  return Write("DET SAMP;*WAI");
		case DetNegPeak : return Write("DET NEG;*WAI");
		default:return false;
	}
	return false;
}

bool FSUP::SetPeakSearch(PeakSearchTypes Type)
{
	switch (Type) {
		case PeakHighest :   return Write("CALC:MARK1:MAX;*WAI");
		case PeakNextHigh :  return Write("CALC:MARK1:MAX:NEXT;*WAI");
		case PeakNextRight : return Write("CALC:MARK1:MAX:RIGH;*WAI");
		case PeakNextLeft :  return Write("CALC:MARK1:MAX:LEFT;*WAI");
		default:return false;
	}
	return false;
}

bool FSUP::SetRefLevel(double dRef)
{
	return Write("DISP:WIND:TRAC:Y:RLEV %f dbm;*WAI",dRef);
}

bool FSUP::GetRefLevel(double &dRef)
{
	char szBuf[256];
	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("DISP:WIND:TRAC:Y:RLEV?;*WAI"));
	BOOL_CHECK(Read(szBuf,256));
	dRef = atof(szBuf);
	return true;
}

bool FSUP::SetMarker(MarkerTypes Type)
{
	switch (Type) {
		case MarkerOff :    return Write("CALC:MARK1:STAT OFF;*WAI");
		case MarkerNormal : return Write("CALC:MARK1:MODE POS;*WAI");
		case MarkerDelta :  return Write("CALC:MARK1:MODE DELT;*WAI");
		default:return false;
	}
	return false;
}

bool FSUP::SetEnablePreamp(bool bEn)
{
	double dFreq = 0.0;
	BOOL_CHECK(GetMarkerFrequency(dFreq));

	if (bEn)
		return Write("INP:GAIN:STAT ON;*WAI");
	else
		return Write("INP:GAIN:STAT OFF;*WAI");

	return true;
}

bool FSUP::GetMarkerPwr(double &dPwr)
{
	char szBuf[256] = {0};
	double dFreq = 0.0;

	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("CALC:MARK1:Y?;*WAI"));
	BOOL_CHECK(Read(szBuf,256));
	BOOL_CHECK(GetMarkerFrequency(dFreq));

	dPwr = atof(szBuf);
	return true;
}

bool FSUP::GetMarkerFrequency(double &dFreq)
{
	char szBuf[256] = {0};

	memset(szBuf,0,sizeof(szBuf));
	BOOL_CHECK(Write("CALC:MARK1:X?;*WAI"));
	BOOL_CHECK(Read(szBuf, 200));
	dFreq = atof(szBuf);
	return true;
}

bool FSUP::WaitForContinue()
{
	return Write("*WAI");
}

bool FSUP::SweepOnce()
{
	BOOL_CHECK(Write("INIT1:CONT OFF;*WAI;"));
	BOOL_CHECK(Write("INIT1:IMM;*WAI;"));
	return true;
}

bool FSUP::MarkerMove(MarkerMoveTypes Type)
{
	switch (Type) {
		case MarkerToCF :     return Write("CALC:MARK1:FUNC:CENT;*WAI;");
		case MarkerToRefLvl : return Write("CALC:MARK1:FUNC:REF;*WAI;");
		default:return false;
	}
	return false;
}

bool FSUP::MarkerMove(double dFreq)
{
	return Write("CALC:MARK1:X %f HZ;*WAI",dFreq);
}

bool FSUP::SetCalibration(CalibrationTypes Type)
{
	return Write("CAL:ALL;*WAI");
}

bool FSUP::SetAverTrace(bool bEn,int32_t iCnt)
{
	if (bEn) {
		BOOL_CHECK(Write("AVER:COUN %d;*WAI", iCnt));
		BOOL_CHECK(Write("AVER:STAT ON;*WAI"));
	} else {
		BOOL_CHECK(Write("AVER:STAT OFF;*WAI"));
	}
	return true;
}

bool FSUP::SetAverTraceAndGetData(int32_t iAvgCnt,int32_t iPtCnt,double *pData)
{
	BOOL_CHECK(Write("SWE:POIN %d;*WAI",iPtCnt));
	BOOL_CHECK(SetAverTrace(true,iAvgCnt));
	BOOL_CHECK(SweepOnce());
	BOOL_CHECK(Write("FORM ASCii;*WAI"));
	BOOL_CHECK(Write("TRAC? TRACE1;*WAI"));

	int iBufSize = 18 * iPtCnt;
	char *pBuf = new char[iBufSize];
	BOOL_CHECK_S(Read(pBuf,iBufSize),delete []pBuf);

	int iDataIdx = 0,iLastIdx = 0;
	for (int i = 0; i < iBufSize; i++) {
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

bool FSUP::SetMechAtten(bool bAuto,int32_t iAtten)
{
	return false;
}
