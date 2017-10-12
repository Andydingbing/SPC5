#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxLOLeakageThread.h"
#include "CheckDlg.h"
#include <limits.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CTxLOLeakageThread, CCalBaseThread)

CCalR1CTxLOLeakageThread::CCalR1CTxLOLeakageThread()
{
}

CCalR1CTxLOLeakageThread::CCalR1CTxLOLeakageThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CTxLOLeakageThread::~CCalR1CTxLOLeakageThread()
{
}

BOOL CCalR1CTxLOLeakageThread::InitInstance()
{
	return TRUE;
}

int CCalR1CTxLOLeakageThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CTxLOLeakageThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CTxLOLeakageThread::Run()
{
	return CalTxLOLeakage();
}

int CCalR1CTxLOLeakageThread::CalTxLOLeakage()
{
	try
	{
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx LO Leakage"));
		CCalR1CTxLOLeakageDlg *pParent = (CCalR1CTxLOLeakageDlg *)m_pParent;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CSP3501 *pSP3501 = pParent->m_pSP3501;
		bool bCalX9119 = pParent->m_bCalX9119;
		bool bUseSA = (0 == pParent->m_bUseSA);

		uint64_t uiFreq = 2400000000;
		int16_t iDC_I_L = -10000,iDC_I_R = 10000,iDC_I_M = 0;
		int16_t iDC_Q_L = -10000,iDC_Q_R = 10000,iDC_Q_M = 0;
		int16_t iStep = 0;

		Result Data;
		Data.m_bUseSA = bUseSA;
		InitSinglePos("Calibrating Tx LO Leakage",100);

		if (bCalX9119 || bUseSA) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Spectrum"));
		}

		pSP1401->SetTxAtt(10.0,10.0,10.0,0.0);
		pSP2401->SetDASw(CSP2401R1A::FromRf);
		pSP2401->SetADSw(CSP2401R1A::FromRf);
		pSP2401->SetDDSSrc(CSP2401R1A::All_Zero);
		pSP2401->SetTxPowerComp(0.0);
		pSP2401->SetTxDCOffset(0,0);
		pSP2401->SetTxFilterSw(false);

		if (bCalX9119) {
			CAL_THREAD_TEST_CANCEL();
			double dMkrFreq = 0.0;
			double dMkrPow  = -100;
			X9119TableR1A::Data Xdata;
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
				if (abs(dMkrFreq - 2400000000.0) < 1.0 && dMkrPow >= -70.0) {
					Xdata.m_iValue = iValue;
					pSP1401->GetCalFile()->Add(ICalFile::X9119,(X9119TableR1A::Data *)(&Xdata));
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
			Instrument.SA_SetSpan(10e3);
			Instrument.SA_SetRefLevel(0.0);

			pSP1401->SetIOMode(OutPut);

			CAL_THREAD_TEST_CANCEL();

			iDC_I_L = -10000;
			iDC_I_R = 10000;
			iDC_I_M = 0;
			iDC_Q_L = -10000;
			iDC_Q_R = 10000;
			iDC_Q_M = 0;
			pSP1401->SetTxFreq(uiFreq);
			pSP2401->SetTxDCOffset(iDC_I_M,iDC_Q_M);

			Instrument.SA_SetCF((double)uiFreq);
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

			Data.m_uiFreq = uiFreq;
			Data.m_iDCI = iDC_I_M;
			Data.m_iDCQ = iDC_Q_M;
			Data.m_dPower = dPowerLL;
			pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
			pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
			pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
			pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
			time_t Time;
			time(&Time);
			Data.m_EndTime = *localtime(&Time);
			pSP1401->GetCalFile()->Add(ICalFile::TxLoLeakage,(TxLOLeakageTableR1C::DataF *)&Data);
			
			for (int32_t i = 0;i < SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED);i ++) {
				Data.m_uiFreq = RF_TX_FREQ_STAR + i * RF_TX_FREQ_STEP_CALLED;
				Data.m_iIdx = i;
				pParent->m_Data.at(i) = *(TxLOLeakageTableR1C::DataF *)(&Data);
			}
			SetPos(100,m_nThreadID);
			Instrument.Close();
		}
		else {
			int64_t iAD_LL[512] = {0},iAD_Min = LONG_MAX;

			pSP1401->SetIOMode(Loop);
			pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
			pSP1401->SetRxAtt019Sw(SP1401::Rx_Att_0);
			pSP2401->SetDDC(-92640000.0);

			CAL_THREAD_TEST_CANCEL();

			iDC_I_L = -10000;
			iDC_I_R = 10000;
			iDC_I_M = 0;
			iDC_Q_L = -10000;
			iDC_Q_R = 10000;
			iDC_Q_M = 0;
			pSP1401->SetRxFreq(uiFreq);
			pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
			pSP1401->SetRxAtt019Sw(SP1401::Rx_Att_0);
			pSP1401->SetRxAtt1(20.0);
			pSP1401->SetRxAtt2(20.0);
			pSP1401->SetRxAtt3(0.0);
			pSP1401->SetTxFreq(uiFreq);
			pSP2401->SetTxDCOffset(iDC_I_M,iDC_Q_M);
			Sleep(50);

			CAL_THREAD_TEST_CANCEL();
			iStep = 200;
			memset(iAD_LL,0,sizeof(iAD_LL));
			iAD_Min = GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,1,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
			memset(iAD_LL,0,sizeof(iAD_LL));
			iAD_Min = GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,1,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);

			CAL_THREAD_TEST_CANCEL();
			iStep = 20;
			pSP1401->SetRxAtt1(10.0);
			memset(iAD_LL,0,sizeof(iAD_LL));
			iAD_Min = GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,1,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
			memset(iAD_LL,0,sizeof(iAD_LL));
			iAD_Min = GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,1,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);

			CAL_THREAD_TEST_CANCEL();
			iStep = 5;
			pSP1401->SetRxAtt2(0.0);
			memset(iAD_LL,0,sizeof(iAD_LL));
			iAD_Min = GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,1,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
			memset(iAD_LL,0,sizeof(iAD_LL));
			iAD_Min = GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,1,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);

			CAL_THREAD_TEST_CANCEL();
			iStep = 1;
			pSP1401->SetRxAtt1(0.0);
			for (int32_t iRetry = 0;iRetry < 3;iRetry ++) {
				CAL_THREAD_TEST_CANCEL();
				memset(iAD_LL,0,sizeof(iAD_LL));
				iAD_Min = GetMinDCOffsetI_Rx(pSP1401,pSP2401,iStep,2,&iDC_I_L,&iDC_I_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
				memset(iAD_LL,0,sizeof(iAD_LL));
				iAD_Min = GetMinDCOffsetQ_Rx(pSP1401,pSP2401,iStep,2,&iDC_Q_L,&iDC_Q_R,&iDC_I_M,&iDC_Q_M,iAD_LL);
			}

			Data.m_uiFreq = uiFreq;
			Data.m_iDCI = iDC_I_M;
			Data.m_iDCQ = iDC_Q_M;
			Data.m_dPower = ad2dBc(_0dBFS,iAD_Min);
			pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
			pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
			pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
			pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
			time_t Time;
			time(&Time);
			Data.m_EndTime = *localtime(&Time);
			pSP1401->GetCalFile()->Add(ICalFile::TxLoLeakage,(TxLOLeakageTableR1C::DataF *)&Data);

			for (int32_t i = 0;i < SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED);i ++) {
				Data.m_uiFreq = RF_TX_FREQ_STAR + i * RF_TX_FREQ_STEP_CALLED;
				Data.m_iIdx = i;
				pParent->m_Data.at(i) = *(TxLOLeakageTableR1C::DataF *)(&Data);
			}
			SetPos(100,m_nThreadID);
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

