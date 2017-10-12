#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxAttThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CTxAttThread, CCalBaseThread)

CCalR1CTxAttThread::CCalR1CTxAttThread()
{
}

CCalR1CTxAttThread::CCalR1CTxAttThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CTxAttThread::~CCalR1CTxAttThread()
{
}

BOOL CCalR1CTxAttThread::InitInstance()
{
	return TRUE;
}

int CCalR1CTxAttThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CTxAttThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CTxAttThread::Run()
{
	return CalTxAtt();
}

int CCalR1CTxAttThread::CalTxAtt()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Att"));
		CCalR1CTxAttDlg *pParent = (CCalR1CTxAttDlg *)m_pParent;
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

		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		double dAtt0 = 0.0,dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0;
		double dPowerSA[2] = {0.0,0.0};
		double dPower = R1C_TX_BASE_POWER_OP;
		int32_t iIdxOffset = 0,iIdxOffsetStar = 0;

		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(uiFreq,0,sizeof(uiFreq));
		memset(uiFreqStar,0,sizeof(uiFreqStar));
		memset(uiFreqStop,0,sizeof(uiFreqStop));

		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);

		InitSinglePos("Calibrating Tx Att",iPts);

		TxLOLeakageTableR1C::DataM DataLOLeak;
		TxSidebandTableR1C::DataM DataSideband;
		TxFilter_160M_TableR1C::DataM DataFilter;
		double dCoefReal[TX_FILTER_ORDER_2I] = {0.0};
		double dCoefImag[TX_FILTER_ORDER_2I] = {0.0};
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[0],&DataLOLeak);
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq[0],&DataSideband);
		pSP2401->SetTxDCOffset((uint16_t)(DataLOLeak.m_iDCI),(uint16_t)(DataLOLeak.m_iDCQ));
		pSP2401->SetTxPhaseRotateI((double)(DataSideband.m_fTh));
		pSP2401->SetTxAmplitudeBalance(DataSideband.m_uiAmI,DataSideband.m_uiAmQ);
		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetDUCDDS(0.0);
		pSP2401->SetTxFilterSw(true);
		pSP1401->SetRxAtt019Sw(SP1401::Rx_Att_19);
		pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
		pSP1401->SetRxAtt(30.0,30.0,30.0);
		
		if (CalOP(Mode)) {
			CallingMode = CAL_OP;
			pSP1401->SetIOMode(OutPut);
			ResultOP Data;
			TxPowerOPTableR1C::DataM DataPowerOP;

			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 25.0);		//10 ~ -5
			Instrument.SA_SetEnablePreamp(false);
			Instrument.SA_SetMechAtten(true);
			Instrument.SA_SetAverTrace(false,0);
			
			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.SA_SetCF(uiFreq[i] + 20000000.0);

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerOP->Get(uiFreq[i],R1C_TX_BASE_POWER_OP,&DataPowerOP);
				pSP1401->SetTxAtt0(DataPowerOP.m_sAtt0 / 2.0);
				pSP1401->SetTxAtt1(DataPowerOP.m_sAtt1 / 2.0);
				pSP1401->SetTxAtt2(DataPowerOP.m_sAtt2 / 2.0);
				pSP1401->SetTxAtt3(DataPowerOP.m_sAtt3 / 2.0);
				pSP2401->SetTxPowerComp(DataPowerOP.m_fDGain);
				Sleep(2);

				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);

				iIdxOffset = iIdxOffsetStar;
				for (dPower = R1C_TX_ATT_OP_POWER_STAR;dPower > R1C_TX_BASE_POWER_OP;dPower --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerOP->Get(uiFreq[i],dPower,&DataPowerOP);
					pSP1401->SetTxAtt0(DataPowerOP.m_sAtt0 / 2.0);
					pSP1401->SetTxAtt1(DataPowerOP.m_sAtt1 / 2.0);
					pSP1401->SetTxAtt2(DataPowerOP.m_sAtt2 / 2.0);
					pSP1401->SetTxAtt3(DataPowerOP.m_sAtt3 / 2.0);
					pSP2401->SetTxPowerComp(DataPowerOP.m_fDGain);
					Sleep(2);

					Instrument.SA_SweepOnce();
					Instrument.SA_GetMarkerPwr(dPowerSA[1]);
					Data.m_fOffset[iIdxOffset] = (float)((dPower - R1C_TX_BASE_POWER_OP) - (dPowerSA[1] - dPowerSA[0]));
					iIdxOffset ++;
				}
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				Data.m_iIdxOffsetStar = iIdxOffsetStar;
				Data.m_iIdxOffsetStop = iIdxOffset;
				Data.m_uiFreq = uiFreq[i];
				pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
				pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
				pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				pParent->m_TabCtrl.m_DataOP.at(i) = *(TxAttOPTableR1C::DataF *)&Data;
				pSP1401->GetCalFile()->Add(ICalFile::TxAttOP,(TxAttOPTableR1C::DataF *)&Data);
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			iIdxOffsetStar = iIdxOffset;
			pSP1401->GetCalFile()->Write(ICalFile::TxAttOP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 20.0);
			Sleep(10);
			CalOneSegOP(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_OP,R1C_TX_BASE_POWER_OP - 20.0,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 20.0 - 20.0);
			Sleep(10);
			CalOneSegOP(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 1,R1C_TX_BASE_POWER_OP - 20.0 * 2,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 20.0 - 20.0 * 2);
			CalOneSegOP(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 2,R1C_TX_BASE_POWER_OP - 20.0 * 3,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 20.0 - 20.0 * 3);
			Instrument.SA_SetEnablePreamp(true);
			Instrument.SA_SetMechAtten(false,0);
			Instrument.SA_SetAverTrace(true,3);
			Sleep(10);
			CalOneSegOP(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 3,R1C_TX_BASE_POWER_OP - 20.0 * 4,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_OP + 20.0 - 20.0 * 4);
			Instrument.SA_SetAverTrace(true,6);
			Sleep(10);
			CalOneSegOP(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_OP - 20.0 * 4,R1C_TX_ATT_OP_POWER_STOP,R1C_TX_ATT_STEP);
		}
		
		if (CalOP(Mode) && CalIO(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Output Complete,IO<===>Sepctrum"));
		}

		if (CalIO(Mode)) {
			Instrument.SA_Reset();
			Instrument.SA_SetSpan(20000.0);
			Instrument.SA_SetRBW(1000.0,true);
			Instrument.SA_SetVBW(1000.0,true);
			Instrument.SA_SetEnablePreamp(false);
			Instrument.SA_SetMechAtten(true);
			Instrument.SA_SetAverTrace(false,0);
			CallingMode = CAL_IO;
			pSP1401->SetIOMode(IO);
			ResultIO Data;
			TxPowerIOTableR1C::DataM DataPowerIO;
			iIdxOffset = iIdxOffsetStar = 0;

			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_IO + 25.0);		//0 ~ -12
			Instrument.SA_SetEnablePreamp(false);
			Instrument.SA_SetMechAtten(true);
			Instrument.SA_SetAverTrace(false,0);

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetTxFilter(dCoefReal,dCoefImag);
				pSP1401->SetTxFreq(uiFreq[i]);
				Instrument.SA_SetCF(uiFreq[i] + 20000000.0);

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerIO->Get(uiFreq[i],R1C_TX_BASE_POWER_IO,&DataPowerIO);
				pSP1401->SetTxAtt0(DataPowerIO.m_sAtt0 / 2.0);
				pSP1401->SetTxAtt1(DataPowerIO.m_sAtt1 / 2.0);
				pSP1401->SetTxAtt2(DataPowerIO.m_sAtt2 / 2.0);
				pSP1401->SetTxAtt3(DataPowerIO.m_sAtt3 / 2.0);
				pSP2401->SetTxPowerComp(DataPowerIO.m_fDGain);
				Sleep(2);

				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);

				iIdxOffset = iIdxOffsetStar;
				for (dPower = R1C_TX_ATT_IO_POWER_STAR;dPower > R1C_TX_BASE_POWER_IO;dPower --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerIO->Get(uiFreq[i],dPower,&DataPowerIO);
					pSP1401->SetTxAtt0(DataPowerIO.m_sAtt0 / 2.0);
					pSP1401->SetTxAtt1(DataPowerIO.m_sAtt1 / 2.0);
					pSP1401->SetTxAtt2(DataPowerIO.m_sAtt2 / 2.0);
					pSP1401->SetTxAtt3(DataPowerIO.m_sAtt3 / 2.0);
					pSP2401->SetTxPowerComp(DataPowerIO.m_fDGain);
					Sleep(2);

					Instrument.SA_SweepOnce();
					Instrument.SA_GetMarkerPwr(dPowerSA[1]);
					Data.m_fOffset[iIdxOffset] = (float)((dPower - R1C_TX_BASE_POWER_IO) - (dPowerSA[1] - dPowerSA[0]));
					iIdxOffset ++;
				}
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				Data.m_iIdxOffsetStar = iIdxOffsetStar;
				Data.m_iIdxOffsetStop = iIdxOffset;
				Data.m_uiFreq = uiFreq[i];
				pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
				pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
				pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				pParent->m_TabCtrl.m_DataIO.at(i) = *(TxAttIOTableR1C::DataF *)&Data;
				pSP1401->GetCalFile()->Add(ICalFile::TxAttIO,(TxAttIOTableR1C::DataF *)&Data);
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			iIdxOffsetStar = iIdxOffset;
			pSP1401->GetCalFile()->Write(ICalFile::TxAttIO);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_IO + 20.0);
			Sleep(10);
			CalOneSegIO(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_IO,R1C_TX_BASE_POWER_IO - 20.0,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_IO + 20.0 - 20.0);
			Sleep(10);
			CalOneSegIO(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_IO - 20.0 * 1,R1C_TX_BASE_POWER_IO - 20.0 * 2,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_IO + 20.0 - 20.0 * 2);
			Instrument.SA_SetEnablePreamp(true);
			Instrument.SA_SetMechAtten(false,0);
			Instrument.SA_SetAverTrace(true,3);
			Sleep(10);
			CalOneSegIO(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_IO - 20.0 * 2,R1C_TX_BASE_POWER_IO - 20.0 * 3,R1C_TX_ATT_STEP);

			CAL_THREAD_TEST_CANCEL();
			Instrument.SA_SetRefLevel(R1C_TX_BASE_POWER_IO + 20.0 - 20.0 * 3);
			Instrument.SA_SetAverTrace(true,6);
			Sleep(10);
			CalOneSegIO(CallingMode,pSP1401,pSP2401,iPts,uiFreq,uiFreqStar,uiFreqStop,&iIdxOffsetStar,R1C_TX_BASE_POWER_IO - 20.0 * 3,R1C_TX_ATT_IO_POWER_STOP,R1C_TX_ATT_STEP);
		}
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}

