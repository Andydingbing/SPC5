#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxSideBandThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1ATxSideBandThread, CCalBaseThread)

CCalR1ATxSideBandThread::CCalR1ATxSideBandThread()
{
}

CCalR1ATxSideBandThread::CCalR1ATxSideBandThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1ATxSideBandThread::~CCalR1ATxSideBandThread()
{
}

BOOL CCalR1ATxSideBandThread::InitInstance()
{
	return TRUE;
}

int CCalR1ATxSideBandThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1ATxSideBandThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1ATxSideBandThread::Run()
{
	return CalTxSideBand();
}

int CCalR1ATxSideBandThread::CalTxSideBand()
{
	try
	{
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Sideband"));
		CCalR1ATxSideBandDlg *pParent = (CCalR1ATxSideBandDlg *)m_pParent;
		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		bool bUseSA = (0 == pParent->m_bUseSA);
		double dThL = -10.0,dThR = 10.0,dThM = 0.0;
		uint16_t uiAM_I_L = 8192 - 500,uiAM_I_R = 8192 + 500,uiAM_I_M = 8192;
		uint16_t uiAM_Q_L = 8192 - 500,uiAM_Q_R = 8192 + 500,uiAM_Q_M = 8192;
		double dStepTh = 0.0;
		uint16_t uiStepAm = 0;
		int16_t iDCI = 0,iDCQ = 0;
		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		memset(uiFreq,0,sizeof(uiFreq));

		Result Data;
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq);
		InitSinglePos("Calibrating Tx Sideband",iPts);

		if (bUseSA) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("TX<===>Spectrum"));
		}

		pSP1401->SetTxPAAttSw(SP1401::Tx_Att);
		pSP1401->SetTxAtt(10,0,0);
		pSP2401->SetDASw(CSP2401R1A::FromRf);
		pSP2401->SetADSw(CSP2401R1A::FromRf);
		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetTxPowerComp(0.0);
		pSP2401->SetTxPhaseRotateI(0.0);
		pSP2401->SetTxAmplitudeBalance(8192,8192);

		if (bUseSA) {
			double dPowerSb = 0.0,dPowerSA = 0.0;

			Instrument.Init();
			Instrument.HasSA();
			Instrument.SA_Reset();
			Instrument.SA_SetRefLevel(-10.0);
			Instrument.SA_SetSpan(1e6);
			Instrument.SA_SetRBW(50e3,false);
			Instrument.SA_SetVBW(10e3,false);
//			Instrument.SA_SetAverTrace(true,3);

			pSP1401->SetIOMode(OutPut);

			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();
				dThL = -10.0;
				dThR = 10.0;
				uiAM_I_L = 8192 - 500;uiAM_I_R = 8192 + 500;uiAM_I_M = 8192;
				uiAM_Q_L = 8192 - 500;uiAM_Q_R = 8192 + 500;uiAM_Q_M = 8192;

				pSP1401->SetTxFreq(uiFreq[i]);
				pSP2401->SetTxPhaseRotateI(0.0);
				pSP2401->SetTxAmplitudeBalance(8192,8192);
				pSP2401->SetDUCDDS(0.0);
				((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[i],iDCI,iDCQ);
				pSP2401->SetTxDCOffset(iDCI,iDCQ);

				Instrument.SA_SetCF((double)(uiFreq[i] - 20000000));
				Instrument.SA_SetRefLevel(-10.0);
//				Instrument.SA_SetAverTrace(false);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);

				while ((dThR - dThL) >= 1) {
					CAL_THREAD_TEST_CANCEL();
					dThM = round((dThL + dThR) / 2.0,1);
					if (DerivativeTh_SA(pSP2401,dThM,1,dPowerSb) > 0)
						dThR = dThM;
					else
						dThL = dThM;
				}
				Instrument.SA_SetRefLevel(-40.0);
//				Instrument.SA_SetAverTrace(true,3);

				dThL = dThM - 0.5;
				dThR = dThM + 0.5;
				while ((dThR - dThL) >= 1) {
					CAL_THREAD_TEST_CANCEL();
					dThM = round((dThL + dThR) / 2.0,1);
					if (DerivativeTh_SA(pSP2401,dThM,0.1,dPowerSb) > 0)
						dThR = dThM;
					else
						dThL = dThM;
				}

				double dThM_Min = dThM;
				for (dThM = dThL; dThM <= dThR; dThM += 0.1) {
					CAL_THREAD_TEST_CANCEL();
					pSP2401->SetTxPhaseRotateI(dThM);
					Instrument.SA_SweepOnce();
					Instrument.SA_GetMarkerPwr(dPowerSA);
					if (dPowerSA < dPowerSb) {
						dPowerSb = dPowerSA;
						dThM_Min = dThM;
					}
				}
				dThM = dThM_Min;
				pSP2401->SetTxPhaseRotateI(dThM);

				while ((uiAM_I_R - uiAM_I_L) >= 2 ) {
					CAL_THREAD_TEST_CANCEL();
					uiAM_I_M = (uint16_t)((uiAM_I_L + uiAM_I_R) / 2);
					if (DerivativeAmI_SA(pSP2401,uiAM_I_M,uiAM_Q_M,10,dPowerSb) > 0)
						uiAM_I_R = uiAM_I_M;
					else
						uiAM_I_L = uiAM_I_M;
					if (dPowerSb < -90.0)
						break;
				}
				while ((uiAM_Q_R - uiAM_Q_L) >= 2) {
					CAL_THREAD_TEST_CANCEL();
					uiAM_Q_M = (uint16_t)((uiAM_Q_L + uiAM_Q_R) / 2);
					if (DerivativeAmQ_SA(pSP2401,uiAM_I_M,uiAM_Q_M,10,dPowerSb) > 0)
						uiAM_Q_R = uiAM_Q_M;
					else
						uiAM_Q_L = uiAM_Q_M;
					if (dPowerSb < -90.0)
						break;
				}

				dThM_Min = dThM;
				dThL = dThM - 0.5;
				dThR = dThM + 0.5;
				for (dThM = dThL; dThM <= dThR; dThM += 0.1) {
					pSP2401->SetTxPhaseRotateI(dThM);
					Instrument.SA_SweepOnce();
					Instrument.SA_GetMarkerPwr(dPowerSA);
					if (dPowerSA < dPowerSb) {
						dPowerSb = dPowerSA;
						dThM_Min = dThM;
					}
				}
				dThM = dThM_Min;
				pSP2401->SetTxPhaseRotateI(dThM);

				for (int16_t iRetry = 0;iRetry < 2;iRetry ++) {
					dThM_Min = dThM;
					dThL = dThM - 0.2;
					dThR = dThM + 0.2;
					for (dThM = dThL; dThM <= dThR; dThM += 0.1) {
						pSP2401->SetTxPhaseRotateI(dThM);
						Instrument.SA_SweepOnce();
						Instrument.SA_GetMarkerPwr(dPowerSA);
						if (dPowerSA < dPowerSb) {
							dPowerSb = dPowerSA;
							dThM_Min = dThM;
						}
					}
					dThM = dThM_Min;
					pSP2401->SetTxPhaseRotateI(dThM);

					if (DerivativeAmI_SA(pSP2401,uiAM_I_M,uiAM_Q_M,1,dPowerSb) > 0) {
						while (-- uiAM_I_M) {
							if (DerivativeAmI_SA(pSP2401,uiAM_I_M,uiAM_Q_M,1,dPowerSb) < 0)
								break;
						}
					}
					else {
						while (++ uiAM_I_M) {
							if (DerivativeAmI_SA(pSP2401,uiAM_I_M,uiAM_Q_M,1,dPowerSb) > 0)
								break;
						}
					}

					if (DerivativeAmQ_SA(pSP2401,uiAM_I_M,uiAM_Q_M,1,dPowerSb) > 0) {
						while (-- uiAM_Q_M) {
							if (DerivativeAmQ_SA(pSP2401,uiAM_I_M,uiAM_Q_M,1,dPowerSb) < 0)
								break;
						}
					}
					else {
						while (++ uiAM_Q_M) {
							if (DerivativeAmQ_SA(pSP2401,uiAM_I_M,uiAM_Q_M,1,dPowerSb) > 0)
								break;
						}
					}
					if (dPowerSb < -90.0)
						break;
				}

				Data.m_iIdx   = i;
				Data.m_uiFreq = uiFreq[i];
				Data.m_dTh    = dThM;
				Data.m_uiAmI  = uiAM_I_M;
				Data.m_uiAmQ  = uiAM_Q_M;
				pSP1401->GetCalFile()->Add(ICalFile::TxSideband,(TxSidebandTableR1A::DataF *)&Data);
				pParent->m_Data.at(i) = *(TxSidebandTableR1A::DataF *)(&Data);
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_SIDEBAND_SHOW_CALLING,0,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			Instrument.Close();
		}
		else {
			double dPowerSb[512] = {0.0};

			pSP1401->SetIOMode(Loop);
			pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
			pSP1401->SetRxAtt1(25.0);
			pSP1401->SetRxAtt2(10);
			pSP2401->SetDDC(-92640000.0);
			pSP2401->SetRxFilter2NB();

			pSP1401->SetPowerMeasSrc(CSP1401R1A::PowerMeasFreeRun,false);
			pSP1401->SetPowerMeasSamples(327680);

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				dThL = -5.0;
				dThR = 5.0;
				uiAM_I_L = 8192 - 100;uiAM_I_R = 8192 + 100;uiAM_I_M = 8192;
				uiAM_Q_L = 8192 - 100;uiAM_Q_R = 8192 + 100;uiAM_Q_M = 8192;

				pSP1401->SetRxFreq((uiFreq[i] < 120000000 ? 120000000 : uiFreq[i]) - 20000000);
				pSP1401->SetTxFreq(uiFreq[i] < 120000000 ? 120000000 : uiFreq[i]);
				pSP2401->SetTxPhaseRotateI(0.0);
				pSP2401->SetTxAmplitudeBalance(8192,8192);
				((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[i] < 120000000 ? 120000000 : uiFreq[i],iDCI,iDCQ);
				pSP2401->SetTxDCOffset(iDCI,iDCQ);
				Sleep(50);

				CAL_THREAD_TEST_CANCEL();
				dStepTh = 2.0;
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinThI_Rx(pSP1401,pSP2401,dStepTh,0.5,&dThL,&dThR,&dThM,dPowerSb);

				dStepTh = 0.5;
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinThI_Rx(pSP1401,pSP2401,dStepTh,0.4,&dThL,&dThR,&dThM,dPowerSb);

				CAL_THREAD_TEST_CANCEL();
				dStepTh = 0.1;
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinThI_Rx(pSP1401,pSP2401,dStepTh,1.0,&dThL,&dThR,&dThM,dPowerSb);

				CAL_THREAD_TEST_CANCEL();
				uiStepAm = 20;
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinAmI_Rx(pSP1401,pSP2401,uiStepAm,1,&uiAM_I_L,&uiAM_I_R,&uiAM_I_M,&uiAM_Q_M,dPowerSb);
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinAmQ_Rx(pSP1401,pSP2401,uiStepAm,1,&uiAM_Q_L,&uiAM_Q_R,&uiAM_I_M,&uiAM_Q_M,dPowerSb);

				uiStepAm = 5;
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinAmI_Rx(pSP1401,pSP2401,uiStepAm,1,&uiAM_I_L,&uiAM_I_R,&uiAM_I_M,&uiAM_Q_M,dPowerSb);
				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinAmQ_Rx(pSP1401,pSP2401,uiStepAm,1,&uiAM_Q_L,&uiAM_Q_R,&uiAM_I_M,&uiAM_Q_M,dPowerSb);

				memset(dPowerSb,0,sizeof(dPowerSb));
				GetMinThI_Rx(pSP1401,pSP2401,dStepTh,1.0,&dThL,&dThR,&dThM,dPowerSb);

				CAL_THREAD_TEST_CANCEL();
				uiStepAm = 1;
				for (int32_t iRetry = 0;iRetry < 3;iRetry ++) {
					CAL_THREAD_TEST_CANCEL();
					memset(dPowerSb,0,sizeof(dPowerSb));
					GetMinAmI_Rx(pSP1401,pSP2401,uiStepAm,2,&uiAM_I_L,&uiAM_I_R,&uiAM_I_M,&uiAM_Q_M,dPowerSb);
					memset(dPowerSb,0,sizeof(dPowerSb));
					GetMinAmQ_Rx(pSP1401,pSP2401,uiStepAm,2,&uiAM_Q_L,&uiAM_Q_R,&uiAM_I_M,&uiAM_Q_M,dPowerSb);
				}

				Data.m_iIdx   = i;
				Data.m_uiFreq = uiFreq[i];
				Data.m_dTh    = dThM;
				Data.m_uiAmI  = uiAM_I_M;
				Data.m_uiAmQ  = uiAM_Q_M;
				pSP1401->GetCalFile()->Add(ICalFile::TxSideband,(TxSidebandTableR1A::DataF *)&Data);
				pParent->m_Data.at(i) = *(TxSidebandTableR1A::DataF *)(&Data);
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_SIDEBAND_SHOW_CALLING,0,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
		}
		pSP1401->GetCalFile()->Write(ICalFile::TxSideband);
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}

double CCalR1ATxSideBandThread::DerivativeTh_SA(CSP2401R1A *pSP2401,double dTh,double dDelta,double &dPower)
{
	double dPowerSA[2] = {0.0,0.0};
	pSP2401->SetTxPhaseRotateI(dTh - dDelta);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[0]);

	pSP2401->SetTxPhaseRotateI(dTh);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[1]);
	dPower = dPowerSA[1];

	return ((dPowerSA[1] - dPowerSA[0]) / dDelta);
}

