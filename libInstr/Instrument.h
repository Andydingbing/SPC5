#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include <vector>
#include "N9010A.h"
#include "N9020A.h"
#include "N9030A.h"
#include "E4440A.h"
#include "E4443A.h"
#include "E4445A.h"
#include "FSUP.h"
#include "N5182A.h"
#include "N5182B.h"
#include "E4438C.h"
#include "E8267D.h"
#include "SMF100A.h"
#include "RSNRP.h"

using namespace std;

class CInstrExpt
{
public:
    CInstrExpt(const char* format,...);
	char* GetExpt();
private:
	char m_szBuf[256];
};

#define Instrument CInstrument::Instance()

class CInstrument
{
protected:
	CInstrument(void);
	CInstrument(CInstrument &){}
public:
	static CInstrument &Instance();
	virtual ~CInstrument(void);
	void SetEnableExpt(bool bEn);
public:
	virtual bool Init();
	virtual bool Close();
	virtual bool HasSA();
	virtual bool HasSG();
	virtual bool HasPM();
	virtual void SetSA(char* pDes);
	virtual void SetSG(char* pDes);
	virtual void SetPM(char* pDes);
public:
	virtual bool SA_Reset();
	virtual bool SA_SetCF(double dFreq);
	virtual bool SA_SetSpan(double dFreq);
	virtual bool SA_SetRBW(double dFreq, bool bAuto);
	virtual bool SA_SetVBW(double dFreq, bool bAuto);
	virtual bool SA_SetDectector(ISA::DetectorTypes Type);
	virtual bool SA_SetPeakSearch(ISA::PeakSearchTypes Type);
	virtual bool SA_SetRefLevel(double dRef);
	virtual bool SA_GetRefLevel(double &dRef);
	virtual bool SA_SetRefAuto();
	virtual bool SA_SetMarker(ISA::MarkerTypes Type);
	virtual bool SA_GetMarkerPwr(double &dPwr);
	virtual bool SA_GetMarkerFrequency(double &dFreq);
	virtual bool SA_SetEnablePreamp(bool bEn);
	virtual bool SA_MarkerMove(ISA::MarkerMoveTypes Type);
	virtual bool SA_MarkerMove(double dFreq);
	virtual bool SA_SweepOnce();
	virtual bool SA_SetCalibration(ISA::CalibrationTypes Type);
	virtual bool SA_SetAverTrace(bool bEn,int32_t iCnt);
	virtual bool SA_SetAverTraceAndGetData(int32_t iAvgCnt,int32_t iPtCnt,double *pData);
	virtual bool SA_SetMechAtten(bool bAuto,int32_t iAtten = 0);
public:
	virtual bool SG_Reset();
	virtual bool SG_SetCW(double dFreq);
	virtual bool SG_GetCW(double &dFreq);
	virtual bool SG_SetPL(double dPwr);
	virtual bool SG_GetPL(double &dPwr);
	virtual bool SG_SetOutputEnable(bool bEn);
	virtual bool SG_SetModEnable(bool bEn);
public:
	virtual bool PM_Reset();
	virtual bool PM_SetFrequency(double dFreq);
	virtual bool PM_GetPwr(double &dPwr);
	virtual bool PM_GetPwr(double dFreq,double &dPwr);
private:
	virtual bool InsInstr(char* pIDN,ViRsrc pDes);
private:
	bool m_bEnExpt;
	char m_szSA[16];
	char m_szSG[16];
	char m_szPM[16];
	ISA* m_pSA;
	ISG* m_pSG;
	IPM* m_pPM;
private:
	vector<ISA *> m_SUPP_SA;
	vector<ISG *> m_SUPP_SG;
	vector<IPM *> m_SUPP_PM;
};

#endif
