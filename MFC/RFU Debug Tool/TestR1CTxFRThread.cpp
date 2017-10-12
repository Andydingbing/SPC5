#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CTxFRThread.h"
#include "CheckDlg.h"
#include "algorithm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int32_t (CTestR1CTxFRThread::*pBeginTestFR)();

IMPLEMENT_DYNCREATE(CTestR1CTxFRThread, CCSEWinThread)

CTestR1CTxFRThread::CTestR1CTxFRThread()
{
}

CTestR1CTxFRThread::CTestR1CTxFRThread(CWnd *pParent,TestRFFRInfo *pInfo)
{
	m_pParent = pParent;
	m_TestRFFRInfo.m_pSP1401 = pInfo->m_pSP1401;
	m_TestRFFRInfo.m_pSP2401 = pInfo->m_pSP2401;
	m_TestRFFRInfo.m_strFreqStar = pInfo->m_strFreqStar;
	m_TestRFFRInfo.m_strFreqStop = pInfo->m_strFreqStop;
	m_TestRFFRInfo.m_strFreqStep = pInfo->m_strFreqStep;
	m_iFuncIdx = 0;
}

CTestR1CTxFRThread::CTestR1CTxFRThread(CWnd *pParent,TestBBFRInfo *pInfo)
{
	m_pParent = pParent;
	m_TestBBFRInfo.m_pSP1401 = pInfo->m_pSP1401;
	m_TestBBFRInfo.m_pSP2401 = pInfo->m_pSP2401;
	m_TestBBFRInfo.m_uiRFFreq = pInfo->m_uiRFFreq;
	m_TestBBFRInfo.m_uiFreqStep = pInfo->m_uiFreqStep;
	m_iFuncIdx = 1;
}

CTestR1CTxFRThread::~CTestR1CTxFRThread()
{
}

BOOL CTestR1CTxFRThread::InitInstance()
{
	return TRUE;
}

int CTestR1CTxFRThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1CTxFRThread, CWinThread)
END_MESSAGE_MAP()


int CTestR1CTxFRThread::Run()
{
	pBeginTestFR BeginTestFR[] = {&CTestR1CTxFRThread::TestRFFR,
								  &CTestR1CTxFRThread::TestBBFR};
	(this->*BeginTestFR[m_iFuncIdx])();
	return 0;
}

int32_t CTestR1CTxFRThread::TestRFFR()
{
	try
	{
		Instrument.Init();
		Instrument.HasSA();
		Instrument.SA_Reset();
		Instrument.SA_SetRefLevel(0);
		Instrument.SA_SetSpan(1e6);
		Instrument.SA_SetRBW(0,true);
		Instrument.SA_SetVBW(0,true);

		CSP1401R1C *pSP1401 = m_TestRFFRInfo.m_pSP1401;
		CSP2401R1A *pSP2401 = m_TestRFFRInfo.m_pSP2401;
		CString strFreqStar = m_TestRFFRInfo.m_strFreqStar;
		CString strFreqStop = m_TestRFFRInfo.m_strFreqStop;
		CString strFreqStep = m_TestRFFRInfo.m_strFreqStep;
		double dPowerSA = 0.0;
		Result Data;

		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetTxDCOffset(0,0);
		pSP2401->SetTxPhaseRotateI(0.0);
		pSP2401->SetTxAmplitudeBalance(8192,8192);

		int64_t iFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE)];
		int64_t iFreqStar[16];
		int64_t iFreqStop[16];
		memset(iFreq,0,sizeof(iFreq));
		memset(iFreqStar,0,sizeof(iFreqStar));
		memset(iFreqStop,0,sizeof(iFreqStop));
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,iFreq,iFreqStar,iFreqStop);
		InitSinglePos("Testing RF Frequency Response",iPts);

		for (int32_t i = 0;i < iPts;i ++) {
			TEST_THREAD_TEST_CANCEL_S(Instrument.Close(););
			pSP1401->SetTxFreq(iFreq[i]);

			Instrument.SA_SetCF((double)iFreq[i] + 20000000.0);
			Instrument.SA_SweepOnce();
			Instrument.SA_SetPeakSearch(ISA::PeakHighest);
			Instrument.SA_GetMarkerPwr(dPowerSA);
			Data.m_iFreq = iFreq[i];
			Data.m_iSec = freqsection(iFreqStar,iFreqStop,iFreq[i]);
			Data.m_dRes = dPowerSA;
			::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_TX_FR_SHOW,0,(LPARAM)&Data);
			SetPos(i + 1,m_nThreadID);
		}
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	catch (CInstrExpt &expt)
	{
		CseMessageBox(expt.GetExpt());
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	return 0;
}

int32_t CTestR1CTxFRThread::TestBBFR()
{
	try
	{
		Instrument.Init();
		Instrument.HasSA();
		Instrument.SA_Reset();
		Instrument.SA_SetRefLevel(0);
		Instrument.SA_SetSpan((double)(m_TestBBFRInfo.m_uiFreqStep));
//		Instrument.SA_SetRBW(0,true);
//		Instrument.SA_SetVBW(0,true);

		CSP1401R1C *pSP1401 = m_TestBBFRInfo.m_pSP1401;
		CSP2401R1A *pSP2401 = m_TestBBFRInfo.m_pSP2401;
		int64_t iFreqStep = (int64_t)(m_TestBBFRInfo.m_uiFreqStep);
		double dPowerSA = 0.0;
		Result Data;
		Data.m_iSec = 0;

		InitSinglePos("Testing Tx Baseband Frequency Response",(int)SERIE_SIZE(MAX_BW / 2 * -1,MAX_BW / 2,iFreqStep));

		pSP1401->SetTxFreq(m_TestBBFRInfo.m_uiRFFreq);
		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetTxDCOffset(0,0);
		pSP2401->SetTxPhaseRotateI(0.0);
		pSP2401->SetTxAmplitudeBalance(8192,8192);
		
		for (int64_t iFreq = MAX_BW / 2 * -1;iFreq <= MAX_BW / 2;iFreq += iFreqStep) {
			TEST_THREAD_TEST_CANCEL_S(Instrument.Close(););
			if (0 == iFreq) {
				Data.m_iFreq = iFreq;
				::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_TX_FR_SHOW,0,(LPARAM)&Data);
				continue;
			}
			pSP2401->SetDDS1((double)iFreq);
			Instrument.SA_SetCF((double)(m_TestBBFRInfo.m_uiRFFreq + iFreq));
			Instrument.SA_SweepOnce();
			Instrument.SA_SetPeakSearch(ISA::PeakHighest);
			Instrument.SA_GetMarkerPwr(dPowerSA);
			Data.m_iFreq = iFreq;
			Data.m_dRes = dPowerSA;
			::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_TX_FR_SHOW,0,(LPARAM)&Data);
			SetPos(SERIE_INDEX(iFreq,MAX_BW / 2 * -1,iFreqStep) + 1,m_nThreadID);
			Sleep(100);
		}
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	catch (CInstrExpt &expt)
	{
		CseMessageBox(expt.GetExpt());
		TEST_THREAD_ABOART_S(Instrument.Close(););
	}
	return 0;
}