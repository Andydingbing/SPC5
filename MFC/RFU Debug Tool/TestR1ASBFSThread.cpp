#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1ASBFSThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int32_t (CTestR1ASBFSThread::*pBeginTestSBFS)();

IMPLEMENT_DYNCREATE(CTestR1ASBFSThread, CCSEWinThread)

CTestR1ASBFSThread::CTestR1ASBFSThread()
{
}

CTestR1ASBFSThread::CTestR1ASBFSThread(CWnd *pParent,int32_t iFuncIdx)
{
	m_pParent = pParent;
	m_iFuncIdx = iFuncIdx;
}

CTestR1ASBFSThread::~CTestR1ASBFSThread()
{
}

BOOL CTestR1ASBFSThread::InitInstance()
{
	return TRUE;
}

int CTestR1ASBFSThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1ASBFSThread, CWinThread)
END_MESSAGE_MAP()


int CTestR1ASBFSThread::Run()
{
	pBeginTestSBFS BeginTestStability[] = {&CTestR1ASBFSThread::TestDynDown,
										   &CTestR1ASBFSThread::TestSN,
										   &CTestR1ASBFSThread::TestLO};
	(this->*BeginTestStability[m_iFuncIdx])();
	return 0;
}

int32_t CTestR1ASBFSThread::TestDynDown()
{
	CString strPathK7_0,strPathK7_1;
	CTestR1ASBFSDlg *pParent = (CTestR1ASBFSDlg *)m_pParent;
	pParent->m_BitFile_K7_0.GetWindowText(strPathK7_0);
	pParent->m_BitFile_K7_1.GetWindowText(strPathK7_1);
	int32_t iTimes = (int32_t)(((CTestR1ASBFSDlg *)m_pParent)->m_iRepeatTimes);
	CSP3301 *pSP3301 = pParent->m_pSP3301;
	viPCIDev *pS6 = pParent->m_S6;
	BOOL bK7_0 = pParent->m_bDynDownK7_0;
	BOOL bK7_1 = pParent->m_bDynDownK7_1;
	
	InitSinglePos("Testing FPGA Program Bit Stability",iTimes);

	for (int32_t i = 0;i < iTimes;i ++) {
		TEST_THREAD_TEST_CANCEL();
		if (bK7_0) {
			if (pSP3301->m_pSP2401[2]->SetFpgaBit((LPSTR)(LPCTSTR)strPathK7_0)) {
				CseMessageBox("K7_0 Fail");
				TEST_THREAD_ABOART();
			}
		}
		TEST_THREAD_TEST_CANCEL();
		if (bK7_1) {
			if (pSP3301->m_pSP2401[0]->SetFpgaBit((LPSTR)(LPCTSTR)strPathK7_1)) {
				CseMessageBox("K7_1 Fail");
				TEST_THREAD_ABOART();
			}
		}
		SetPos(i + 1,m_nThreadID);
		pParent->m_iCurrentCount = (int)(i + 1);
		::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
	TEST_THREAD_ABOART();
	return 0;
}

int32_t CTestR1ASBFSThread::TestSN()
{
	int32_t iTimes = (int32_t)(((CTestR1ASBFSDlg *)m_pParent)->m_iRepeatTimes);
	CTestR1ASBFSDlg *pParent = (CTestR1ASBFSDlg *)m_pParent;
	CSP1401R1A *pSP1401 = pParent->m_pSP1401;
	char szSN[64] = {0};
	memset(szSN,0,sizeof(szSN));

	InitSinglePos("Testing SN Read Stability",iTimes);

	for (int32_t i = 0;i < iTimes;i ++) {
		TEST_THREAD_TEST_CANCEL();
		if (pSP1401->GetSN(szSN)) {
			CseMessageBox("SN Read Fail");
			TEST_THREAD_ABOART();
		}
		if (!ISP1401::IsValidSN(szSN)) {
			CseMessageBox("SN : %s Invalid",szSN);
			TEST_THREAD_ABOART();
		}
		::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_SB_STABILITY_SN_SHOW,0,(LPARAM)&szSN);
		SetPos(i + 1,m_nThreadID);
		pParent->m_iCurrentCount = (int)(i + 1);
		::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
	TEST_THREAD_ABOART();
	return 0;
}

int32_t CTestR1ASBFSThread::TestLO()
{
	int32_t iTimes = (int32_t)(((CTestR1ASBFSDlg *)m_pParent)->m_iRepeatTimes);
	CTestR1ASBFSDlg *pParent = (CTestR1ASBFSDlg *)m_pParent;
	CSP1401R1A *pSP1401 = pParent->m_pSP1401;
	IsLOLock Lock = {false,false,false,false};
	
	uint64_t uiTxLO1FreqStar = 0,uiTxLO1FreqStop = 0,uiTxLO1FreqStep = 0;
	uint64_t uiTxLO2FreqStar = 0,uiTxLO2FreqStop = 0,uiTxLO2FreqStep = 0;
	uint64_t uiRxLO1FreqStar = 0,uiRxLO1FreqStop = 0,uiRxLO1FreqStep = 0;
	uint64_t uiRxLO2FreqStar = 0,uiRxLO2FreqStop = 0,uiRxLO2FreqStep = 0;
	str2freq(pParent->m_strTxLO1FreqStar,uiTxLO1FreqStar);
	str2freq(pParent->m_strTxLO1FreqStop,uiTxLO1FreqStop);
	str2freq(pParent->m_strTxLO1FreqStep,uiTxLO1FreqStep);
	str2freq(pParent->m_strTxLO2FreqStar,uiTxLO2FreqStar);
	str2freq(pParent->m_strTxLO2FreqStop,uiTxLO2FreqStop);
	str2freq(pParent->m_strTxLO2FreqStep,uiTxLO2FreqStep);
	str2freq(pParent->m_strRxLO1FreqStar,uiRxLO1FreqStar);
	str2freq(pParent->m_strRxLO1FreqStop,uiRxLO1FreqStop);
	str2freq(pParent->m_strRxLO1FreqStep,uiRxLO1FreqStep);
	str2freq(pParent->m_strRxLO2FreqStar,uiRxLO2FreqStar);
	str2freq(pParent->m_strRxLO2FreqStop,uiRxLO2FreqStop);
	str2freq(pParent->m_strRxLO2FreqStep,uiRxLO2FreqStep);
	int32_t iPts[4] = {(int32_t)SERIE_SIZE(uiTxLO1FreqStar,uiTxLO1FreqStop,uiTxLO1FreqStep),
					   (int32_t)SERIE_SIZE(uiTxLO2FreqStar,uiTxLO2FreqStop,uiTxLO2FreqStep),
					   (int32_t)SERIE_SIZE(uiRxLO1FreqStar,uiRxLO1FreqStop,uiRxLO1FreqStep),
					   (int32_t)SERIE_SIZE(uiRxLO2FreqStar,uiRxLO2FreqStop,uiRxLO2FreqStep)};
	BOOL bTest[4] = {pParent->m_bTestTxLO1,pParent->m_bTestTxLO2,pParent->m_bTestRxLO1,pParent->m_bTestRxLO2};
	uint64_t uiFreq = 0;
	int32_t iFreqIdx = 0;

	for (int32_t i = 0;i < iTimes;i ++) {
		TEST_THREAD_TEST_CANCEL();
		if (0 == i) {
			if (bTest[CSP1401R1A::Tx_LO_1]) {
				uiFreq = uiTxLO1FreqStar;
				if (pSP1401->SetLO(CSP1401R1A::Tx_LO_1,uiFreq)) {
					CseMessageBox("Set Tx LO1 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Tx_LO_1,Lock.m_bTxLO1)) {
					CseMessageBox("Detect Tx LO1 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bTxLO1) {
					CseMessageBox("Tx LO1 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
			}
			if (bTest[CSP1401R1A::Tx_LO_2]) {
				uiFreq = uiTxLO2FreqStar;
				if (pSP1401->SetLO(CSP1401R1A::Tx_LO_2,uiFreq)) {
					CseMessageBox("Set Tx LO2 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Tx_LO_2,Lock.m_bTxLO2)) {
					CseMessageBox("Detect Tx LO2 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bTxLO2) {
					CseMessageBox("Tx LO2 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
			}
			if (bTest[CSP1401R1A::Rx_LO_1]) {
				uiFreq = uiRxLO1FreqStar;
				if (pSP1401->SetLO(CSP1401R1A::Rx_LO_1,uiFreq)) {
					CseMessageBox("Set Rx LO1 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Rx_LO_1,Lock.m_bRxLO1)) {
					CseMessageBox("Detect Rx LO1 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bRxLO1) {
					CseMessageBox("Rx LO1 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
			}
			if (bTest[CSP1401R1A::Rx_LO_2]) {
				uiFreq = uiRxLO2FreqStar;
				if (pSP1401->SetLO(CSP1401R1A::Rx_LO_2,uiFreq)) {
					CseMessageBox("Set Rx LO2 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Rx_LO_2,Lock.m_bRxLO2)) {
					CseMessageBox("Detect Rx LO2 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bRxLO2) {
					CseMessageBox("Rx LO2 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
			}
			::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_SB_STABILITY_LO_SHOW,0,(LPARAM)&Lock);
		}
		if (bTest[CSP1401R1A::Tx_LO_1]) {
			InitSinglePos("Testing Tx LO1 Lock Stability",iPts[0]);
			iFreqIdx = 0;
			for (uiFreq = uiTxLO1FreqStar;uiFreq <= uiTxLO1FreqStop;uiFreq += uiTxLO1FreqStep) {
				TEST_THREAD_TEST_CANCEL();
				pParent->m_uiTxLO1Cur = (ULONGLONG)uiFreq;
				::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
				if (pSP1401->SetLO(CSP1401R1A::Tx_LO_1,uiFreq)) {
					CseMessageBox("Set Tx LO1 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Tx_LO_1,Lock.m_bTxLO1)) {
					CseMessageBox("Detect Tx LO1 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bTxLO1) {
					CseMessageBox("Tx LO1 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
				iFreqIdx ++;
				SetPos(iFreqIdx,m_nThreadID);
			}
		}
		if (bTest[CSP1401R1A::Tx_LO_2]) {
			InitSinglePos("Testing Tx LO2 Lock Stability",iPts[1]);
			iFreqIdx = 0;
			for (uiFreq = uiTxLO2FreqStar;uiFreq <= uiTxLO2FreqStop;uiFreq += uiTxLO2FreqStep) {
				TEST_THREAD_TEST_CANCEL();
				pParent->m_uiTxLO2Cur = (ULONGLONG)uiFreq;
				::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
				if (pSP1401->SetLO(CSP1401R1A::Tx_LO_2,uiFreq)) {
					CseMessageBox("Set Tx LO2 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Tx_LO_2,Lock.m_bTxLO2)) {
					CseMessageBox("Detect Tx LO2 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bTxLO2) {
					CseMessageBox("Tx LO2 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
				iFreqIdx ++;
				SetPos(iFreqIdx,m_nThreadID);
			}
		}
		if (bTest[CSP1401R1A::Rx_LO_1]) {
			InitSinglePos("Testing Rx LO1 Lock Stability",iPts[2]);
			iFreqIdx = 0;
			for (uiFreq = uiRxLO1FreqStar;uiFreq <= uiRxLO1FreqStop;uiFreq += uiRxLO1FreqStep) {
				TEST_THREAD_TEST_CANCEL();
				pParent->m_uiRxLO1Cur = (ULONGLONG)uiFreq;
				::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
				if (pSP1401->SetLO(CSP1401R1A::Rx_LO_1,uiFreq)) {
					CseMessageBox("Set Rx LO1 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Rx_LO_1,Lock.m_bRxLO1)) {
					CseMessageBox("Detect Rx LO1 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bRxLO1) {
					CseMessageBox("Rx LO1 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
				iFreqIdx ++;
				SetPos(iFreqIdx,m_nThreadID);
			}
		}
		if (bTest[CSP1401R1A::Rx_LO_2]) {
			InitSinglePos("Testing Rx LO2 Lock Stability",iPts[3]);
			iFreqIdx = 0;
			for (uiFreq = uiRxLO2FreqStar;uiFreq <= uiRxLO2FreqStop;uiFreq += uiRxLO2FreqStep) {
				TEST_THREAD_TEST_CANCEL();
				pParent->m_uiRxLO2Cur = (ULONGLONG)uiFreq;
				::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
				if (pSP1401->SetLO(CSP1401R1A::Rx_LO_2,uiFreq)) {
					CseMessageBox("Set Rx LO2 @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (pSP1401->DetLO(CSP1401R1A::Rx_LO_2,Lock.m_bRxLO2)) {
					CseMessageBox("Detect Rx LO2 Lock @ %lldHz Fail",uiFreq);
					TEST_THREAD_ABOART();
				}
				if (!Lock.m_bRxLO2) {
					CseMessageBox("Rx LO2 @ %lldHz May Not Lock",uiFreq);
					TEST_THREAD_ABOART();
				}
				iFreqIdx ++;
				SetPos(iFreqIdx,m_nThreadID);
			}
		}
		::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_TEST_SB_STABILITY_LO_SHOW,0,(LPARAM)&Lock);
		pParent->m_iCurrentCount = (int)(i + 1);
		::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
	TEST_THREAD_ABOART();
	return 0;
}