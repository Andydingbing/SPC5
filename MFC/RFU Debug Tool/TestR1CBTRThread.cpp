#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CBTRThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestR1CBTRThread::Result::Result()
{
	memset(m_dTemp,0,sizeof(m_dTemp));
	m_bAddLiner = false;
}

IMPLEMENT_DYNCREATE(CTestR1CBTRThread, CCSEWinThread)

CTestR1CBTRThread::CTestR1CBTRThread()
{
}

CTestR1CBTRThread::CTestR1CBTRThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CTestR1CBTRThread::~CTestR1CBTRThread()
{
}

BOOL CTestR1CBTRThread::InitInstance()
{
	return TRUE;
}

int CTestR1CBTRThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1CBTRThread, CWinThread)
END_MESSAGE_MAP()


int CTestR1CBTRThread::Run()
{
#define POST_CLEAR	SetBlowerToDefault(pSP3501);if (fp)	fclose(fp);

	DECLARE_R1C_GETTEMP_FUNCTION_MAP()

	CSP3301 *pSP3301[MAX_RFU] = {&SP3301_0,&SP3301_1,&SP3301_2,&SP3301_3,&SP3301_4};
	CSP3501 *pSP3501 = &SP3501;
	CSP3301::Active Act[MAX_RFU];
	bool bSP1401Exist[MAX_RFU] = {false,false,false,false,false};
	Result Data;

	double dEnvTemp = ALL_DLG->m_pTestR1CBTRDlg[0].m_dEnvTemp;
	int32_t iSpeedStep = ALL_DLG->m_pTestR1CBTRDlg[0].m_iSpeedStep;
	int32_t iKeepTime = ALL_DLG->m_pTestR1CBTRDlg[0].m_iKeepTime;
	int32_t iSpeedStar = 50;
	CString strPath = "";
	FILE *fp = NULL;
	((CEdit *)(ALL_DLG->m_pTestR1CBTRDlg[0].GetDlgItem(IDC_EDIT4)))->GetWindowText(strPath);
	bool bWriteF = (strPath == "" ? false : true);
	if (bWriteF) {
		fp = fopen((LPSTR)(LPCTSTR)strPath,"w");
		if (NULL == fp) {
			CseMessageBox("Create %s Fail!",strPath);
			TEST_THREAD_ABOART();
			return 0;
		}
		fprintf(fp,"%.3f℃\n",dEnvTemp);
		fprintf(fp,"speed ");
	}

	pGetTemp tGetTemp[8] = {&CSP1401R1C::GetTemp0RxLO2,
							&CSP1401R1C::GetTemp1RxLNA,
							&CSP1401R1C::GetTemp2RxLO1,
							&CSP1401R1C::GetTemp3RxPA1,
							&CSP1401R1C::GetTemp4TxLO2,
							&CSP1401R1C::GetTemp5TxLO1,
							&CSP1401R1C::GetTemp6TxPA4,
							&CSP1401R1C::GetTemp7TxPA3};

	for (int32_t i = 0;i < MAX_RFU;i ++) {
		pSP3301[i]->Boot();
		Act[i] = pSP3301[i]->GetActive();
		for (int32_t j = 0;j < MAX_RF;j++) {
			if (Act[i].m_bSP1401[j]) {
				bSP1401Exist[i] = true;
				break;
			}
		}
	}

	if (bWriteF) {
		CString strTempName[8] = {"rx_lo2","rx_lna","rx_lo1","rx_pa1","tx_lo2","tx_lo1","tx_pa4","tx_pa3"};
		for (int32_t i = 0;i < MAX_RFU;i ++) {
			for (int32_t j = 0;j < MAX_RF;j ++) {
				if (Act[i].m_bSP1401[j]) {
					for (int32_t k = 0;k < 8;k ++)
						fprintf(fp,"rfu%d_rf%d_temp%d_%s ",i,j,k,strTempName[k]);
				}
			}
		}
		fprintf(fp,"\n");
	}

	pSP3501->SetBlower(iSpeedStar);

	COleDateTime OneStepBeginTime = COleDateTime::GetCurrentTime();
	COleDateTime OneStepEndTime = OneStepBeginTime;
	COleDateTimeSpan OneStepTime = OneStepEndTime - OneStepBeginTime;
	COleDateTimeSpan KeepTime(0,0,(int)iKeepTime,0);

	InitSinglePos("风扇转速vs温度关系:Preheating 30 Minutes...",30);
	while (OneStepTime.GetTotalMinutes() < 30) {	//preheat 30 min
		TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
		for (int32_t i = 0;i < MAX_RFU;i ++) {
			TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
			for (int32_t j = 0;j < MAX_RF;j ++) {
				TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
				if (Act[i].m_bSP1401[j]) {
					for (int32_t k = 0;k < 8;k ++)
						(pSP3301[i]->m_pSP1401R1C[j]->*tGetR1CTemp[k])(&Data.m_dTemp[j][k]);
				}
			}
			Data.m_Time = COleDateTime::GetCurrentTime();
			if (bSP1401Exist[i])
				::SendMessage(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd(),WM_CSE_TEST_BTR_SHOW,0,(LPARAM)&Data);
		}
		Sleep(10000);
		OneStepEndTime = COleDateTime::GetCurrentTime();
		OneStepTime = OneStepEndTime - OneStepBeginTime;
		SetPos(OneStepTime.GetTotalMinutes(),m_nThreadID);
	}

	InitSinglePos("风扇转速vs温度关系:Testing...",SERIE_SIZE(iSpeedStar,255,iSpeedStep));
	for (int32_t iSpeed = iSpeedStar + iSpeedStep;iSpeed <= 255;iSpeed += iSpeedStep) {
		if (iSpeed > iSpeedStar + iSpeedStep) {
			Data.m_bAddLiner = true;
			for (int32_t i = 0;i < MAX_RFU;i ++) {
				if (bSP1401Exist[i])
					::SendMessage(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd(),WM_CSE_TEST_BTR_SHOW,0,(LPARAM)&Data);
			}
		}

		pSP3501->SetBlower(iSpeed);

		Data.m_iCurSpeed = iSpeed;
		Data.m_bAddLiner = false;
		OneStepBeginTime = OneStepEndTime = COleDateTime::GetCurrentTime();
		OneStepTime = OneStepEndTime - OneStepBeginTime;

		while (OneStepTime <= KeepTime) {
			TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
			if (bWriteF)
				fprintf(fp,"%-3d ",Data.m_iCurSpeed);
			for (int32_t i = 0;i < MAX_RFU;i ++) {
				TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);
				for (int32_t j = 0;j < MAX_RF;j ++) {
					TEST_THREAD_TEST_CANCEL_S(POST_CLEAR);;
					if (Act[i].m_bSP1401[j]) {
						for (int32_t k = 0;k < 8;k ++) {
							(pSP3301[i]->m_pSP1401R1C[j]->*tGetR1CTemp[k])(&Data.m_dTemp[j][k]);
							if (bWriteF)
								fprintf(fp,"%.3f ",Data.m_dTemp[j][k]);
						}
					}
				}
				Data.m_Time = OneStepEndTime = COleDateTime::GetCurrentTime();
				OneStepTime = OneStepEndTime - OneStepBeginTime;
				if (bSP1401Exist[i])
					::SendMessage(ALL_DLG->m_pTestR1CBTRDlg[i].GetSafeHwnd(),WM_CSE_TEST_BTR_SHOW,0,(LPARAM)&Data);
				Sleep(10);
			}
			if (bWriteF)
				fprintf(fp,"\n");
			Sleep(OneStepTime.GetTotalSeconds() < KeepTime.GetTotalSeconds() / 3 ? 10000 : 2000);
		}
		SetPos(SERIE_INDEX(iSpeed,iSpeedStar,iSpeedStep),m_nThreadID);
	}
	TEST_THREAD_ABOART_S(POST_CLEAR);
	return 0;
}

void CTestR1CBTRThread::SetBlowerToDefault(CSP3501 *pSP3501)
{
	pSP3501->SetBlower(100);
}