double CCalR1CTxLOLeakageThread::DerivativeDCOffsetI_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower)
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

double CCalR1CTxLOLeakageThread::DerivativeDCOffsetQ_SA(CSP2401R1A *pSP2401,int16_t iDC_I_m,int16_t iDC_Q_m,int16_t iDelta,double &dPower)
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

int64_t CCalR1CTxLOLeakageThread::GetMinDCOffsetI_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_I_L,int16_t *pDC_I_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD)
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
	return iAD_Min;
}

int64_t CCalR1CTxLOLeakageThread::GetMinDCOffsetQ_Rx(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int16_t iStep,int16_t iLRCoef,int16_t *pDC_Q_L,int16_t *pDC_Q_R,int16_t *pDC_I_M,int16_t *pDC_Q_M,int64_t *pAD)
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
	return iAD_Min;
}


IMPLEMENT_DYNCREATE(CExportR1CTxLoLeakageThread, CWinThread)

CExportR1CTxLoLeakageThread::CExportR1CTxLoLeakageThread()
{
}

CExportR1CTxLoLeakageThread::CExportR1CTxLoLeakageThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CTxLoLeakageThread::~CExportR1CTxLoLeakageThread()
{
}

BOOL CExportR1CTxLoLeakageThread::InitInstance()
{
	return TRUE;
}

int CExportR1CTxLoLeakageThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CTxLoLeakageThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CTxLoLeakageThread::Run()
{
	CCalR1CTxLOLeakageDlg *pParent = (CCalR1CTxLOLeakageDlg *)m_pParent;
	int32_t i = 0;

	InitSinglePos("Exporting Tx LO Leakage",100);

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxLoLeakage);
	for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_CALLED) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq,&(pParent->m_Data.at(i)));
		i ++;
	}
	SetPos(100,m_nThreadID);
	CAL_THREAD_ABOART();
	return 0;
}
