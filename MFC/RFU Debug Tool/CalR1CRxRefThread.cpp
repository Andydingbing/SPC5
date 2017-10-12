#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxRefThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CRxRefThread, CCalBaseThread)

CCalR1CRxRefThread::CCalR1CRxRefThread()
{
}

CCalR1CRxRefThread::CCalR1CRxRefThread(CWnd *pParent) : m_iSGPow(-160)
													  , m_dPMIL(PM_IL)
{
	m_pParent = pParent;
}

CCalR1CRxRefThread::~CCalR1CRxRefThread()
{
}

BOOL CCalR1CRxRefThread::InitInstance()
{
	return TRUE;
}

int CCalR1CRxRefThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CRxRefThread, CWinThread)
END_MESSAGE_MAP()


int CCalR1CRxRefThread::Run()
{
	return CalRxRef();
}

int CCalR1CRxRefThread::CalRxRef()
{
	try
	{
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Rx Reference"));
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Rx<===>Z28<===>Signal Generator"));
		Instrument.Init();
		Instrument.HasPM();
		Instrument.HasSG();
		Instrument.PM_Reset();
		Instrument.SG_Reset();
		Instrument.SG_SetModEnable(false);
		Instrument.SG_SetPL(-60.0);
		Instrument.SG_SetOutputEnable(true);

		CCalR1CRxRefDlg *pParent = (CCalR1CRxRefDlg *)m_pParent;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		CalIOMode CalMode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;

		int32_t iLNAAtt = 0;
		int32_t iAtt019 = 0;
		double	dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0;

		int32_t iRef = 30;
		int64_t iAD = 0;
		double  ddBc = 0.0;
		int32_t iAjust = 0;

		RxFilter_160M_TableR1C::DataM DataFilter;
		double dCoefReal[RX_FILTER_ORDER] = {0.0};
		double dCoefImag[RX_FILTER_ORDER] = {0.0};

		uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(uiFreq,0,sizeof(uiFreq));
		memset(uiFreqStar,0,sizeof(uiFreqStar));
		memset(uiFreqStop,0,sizeof(uiFreqStop));

		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);
		InitSinglePos("Calibrating Rx Reference",iPts);

		pSP1401->SetTxAtt(30.0,30.0,30.0,30.0);
		pSP1401->SetRxIFFilterSw(ISP1401::_160MHz);
		pSP2401->SetTxPowerComp(-5.0);
		pSP2401->SetRxPowerComp(0);
		pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
		pSP1401->GetCalFile()->SetBW(_160M);

		if (CalOP(CalMode)) {
			ResultOP DataOP;
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetIOMode(OutPut);
			CallingMode = CAL_OP;

			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();
				DataOP.m_iIdx = i;
				DataOP.m_uiFreq = uiFreq[i];

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetRxFilter(dCoefReal,dCoefImag);
				pSP1401->SetRxFreq(uiFreq[i]);

				for (int32_t j = 0;j < ARRAY_SIZE(DataOP.m_State);j ++) {
					RxRefOPTableR1C::GuessBase(j,iRef,iLNAAtt,iAtt019,dAtt1,dAtt2,dAtt3);
					AjustSG((double)uiFreq[i],iRef);
					pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(iLNAAtt));
					pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(iAtt019));
					pSP1401->SetRxAtt(dAtt1,dAtt2,dAtt3);
					Sleep(2);
					GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
					ddBc = ad2dBc(_0dBFS,iAD);

					iAjust = 0;
					while (abs(ddBc) > 0.5) {
						if (-ddBc > dAtt3) {
							dAtt2 += dAtt3 + ddBc;
							dAtt3 = 0;
							dAtt2 = dAtt2 < 0.0 ? 0.0 : dAtt2;
						}
						else {
							dAtt3 += ddBc;
						}
						dAtt3 = dAtt3 < 0.0 ? 0.0 : dAtt3;
						dAtt3 = dAtt3 > 31.5 ? 31.5 : dAtt3;
						hmc624(&dAtt2);
						hmc624(&dAtt3);
						pSP1401->SetRxAtt2(dAtt2);
						pSP1401->SetRxAtt3(dAtt3);
						Sleep(2);

						GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
						ddBc = ad2dBc(_0dBFS,iAD);

						if (++iAjust > 10)
							break;
					}
					DataOP.m_iRefIdx = j;
					DataOP.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
					DataOP.m_State[j].m_iLNAAtt = (int16_t)iLNAAtt;
					DataOP.m_State[j].m_iAtt019 = (int16_t)iAtt019;
					DataOP.m_State[j].m_fAtt1 = (float)dAtt1;
					DataOP.m_State[j].m_fAtt2 = (float)dAtt2;
					DataOP.m_State[j].m_fAtt3 = (float)dAtt3;
					DataOP.m_State[j].m_iADOffset = _0dBFS - iAD;
					pSP1401->GetTemp0RxLO2(&DataOP.m_State[j].m_dTemp[0]);
					pSP1401->GetTemp1RxLNA(&DataOP.m_State[j].m_dTemp[1]);
					pSP1401->GetTemp2RxLO1(&DataOP.m_State[j].m_dTemp[2]);
					pSP1401->GetTemp3RxPA1(&DataOP.m_State[j].m_dTemp[3]);
				}
				time_t Time;
				time(&Time);
				DataOP.m_EndTime = *localtime(&Time);
				pParent->m_DataOP.at(i) = *(RxRefOPTableR1C::DataF *)(&DataOP);
				pSP1401->GetCalFile()->Add(ICalFile::RxRefOP,(RxRefOPTableR1C::DataF *)(&DataOP));
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_REF_SHOW,(WPARAM)&CallingMode,(LPARAM)&DataOP);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxRefOP);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->SaveAs("c:\\rx_ref_op.txt");
		}
		if (CalIO(CalMode)) {
			Instrument.PM_Reset();
			Instrument.SG_Reset();
			Instrument.SG_SetModEnable(false);
			Instrument.SG_SetPL(-60.0);
			Instrument.SG_SetOutputEnable(true);
			ResultIO DataIO;
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetIOMode(IO);
			CallingMode = CAL_IO;

			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();
				DataIO.m_iIdx = i;
				DataIO.m_uiFreq = uiFreq[i];

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetRxFilter(dCoefReal,dCoefImag);
				pSP1401->SetRxFreq(uiFreq[i]);

				for (int32_t j = 0;j < ARRAY_SIZE(DataIO.m_State);j ++) {
					RxRefIOTableR1C::GuessBase(j,iRef,iLNAAtt,iAtt019,dAtt1,dAtt2,dAtt3);
					AjustSG((double)uiFreq[i],iRef);
					pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(iLNAAtt));
					pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(iAtt019));
					pSP1401->SetRxAtt(dAtt1,dAtt2,dAtt3);
					Sleep(2);
					GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
					ddBc = ad2dBc(_0dBFS,iAD);

					iAjust = 0;
					while (abs(ddBc) > 0.5) {
						if (-ddBc > dAtt3) {
							dAtt2 += dAtt3 + ddBc;
							dAtt3 = 0;
							dAtt2 = dAtt2 < 0.0 ? 0.0 : dAtt2;
						}
						else {
							dAtt3 += ddBc;
						}
						dAtt3 = dAtt3 < 0.0 ? 0.0 : dAtt3;
						dAtt3 = dAtt3 > 31.5 ? 31.5 : dAtt3;
						hmc624(&dAtt2);
						hmc624(&dAtt3);
						pSP1401->SetRxAtt2(dAtt2);
						pSP1401->SetRxAtt3(dAtt3);
						Sleep(2);

						GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
						ddBc = ad2dBc(_0dBFS,iAD);

						if (++iAjust > 10)
							break;
					}
					DataIO.m_iRefIdx = j;
					DataIO.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
					DataIO.m_State[j].m_iLNAAtt = (int16_t)iLNAAtt;
					DataIO.m_State[j].m_iAtt019 = (int16_t)iAtt019;
					DataIO.m_State[j].m_fAtt1 = (float)dAtt1;
					DataIO.m_State[j].m_fAtt2 = (float)dAtt2;
					DataIO.m_State[j].m_fAtt3 = (float)dAtt3;
					DataIO.m_State[j].m_iADOffset = _0dBFS - iAD;
					pSP1401->GetTemp0RxLO2(&DataIO.m_State[j].m_dTemp[0]);
					pSP1401->GetTemp1RxLNA(&DataIO.m_State[j].m_dTemp[1]);
					pSP1401->GetTemp2RxLO1(&DataIO.m_State[j].m_dTemp[2]);
					pSP1401->GetTemp3RxPA1(&DataIO.m_State[j].m_dTemp[3]);
				}
				time_t Time;
				time(&Time);
				DataIO.m_EndTime = *localtime(&Time);
				pParent->m_DataIO.at(i) = *(RxRefIOTableR1C::DataF *)(&DataIO);
				pSP1401->GetCalFile()->Add(ICalFile::RxRefIO,(RxRefIOTableR1C::DataF *)(&DataIO));
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_REF_SHOW,(WPARAM)&CallingMode,(LPARAM)&DataIO);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxRefIO);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->SaveAs("c:\\rx_ref_io.txt");
		}
		Instrument.Close();
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		AfxMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}

