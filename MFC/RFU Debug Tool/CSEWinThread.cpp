#include "stdafx.h"
#include "CSEWinThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCSEWinThread, CWinThread)

BOOL CCSEWinThread::m_bRunning = FALSE;
CString CCSEWinThread::m_strProc = "";

CCSEWinThread::CCSEWinThread()
{
}

CCSEWinThread::~CCSEWinThread()
{
}

BOOL CCSEWinThread::InitInstance()
{
	m_bAutoDelete = TRUE;
	return TRUE;
}

int CCSEWinThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCSEWinThread, CWinThread)
END_MESSAGE_MAP()