int CCalR1CTxAttThread::CalOneSegOP(CalIOMode CallingMode,CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int32_t iPts,uint64_t *pFreq,uint64_t *pFreqStar,uint64_t *pFreqStop,int32_t *pIdxOffsetStar,double dPowerStar,double dPowerStop,double dPowerStep)
{
	TxFilter_160M_TableR1C::DataM DataFilter;
	double dCoefReal[TX_FILTER_ORDER_2I] = {0.0};
	double dCoefImag[TX_FILTER_ORDER_2I] = {0.0};
	ResultOP Data;
	TxPowerOPTableR1C::DataM DataPowerOP;
	double dPowerSA[2] = {0.0,0.0};
	double dPower = 0.0;
	int32_t iIdxOffset = 0;
	float fOffsetBase = 0.0;

	for (int32_t i = 0;i < iPts;i ++) {
		CAL_THREAD_TEST_CANCEL();
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(pFreq[i],&DataFilter);
		DataFilter._2Double(dCoefReal,dCoefImag);
		pSP2401->SetTxFilter(dCoefReal,dCoefImag);
		pSP1401->SetTxFreq(pFreq[i]);
		Instrument.SA_SetCF(pFreq[i] + 20000000.0);

		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxAttOP->Get(pFreq[i],&Data);
		fOffsetBase = R1C_TX_BASE_POWER_OP == dPowerStar ? 0.0f : Data.m_fOffset[*pIdxOffsetStar - 1];
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerOP->Get(pFreq[i],dPowerStar,&DataPowerOP);
		pSP1401->SetTxAtt0(DataPowerOP.m_sAtt0 / 2.0);
		pSP1401->SetTxAtt1(DataPowerOP.m_sAtt1 / 2.0);
		pSP1401->SetTxAtt2(DataPowerOP.m_sAtt2 / 2.0);
		pSP1401->SetTxAtt3(DataPowerOP.m_sAtt3 / 2.0);
		pSP2401->SetTxPowerComp(DataPowerOP.m_fDGain);
		Sleep(2);

		Instrument.SA_SweepOnce();
		Instrument.SA_SetPeakSearch(ISA::PeakHighest);
		Instrument.SA_GetMarkerPwr(dPowerSA[0]);

		iIdxOffset = *pIdxOffsetStar;
		for (dPower = dPowerStar + dPowerStep;dPower >= dPowerStop;dPower += dPowerStep) {
			CAL_THREAD_TEST_CANCEL();
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerOP->Get(pFreq[i],dPower,&DataPowerOP);
			pSP1401->SetTxAtt0(DataPowerOP.m_sAtt0 / 2.0);
			pSP1401->SetTxAtt1(DataPowerOP.m_sAtt1 / 2.0);
			pSP1401->SetTxAtt2(DataPowerOP.m_sAtt2 / 2.0);
			pSP1401->SetTxAtt3(DataPowerOP.m_sAtt3 / 2.0);
			pSP2401->SetTxPowerComp(DataPowerOP.m_fDGain);
			Sleep(2);

			Instrument.SA_SweepOnce();
			Instrument.SA_GetMarkerPwr(dPowerSA[1]);
			Data.m_fOffset[iIdxOffset] = (float)((dPower - dPowerStar) - (dPowerSA[1] - dPowerSA[0]));
			Data.m_fOffset[iIdxOffset] += fOffsetBase;
			iIdxOffset ++;
		}
		Data.m_iIdx = i;
		Data.m_iSec = freqsection((int64_t *)pFreqStar,(int64_t *)pFreqStop,(int64_t)pFreq[i]);
		Data.m_iIdxOffsetStar = *pIdxOffsetStar;
		Data.m_iIdxOffsetStop = iIdxOffset;
		Data.m_uiFreq = pFreq[i];
		pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
		pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
		pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
		pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
		time_t Time;
		time(&Time);
		Data.m_EndTime = *localtime(&Time);
		((CCalR1CTxAttDlg *)m_pParent)->m_TabCtrl.m_DataOP.at(i) = *(TxAttOPTableR1C::DataF *)&Data;
		pSP1401->GetCalFile()->Add(ICalFile::TxAttOP,(TxAttOPTableR1C::DataF *)&Data);
		SetPos(i + 1,m_nThreadID);
		::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
	}
	*pIdxOffsetStar = iIdxOffset;
	pSP1401->GetCalFile()->Write(ICalFile::TxAttOP);
	return 0;
}

