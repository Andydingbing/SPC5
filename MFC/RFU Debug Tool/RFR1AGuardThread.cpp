#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1AGuardThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRFR1AGuardThread::Result::Result()
{
	m_iIdx = 0;
	for (int32_t i = 0;i < MAX_RF * 2;i ++)
		m_dTemp[i] = -100.0;
}

IMPLEMENT_DYNCREATE(CRFR1AGuardThread, CWinThread)

CRFR1AGuardThread::CRFR1AGuardThread()
{
}

CRFR1AGuardThread::CRFR1AGuardThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CRFR1AGuardThread::~CRFR1AGuardThread()
{
}

BOOL CRFR1AGuardThread::InitInstance()
{
	return TRUE;
}

int CRFR1AGuardThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CRFR1AGuardThread, CWinThread)
END_MESSAGE_MAP()


int CRFR1AGuardThread::Run()
{
	CSP1401R1A *pSP1401;
	Result Data;

	while (1) {
		TEST_THREAD_TEST_CANCEL();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			TEST_THREAD_TEST_CANCEL();
			pSP1401 = ((CRFR1AGuardDlg *)m_pParent)->m_pSP1401.at(i);
			pSP1401->GetTxTemp(Data.m_dTemp[i * 2]);
			pSP1401->GetRxTemp(Data.m_dTemp[i * 2 + 1]);
		}
		Data.m_Time = COleDateTime::GetCurrentTime();
		::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_RF_R1A_TEMP_GUARD_SHOW,0,(LPARAM)(&Data));
		Data.m_iIdx ++;
		Sleep(2000);
	}
	return 0;
}