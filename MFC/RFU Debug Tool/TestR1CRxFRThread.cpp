#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CRxFRThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int32_t (CTestR1CRxFRThread::*pBeginTestFR)();

IMPLEMENT_DYNCREATE(CTestR1CRxFRThread, CCSEWinThread)

CTestR1CRxFRThread::CTestR1CRxFRThread()
{
}

CTestR1CRxFRThread::CTestR1CRxFRThread(CWnd *pParent,TestRFFRInfo *pInfo)
{
	m_pParent = pParent;
	m_TestRFFRInfo.m_pSP1401 = pInfo->m_pSP1401;
	m_TestRFFRInfo.m_pSP2401 = pInfo->m_pSP2401;
	m_TestRFFRInfo.m_strFreqStar = pInfo->m_strFreqStar;
	m_TestRFFRInfo.m_strFreqStop = pInfo->m_strFreqStop;
	m_TestRFFRInfo.m_strFreqStep = pInfo->m_strFreqStep;
	m_iFuncIdx = 0;
}

CTestR1CRxFRThread::CTestR1CRxFRThread(CWnd *pParent,TestIFFRInfo *pInfo)
{
	m_pParent = pParent;
	m_TestIFFRInfo.m_pSP1401 = pInfo->m_pSP1401;
	m_TestIFFRInfo.m_pSP2401 = pInfo->m_pSP2401;
	m_TestIFFRInfo.m_uiRFFreq = pInfo->m_uiRFFreq;
	m_TestIFFRInfo.m_uiFreqStep = pInfo->m_uiFreqStep;
	m_iFuncIdx = 1;
}

CTestR1CRxFRThread::~CTestR1CRxFRThread()
{
}

BOOL CTestR1CRxFRThread::InitInstance()
{
	return TRUE;
}

int CTestR1CRxFRThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1CRxFRThread, CWinThread)
END_MESSAGE_MAP()


int CTestR1CRxFRThread::Run()
{
	pBeginTestFR BeginTestFR[] = {&CTestR1CRxFRThread::TestRFFR,
								  &CTestR1CRxFRThread::TestIFFR};
	(this->*BeginTestFR[m_iFuncIdx])();
	return 0;
}

int32_t CTestR1CRxFRThread::TestRFFR()
{
	try
	{
		Instrument.Init();
		Instrument.HasSG();
		Instrument.SG_Reset();
		Instrument.SG_SetModEnable(false);
		Instrument.SG_SetOutputEnable(false);
		Instrument.SG_SetPL(-10.0);

		CSP1401R1C *pSP1401 = m_TestRFFRInfo.m_pSP1401;
		CSP2401R1A *pSP2401 = m_TestRFFRInfo.m_pSP2401;
		CString strFreqStar = m_TestRFFRInfo.m_strFreqStar;
		CString strFreqStop = m_TestRFFRInfo.m_strFreqStop;
		CString strFreqStep = m_TestRFFRInfo.m_strFreqStep;
		int64_t iAD = 0,iAD_Sum = 0;;
		Result Data;

		int64_t iFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C)];
		int64_t iFreqStar[16];
		int64_t iFreqStop[16];
		memset(iFreq,0,ARRAY_SIZE(iFreq));
		memset(iFreqStar,0,ARRAY_SIZE(iFreqStar));
		memset(iFreqStop,0,ARRAY_SIZE(iFreqStop));
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,iFreq,iFreqStar,iFreqStop);
		InitSinglePos("Testing Rx RF Frequency Response",iPts);

		pSP2401->SetADSw(CSP2401R1A::FromRf);
		pSP2401->SetRxPowerComp(0);
		pSP2401->SetDDC(-92.64e6);
		Instrument.SG_SetOutputEnable(true);

		for (int32_t i = 0; i < iPts ;i ++) {
			TEST_THREAD_TEST_CANCEL_S(Instrument.Close(););
			pSP1401->SetRxFreq(iFreq[i]);
			Instrument.SG_SetCW((double)iFreq[i]);
			Sleep(100);
			iAD_Sum = 0;
			for (int32_t iAvg = 0;iAvg < 5;iAvg ++) {
				pSP1401->GetADS5474(iAD);
				iAD_Sum += iAD;
			}
			iAD = iAD_Sum / 5;

			Data.m_iFreq = iFreq[i];
			Data.m_iSec = freqsection(iFreqStar,iFreqStop,iFreq[i]);
			Data.m_dRes = ad2dBc(_0dBFS,iAD);
			::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_RX_FR_SHOW,0,(LPARAM)(&Data));
			SetPos(i + 1,m_nThreadID);
		}
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	return 0;
}

int32_t CTestR1CRxFRThread::TestIFFR()
{
	try {
		Instrument.Init();
		Instrument.HasSG();
		Instrument.SG_Reset();
		Instrument.SG_SetModEnable(false);
		Instrument.SG_SetOutputEnable(false);
		Instrument.SG_SetPL(-10.0);

		CSP1401R1C *pSP1401 = m_TestIFFRInfo.m_pSP1401;
		CSP2401R1A *pSP2401 = m_TestIFFRInfo.m_pSP2401;
		int64_t iFreqStep = (int64_t)m_TestIFFRInfo.m_uiFreqStep;
		int64_t iAD = 0,iAD_Sum = 0;
		Result Data;
		Data.m_iSec = 0;

		InitSinglePos("Testing Rx IF Filter Frequency Response",(int)SERIE_SIZE(MAX_BW / 2 * -1,MAX_BW / 2,iFreqStep));

		pSP1401->SetIOMode(OutPut);
		pSP1401->SetPowerMeasSrc(CSP1401R1C::PowerMeasFreeRun,false);
		pSP1401->SetPowerMeasTimeout(6553600);
		pSP1401->SetPowerMeasSamples(102400);
		pSP1401->SetTxAtt(30.0,30.0,30.0,30.0);
		pSP1401->SetRxIFFilterSw(ISP1401::_160MHz);
		pSP1401->SetRxFreq(m_TestIFFRInfo.m_uiRFFreq);
		pSP2401->SetDDC(-92640000.0);
		pSP2401->SetRxPowerComp(0);
		pSP2401->SetADSw(CSP2401R1A::FromRf);
		Instrument.SG_SetOutputEnable(true);

		for (int64_t iFreq = MAX_BW / 2 * -1;iFreq <= MAX_BW / 2;iFreq += iFreqStep) {
			TEST_THREAD_TEST_CANCEL_S(Instrument.Close(););
			Instrument.SG_SetCW(m_TestIFFRInfo.m_uiRFFreq + (double)iFreq);
			Sleep(100);

			CSP1401R1C::PowerMeasState State = CSP1401R1C::IDLE;
			pSP1401->PowerMeasAbort();
			pSP1401->PowerMeasStart();

			while (State != CSP1401R1C::DONE) {
				pSP1401->GetPowerMeasState(State);
				if (State == CSP1401R1C::TT)
					Data.m_dRes = -100.0;
			}
			pSP1401->GetPowerMeasPower(Data.m_dRes);

			Data.m_iFreq = iFreq;
			::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_RX_FR_SHOW,0,(LPARAM)&Data);
			SetPos(SERIE_INDEX(iFreq,MAX_BW / 2 * -1,iFreqStep) + 1,m_nThreadID);
		}
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	return 0;
}