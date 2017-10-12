#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "DMAThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int32_t (CDMAThread::*pBeginTest)();

IMPLEMENT_DYNCREATE(CDMAThread, CWinThread)

CDMAThread::CDMAThread()
{
}

CDMAThread::CDMAThread(CWnd *pParent)
{
	m_pParent = pParent;
	m_iFuncIdx = 0;
}

CDMAThread::~CDMAThread()
{
}

BOOL CDMAThread::InitInstance()
{
	Log->SetEnable(L_REG,false);
	return TRUE;
}

int CDMAThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CDMAThread, CWinThread)
END_MESSAGE_MAP()


int CDMAThread::Run()
{
	pBeginTest BeginTest[] = {&CDMAThread::TestWR};
	(this->*BeginTest[m_iFuncIdx])();
	return 0;
}

int32_t CDMAThread::TestWR()
{
#define POST_CLR \
	delete []pBuf

	CDMADlg *pParent = (CDMADlg *)m_pParent;
	ISP1401 *pSP1401 = pParent->m_pSP1401;
	uint32_t uiSamples = pParent->m_uiTestSamples;
	uint32_t uiTimes = pParent->m_uiTestTimes;
	bool bTestR = pParent->m_bTestR;
	bool bTestW = pParent->m_bTestW;

	int32_t iErrCnt = 0;
	uint32_t *pBuf = new uint32_t[uiSamples];
	for (uint32_t i = 0;i < uiSamples; i ++)
		pBuf[i] = i;

	pSP1401->SetIQCapSamples(uiSamples);
	DDR->FpgaReadAll(pSP1401->GetK7(),pBuf,uiSamples);

	InitSinglePos("DMA W/R Test",(int32_t)uiTimes);
	for (uint32_t i = 0;i < uiTimes;i ++) {
		TEST_THREAD_TEST_CANCEL_S(POST_CLR);
		pParent->m_uiTestCurTimes = i;
		::PostMessage(pParent->GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,NULL,NULL);
		if (bTestR)
			DDR->FpgaReadAll(pSP1401->GetK7(),pBuf,uiSamples);
		if (bTestW) {
			pSP1401->IQCap();
			memset(pBuf,0,sizeof(uint32_t) * uiSamples);
			DDR->WDumpToBuf((int32_t *)pBuf,uiSamples);
			for (uint32_t j = 0;j < uiSamples;j ++) {
				TEST_THREAD_TEST_CANCEL_S(POST_CLR);
				if (pBuf[j] != j) {
					iErrCnt ++;
					Log->AddMsgList(-1,"error%4d:send:%#10x receive:%#10x",iErrCnt,j,pBuf[j]);
				}
			}
		}
		SetPos(i + 1,0);
	}
	TEST_THREAD_ABOART_S(POST_CLR);
	return 0;
}