double CCalR1ATxSideBandThread::DerivativeAmI_SA(CSP2401R1A *pSP2401,uint16_t uiAmI,uint16_t uiAmQ,uint16_t uiDelta,double &dPower)
{
	double dPowerSA[2] = {0.0,0.0};

	pSP2401->SetTxAmplitudeBalance(uiAmI - uiDelta,uiAmQ);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[0]);

	pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[1]);
	dPower = dPowerSA[1];

	return ((dPowerSA[1] - dPowerSA[0]) / uiDelta);
}

double CCalR1ATxSideBandThread::DerivativeAmQ_SA(CSP2401R1A *pSP2401,uint16_t uiAmI,uint16_t uiAmQ,uint16_t uiDelta,double &dPower)
{
	double dPowerSA[2] = {0,0};

	pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ - uiDelta);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[0]);

	pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ);
	Instrument.SA_SweepOnce();
	Instrument.SA_GetMarkerPwr(dPowerSA[1]);
	dPower = dPowerSA[1];

	return ((dPowerSA[1] - dPowerSA[0]) / uiDelta);
}

void CCalR1ATxSideBandThread::GetMinThI_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,double dStep,double dLRCoef,double *pThL,double *pThR,double *pThM,double *pPower)
{
	double dPowerMin = (double)LONG_MAX;
	CSP1401R1A::PowerMeasState State = CSP1401R1A::IDLE;
	int16_t iDC = 0,iDCMin = 0;

	for (*pThM = *pThL;*pThM <= *pThR;*pThM += dStep) {
		pSP2401->SetTxPhaseRotateI(*pThM);
		MeasOnce_Rx(pSP1401,&pPower[iDC]);
		if (pPower[iDC] < dPowerMin) {
			dPowerMin = pPower[iDC];
			iDCMin = iDC;
		}
		iDC ++;
	}
	*pThM = *pThL + iDCMin * dStep;
	*pThL = *pThM - dStep * dLRCoef;
	*pThR = *pThM + dStep * dLRCoef;
}