int CCalR1CTxAttThread::CalOneSegIO(CalIOMode CallingMode,CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,int32_t iPts,uint64_t *pFreq,uint64_t *pFreqStar,uint64_t *pFreqStop,int32_t *pIdxOffsetStar,double dPowerStar,double dPowerStop,double dPowerStep)
{
	TxFilter_160M_TableR1C::DataM DataFilter;
	double dCoefReal[TX_FILTER_ORDER_2I] = {0.0};
	double dCoefImag[TX_FILTER_ORDER_2I] = {0.0};
	ResultIO Data;
	TxPowerIOTableR1C::DataM DataPowerIO;
	double dPowerSA[2] = {0.0,0.0};
	double dPower = 0.0;
	int32_t iIdxOffset = 0;
	float fOffsetBase = 0.0;

	for (int32_t i = 0;i < iPts;i ++) {
		CAL_THREAD_TEST_CANCEL();
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(pFreq[i],&DataFilter);
		DataFilter._2Double(dCoefReal,dCoefImag);
		pSP2401->SetTxFilter(dCoefReal,dCoefImag);
		pSP1401->SetTxFreq(pFreq[i]);
		Instrument.SA_SetCF(pFreq[i] + 20000000.0);

		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxAttIO->Get(pFreq[i],&Data);
		fOffsetBase = R1C_TX_BASE_POWER_IO == dPowerStar ? 0.0f : Data.m_fOffset[*pIdxOffsetStar - 1];
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerIO->Get(pFreq[i],dPowerStar,&DataPowerIO);
		pSP1401->SetTxAtt0(DataPowerIO.m_sAtt0 / 2.0);
		pSP1401->SetTxAtt1(DataPowerIO.m_sAtt1 / 2.0);
		pSP1401->SetTxAtt2(DataPowerIO.m_sAtt2 / 2.0);
		pSP1401->SetTxAtt3(DataPowerIO.m_sAtt3 / 2.0);
		pSP2401->SetTxPowerComp(DataPowerIO.m_fDGain);
		Sleep(2);

		Instrument.SA_SweepOnce();
		Instrument.SA_SetPeakSearch(ISA::PeakHighest);
		Instrument.SA_GetMarkerPwr(dPowerSA[0]);

		iIdxOffset = *pIdxOffsetStar;
		for (dPower = dPowerStar + dPowerStep;dPower >= dPowerStop;dPower += dPowerStep) {
			CAL_THREAD_TEST_CANCEL();
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxPowerIO->Get(pFreq[i],dPower,&DataPowerIO);
			pSP1401->SetTxAtt0(DataPowerIO.m_sAtt0 / 2.0);
			pSP1401->SetTxAtt1(DataPowerIO.m_sAtt1 / 2.0);
			pSP1401->SetTxAtt2(DataPowerIO.m_sAtt2 / 2.0);
			pSP1401->SetTxAtt3(DataPowerIO.m_sAtt3 / 2.0);
			pSP2401->SetTxPowerComp(DataPowerIO.m_fDGain);
			Sleep(2);

			Instrument.SA_SweepOnce();
			Instrument.SA_GetMarkerPwr(dPowerSA[1]);
			Data.m_fOffset[iIdxOffset] = (float)((dPower - dPowerStar) - (dPowerSA[1] - dPowerSA[0]));
			Data.m_fOffset[iIdxOffset] += fOffsetBase;
			iIdxOffset ++;
		}
		Data.m_iIdx = i;
		Data.m_iSec = freqsection((int64_t *)pFreqStar,(int64_t *)pFreqStop,(int64_t)pFreq[i]);
		Data.m_iIdxOffsetStar = *pIdxOffsetStar;
		Data.m_iIdxOffsetStop = iIdxOffset;
		Data.m_uiFreq = pFreq[i];
		pSP1401->GetTemp4TxLO2(&Data.m_dTemp[0]);
		pSP1401->GetTemp5TxLO1(&Data.m_dTemp[1]);
		pSP1401->GetTemp6TxPA4(&Data.m_dTemp[2]);
		pSP1401->GetTemp7TxPA3(&Data.m_dTemp[3]);
		time_t Time;
		time(&Time);
		Data.m_EndTime = *localtime(&Time);
		((CCalR1CTxAttDlg *)m_pParent)->m_TabCtrl.m_DataIO.at(i) = *(TxAttIOTableR1C::DataF *)&Data;
		pSP1401->GetCalFile()->Add(ICalFile::TxAttIO,(TxAttIOTableR1C::DataF *)&Data);
		SetPos(i + 1,m_nThreadID);
		::PostMessage(m_pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
	}
	*pIdxOffsetStar = iIdxOffset;
	pSP1401->GetCalFile()->Write(ICalFile::TxAttIO);
	return 0;
}


IMPLEMENT_DYNCREATE(CExportR1CTxAttThread, CWinThread)

CExportR1CTxAttThread::CExportR1CTxAttThread()
{
}

CExportR1CTxAttThread::CExportR1CTxAttThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CTxAttThread::~CExportR1CTxAttThread()
{
}

BOOL CExportR1CTxAttThread::InitInstance()
{
	return TRUE;
}

int CExportR1CTxAttThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CTxAttThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CTxAttThread::Run()
{
	CCalR1CTxAttDlg *pParent = (CCalR1CTxAttDlg *)m_pParent;
 	CalIOMode Mode = CAL_OP;
 	
 	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
 	memset(uiFreq,0,sizeof(uiFreq));
 	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);
 	
 	InitSinglePos("Exporting Tx Att",iPts);
 		
 	CCalR1CTxAttThread::ResultOP DataOP;
 	CCalR1CTxAttThread::ResultIO DataIO;
 	DataOP.m_iIdx = 0;
 	DataOP.m_iSec = 0;
	DataOP.m_iIdxOffsetStar = 0;
	DataOP.m_iIdxOffsetStop = ARRAY_SIZE(DataOP.m_fOffset);
 	DataIO.m_iIdx = 0;
 	DataIO.m_iSec = 0;
	DataIO.m_iIdxOffsetStar = 0;
	DataIO.m_iIdxOffsetStop = ARRAY_SIZE(DataIO.m_fOffset);
 	
 	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxAttOP);
	for (int32_t i = 0;i < iPts;i ++) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxAttOP->Get(uiFreq[i],(TxAttOPTableR1C::DataF *)&DataOP);
		pParent->m_TabCtrl.m_DataOP.at(DataOP.m_iIdx) = *(TxAttOPTableR1C::DataF *)(&DataOP);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&Mode,(LPARAM)&DataOP);
		SetPos(i + 1,m_nThreadID);
		DataOP.m_iIdx ++;
	}
 	
	Mode = CAL_IO;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxAttIO);
	for (int32_t i = 0;i < iPts;i ++) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxAttIO->Get(uiFreq[i],(TxAttIOTableR1C::DataF *)&DataIO);
		pParent->m_TabCtrl.m_DataIO.at(DataIO.m_iIdx) = *(TxAttIOTableR1C::DataF *)(&DataIO);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&Mode,(LPARAM)&DataIO);
		SetPos(i + 1,m_nThreadID);
		DataIO.m_iIdx ++;
	}
	CAL_THREAD_ABOART();
	return 0;
}