#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxFilOffThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CTxFilOffThread, CCalBaseThread)

CCalR1CTxFilOffThread::CCalR1CTxFilOffThread()
{
}

CCalR1CTxFilOffThread::CCalR1CTxFilOffThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CTxFilOffThread::~CCalR1CTxFilOffThread()
{
}

BOOL CCalR1CTxFilOffThread::InitInstance()
{
	return TRUE;
}

int CCalR1CTxFilOffThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CTxFilOffThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CTxFilOffThread::Run()
{
	return CalTxFilOff();
}

int CCalR1CTxFilOffThread::CalTxFilOff()
{
#define POST_CLEAR pSP1401->GetCalFile()->SetBW(_160M);

	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Filter Offset"));
		CCalR1CTxFilOffDlg *pParent = (CCalR1CTxFilOffDlg *)m_pParent;
		CalIOMode Mode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;

		if (CalOP(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Spectrum"));
		}
		if (CalIO(Mode) && !CalOP(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("IO<===>Spectrum"));
		}

		Instrument.Init();
		Instrument.HasSA();
		Instrument.SA_Reset();
		Instrument.SA_SetSpan(20000.0);
		Instrument.SA_SetRBW(1000.0,true);
		Instrument.SA_SetVBW(1000.0,true);
		Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 20.0);

		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;

		double dPower[2] = {0.0,0.0};

		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(uiFreq,0,sizeof(uiFreq));
		memset(uiFreqStar,0,sizeof(uiFreqStar));
		memset(uiFreqStop,0,sizeof(uiFreqStop));

		Result Data;
		TxLOLeakageTableR1C::DataM	DataLOLeak;
		TxSidebandTableR1C::DataM	DataSideband;
		TxFilter_160M_TableR1C::DataM DataFilter_160;
		TxFilter_80M_TableR1C::DataM  DataFilter_80;
		double dCoefReal[TX_FILTER_ORDER_2I] = {0.0};
		double dCoefImag[TX_FILTER_ORDER_2I] = {0.0};
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);

		InitSinglePos("Calibrating Tx Filter Offset",iPts);

		pSP1401->GetCalFile()->SetBW(_80M);
		pSP1401->GetCalFile()->Map2Buf(ICalFile::TxFil_160);
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Map2Mem();

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
			
			TxPowerOPTableR1C::DataM DataPower;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);

				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.SA_SetCF((double)(uiFreq[i] + 20000000));

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerOP->Get(uiFreq[i],R1C_TX_BASE_POWER_OP,&DataPower);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq[i],&DataFilter_160);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_80M->Get(uiFreq[i],&DataFilter_80);
				DataFilter_160._2Double(dCoefReal,dCoefImag);

				pSP1401->SetTxAtt0(DataPower.m_sAtt0 / 2.0);
				pSP1401->SetTxAtt1(DataPower.m_sAtt1 / 2.0);
				pSP1401->SetTxAtt2(DataPower.m_sAtt2 / 2.0);
				pSP1401->SetTxAtt3(DataPower.m_sAtt3 / 2.0);
				pSP2401->SetTxPowerComp((double)(DataPower.m_fDGain));
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				Sleep(5);

				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPower[0]);

				DataFilter_80._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				Sleep(5);

				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPower[1]);

				Data.m_uiFreq = uiFreq[i];
				Data.m_dOffset = dPower[0] - dPower[1];
				pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
				pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
				pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				pParent->m_TabCtrl.m_DataOP_80.at(i) = *(TxFilterOffTableR1C::DataF *)(&Data);
				pSP1401->GetCalFile()->Add(ICalFile::TxFilOffOP_80,(TxFilterOffTableR1C::DataF *)&Data);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_OFF_80_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::TxFilOffOP_80);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilOffOP_80M->SaveAs("c:\\tx_filter_off_op_80.txt");
		}
		
		if (CalOP(Mode) && CalIO(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Output Complete,IO<===>Spectrum"));
		}

		if (CalIO(Mode)) {
			Instrument.SA_Reset();
			Instrument.SA_SetSpan(20000.0);
			Instrument.SA_SetRBW(1000.0,true);
			Instrument.SA_SetVBW(1000.0,true);
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_IO + 20.0);
			CallingMode = CAL_IO;
			pSP1401->SetIOMode(IO);

			TxPowerIOTableR1C::DataM DataPower;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);

				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.SA_SetCF((double)(uiFreq[i] + 20000000));

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerIO->Get(uiFreq[i],R1C_TX_BASE_POWER_OP,&DataPower);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq[i],&DataFilter_160);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_80M->Get(uiFreq[i],&DataFilter_80);
				DataFilter_160._2Double(dCoefReal,dCoefImag);

				pSP1401->SetTxAtt0(DataPower.m_sAtt0 / 2.0);
				pSP1401->SetTxAtt1(DataPower.m_sAtt1 / 2.0);
				pSP1401->SetTxAtt2(DataPower.m_sAtt2 / 2.0);
				pSP1401->SetTxAtt3(DataPower.m_sAtt3 / 2.0);
				pSP2401->SetTxPowerComp((double)(DataPower.m_fDGain));
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				Sleep(5);

				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPower[0]);

				DataFilter_80._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				Sleep(5);

				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPower[1]);

				Data.m_uiFreq = uiFreq[i];
				Data.m_dOffset = dPower[0] - dPower[1];
				pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
				pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
				pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				pParent->m_TabCtrl.m_DataIO_80.at(i) = *(TxFilterOffTableR1C::DataF *)(&Data);
				pSP1401->GetCalFile()->Add(ICalFile::TxFilOffIO_80,(TxFilterOffTableR1C::DataF *)&Data);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_OFF_80_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::TxFilOffIO_80);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilOffIO_80M->SaveAs("c:\\tx_filter_off_io_80.txt");
		}
		CAL_THREAD_ABOART_S(POST_CLEAR);
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}


