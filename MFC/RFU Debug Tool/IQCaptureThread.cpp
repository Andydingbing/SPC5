#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "IQCaptureThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CIQCaptureThread, CWinThread)

CIQCaptureThread::CIQCaptureThread()
{

}

CIQCaptureThread::~CIQCaptureThread()
{
}

CIQCaptureThread::CIQCaptureThread(CWnd *pParent)
{
	m_pParent = pParent;
}

BOOL CIQCaptureThread::InitInstance()
{
	return TRUE;
}

int CIQCaptureThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CIQCaptureThread, CWinThread)
END_MESSAGE_MAP()


int CIQCaptureThread::Run()
{
	bool bRFCap = false,bBBCap = false;
	int i = 0;
	uint32_t uiSamples = 4096;
	uint32_t uiSamplesOld = uiSamples;
	ISP1401 *pSP1401 = ALL_DLG->m_pRFIQCapDlg[0].m_pSP1401;
	Result Data;
	Data.m_pI = new int16_t[uiSamplesOld];
	Data.m_pQ = new int16_t[uiSamplesOld];
	Data.m_pXTD = new double[uiSamplesOld];
	Data.m_pXDFT = new double[uiSamplesOld];
	Data.m_pdB = new double[(245760000 / 2) / (245760000 / uiSamplesOld) * 2];

#define POST_CLEAR			\
	delete []Data.m_pdB;	\
	delete []Data.m_pXDFT;	\
	delete []Data.m_pXTD;	\
	delete []Data.m_pQ;		\
	delete []Data.m_pI;

	while (1) {
		TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
		for (i = 0;i < MAX_RF;i ++) {
			if (::IsWindowVisible(ALL_DLG->m_pRFIQCapDlg[i].GetSafeHwnd())) {
				::SendMessage(ALL_DLG->m_pRFIQCapDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_TRUE,0,0);
				bRFCap = true;
				uiSamples = uint32_t(ALL_DLG->m_pRFIQCapDlg[i].m_uiCapSamples);
				pSP1401 = ALL_DLG->m_pRFIQCapDlg[i].m_pSP1401;
				break;
			}
			if (::IsWindowVisible(ALL_DLG->m_pBBIQCapDlg[i].GetSafeHwnd())) {
				::SendMessage(ALL_DLG->m_pBBIQCapDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_TRUE,0,0);
				bBBCap = true;
				uiSamples = uint32_t(ALL_DLG->m_pBBIQCapDlg[i].m_uiCapSamples);
				pSP1401 = ALL_DLG->m_pBBIQCapDlg[i].m_pSP1401;
				break;
			}
		}
		if (bRFCap || bBBCap) {
			if (uiSamples > uiSamplesOld) {
				uiSamplesOld = uiSamples;
				delete []Data.m_pdB;
				delete []Data.m_pXDFT;
				delete []Data.m_pXTD;
				delete []Data.m_pQ;
				delete []Data.m_pI;
				Data.m_pI = new int16_t[uiSamplesOld];
				Data.m_pQ = new int16_t[uiSamplesOld];
				Data.m_pXTD = new double[uiSamplesOld];
				Data.m_pXDFT = new double[uiSamplesOld];
				Data.m_pdB = new double[(245760000 / 2) / (245760000 / uiSamplesOld) * 2];
			}
//			pSP1401->SetIQCapSamples(uiSamples);
			pSP1401->IQCap();
		}
		if (bRFCap) {
			if (ALL_DLG->m_pRFIQCapDlg[i].m_bShowingDFT)
				DFT(pSP1401,245760000,uiSamples,&Data.m_iPts,Data.m_pXDFT,Data.m_pdB);
			else {
				TimeDomain(pSP1401,uiSamples,Data.m_pXTD,Data.m_pI,Data.m_pQ);
				Data.m_iPts = uiSamples;
			}
			::SendMessage(ALL_DLG->m_pRFIQCapDlg[i].GetSafeHwnd(),WM_CSE_ALL_IQ_CAP_SHOW,0,(LPARAM)&Data);
		}
		if (bBBCap) {
			if (ALL_DLG->m_pBBIQCapDlg[i].m_bShowingDFT)
				DFT(pSP1401,245760000,uiSamples,&Data.m_iPts,Data.m_pXDFT,Data.m_pdB);
			else {
				TimeDomain(pSP1401,uiSamples,Data.m_pXTD,Data.m_pI,Data.m_pQ);
				Data.m_iPts = uiSamples;
			}
			::SendMessage(ALL_DLG->m_pBBIQCapDlg[i].GetSafeHwnd(),WM_CSE_ALL_IQ_CAP_SHOW,0,(LPARAM)&Data);
		}
	}
	return 0;
}


IMPLEMENT_DYNCREATE(CPowMeasThread, CWinThread)

CPowMeasThread::CPowMeasThread()
{

}

CPowMeasThread::~CPowMeasThread()
{
}

CPowMeasThread::CPowMeasThread(CWnd *pParent,PowMeasInfo *pInfo)
{
	ParentWnd = pParent;
	m_powmeasinfo.RF = pInfo->RF;
	m_powmeasinfo.m_PMSrc = pInfo->m_PMSrc;
}

BOOL CPowMeasThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPowMeasThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPowMeasThread, CWinThread)
END_MESSAGE_MAP()


// CPowMeasThread 消息处理程序

int CPowMeasThread::Run()
{
	double power = 0.0;
	CSP1401R1A::PowerMeasState state = CSP1401R1A::IDLE;
	CSP1401R1A *RF = m_powmeasinfo.RF;
	while (1) {
		state = CSP1401R1A::IDLE;
		STACHK(RF->SetPowerMeasSrc(m_powmeasinfo.m_PMSrc,true));
		STACHK(RF->SetPowerMeasTimeout(6553500));
		STACHK(RF->SetPowerMeasSamples(102400));
		STACHK(RF->SetPowerMeasTrigThreshold(0.0));
		STACHK(RF->PowerMeasAbort());
		STACHK(RF->PowerMeasStart());

		while (state != CSP1401R1A::DONE) {
			STACHK(RF->GetPowerMeasState(state));
			if (state == CSP1401R1A::TT)
				break;
		}
		STACHK(RF->GetPowerMeasPower(power));
		Log.stdprintf("%*f\n",(RF->GetRfIdx() + 1) * 10,power);
		Sleep(100);
	}
	
	return 0;
}
