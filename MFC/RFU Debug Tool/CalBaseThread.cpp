#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalBaseThread.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalBaseThread, CWinThread)

CCalBaseThread::CCalBaseThread()
{
	m_bAutoDelete = TRUE;
}

CCalBaseThread::~CCalBaseThread()
{
}

BOOL CCalBaseThread::InitInstance()
{
	return TRUE;
}

int CCalBaseThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalBaseThread, CWinThread)
END_MESSAGE_MAP()


void CCalBaseThread::GetADS5474(ISP1401 *pSP1401,int64_t &iAD,int32_t iAvgTime)
{
	iAD = 0;
	int64_t iADAvg = 0;
	for (int32_t avg = 0;avg < iAvgTime;avg ++) {
		pSP1401->GetADS5474(iADAvg);
		iAD += iADAvg;
	}
	iAD /= iAvgTime;
}