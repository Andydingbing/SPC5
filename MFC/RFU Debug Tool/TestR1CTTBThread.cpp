#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CTTBThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTestR1CTTBThread, CCSEWinThread)

CTestR1CTTBThread::CTestR1CTTBThread()
{
}

CTestR1CTTBThread::CTestR1CTTBThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CTestR1CTTBThread::~CTestR1CTTBThread()
{
}

BOOL CTestR1CTTBThread::InitInstance()
{
	return TRUE;
}

int CTestR1CTTBThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1CTTBThread, CWinThread)
END_MESSAGE_MAP()


int CTestR1CTTBThread::Run()
{
#define POST_CLEAR	SetBlowerToDefault(pSP3501);if (fp)	fclose(fp);

	DECLARE_R1C_GETTEMP_FUNCTION_MAP()

	Instrument.Init();
	Instrument.HasPM();
	Instrument.PM_Reset();

	CTestR1CTTBDlg *pParent = (CTestR1CTTBDlg *)m_pParent;
	CSP3501 *pSP3501 = &SP3501;
	CSP1401R1C *pSP1401 = pParent->m_pSP1401;
	CSP2401R1A *pSP2401 = pParent->m_pSP2401;
	int32_t iSpeedStep = pParent->m_iSpeedStep;
	int32_t iKeepTime = pParent->m_iKeepTime;
	int32_t iSpeedStar = 50;
	CString strFreqStar = pParent->m_strFreqStar;
	CString strFreqStop = pParent->m_strFreqStop;
	CString strFreqStep = pParent->m_strFreqStep;

	uint64_t uiFreq[RF_TX_FREQ_PTS_CALLED];
	int32_t iFreqPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq);

	Result Data;
	double dTemp = 0.0;

	CString strPath = "";
	FILE *fp = NULL;
	((CEdit *)(pParent->GetDlgItem(IDC_EDIT4)))->GetWindowText(strPath);
	bool bWriteF = (strPath == "" ? false : true);

	pGetTemp tGetTemp[8] = {&CSP1401R1C::GetTemp0RxLO2,
							&CSP1401R1C::GetTemp1RxLNA,
							&CSP1401R1C::GetTemp2RxLO1,
							&CSP1401R1C::GetTemp3RxPA1,
							&CSP1401R1C::GetTemp4TxLO2,
							&CSP1401R1C::GetTemp5TxLO1,
							&CSP1401R1C::GetTemp6TxPA4,
							&CSP1401R1C::GetTemp7TxPA3};

	double dAtt0 = 0.0,dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0,dDGain = 0.0;
	TxPowerOPTableR1C::GuessBase(dAtt0,dAtt1,dAtt2,dAtt3,dDGain);
	pSP1401->SetIOMode(OutPut);
	pSP1401->SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3);
	pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
	pSP2401->SetDDS1(20000000.0);
	pSP2401->SetTxPowerComp(dDGain);
	pSP2401->SetTxFilterSw(false);
	pSP3501->SetBlower(iSpeedStar);

	COleDateTime OneStepBeginTime = COleDateTime::GetCurrentTime();
	COleDateTime OneStepEndTime = OneStepBeginTime;
	COleDateTimeSpan OneStepTime = OneStepEndTime - OneStepBeginTime;
	COleDateTimeSpan KeepTime(0,0,(int)iKeepTime,0);

	InitSinglePos("Tx Base Power vs RF Temperature:Preheating 10 Minutes...",10);
	while (OneStepTime.GetTotalMinutes() < 10) {	//preheat 10 min
		TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
		Sleep(10000);
		OneStepEndTime = COleDateTime::GetCurrentTime();
		OneStepTime = OneStepEndTime - OneStepBeginTime;
		SetPos(OneStepTime.GetTotalMinutes(),m_nThreadID);
	}

	InitSinglePos("Tx Base Power vs RF Temperature:Testing...",SERIE_SIZE(iSpeedStar,255,iSpeedStep));
	for (int32_t iSpeed = iSpeedStar;iSpeed <= 255;iSpeed += iSpeedStep) {
		pSP3501->SetBlower(iSpeed);

		OneStepBeginTime = OneStepEndTime = COleDateTime::GetCurrentTime();
		OneStepTime = OneStepEndTime - OneStepBeginTime;

		while (OneStepTime <= KeepTime) {
			TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);

			for (int32_t i = 0;i < iFreqPts;i ++) {
				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.PM_SetFrequency((double)(uiFreq[i]));
				Sleep(10);
				Instrument.PM_GetPwr(Data.m_dPower);

				Data.m_dTemp = 0.0;
				for (int32_t iTemp = 4;iTemp < 8;i ++) {
					(pSP1401->*tGetR1CTemp[iTemp])(&dTemp);
					Data.m_dTemp += dTemp; 
				}
				Data.m_dTemp /= 4.0;
				Data.m_iFreqIdx = i;
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_TEST_TTB_SHOW,0,(LPARAM)&Data);
			}
			
			Sleep(10);
		}
		SetPos(SERIE_INDEX(iSpeed,iSpeedStar,iSpeedStep),m_nThreadID);
	}
	TEST_THREAD_ABOART_S(POST_CLEAR);
	return 0;
}

void CTestR1CTTBThread::SetBlowerToDefault(CSP3501 *pSP3501)
{
	pSP3501->SetBlower(100);
}