void CCalR1ATxSideBandThread::GetMinAmI_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,uint16_t uiStep,uint16_t uiLRCoef,uint16_t *pAm_I_L,uint16_t *pAm_I_R,uint16_t *pAm_I_M,uint16_t *pAm_Q_M,double *pPower)
{
	double dPowerMin = (double)LONG_MAX;
	CSP1401R1A::PowerMeasState State = CSP1401R1A::IDLE;
	int16_t iDC = 0,iDCMin = 0;

	for (*pAm_I_M = *pAm_I_L;*pAm_I_M <= *pAm_I_R;*pAm_I_M += uiStep) {
		pSP2401->SetTxAmplitudeBalance(*pAm_I_M,*pAm_Q_M);
		MeasOnce_Rx(pSP1401,&pPower[iDC]);
		if (pPower[iDC] < dPowerMin) {
			dPowerMin = pPower[iDC];
			iDCMin = iDC;
		}
		iDC ++;
	}
	*pAm_I_M = *pAm_I_L + iDCMin * uiStep;
	*pAm_I_L = *pAm_I_M - uiStep * uiLRCoef;
	*pAm_I_R = *pAm_I_M + uiStep * uiLRCoef;
}

void CCalR1ATxSideBandThread::GetMinAmQ_Rx(CSP1401R1A *pSP1401,CSP2401R1A *pSP2401,uint16_t uiStep,uint16_t uiLRCoef,uint16_t *pAm_Q_L,uint16_t *pAm_Q_R,uint16_t *pAm_I_M,uint16_t *pAm_Q_M,double *pPower)
{
	double dPowerMin = (double)LONG_MAX;
	CSP1401R1A::PowerMeasState State = CSP1401R1A::IDLE;
	int16_t iDC = 0,iDCMin = 0;

	for (*pAm_Q_M = *pAm_Q_L;*pAm_Q_M <= *pAm_Q_R;*pAm_Q_M += uiStep) {
		pSP2401->SetTxAmplitudeBalance(*pAm_I_M,*pAm_Q_M);
		MeasOnce_Rx(pSP1401,&pPower[iDC]);
		if (pPower[iDC] < dPowerMin) {
			dPowerMin = pPower[iDC];
			iDCMin = iDC;
		}
		iDC ++;
	}
	*pAm_Q_M = *pAm_Q_L + iDCMin * uiStep;
	*pAm_Q_L = *pAm_Q_M - uiStep * uiLRCoef;
	*pAm_Q_R = *pAm_Q_M + uiStep * uiLRCoef;
}

