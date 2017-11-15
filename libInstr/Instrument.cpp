#include "viPCIDev.h"
#include "Instrument.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define INSTR_CHECK(func)					\
	if (!(func)) {							\
		if (m_bEnExpt) {					\
			throw CInstrExpt(__FUNCTION__);	\
			return false;					\
		}									\
		return false;						\
	}

#ifdef _MSC_VER
#pragma comment(lib,"visa64.lib")
#endif

CInstrExpt::CInstrExpt(const char *format,...)
{
	memset(m_szBuf,0,sizeof(m_szBuf));
	va_list ap;
	va_start(ap,format);
    vsprintf(m_szBuf,format,ap);
	va_end(ap);
}

char* CInstrExpt::GetExpt()
{
	return m_szBuf;
}

CInstrument::CInstrument(void)
{
	m_SUPP_SA.clear();
	m_SUPP_SA.push_back(new N9010A);
	m_SUPP_SA.push_back(new N9020A);
	m_SUPP_SA.push_back(new N9030A);
	m_SUPP_SA.push_back(new E4440A);
	m_SUPP_SA.push_back(new E4443A);
	m_SUPP_SA.push_back(new E4445A);
	m_SUPP_SA.push_back(new FSUP);

	m_SUPP_SG.clear();
	m_SUPP_SG.push_back(new N5182A);
	m_SUPP_SG.push_back(new N5182B);
	m_SUPP_SG.push_back(new E4438C);
	m_SUPP_SG.push_back(new E8267D);
	m_SUPP_SG.push_back(new SMF100A);

	m_SUPP_PM.clear();
	m_SUPP_PM.push_back(new RSNRP);

	m_bEnExpt = true;
	memset(m_szSA,0,sizeof(m_szSA));
	memset(m_szSG,0,sizeof(m_szSG));
	memset(m_szPM,0,sizeof(m_szPM));
	m_pSA = NULL;
	m_pSG = NULL;
	m_pPM = NULL;
	viPCIDev::OpenDefaultRM();
}

CInstrument& CInstrument::Instance()
{
	static CInstrument g_Instr;
	return g_Instr;
}

CInstrument::~CInstrument(void)
{
	for (int32_t i = 0;i < (int32_t)(m_SUPP_SA.size());i ++) {
		if (m_SUPP_SA.at(i)) {
			delete m_SUPP_SA.at(i);
			m_SUPP_SA.at(i) = NULL;
		}
	}
	m_SUPP_SA.clear();

	for (int32_t i = 0;i < (int32_t)(m_SUPP_SG.size());i ++) {
		if (m_SUPP_SG.at(i)) {
			delete m_SUPP_SG.at(i);
			m_SUPP_SG.at(i) = NULL;
		}
	}
	m_SUPP_SG.clear();

	for (int32_t i = 0;i < (int32_t)(m_SUPP_PM.size());i ++) {
		if (m_SUPP_PM.at(i)) {
			delete m_SUPP_PM.at(i);
			m_SUPP_PM.at(i) = NULL;
		}
	}
	m_SUPP_PM.clear();
}

void CInstrument::SetEnableExpt(bool bEn)
{
	m_bEnExpt = bEn;
}

