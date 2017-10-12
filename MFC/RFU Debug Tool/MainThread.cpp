#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "MainThread.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int (CMainThread::*pMainThread)();
pMainThread tMainThread[1] = {&CMainThread::InitDevice};

IMPLEMENT_DYNCREATE(CMainThread, CWinThread)

CMainThread::CMainThread()
{
}

CMainThread::CMainThread(CWnd *pParent,int32_t iFuncIdx)
{
	m_pParent = pParent;
	m_iFuncIdx = iFuncIdx;
}

CMainThread::~CMainThread()
{
}

BOOL CMainThread::InitInstance()
{
	return TRUE;
}

int CMainThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CMainThread, CWinThread)
END_MESSAGE_MAP()


int CMainThread::Run()
{
	return (this->*tMainThread[m_iFuncIdx])();
}

int CMainThread::InitDevice()
{
	while (!::IsWindowVisible(g_pLogDlg->GetSafeHwnd()));
	m_bRunning = TRUE;
	CMainDlg *pParent = (CMainDlg *)m_pParent;
	CSP3301 *pSP3301 = pParent->m_pTabCtrl->m_pSP3301;
	CSP3501 *pSP3501 = pParent->m_pTabCtrl->m_pSP3501;
	CSP3301::RFUInfo Ver;
	char *pDriVer = NULL;
	uint32_t uiK7_0_Ver = 0;
	uint32_t uiK7_1_Ver = 0;
	uint32_t uiS6_Ver = 0;
	Ver.m_uiK7_0_Ver = uiK7_0_Ver;
	Ver.m_uiK7_1_Ver = uiK7_1_Ver;
	Ver.m_uiS6_Ver = uiS6_Ver;
	pDriVer = const_cast<char *>(pSP3301->GetDriverVer());
	(CMainFrame*)AfxGetMainWnd()->SendMessage(WM_CSE_SWHW_VERSION,(WPARAM)pDriVer,(LPARAM)&Ver);

	InitSinglePos("Initialization",100);

	SetPos(20,m_nThreadID);
	pSP3501->OpenBoard();
	SetPos(60,m_nThreadID);

	pSP3301->SetLoadBit(false,false);
	pSP3301->LoadBit();
	if (pSP3301->Boot()) {
		Log->AddMsgList(-1,"boot fail");
		CAL_THREAD_ABOART();
	}
	SetPos(90,m_nThreadID);

	Ver.m_uiK7_0_Ver  = pSP3301->GetInfo().m_uiK7_0_Ver;
	Ver.m_uiK7_1_Ver  = pSP3301->GetInfo().m_uiK7_1_Ver;
	Ver.m_uiS6_Ver	  = pSP3301->GetInfo().m_uiS6_Ver;
	pDriVer = const_cast<char *>(pSP3301->GetDriverVer());

	(CMainFrame*)AfxGetMainWnd()->SendMessage(WM_CSE_SWHW_VERSION,(WPARAM)pDriVer,(LPARAM)(&Ver));

	SetPos(100,m_nThreadID);
	Log->AddMsgList(0,"booted");
	
	::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_SINGLE_THREAD_DONE,NULL,NULL);
	return 0;
}