void CCalR1ATxSideBandThread::MeasOnce_Rx(CSP1401R1A *pSP1401,double *pPowerAvg)
{
	double dPower[5] = {0.0};
	CSP1401R1A::PowerMeasState State = CSP1401R1A::RUNNING;

	for (int32_t iAvg = 0;iAvg < 3;iAvg ++) {
		pSP1401->PowerMeasAbort();
		while (State != CSP1401R1A::IDLE)
			pSP1401->GetPowerMeasState(State);
		pSP1401->PowerMeasStart();
		while (State != CSP1401R1A::DONE) {
			pSP1401->GetPowerMeasState(State);
			if (State == CSP1401R1A::TT)
				break;
		}
		pSP1401->GetPowerMeasPower(dPower[iAvg]);
		*pPowerAvg += dPower[iAvg];
	}
	*pPowerAvg /= 3.0;
}


IMPLEMENT_DYNCREATE(CExportR1ATxSideBandThread, CWinThread)

CExportR1ATxSideBandThread::CExportR1ATxSideBandThread()
{
}

CExportR1ATxSideBandThread::CExportR1ATxSideBandThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1ATxSideBandThread::~CExportR1ATxSideBandThread()
{
}

BOOL CExportR1ATxSideBandThread::InitInstance()
{
	return TRUE;
}

int CExportR1ATxSideBandThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1ATxSideBandThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1ATxSideBandThread::Run()
{
	CCalR1ATxSideBandDlg *pParent = (CCalR1ATxSideBandDlg *)m_pParent;

	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	CCalR1ATxSideBandThread::Result Data;
	Data.m_iIdx = 0;

	InitSinglePos("Exporting Tx Sideband",100);

	for (int32_t i = 0;i < iPts;i ++) {
		((CalFileR1A *)(pParent->m_pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq[i],Data.m_dTh,Data.m_uiAmI,Data.m_uiAmQ);
		Data.m_uiFreq = uiFreq[i];
		pParent->m_Data.at(Data.m_iIdx) = *(TxSidebandTableR1A::DataF *)(&Data);
		Data.m_iIdx ++;
	}
	SetPos(100,m_nThreadID);
	CAL_THREAD_ABOART();
	return 0;
}