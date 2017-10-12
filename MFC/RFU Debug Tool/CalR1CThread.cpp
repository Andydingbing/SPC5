#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CThread, CWinThread)

CCalR1CThread::CCalR1CThread()
{
}

CCalR1CThread::CCalR1CThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CThread::~CCalR1CThread()
{
}

BOOL CCalR1CThread::InitInstance()
{
	return TRUE;
}

int CCalR1CThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CThread, CWinThread)
END_MESSAGE_MAP()


int CCalR1CThread::Run()
{
	int32_t i = 0;
	vector<CCalBaseThread *>::iterator iter;
	CCalR1CDlg *pParent = (CCalR1CDlg *)m_pParent;
	int32_t iRfIdx = pParent->m_pSP1401->GetRfIdx();
	m_Thread.clear();

	ALL_DLG->m_pCalR1CTxLOLeakageDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CTxLOLeakageDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CTxSidebandDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CTxSidebandDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CTxFilterDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CTxFilterDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CTxPowerDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CTxPowerDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CTxAttDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CTxAttDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CTxFilOffDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CTxFilOffDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CRxFilterDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CRxFilterDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CRxRefDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CRxRefDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CRxAttDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CRxAttDlg[iRfIdx].ResetShowCtrl();
	ALL_DLG->m_pCalR1CRxFilOffDlg[iRfIdx].CtrlPrepare();
	ALL_DLG->m_pCalR1CRxFilOffDlg[iRfIdx].ResetShowCtrl();

	m_Thread.push_back(new CCalR1CTxLOLeakageThread(&ALL_DLG->m_pCalR1CTxLOLeakageDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CTxSidebandThread(&ALL_DLG->m_pCalR1CTxSidebandDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CTxFilterThread(&ALL_DLG->m_pCalR1CTxFilterDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CTxPowerThread(&ALL_DLG->m_pCalR1CTxPowerDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CTxAttThread(&ALL_DLG->m_pCalR1CTxAttDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CTxFilOffThread(&ALL_DLG->m_pCalR1CTxFilOffDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CRxFilterThread(&ALL_DLG->m_pCalR1CRxFilterDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CRxRefThread(&ALL_DLG->m_pCalR1CRxRefDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CRxAttThread(&ALL_DLG->m_pCalR1CRxAttDlg[iRfIdx]));
	m_Thread.push_back(new CCalR1CRxFilOffThread(&ALL_DLG->m_pCalR1CRxFilOffDlg[iRfIdx]));

	for (iter = m_Thread.begin();iter != m_Thread.end();iter ++) {
		(*iter)->CreateThread();
		(*iter)->SuspendThread();
	}

	for (iter = m_Thread.begin();iter != m_Thread.end();iter ++) {
		CCSEWinThread::m_bRunning = TRUE;
		(*iter)->ResumeThread();
		WaitForSingleObject((*iter)->m_hThread,INFINITE);
		::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_SINGLE_THREAD_DONE,NULL,(LPARAM)&i);
		i ++;
	}
	CAL_THREAD_ABOART();
	return 0;
}