bool CInstrument::Init()
{
	ViFindList FindList = 0;
	ViUInt32 uiRetCnt = 0;
	ViUInt32 uiRet = 0;
	ViChar szDes[256] = {0};
	ViChar szIDN[256] = {0};
	ViSession viSession = 0;
    ViChar szExpr[32] = "GPIB?*INSTR";

	m_pSA = NULL;
	m_pSG = NULL;
	m_pPM = NULL;

    if (viFindRsrc(viPCIDev::GetDefaultRM(),szExpr,&FindList,&uiRetCnt,szDes) < VI_SUCCESS) {
        throw CInstrExpt("viFindRsrc GPIB?*INSTR");
		return false;
	}
	while (uiRetCnt --) {
		if (viOpen(viPCIDev::GetDefaultRM(),szDes,VI_NULL,VI_NULL,&viSession) < VI_SUCCESS) {
			viFindNext(FindList,szDes);
			continue;
		}
		if (viWrite(viSession,(ViBuf)"*IDN?",5,&uiRet) < VI_SUCCESS) {
			viClose(viSession);
			viFindNext(FindList,szDes);
			continue;
		}
		if (viRead(viSession,(ViPBuf)szIDN,256,&uiRet) < VI_SUCCESS) {
			viClose(viSession);
			viFindNext(FindList,szDes);
			continue;
		}
		viClose(viSession);
		InsInstr(szIDN,szDes);
		viFindNext(FindList,szDes);
	}
	viClose(FindList);

	ViStatus Status = VI_SUCCESS;
	ViInt32 iNumSensors  = 0;
	char szSensorName[256] = {0};
	char szSensorType[256] = {0};
	char szSensorSN[256] = {0};

	for (int32_t iRetry = 0;iRetry < 2;iRetry ++) {
		if ((Status = rsnrpz_GetSensorCount(0, &iNumSensors)) != VI_SUCCESS) {
			ViChar szMsg[256] = {0};
			rsnrpz_error_message(VI_NULL,Status,szMsg);
			throw CInstrExpt("%s",szMsg);
			return false;
		}
		if (iNumSensors > 0)
			break;
	}
	if (iNumSensors < 1)
		return true;

	for (ViInt32 i = 1;i <= iNumSensors;i ++) {
		long lStat = 0;
		ViBoolean bMeasComplete = VI_FALSE;
		if ((Status = rsnrpz_GetSensorInfo(0,i,szSensorName,szSensorType,szSensorSN)) != VI_SUCCESS) {
			ViChar szMsg[256] = {0};
			rsnrpz_error_message(VI_NULL,Status,szMsg);
			throw CInstrExpt("%s",szMsg);
			return false;
		}
        InsInstr(const_cast<char *>("ROHDE&SCHWARZ,NRP"),szSensorName);
	}
	return true;
}

bool CInstrument::InsInstr(char* pIDN,ViRsrc pDes)
{
	bool bSpecifySA = (strcmp(m_szSA,"") ? true : false);
	bool bSpecifySG = (strcmp(m_szSG,"") ? true : false);
	bool bSpecifyPM = (strcmp(m_szPM,"") ? true : false);

	for (int32_t i = 0;i < (int32_t)(m_SUPP_SA.size());i ++) {
		if (strstr(pIDN,bSpecifySA ? m_szSA : m_SUPP_SA.at(i)->GetDes())) {
			if (!m_SUPP_SA.at(i)->Init(pDes))
				continue;
			else {
				m_pSA = m_SUPP_SA.at(i);
				return true;
			}
		}
	}

	for (int32_t i = 0;i < (int32_t)(m_SUPP_SG.size());i ++) {
		if (strstr(pIDN,bSpecifySG ? m_szSG : m_SUPP_SG.at(i)->GetDes())) {
			if (!m_SUPP_SG.at(i)->Init(pDes))
				continue;
			else {
				m_pSG = m_SUPP_SG.at(i);
				return true;
			}
		}
	}

	for (int32_t i = 0;i < (int32_t)(m_SUPP_PM.size());i ++) {
		if (strstr(pIDN,bSpecifyPM ? m_szPM : m_SUPP_PM.at(i)->GetDes())) {
			if (!m_SUPP_PM.at(i)->Init(pDes))
				continue;
			else {
				m_pPM = m_SUPP_PM.at(i);
				return true;
			}
		}
	}
	return true;
}

bool CInstrument::Close()
{
	if (m_pPM) {
		INSTR_CHECK(m_pPM->Close());
	}

	return true;
}

bool CInstrument::HasSA()
{
	if (!m_pSA) {
		if (m_bEnExpt) {
			throw CInstrExpt("%s Disconnect",strcmp(m_szSA,"") ? m_szSA : "SA");
			return false;
		}
	}
	return true;
}

bool CInstrument::HasSG()
{
	if (!m_pSG) {
		if (m_bEnExpt) {
			throw CInstrExpt("%s Disconnect",strcmp(m_szSG,"") ? m_szSG : "SG");
			return false;
		}
	}
	return true;
}

bool CInstrument::HasPM()
{
	if (!m_pPM) {
		if (m_bEnExpt) {
			throw CInstrExpt("%s Disconnect",strcmp(m_szPM,"") ? m_szPM : "PM");
			return false;
		}
	}
	return true;
}

void CInstrument::SetSA(char* pDes)
{
    strcpy(m_szSA,pDes);
}

void CInstrument::SetSG(char* pDes)
{
    strcpy(m_szSG,pDes);
}

void CInstrument::SetPM(char* pDes)
{
    strcpy(m_szPM,pDes);
}

