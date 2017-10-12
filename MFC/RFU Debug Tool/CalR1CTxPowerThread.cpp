#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxPowerThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CTxPowerThread, CCalBaseThread)

CCalR1CTxPowerThread::CCalR1CTxPowerThread()
{
}

CCalR1CTxPowerThread::CCalR1CTxPowerThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CTxPowerThread::~CCalR1CTxPowerThread()
{
}

BOOL CCalR1CTxPowerThread::InitInstance()
{
	return TRUE;
}

int CCalR1CTxPowerThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CTxPowerThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CTxPowerThread::Run()
{
	return CalTxPower();
}

int CCalR1CTxPowerThread::CalTxPower()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Power"));
		CCalR1CTxPowerDlg *pParent = (CCalR1CTxPowerDlg *)m_pParent;
		CalIOMode Mode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;

		if (CalOP(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Power Meter"));
		}
		if (CalIO(Mode) && !CalOP(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("IO<===>Power Meter"));
		}

		Instrument.Init();
		Instrument.HasPM();
		Instrument.PM_Reset();

		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		double dAtt0 = 0.0,dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0;
		double dPMPower = -100.0,dPowerOffset = 0.0;
		double dDGain = -5.0;
		int32_t iAjust = 0;

		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(uiFreq,0,sizeof(uiFreq));
		memset(uiFreqStar,0,sizeof(uiFreqStar));
		memset(uiFreqStop,0,sizeof(uiFreqStop));

		TxLOLeakageTableR1C::DataM	DataLOLeak;
		TxSidebandTableR1C::DataM	DataSideband;
		TxFilter_160M_TableR1C::DataM DataFilter;
		double dCoefReal[TX_FILTER_ORDER_2I] = {0.0};
		double dCoefImag[TX_FILTER_ORDER_2I] = {0.0};
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);

		InitSinglePos("Calibrating Tx Power",iPts);

		pSP1401->GetCalFile()->SetBW(_160M);
		pSP1401->SetRxAtt019Sw(SP1401::Rx_Att_19);
		pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
		pSP1401->SetRxAtt(30.0,30.0,30.0);
		pSP2401->SetTxFilterSw(true);
		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetDUCDDS(0.0);

		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq[0],&DataSideband);
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[0],&DataLOLeak);
		pSP2401->SetTxPhaseRotateI((double)(DataSideband.m_fTh));
		pSP2401->SetTxAmplitudeBalance(DataSideband.m_uiAmI,DataSideband.m_uiAmQ);
		pSP2401->SetTxDCOffset((uint16_t)(DataLOLeak.m_iDCI),(uint16_t)(DataLOLeak.m_iDCQ));

		if (CalOP(Mode)) {
			CallingMode = CAL_OP;
			pSP1401->SetIOMode(OutPut);
			ResultOP Data;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.PM_SetFrequency((double)(uiFreq[i] + 20000000));

				dDGain = -5.0;
				TxPowerOPTableR1C::GuessBase(dAtt0,dAtt1,dAtt2,dAtt3,dDGain);
				pSP2401->SetTxPowerComp(dDGain);
				pSP1401->SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3);
				Sleep(5);

				Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
				dPowerOffset = R1C_TX_BASE_POWER_OP - dPMPower;

				iAjust = 0;
				while (abs(dPowerOffset) > 0.5) {
					if (dPowerOffset > dAtt0) {		//even if dAtt0 == 0.0,this is still correct
						dAtt1 -= (dPowerOffset - dAtt0);
						dAtt0 = 0.0;
						dAtt1 = dAtt1 < 0.0 ? 0.0 : dAtt1;
					}
					else {
						dAtt0 -= dPowerOffset;
					}
					dAtt0 = dAtt0 > 20.0 ? 20.0 : dAtt0;
					hmc624(&dAtt0);
					hmc624(&dAtt1);
					pSP1401->SetTxAtt0(dAtt0);
					pSP1401->SetTxAtt1(dAtt1);
					Sleep(5);

					Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
					dPowerOffset = R1C_TX_BASE_POWER_OP - dPMPower;

					if (++iAjust > 10)
						break;
				};
				iAjust = 0;
				while (abs(dPowerOffset) > 0.1) {
					dDGain += dPowerOffset;
					pSP2401->SetTxPowerComp(dDGain);

					Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
					dPowerOffset = R1C_TX_BASE_POWER_OP - dPMPower;

					if (++iAjust > 10)
						break;
				}
				Data.m_uiFreq = uiFreq[i];
				Data.m_dDGain = dDGain;
				Data.m_dAtt0  = dAtt0;
				Data.m_dAtt1  = dAtt1;
				Data.m_dAtt2  = dAtt2;
				Data.m_dAtt3  = dAtt3;
				pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
				pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
				pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				pParent->m_DataOP.at(i) = *(TxPowerOPTableR1C::DataF *)(&Data);
				pSP1401->GetCalFile()->Add(ICalFile::TxPowerOP,(TxPowerOPTableR1C::DataF *)&Data);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::TxPowerOP);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerOP->SaveAs("c:\\tx_power_op.txt");
		}
		
		if (CalOP(Mode) && CalIO(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Output Complete,IO<===>Power Meter"));
		}

		if (CalIO(Mode)) {
			Instrument.PM_Reset();
			CallingMode = CAL_IO;
			pSP1401->SetIOMode(IO);
			ResultIO Data;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.PM_SetFrequency((double)(uiFreq[i] + 20000000));

				dDGain = -5.0;
				TxPowerIOTableR1C::GuessBase(dAtt0,dAtt1,dAtt2,dAtt3,dDGain);
				pSP2401->SetTxPowerComp(dDGain);
				pSP1401->SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3);
				Sleep(5);

				Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
				dPowerOffset = R1C_TX_BASE_POWER_IO - dPMPower;

				iAjust = 0;
				while (abs(dPowerOffset) > 0.5) {
					if (dPowerOffset > dAtt0) {		//even if dAtt0 == 0.0,this is still correct
						dAtt1 -= (dPowerOffset - dAtt0);
						dAtt0 = 0.0;
						dAtt1 = dAtt1 < 0.0 ? 0.0 : dAtt1;
					}
					else {
						dAtt0 -= dPowerOffset;
					}
					dAtt0 = dAtt0 > 20.0 ? 20.0 : dAtt0;
					hmc624(&dAtt0);
					hmc624(&dAtt1);
					pSP1401->SetTxAtt0(dAtt0);
					pSP1401->SetTxAtt1(dAtt1);
					Sleep(5);

					Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
					dPowerOffset = R1C_TX_BASE_POWER_IO - dPMPower;

					if (++iAjust > 10)
						break;
				};
				iAjust = 0;
				while (abs(dPowerOffset) > 0.1) {
					dDGain += dPowerOffset;
					pSP2401->SetTxPowerComp(dDGain);

					Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
					dPowerOffset = R1C_TX_BASE_POWER_IO - dPMPower;

					if (++iAjust > 10)
						break;
				}
				Data.m_uiFreq = uiFreq[i];
				Data.m_dDGain = dDGain;
				Data.m_dAtt0  = dAtt0;
				Data.m_dAtt1  = dAtt1;
				Data.m_dAtt2  = dAtt2;
				Data.m_dAtt3  = dAtt3;
				pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
				pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
				pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				pParent->m_DataIO.at(i) = *(TxPowerIOTableR1C::DataF *)(&Data);
				pSP1401->GetCalFile()->Add(ICalFile::TxPowerIO,(TxPowerIOTableR1C::DataF *)&Data);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::TxPowerIO);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerIO->SaveAs("c:\\tx_power_io.txt");
		}
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}