IMPLEMENT_DYNCREATE(CExportR1CTxFilOffThread, CWinThread)

CExportR1CTxFilOffThread::CExportR1CTxFilOffThread()
{
}

CExportR1CTxFilOffThread::CExportR1CTxFilOffThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CTxFilOffThread::~CExportR1CTxFilOffThread()
{
}

BOOL CExportR1CTxFilOffThread::InitInstance()
{
	return TRUE;
}

int CExportR1CTxFilOffThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CTxFilOffThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CTxFilOffThread::Run()
{
	CCalR1CTxFilOffDlg *pParent = (CCalR1CTxFilOffDlg *)m_pParent;
	CalIOMode Mode = CAL_OP;

	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	InitSinglePos("Exporting Tx Filter Offset",iPts);
	
	CCalR1CTxFilOffThread::Result Data;
	Data.m_iIdx = 0;
	Data.m_iSec = 0;

	pParent->m_pSP1401->GetCalFile()->SetBW(_80M);
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxFilOffOP_80);
	for (int32_t i = 0;i < iPts;i ++) {
		Data.m_iIdx  = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxFilOffOP_80M->Get(uiFreq[i],(TxFilterOffTableR1C::DataF *)&Data);
		pParent->m_TabCtrl.m_DataOP_80.at(Data.m_iIdx) = *(TxFilterOffTableR1C::DataF *)(&Data);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_OFF_80_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
		SetPos(i + 1,m_nThreadID);
	}

	Mode = CAL_IO;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxFilOffIO_80);
	for (int32_t i = 0;i < iPts;i ++) {
		Data.m_iIdx = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxFilOffIO_80M->Get(uiFreq[i],(TxFilterOffTableR1C::DataF *)&Data);
		pParent->m_TabCtrl.m_DataIO_80.at(Data.m_iIdx) = *(TxFilterOffTableR1C::DataF *)(&Data);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_OFF_80_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
		SetPos(i + 1,m_nThreadID);
	}
	pParent->m_pSP1401->GetCalFile()->SetBW(_160M);
	CAL_THREAD_ABOART();
	return 0;
}