bool CInstrument::SA_Reset()
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->Reset());
	return true;
}

bool CInstrument::SA_SetCF(double dFreq)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetCF(dFreq));
	return true;
}

bool CInstrument::SA_SetSpan(double dFreq)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetSpan(dFreq));
	return true;
}

bool CInstrument::SA_SetRBW(double dFreq, bool bAuto)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetRBW(dFreq,bAuto));
	return true;
}

bool CInstrument::SA_SetVBW(double dFreq, bool bAuto)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetVBW(dFreq,bAuto));
	return true;
}

bool CInstrument::SA_SetDectector(ISA::DetectorTypes Type)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetDectector(Type));
	return true;
}

bool CInstrument::SA_SetPeakSearch(ISA::PeakSearchTypes Type)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetPeakSearch(Type));
	return true;
}

bool CInstrument::SA_SetRefLevel(double dRef)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetRefLevel(dRef));
	return true;
}

bool CInstrument::SA_GetRefLevel(double &dRef)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->GetRefLevel(dRef));
	return true;
}

bool CInstrument::SA_SetRefAuto()
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetRefAuto());
	return true;
}

bool CInstrument::SA_SetMarker(ISA::MarkerTypes Type)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetMarker(Type));
	return true;
}

bool CInstrument::SA_GetMarkerPwr(double &dPwr)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->GetMarkerPwr(dPwr));
	return true;
}

bool CInstrument::SA_GetMarkerFrequency(double &dFreq)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->GetMarkerFrequency(dFreq));
	return true;
}

bool CInstrument::SA_SetEnablePreamp(bool bEn)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetEnablePreamp(bEn));
	return true;
}

bool CInstrument::SA_MarkerMove(ISA::MarkerMoveTypes Type)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->MarkerMove(Type));
	return true;
}

bool CInstrument::SA_MarkerMove(double dFreq)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->MarkerMove(dFreq));
	return true;
}

bool CInstrument::SA_SweepOnce()
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SweepOnce());
	return true;
}

bool CInstrument::SA_SetCalibration(ISA::CalibrationTypes Type)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetCalibration(Type));
	return true;
}

bool CInstrument::SA_SetAverTrace(bool bEn,int32_t iCnt)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetAverTrace(bEn,iCnt));
	return true;
}

bool CInstrument::SA_SetAverTraceAndGetData(int32_t iAvgCnt,int32_t iPtCnt,double *pData)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetAverTraceAndGetData(iAvgCnt,iPtCnt,pData));
	return true;
}

bool CInstrument::SA_SetMechAtten(bool bAuto,int32_t iAtten)
{
	BOOL_CHECK(HasSA());
	INSTR_CHECK(m_pSA->SetMechAtten(bAuto,iAtten));
	return true;
}

bool CInstrument::SG_Reset()
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->Reset());
	return true;
}

bool CInstrument::SG_SetCW(double dFreq)
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->SetCW(dFreq));
	return true;
}

bool CInstrument::SG_GetCW(double &dFreq)
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->GetCW(dFreq));
	return true;
}

bool CInstrument::SG_SetPL(double dPwr)
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->SetPL(dPwr));
	return true;
}

bool CInstrument::SG_GetPL(double &dPwr)
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->GetPL(dPwr));
	return true;
}

bool CInstrument::SG_SetOutputEnable(bool bEn)
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->SetOutputEnable(bEn));
	return true;
}

bool CInstrument::SG_SetModEnable(bool bEn)
{
	BOOL_CHECK(HasSG());
	INSTR_CHECK(m_pSG->SetModEnable(bEn));
	return true;
}

bool CInstrument::PM_Reset()
{
	BOOL_CHECK(HasPM());
	INSTR_CHECK(m_pPM->Reset());
	return true;
}

bool CInstrument::PM_SetFrequency(double dFreq)
{
	BOOL_CHECK(HasPM());
	INSTR_CHECK(m_pPM->SetFrequency(dFreq));
	return true;
}

bool CInstrument::PM_GetPwr(double &dPwr)
{
	BOOL_CHECK(HasPM());
	INSTR_CHECK(m_pPM->GetPwr(dPwr));
	return true;
}

bool CInstrument::PM_GetPwr(double dFreq,double &dPwr)
{
	BOOL_CHECK(HasPM());
	INSTR_CHECK(m_pPM->GetPwr(dFreq,dPwr));
	return true;
}