IMPLEMENT_DYNCREATE(CExportR1CTxPowerThread, CWinThread)

CExportR1CTxPowerThread::CExportR1CTxPowerThread()
{
}

CExportR1CTxPowerThread::CExportR1CTxPowerThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CTxPowerThread::~CExportR1CTxPowerThread()
{
}

BOOL CExportR1CTxPowerThread::InitInstance()
{
	return TRUE;
}

int CExportR1CTxPowerThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CTxPowerThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CTxPowerThread::Run()
{
	CCalR1CTxPowerDlg *pParent = (CCalR1CTxPowerDlg *)m_pParent;
	CalIOMode Mode = CAL_OP;

	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	InitSinglePos("Exporting Tx Power",iPts);
	
	CCalR1CTxPowerThread::ResultOP DataOP;
	CCalR1CTxPowerThread::ResultIO DataIO;
	DataOP.m_iIdx = 0;
	DataOP.m_iSec = 0;
	DataIO.m_iIdx = 0;
	DataIO.m_iSec = 0;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxPowerOP);
	for (int32_t i = 0;i < iPts;i ++) {
		DataOP.m_iIdx  = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxPowerOP->GetBase(uiFreq[i],(TxPowerOPTableR1C::DataF *)&DataOP);
		pParent->m_DataOP.at(DataOP.m_iIdx) = *(TxPowerOPTableR1C::DataF *)(&DataOP);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&Mode,(LPARAM)&DataOP);
		SetPos(i + 1,m_nThreadID);
	}

	Mode = CAL_IO;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxPowerIO);
	for (int32_t i = 0;i < iPts;i ++) {
		DataIO.m_iIdx = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxPowerIO->GetBase(uiFreq[i],(TxPowerIOTableR1C::DataF *)&DataIO);
		pParent->m_DataIO.at(DataIO.m_iIdx) = *(TxPowerIOTableR1C::DataF *)(&DataIO);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&Mode,(LPARAM)&DataIO);
		SetPos(i + 1,m_nThreadID);
	}
	CAL_THREAD_ABOART();
	return 0;
}