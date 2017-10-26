#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxLOLeakageThread.h"
#include "CheckDlg.h"
#include <limits.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1ATxLOLeakageThread, CCalBaseThread)

CCalR1ATxLOLeakageThread::CCalR1ATxLOLeakageThread()
{
}

CCalR1ATxLOLeakageThread::CCalR1ATxLOLeakageThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1ATxLOLeakageThread::~CCalR1ATxLOLeakageThread()
{
}

BOOL CCalR1ATxLOLeakageThread::InitInstance()
{
	return TRUE;
}

int CCalR1ATxLOLeakageThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1ATxLOLeakageThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1ATxLOLeakageThread::Run()
{
	return CalTxLOLeakage();
}

int CCalR1ATxLOLeakageThread::CalTxLOLeakage()
{
	try 
	{
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx LO Leakage"));
		CCalR1ATxLOLeakageDlg *pParent = (CCalR1ATxLOLeakageDlg *)m_pParent;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		CSP3501 *pSP3501 = pParent->m_pSP3501;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		bool bCalX9119 = pParent->m_bCalX9119;
		bool bUseSA = (0 == pParent->m_bUseSA);

		int16_t iDC_I_L = -10000,iDC_I_R = 10000,iDC_I_M = 0;
		int16_t iDC_Q_L = -10000,iDC_Q_R = 10000,iDC_Q_M = 0;
		int16_t iStep = 0;
		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		memset(uiFreq,0,ARRAY_SIZE(uiFreq));

		Result Data;
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq);
		InitSinglePos("Calibrating Tx LO Leakage",iPts);

		if (bCalX9119 || bUseSA) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Spectrum"));
		}

		pSP1401->SetTxPAAttSw(SP1401::Tx_Att);
		pSP1401->SetTxAtt(10,0,0);
		pSP2401->SetDASw(CSP2401R1A::FromRf);
		pSP2401->SetADSw(CSP2401R1A::FromRf);
		pSP2401->SetDDSSrc(CSP2401R1A::All_Zero);
		pSP2401->SetTxPowerComp(0.0);
		pSP2401->SetTxDCOffset(0,0);

		if (bCalX9119) {
			CAL_THREAD_TEST_CANCEL();
			double dMkrFreq = 0.0;
			double dMkrPow  = -100;
			X9119Table::Data Xdata;
			pSP1401->SetIOMode(OutPut);
			pSP1401->SetTxFreq(2400000000);
			Instrument.Init();
			Instrument.HasSA();
			Instrument.SA_Reset();
			Instrument.SA_SetCF(2400000000.0);
			Instrument.SA_SetSpan(5000.0);
			Instrument.SA_SetRBW(200.0,false);
			Instrument.SA_SetVBW(300.0,false);
			Instrument.SA_SetRefLevel(-10.0);
			for (int16_t iValue = 0;iValue < 1023;iValue ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP3501->Vol9119(iValue);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerFrequency(dMkrFreq);
				Instrument.SA_GetMarkerPwr(dMkrPow);
				if (abs(dMkrFreq - 2400000000.0) < 1.0 && dMkrPow >= -60.0) {
					Xdata.m_iValue = iValue;
					pSP1401->GetCalFile()->Add(ICalFile::X9119,(X9119Table::Data *)(&Xdata));
					pSP1401->GetCalFile()->Write(ICalFile::X9119);
					break;
				}
			}
			Instrument.Close();
		}

		if (bUseSA) {
			double dPowerLL = 0.0,dPowerSA = 0.0;

			Instrument.Init();
			Instrument.HasSA();
			Instrument.SA_Reset();
			Instrument.SA_SetSpan(20e3);
			Instrument.SA_SetRefLevel(0.0);

			pSP1401->SetIOMode(OutPut);

			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();

				iDC_I_L = -10000;
				iDC_I_R = 10000;
				iDC_I_M = 0;
				iDC_Q_L = -10000;
				iDC_Q_R = 10000;
				iDC_Q_M = 0;
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP2401->SetTxDCOffset(iDC_I_M,iDC_Q_M);

				Instrument.SA_SetCF((double)uiFreq[i]);
				Instrument.SA_SetRefLevel(0.0);
				Instrument.SA_SweepOnce();
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);

				iStep = 200;
				while ((iDC_I_R - iDC_I_L) > 4) {
					CAL_THREAD_TEST_CANCEL();
					iDC_I_M = (int16_t)round((iDC_I_L + iDC_I_R) / 2.0,0);
					if (DerivativeDCOffsetI_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) > 0)
						iDC_I_R = iDC_I_M;
					else
						iDC_I_L = iDC_I_M;
					if (dPowerLL < -80.0)
						break;
				}
				while ((iDC_Q_R - iDC_Q_L) > 4) {
					CAL_THREAD_TEST_CANCEL();
					iDC_Q_M = (int16_t)round((iDC_Q_L + iDC_Q_R) / 2.0,0);
					if (DerivativeDCOffsetQ_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) > 0)
						iDC_Q_R = iDC_Q_M;
					else
						iDC_Q_L = iDC_Q_M;
					if (dPowerLL < -80.0)
						break;
				}

				iStep = 20;
				iDC_I_L = iDC_I_M - 100;
				iDC_I_R = iDC_I_M + 100;
				if (dPowerLL <= -40.0)
					Instrument.SA_SetRefLevel(-40.0);
				while ((iDC_I_R - iDC_I_L) > 4) {
					CAL_THREAD_TEST_CANCEL();
					if (dPowerLL < -90.0)
						break;
					iDC_I_M = (int16_t)round((iDC_I_L + iDC_I_R) / 2.0,0);
					if (DerivativeDCOffsetI_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) > 0)
						iDC_I_R = iDC_I_M;
					else
						iDC_I_L = iDC_I_M;
				}
				iDC_Q_L = iDC_Q_M - 100;
				iDC_Q_R = iDC_Q_M + 100;
				while ((iDC_Q_R - iDC_Q_L) > 4) {
					CAL_THREAD_TEST_CANCEL();
					if (dPowerLL < -90.0)
						break;
					iDC_Q_M = (int16_t)round((iDC_Q_L + iDC_Q_R) / 2.0,0);
					if (DerivativeDCOffsetQ_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) > 0)
						iDC_Q_R = iDC_Q_M;
					else
						iDC_Q_L = iDC_Q_M;
				}

				if (dPowerLL <= -40.0)
					Instrument.SA_SetRefLevel(-40.0);
				iStep = 1;
				for (int16_t iRetry = 0;iRetry < 1;iRetry ++) {
					if (DerivativeDCOffsetI_SA(pSP2401,iDC_I_M,iDC_Q_M,1,dPowerLL) > 0) {
						while ( -- iDC_I_M) {
							if (DerivativeDCOffsetI_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) < 0)
								break;
						}
					}
					else {
						while ( ++ iDC_I_M) {
							if (DerivativeDCOffsetI_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) > 0)
								break;
						}
					}

					if (DerivativeDCOffsetQ_SA(pSP2401,iDC_I_M,iDC_Q_M,1,dPowerLL) > 0) {
						while ( -- iDC_Q_M) {
							if (DerivativeDCOffsetQ_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) < 0)
								break;
						}
					}
					else {
						while ( ++ iDC_Q_M) {
							if (DerivativeDCOffsetQ_SA(pSP2401,iDC_I_M,iDC_Q_M,iStep,dPowerLL) > 0)
								break;
						}
					}
				}

				int16_t iDC_I_M_Min = iDC_I_M;
				int16_t iDC_Q_M_Min = iDC_Q_M;
