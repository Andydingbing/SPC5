#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxFilOffThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CRxFilOffThread, CCalBaseThread)

CCalR1CRxFilOffThread::Result::Result()
{
	for (int32_t i = 0;i < ARRAY_SIZE(m_ddBc);i ++)
		m_ddBc[i] = 0.0;
	m_iIdx = 0;
	m_iSec = 0;
}

CCalR1CRxFilOffThread::CCalR1CRxFilOffThread()
{
}

CCalR1CRxFilOffThread::CCalR1CRxFilOffThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CRxFilOffThread::~CCalR1CRxFilOffThread()
{
}

BOOL CCalR1CRxFilOffThread::InitInstance()
{
	return TRUE;
}

int CCalR1CRxFilOffThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CRxFilOffThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CRxFilOffThread::Run()
{
	return CalRxFilOff();
}

int CCalR1CRxFilOffThread::CalRxFilOff()
{
#define POST_CLEAR pSP1401->GetCalFile()->SetBW(_160M);

	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Rx Filter Offset"));
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Rx<===>Z28<===>Signal Generator"));
		Instrument.Init();
		Instrument.HasPM();
		Instrument.HasSG();
		Instrument.PM_Reset();
		Instrument.SG_Reset();
		Instrument.SG_SetModEnable(false);
		Instrument.SG_SetPL(-60.0);
		Instrument.SG_SetOutputEnable(true);

		CCalR1CRxFilOffDlg *pParent = (CCalR1CRxFilOffDlg *)m_pParent;
		CalIOMode Mode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;

		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;

		int64_t iAD[2] = {0,0};
		double dPowerSG[3] = {10.0 + PM_IL,10.0 + PM_IL,-20.0 + PM_IL};
		for (int32_t i = 0;i < ARRAY_SIZE(dPowerSG);i ++)	//for preventing saturation
			dPowerSG[i] -= 10.0;

		uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(uiFreq,0,sizeof(uiFreq));
		memset(uiFreqStar,0,sizeof(uiFreqStar));
		memset(uiFreqStop,0,sizeof(uiFreqStop));

		Result Data;
		RxFilter_160M_TableR1C::DataM DataFilter_160;
		RxFilter_80M_TableR1C::DataM  DataFilter_80;
		double dCoefReal[RX_FILTER_ORDER] = {0.0};
		double dCoefImag[RX_FILTER_ORDER] = {0.0};
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);

		InitSinglePos("Calibrating Rx Filter Offset",iPts);

		pSP1401->GetCalFile()->SetBW(_80M);
		pSP1401->GetCalFile()->Map2Buf(ICalFile::RxFil_160);
		((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Map2Mem();

		pSP1401->SetTxAtt(30.0,30.0,30.0,30.0);
		pSP1401->SetRxIFFilterSw(ISP1401::_160MHz);
		pSP2401->SetTxPowerComp(-5.0);
		pSP2401->SetRxPowerComp(0);
		pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);

		if (CalOP(Mode)) {
			CallingMode = CAL_OP;
			pSP1401->SetIOMode(OutPut);
			
			RxRefOPTableR1C::DataM DataRef;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
				Instrument.SG_SetCW((double)(uiFreq[i]));
				pSP1401->SetRxFreq(uiFreq[i]);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->GetBase(uiFreq[i],&DataRef);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq[i],&DataFilter_160);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_80M->Get(uiFreq[i],&DataFilter_80);

				for (int32_t j = 0;j < ARRAY_SIZE(Data.m_iOffset);j ++) {
					CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
					Instrument.SG_SetPL(dPowerSG[j]);
					DataFilter_160._2Double(dCoefReal,dCoefImag);
					pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
					pSP2401->SetRxFilter(dCoefReal,dCoefImag);
					pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(DataRef.m_State[j].m_sLNAAtt));
					pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(DataRef.m_State[j].m_sAtt019));
					pSP1401->SetRxAtt1((double)(DataRef.m_State[j].m_fAtt1));
					pSP1401->SetRxAtt2((double)(DataRef.m_State[j].m_fAtt2));
					pSP1401->SetRxAtt3((double)(DataRef.m_State[j].m_fAtt3));
					pSP2401->SetRxPowerComp(DataRef.m_State[j].m_iADOffset);
					Sleep(5);

					GetADS5474(pSP1401,iAD[0]);

					DataFilter_80._2Double(dCoefReal,dCoefImag);
					pSP2401->SetRxFilterSw(CSP2401R1A::_3D);
					pSP2401->SetRxFilter(dCoefReal,dCoefImag);
					Sleep(5);

					GetADS5474(pSP1401,iAD[1]);
					Data.m_ddBc[j] = ad2dBc(iAD[1],iAD[0]);
					Data.m_iOffset[j] = _0dBFS - dBc2ad(_0dBFS,Data.m_ddBc[j]);
				}

				Data.m_uiFreq = uiFreq[i];
				pSP1401->GetTemp0RxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp1RxLNA(&Data.m_dTemp[1]);
				pSP1401->GetTemp2RxLO1(&Data.m_dTemp[2]);
				pSP1401->GetTemp3RxPA1(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				pParent->m_TabCtrl.m_DataOP_80.at(i) = *(RxFilterOffTableR1C::DataF *)(&Data);
				pSP1401->GetCalFile()->Add(ICalFile::RxFilOffOP_80,(RxFilterOffTableR1C::DataF *)&Data);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_OFF_80_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxFilOffOP_80);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilOffOP_80M->SaveAs("c:\\rx_filter_off_op_80.txt");
		}
		if (CalIO(Mode)) {
			CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
			CallingMode = CAL_IO;
			pSP1401->SetIOMode(IO);

			RxRefIOTableR1C::DataM DataRef;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
				Instrument.SG_SetCW((double)(uiFreq[i]));
				pSP1401->SetRxFreq(uiFreq[i]);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->GetBase(uiFreq[i],&DataRef);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq[i],&DataFilter_160);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_80M->Get(uiFreq[i],&DataFilter_80);

				for (int32_t j = 0;j < ARRAY_SIZE(Data.m_iOffset);j ++) {
					CAL_THREAD_TEST_CANCEL_S(POST_CLEAR);
					Instrument.SG_SetPL(dPowerSG[j]);
					DataFilter_160._2Double(dCoefReal,dCoefImag);
					pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
					pSP2401->SetRxFilter(dCoefReal,dCoefImag);
					pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(DataRef.m_State[j].m_sLNAAtt));
					pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(DataRef.m_State[j].m_sAtt019));
					pSP1401->SetRxAtt1((double)(DataRef.m_State[j].m_fAtt1));
					pSP1401->SetRxAtt2((double)(DataRef.m_State[j].m_fAtt2));
					pSP1401->SetRxAtt3((double)(DataRef.m_State[j].m_fAtt3));
					pSP2401->SetRxPowerComp(DataRef.m_State[j].m_iADOffset);
					Sleep(5);

					GetADS5474(pSP1401,iAD[0]);

					DataFilter_80._2Double(dCoefReal,dCoefImag);
					pSP2401->SetRxFilterSw(CSP2401R1A::_3D);
					pSP2401->SetRxFilter(dCoefReal,dCoefImag);
					Sleep(5);

					GetADS5474(pSP1401,iAD[1]);
					Data.m_ddBc[j] = ad2dBc(iAD[1],iAD[0]);
					Data.m_iOffset[j] = _0dBFS - dBc2ad(_0dBFS,Data.m_ddBc[j]);
				}
				Data.m_uiFreq = uiFreq[i];
				pSP1401->GetTemp0RxLO2(&Data.m_dTemp[0]);
				pSP1401->GetTemp1RxLNA(&Data.m_dTemp[1]);
				pSP1401->GetTemp2RxLO1(&Data.m_dTemp[2]);
				pSP1401->GetTemp3RxPA1(&Data.m_dTemp[3]);
				time_t Time;
				time(&Time);
				Data.m_EndTime = *localtime(&Time);
				Data.m_iIdx = i;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				pParent->m_TabCtrl.m_DataIO_80.at(i) = *(RxFilterOffTableR1C::DataF *)(&Data);
				pSP1401->GetCalFile()->Add(ICalFile::RxFilOffIO_80,(RxFilterOffTableR1C::DataF *)&Data);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_OFF_80_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxFilOffIO_80);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilOffIO_80M->SaveAs("c:\\rx_filter_off_io_80.txt");
		}
		CAL_THREAD_ABOART_S(POST_CLEAR);
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}


