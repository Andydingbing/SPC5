#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1AX9119Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTestR1AX9119Thread, CWinThread)

CTestR1AX9119Thread::CTestR1AX9119Thread()
{
}

CTestR1AX9119Thread::CTestR1AX9119Thread(CWnd *pParent)
{
	m_pParent = pParent;
}

CTestR1AX9119Thread::~CTestR1AX9119Thread()
{
}

BOOL CTestR1AX9119Thread::InitInstance()
{
	return TRUE;
}

int CTestR1AX9119Thread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1AX9119Thread, CWinThread)
END_MESSAGE_MAP()


int CTestR1AX9119Thread::Run()
{
	try 
	{
		TEST_THREAD_TEST_CANCEL();
		TEST_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Testing X9119"));
		TEST_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Spectrum"));

		CTestR1AX9119Dlg *pParent = (CTestR1AX9119Dlg *)m_pParent;
		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		uint64_t uiFreq = 0;
		str2freq(pParent->m_strFreq,uiFreq);
		Instrument.Init();
		Instrument.HasSA();
		Instrument.SA_Reset();
		Instrument.SA_SetCF((double)uiFreq);
		Instrument.SA_SetSpan(5000.0);
		Instrument.SA_SetRBW(200.0,false);
		Instrument.SA_SetVBW(300.0,false);
		Instrument.SA_SetRefLevel(-10.0);

		InitSinglePos("Testing X9119",1024);

		double dMkrFreq = 0.0;
		Result Data;

		pSP1401->SetTxFreq(uiFreq);

		for (int16_t iValue = 0;iValue < 1023;iValue ++) {
			TEST_THREAD_TEST_CANCEL();
			SP3501.Vol9119(iValue);
			Instrument.SA_SweepOnce();
			Instrument.SA_SetPeakSearch(ISA::PeakHighest);
			Instrument.SA_GetMarkerFrequency(dMkrFreq);
			Data.m_iValue = iValue;
			Data.m_dFreq = (double)uiFreq - dMkrFreq;
			::SendMessage(pParent->GetSafeHwnd(),WM_CSE_TEST_X9119_SHOW,0,(LPARAM)(&Data));
			SetPos(SERIE_INDEX(iValue,0,1),m_nThreadID);
		}
		Instrument.Close();
		TEST_THREAD_ABOART();
	}
	catch (CInstrExpt &expt)
	{
		AfxMessageBox(expt.GetExpt());
		TEST_THREAD_ABOART();
	}
	return 0;
}