//				Instrument.SA_SetAverTrace(true,3);
				for (int retry = 0;retry < 1;retry ++) {
					iDC_I_L = iDC_I_M - 5;
					iDC_I_R = iDC_I_M + 5;
					for (iDC_I_M = iDC_I_L; iDC_I_M <= iDC_I_R; iDC_I_M ++) {
						pSP2401->SetTxDCOffset(iDC_I_M,iDC_Q_M);
						Instrument.SA_SweepOnce();
						Instrument.SA_GetMarkerPwr(dPowerSA);
						if (dPowerSA < dPowerLL) {
							dPowerLL = dPowerSA;
							iDC_I_M_Min = iDC_I_M;
						}
					}
					iDC_I_M = iDC_I_M_Min;
					for (iDC_Q_M = iDC_Q_L; iDC_Q_M <= iDC_Q_R; iDC_Q_M ++) {
						pSP2401->SetTxDCOffset(iDC_I_M,iDC_Q_M);
						Instrument.SA_SweepOnce();
						Instrument.SA_GetMarkerPwr(dPowerSA);
						if (dPowerSA < dPowerLL) {
							dPowerLL = dPowerSA;
							iDC_Q_M_Min = iDC_Q_M;
						}
					}
					iDC_Q_M = iDC_Q_M_Min;
				}