void CCalR1CRxRefThread::AjustSG(double dFreq,int32_t iPower)
{
	if (iPower == m_iSGPow)
		return;
	double dPMPower = 0.0;
	Instrument.SG_SetCW(dFreq);
	Instrument.SG_SetPL(iPower + m_dPMIL);
	Sleep(2);
	Instrument.PM_SetFrequency(dFreq);
	for (int32_t i = 0;i < 10;i ++) {
		Instrument.PM_GetPwr(dFreq,dPMPower);
		if (abs(iPower - dPMPower) < 0.05)
			break;
		Instrument.SG_SetPL(iPower + m_dPMIL + (iPower - dPMPower));
		Sleep(2);
		m_dPMIL += (iPower - dPMPower);
	}
	m_iSGPow = iPower;
}


IMPLEMENT_DYNCREATE(CExportR1CRxRefThread, CWinThread)

CExportR1CRxRefThread::CExportR1CRxRefThread()
{
}

CExportR1CRxRefThread::CExportR1CRxRefThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CRxRefThread::~CExportR1CRxRefThread()
{
}

BOOL CExportR1CRxRefThread::InitInstance()
{
	return TRUE;
}

int CExportR1CRxRefThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CRxRefThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CRxRefThread::Run()
{
	CCalR1CRxRefDlg *pParent = (CCalR1CRxRefDlg *)m_pParent;
	CalIOMode Mode = CAL_OP;

	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	CCalR1CRxRefThread::ResultOP DataOP;
	CCalR1CRxRefThread::ResultIO DataIO;
	DataOP.m_iIdx = 0;
	DataOP.m_iRefIdx = ARRAY_SIZE(DataOP.m_State) - 1;
	DataOP.m_iSec = 0;
	DataIO.m_iIdx = 0;
	DataIO.m_iRefIdx = DataOP.m_iRefIdx;
	DataIO.m_iSec = 0;

	InitSinglePos("Exporting Rx Reference",iPts);

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRefOP);
	for (int32_t i = 0;i < iPts;i ++) {
		DataOP.m_iIdx = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxRefOP->GetBase(uiFreq[i],&DataOP);
		pParent->m_DataOP.at(DataOP.m_iIdx) = *(RxRefOPTableR1C::DataF *)(&DataOP);
		::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_CAL_RX_REF_SHOW,(WPARAM)&Mode,(LPARAM)&DataOP);
		SetPos(i + 1,m_nThreadID);
	}

	Mode = CAL_IO;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRefIO);
	for (int32_t i = 0;i < iPts;i ++) {
		DataIO.m_iIdx = i;
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxRefIO->GetBase(uiFreq[i],&DataIO);
		pParent->m_DataIO.at(DataIO.m_iIdx) = *(RxRefIOTableR1C::DataF *)(&DataIO);
		::SendMessage(m_pParent->GetSafeHwnd(),WM_CSE_CAL_RX_REF_SHOW,(WPARAM)&Mode,(LPARAM)&DataIO);
		SetPos(i + 1,m_nThreadID);
	}
	CAL_THREAD_ABOART();
	return 0;
}