IMPLEMENT_DYNCREATE(CExportR1CRxFilOffThread, CWinThread)

CExportR1CRxFilOffThread::CExportR1CRxFilOffThread()
{
}

CExportR1CRxFilOffThread::CExportR1CRxFilOffThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CRxFilOffThread::~CExportR1CRxFilOffThread()
{
}

BOOL CExportR1CRxFilOffThread::InitInstance()
{
	return TRUE;
}

int CExportR1CRxFilOffThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CRxFilOffThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CRxFilOffThread::Run()
{
	CCalR1CRxFilOffDlg *pParent = (CCalR1CRxFilOffDlg *)m_pParent;
	CalIOMode Mode = CAL_OP;

	uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	InitSinglePos("Exporting Rx Filter Offset",iPts);
	
	CCalR1CRxFilOffThread::Result Data;
	Data.m_iIdx = 0;
	Data.m_iSec = 0;

	pParent->m_pSP1401->GetCalFile()->SetBW(_80M);
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxFilOffOP_80);
	for (int32_t i = 0;i < iPts;i ++) {
		Data.m_iIdx  = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxFilOffOP_80M->Get(uiFreq[i],(RxFilterOffTableR1C::DataF *)&Data);
		for (int32_t j = 0;j < ARRAY_SIZE(Data.m_ddBc);j ++)
			Data.m_ddBc[j] = ad2dBc(_0dBFS - Data.m_iOffset[j],_0dBFS);
		pParent->m_TabCtrl.m_DataOP_80.at(Data.m_iIdx) = *(RxFilterOffTableR1C::DataF *)(&Data);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_OFF_80_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
		SetPos(i + 1,m_nThreadID);
	}

	Mode = CAL_IO;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxFilOffIO_80);
	for (int32_t i = 0;i < iPts;i ++) {
		Data.m_iIdx = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxFilOffIO_80M->Get(uiFreq[i],(RxFilterOffTableR1C::DataF *)&Data);
		for (int32_t j = 0;j < ARRAY_SIZE(Data.m_ddBc);j ++)
			Data.m_ddBc[j] = ad2dBc(_0dBFS - Data.m_iOffset[j],_0dBFS);
		pParent->m_TabCtrl.m_DataIO_80.at(Data.m_iIdx) = *(RxFilterOffTableR1C::DataF *)(&Data);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_OFF_80_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
		SetPos(i + 1,m_nThreadID);
	}
	pParent->m_pSP1401->GetCalFile()->SetBW(_160M);
	CAL_THREAD_ABOART();
	return 0;
}