//				Instrument.SA_SetAverTrace(false);

				Data.m_iIdx   = i;
				Data.m_uiFreq = uiFreq[i];
				Data.m_iDCI   = iDC_I_M;
				Data.m_iDCQ   = iDC_Q_M;
				pSP1401->GetCalFile()->Add(ICalFile::TxLoLeakage,(TxLOLeakageTableR1A::DataF *)&Data);
				pParent->m_Data.at(i) = *(TxLOLeakageTableR1A::DataF *)(&Data);
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_LOLEAKAGE_SHOW,0,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			Instrument.Close();
		}
		else {
			int64_t iAD_LL[512] = {0},iAD_Min = LONG_MAX;

			pSP1401->SetIOMode(Loop);
			pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
			pSP2401->SetDDC(-92640000.0);
			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();

				iDC_I_L = -10000;
				iDC_I_R = 10000;
				iDC_I_M = 0;
				iDC_Q_L = -10000;
				iDC_Q_R = 10000;
				iDC_Q_M = 0;
				pSP1401->SetRxFreq(uiFreq[i] < 100000000 ? 100000000 : uiFreq[i]);
				pSP1401->SetRxAtt1(20.0);
				pSP1401->SetRxAtt2(20);
				pSP1401->SetTxFreq(uiFreq[i] < 100000000 ? 100000000 : uiFreq[i]);
				pSP2401->SetTxDCOffset(iDC_I_M,iDC_Q_M);
				Sleep(50);

				CAL_THREAD_TEST_CANCEL();
				iStep = 200;
				memset(iAD_LL,0,sizeof(iAD_LL));
				GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,1,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
				memset(iAD_LL,0,sizeof(iAD_LL));
				GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,1,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);

				CAL_THREAD_TEST_CANCEL();
				iStep = 20;
				pSP1401->SetRxAtt1(10.0);
				memset(iAD_LL,0,sizeof(iAD_LL));
				GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,1,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
				memset(iAD_LL,0,sizeof(iAD_LL));
				GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,1,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);

				CAL_THREAD_TEST_CANCEL();
				iStep = 5;
				pSP1401->SetRxAtt2(0);
				memset(iAD_LL,0,sizeof(iAD_LL));
				GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,1,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
				memset(iAD_LL,0,sizeof(iAD_LL));
				GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,1,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);

				CAL_THREAD_TEST_CANCEL();
				iStep = 1;
				pSP1401->SetRxAtt1(0.0);
				for (int32_t iRetry = 0;iRetry < 3;iRetry ++) {
					CAL_THREAD_TEST_CANCEL();
					memset(iAD_LL,0,sizeof(iAD_LL));
					GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,2,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
					memset(iAD_LL,0,sizeof(iAD_LL));
					GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,2,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
				}

				Data.m_iIdx   = i;
				Data.m_uiFreq = uiFreq[i];
				Data.m_iDCI   = iDC_I_M;
				Data.m_iDCQ   = iDC_Q_M;
				pSP1401->GetCalFile()->Add(ICalFile::TxLoLeakage,(TxLOLeakageTableR1A::DataF *)&Data);
				pParent->m_Data.at(i) = *(TxLOLeakageTableR1A::DataF *)(&Data);
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_LOLEAKAGE_SHOW,0,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
		}
		pSP1401->GetCalFile()->Write(ICalFile::TxLoLeakage);
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt)
	{
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}

