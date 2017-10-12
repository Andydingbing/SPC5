#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1CGuardThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRFR1CGuardThread::Result::Result()
{
	m_iIdx = 0;
	for (int32_t i = 0;i < ARRAY_SIZE(m_dTemp);i ++)
		m_dTemp[i] = -100.0;
}

IMPLEMENT_DYNCREATE(CRFR1CGuardThread, CWinThread)

CRFR1CGuardThread::CRFR1CGuardThread()
{
}

CRFR1CGuardThread::CRFR1CGuardThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CRFR1CGuardThread::~CRFR1CGuardThread()
{
}

BOOL CRFR1CGuardThread::InitInstance()
{
	return TRUE;
}

int CRFR1CGuardThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CRFR1CGuardThread, CWinThread)
END_MESSAGE_MAP()


int CRFR1CGuardThread::Run()
{
	DECLARE_R1C_GETTEMP_FUNCTION_MAP()

	CSP1401R1C *pSP1401 = ((CRFR1CDlg *)m_pParent)->m_pSP1401;
	Result Data;

	while (1) {
		TEST_THREAD_TEST_CANCEL();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			if (::IsWindow(ALL_DLG->m_pRFR1CDlg[i])) {
				for (int j = 0;j < ARRAY_SIZE(Data.m_dTemp);j ++) {
					TEST_THREAD_TEST_CANCEL();
					(ALL_DLG->m_pRFR1CDlg[i].m_pSP1401->*tGetR1CTemp[j])(&Data.m_dTemp[j]);
				}
				Data.m_Time = COleDateTime::GetCurrentTime();
				::SendMessage(ALL_DLG->m_pRFR1CDlg[i].GetSafeHwnd(),WM_CSE_RF_R1C_TEMP_GUARD_SHOW,0,(LPARAM)(&Data));
			}
			Sleep(10);
		}
		Data.m_iIdx ++;	
		Sleep(2000);
	}
	return 0;
}