double CCalR1ATxLOLeakageThread::DerivativeDCOffsetI_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower)
{
	double dPowerSA[2] = {0.0,0.0};
	pSP2401->SetTxDCOffset((uint16_t)(iDC_I_m + iDelta),(uint16_t)iDC_Q_m);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[1]);

	pSP2401->SetTxDCOffset((uint16_t)iDC_I_m,(uint16_t)iDC_Q_m);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[0]);
	dPower = dPowerSA[0];

	return (dPowerSA[1] - dPowerSA[0]) / (double)iDelta;
}

double CCalR1ATxLOLeakageThread::DerivativeDCOffsetQ_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower)
{
	double dPowerSA[2] = {0.0,0.0};
	pSP2401->SetTxDCOffset((uint16_t)iDC_I_m,(uint16_t)(iDC_Q_m + iDelta));
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[1]);

	pSP2401->SetTxDCOffset((uint16_t)iDC_I_m,(uint16_t)iDC_Q_m);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[0]);
	dPower = dPowerSA[0];

	return (dPowerSA[1] - dPowerSA[0]) / (double)iDelta;
}

void CCalR1ATxLOLeakageThread::GetMinDCOffsetI_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_I_L,int16_t *pDC_I_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD)
{
	int64_t iAD_Min = LONG_MAX;
	int16_t iDC = 0,iDCMin = 0;
	for (*pDC_I_M = *pDC_I_L;*pDC_I_M <= *pDC_I_R;*pDC_I_M += iStep) {
		pSP2401->SetTxDCOffset(*pDC_I_M,*pDC_Q_M);
		pSP1401->GetADS5474(pAD[iDC]);
		if (pAD[iDC] < iAD_Min) {
			iAD_Min = pAD[iDC];
			iDCMin = iDC;
		}
		iDC ++;
	}
	*pDC_I_M = *pDC_I_L + iDCMin * iStep;
	*pDC_I_L = *pDC_I_M - iStep * iLRCoef;
	*pDC_I_R = *pDC_I_M + iStep * iLRCoef;
}

void CCalR1ATxLOLeakageThread::GetMinDCOffsetQ_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_Q_L,int16_t *pDC_Q_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD)
{
	int64_t iAD_Min = LONG_MAX;
	int16_t iDC = 0,iDCMin = 0;
	for (*pDC_Q_M = *pDC_Q_L;*pDC_Q_M <= *pDC_Q_R;*pDC_Q_M += iStep) {
		pSP2401->SetTxDCOffset(*pDC_I_M,*pDC_Q_M);
		pSP1401->GetADS5474(pAD[iDC]);
		if (pAD[iDC] < iAD_Min) {
			iAD_Min = pAD[iDC];
			iDCMin = iDC;
		}
		iDC ++;
	}
	*pDC_Q_M = *pDC_Q_L + iDCMin * iStep;
	*pDC_Q_L = *pDC_Q_M - iStep * iLRCoef;
	*pDC_Q_R = *pDC_Q_M + iStep * iLRCoef;
}


IMPLEMENT_DYNCREATE(CExportR1ATxLoLeakageThread, CWinThread)

CExportR1ATxLoLeakageThread::CExportR1ATxLoLeakageThread()
{
}

CExportR1ATxLoLeakageThread::CExportR1ATxLoLeakageThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1ATxLoLeakageThread::~CExportR1ATxLoLeakageThread()
{
}

BOOL CExportR1ATxLoLeakageThread::InitInstance()
{
	return TRUE;
}

int CExportR1ATxLoLeakageThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1ATxLoLeakageThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1ATxLoLeakageThread::Run()
{
	CCalR1ATxLOLeakageDlg *pParent = (CCalR1ATxLOLeakageDlg *)m_pParent;

	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	CCalR1ATxLOLeakageThread::Result Data;
	Data.m_iIdx = 0;

	InitSinglePos("Exporting Tx LO Leakage",100);

	for (int32_t i = 0;i < iPts;i ++) {
		((CalFileR1A *)(pParent->m_pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[i],Data.m_iDCI,Data.m_iDCQ);
		Data.m_uiFreq = uiFreq[i];
		pParent->m_Data.at(Data.m_iIdx) = *(TxLOLeakageTableR1A::DataF *)(&Data);
		Data.m_iIdx ++;
	}
	SetPos(100,m_nThreadID);
	CAL_THREAD_ABOART();
	return 0;
}