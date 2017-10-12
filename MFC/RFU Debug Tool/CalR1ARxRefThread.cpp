#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ARxRefThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1ARxRefThread, CCalBaseThread)

CCalR1ARxRefThread::CCalR1ARxRefThread()
{
}

CCalR1ARxRefThread::CCalR1ARxRefThread(CWnd *pParent) : m_iSGPow(-160)
													  , m_dPMIL(PM_IL)
{
	m_pParent = pParent;
}

CCalR1ARxRefThread::~CCalR1ARxRefThread()
{
}

BOOL CCalR1ARxRefThread::InitInstance()
{
	return TRUE;
}

int CCalR1ARxRefThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1ARxRefThread, CWinThread)
END_MESSAGE_MAP()


int CCalR1ARxRefThread::Run()
{
	return CalRxRef();
}

int CCalR1ARxRefThread::CalRxRef()
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

		CCalR1ARxRefDlg *pParent = (CCalR1ARxRefDlg *)m_pParent;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		CalIOMode CalMode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;
		CalRxRefResult Data;

		int32_t iFreqIdx = 0;

		int32_t iLNAAtt_0 = 0;
		double	dAtt1_0 = 0.0;
		int32_t iAtt2_0 = 0;
		double  dAttRf_0 = 0.0;

		double dAttRf = 0.0;
		SP1401::RxLNAAtt LNAAtt = SP1401::Rx_Att;
		double dAtt1 = 0.0;
		int32_t iAtt2 = 0;

		int32_t iRef = 30;
		int64_t iAD = 0;
		double  ddBc = 0.0;

		uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A)];
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq);
		InitSinglePos("Calibrating Rx Reference",iPts);

		pSP1401->SetTxAtt(30,30,30);
		pSP2401->SetTxPowerComp(-5.0);
		pSP2401->SetRxPowerComp(0);
		if (CalOP(CalMode)) {
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetIOMode(OutPut);
			Sleep(20);
			CallingMode = CAL_OP;

			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();	
				pSP1401->SetRxFreq(uiFreq[i]);
				iFreqIdx = (int32_t)(SERIE_INDEX(uiFreq[i],RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1A));

				LNAAtt = SP1401::Rx_Att;
				RxRefTableR1A::GuessBaseOP(iLNAAtt_0,dAtt1_0,iAtt2_0);
				dAtt1 = abs(dAtt1_0);
				iAtt2 = abs(iAtt2_0);
				dAttRf = abs(dAtt1_0) + abs(iAtt2_0);
				pSP1401->SetRxLNAAttSw(iLNAAtt_0 == 0 ? SP1401::Rx_Att : SP1401::Rx_LNA);
				pSP1401->SetRxAtt(dAtt1_0,iAtt2_0);
				Sleep(2);
				AjustSG((double)uiFreq[i],RxRefTableR1A::GetSGPower(0));
				for (int32_t i_adapt = 0;i_adapt < 10;i_adapt ++) {
					CAL_THREAD_TEST_CANCEL();
					GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
					ddBc = ad2dBc(_0dBFS,iAD);
					if (abs(ddBc) > 2.0) {
						pSP1401->SetRxAtt(dAttRf + ddBc,&dAtt1,&iAtt2);
						Sleep(2);
					}
					else {
						dAtt1_0  = dAtt1;
						iAtt2_0  = iAtt2;
						dAttRf_0 = dAtt1_0 + iAtt2_0;
						break;
					}
				}
				for (int32_t iRefIdx = 0;iRefIdx < ARRAY_SIZE(Data.m_StateOP);iRefIdx ++) {
					CAL_THREAD_TEST_CANCEL();
					iRef = R1A_RX_REF_OP_STAR + iRefIdx * R1A_RX_REF_STEP;

					if (LNAAtt != SP1401::Rx_LNA) {
						dAttRf = dAttRf_0 + (iRef - 0);
						dAttRf = (dAttRf >= 0 ? dAttRf : 0);
						pSP1401->SetRxAtt(dAttRf,&dAtt1,&iAtt2);
						Sleep(2);
					}
					dAttRf = (dAttRf >= 0 ? dAttRf : 0);

					AjustSG((double)uiFreq[i],RxRefTableR1A::GetSGPower(iRef));
					for(int32_t i_adapt = 0;i_adapt < 5;i_adapt ++) {
						CAL_THREAD_TEST_CANCEL();
						CCalBaseThread::GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
						if (iRef > 10)
							iAD = dBc2ad(iAD,10 - iRef);
						ddBc = ad2dBc(_0dBFS,iAD);
						if (ddBc < -8.0 && LNAAtt == SP1401::Rx_Att) {
							LNAAtt = SP1401::Rx_LNA;
							dAtt1 = 0.0;
							iAtt2 = 10;
							pSP1401->SetRxLNAAttSw(LNAAtt);
							pSP1401->SetRxAtt1(dAtt1);
							pSP1401->SetRxAtt2(iAtt2);
							Sleep(2);
							GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
							break;
						}
						if (ddBc > -5 && LNAAtt == SP1401::Rx_LNA) {
							GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
							break;
						}
						if (ddBc <= -5 && LNAAtt == SP1401::Rx_LNA) {
							iAtt2 = 0;
							pSP1401->SetRxAtt2(iAtt2);
							Sleep(2);
							GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
							break;
						}
						if (abs(ddBc) > 0.5) {
							dAttRf += ad2dBc(_0dBFS,iAD);
							dAttRf = (dAttRf >= 0 ? dAttRf : 0);
							pSP1401->SetRxAtt(dAttRf,&dAtt1,&iAtt2);
							Sleep(2);
						}
						else
							break;	
					}

					Data.m_iIdx = i;
					Data.m_uiFreq = uiFreq[i];
					Data.m_StateOP[iRefIdx].m_iLNAAtt = LNAAtt;
					Data.m_StateOP[iRefIdx].m_dAtt1 = dAtt1;
					Data.m_StateOP[iRefIdx].m_iAtt2 = iAtt2;
					Data.m_StateOP[iRefIdx].m_iADOffset = _0dBFS - iAD;
					Data.m_iRefIdxOP = iRefIdx;
					pParent->m_Data.at(i) = *(RxRefTableR1A::DataF *)(&Data);
					::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_REF_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
					SetPos(i + 1,m_nThreadID);
				}
				int32_t iIORefIdx = 0;
				int64_t iIOAD_0dBFS = 0;
				int32_t iIOLNAAtt = 0;
				double  dIOAtt1 = 0.0;
				int32_t iIOAtt2 = 0;
				for (double dRefIO = R1A_RX_REF_IO_STAR;dRefIO >= R1A_RX_REF_IO_STOP;dRefIO += R1A_RX_REF_STEP) {
					((CalFileR1A *)(pSP1401->GetCalFile()))->m_pRxRef->GetIO(uiFreq[i],dRefIO,iIOAD_0dBFS,iIOLNAAtt,dIOAtt1,iIOAtt2);
					Data.m_StateIO[iIORefIdx].m_iADOffset = _0dBFS - iIOAD_0dBFS;
					Data.m_StateIO[iIORefIdx].m_iLNAAtt = iIOLNAAtt;
					Data.m_StateIO[iIORefIdx].m_dAtt1 = dIOAtt1;
					Data.m_StateIO[iIORefIdx].m_iAtt2 = iIOAtt2;
					iIORefIdx ++;
				}
				pSP1401->GetCalFile()->Add(ICalFile::RxRef,(RxRefTableR1A::DataF *)(&Data));
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxRef);
		}

		if (CalIO(CalMode)) {
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetIOMode(IO);
			Sleep(20);
			CallingMode = CAL_IO;

			for (int32_t i = 0; i < iPts ;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetRxFreq(uiFreq[i]);
				iFreqIdx = (int32_t)(SERIE_INDEX(uiFreq[i],RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_CALLED_R1A));

				LNAAtt = SP1401::Rx_Att;
				RxRefTableR1A::GuessBaseIO(iLNAAtt_0,dAtt1_0,iAtt2_0);
				dAtt1 = abs(dAtt1_0);
				iAtt2 = abs(iAtt2_0);
				dAttRf = abs(dAtt1_0) + abs(iAtt2_0);
				pSP1401->SetRxLNAAttSw(iLNAAtt_0 == 0 ? SP1401::Rx_Att : SP1401::Rx_LNA);
				pSP1401->SetRxAtt(dAtt1_0,iAtt2_0);
				Sleep(2);
				AjustSG((double)uiFreq[i],RxRefTableR1A::GetSGPower(0));
				for (int32_t i_adapt = 0;i_adapt < 10;i_adapt ++) {
					CAL_THREAD_TEST_CANCEL();
					GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
					ddBc = ad2dBc(_0dBFS,iAD);
					if (abs(ddBc) > 2.0) {
						pSP1401->SetRxAtt(dAttRf + ddBc,&dAtt1,&iAtt2);
						Sleep(2);
					}
					else {
						dAtt1_0  = dAtt1;
						iAtt2_0  = iAtt2;
						dAttRf_0 = dAtt1_0 + iAtt2_0;
						break;
					}
				}
				for (int32_t iRefIdx = 0;iRefIdx < ARRAY_SIZE(Data.m_StateIO);iRefIdx ++) {
					CAL_THREAD_TEST_CANCEL();
					iRef = R1A_RX_REF_IO_STAR + iRefIdx * R1A_RX_REF_STEP;

					if (LNAAtt != SP1401::Rx_LNA) {
						dAttRf = dAttRf_0 + (iRef - 0);
						dAttRf = (dAttRf >= 0 ? dAttRf : 0);
						pSP1401->SetRxAtt(dAttRf,&dAtt1,&iAtt2);
						Sleep(2);
					}
					dAttRf = (dAttRf >= 0 ? dAttRf : 0);

					AjustSG((double)uiFreq[i],RxRefTableR1A::GetSGPower(iRef));
					for(int32_t i_adapt = 0;i_adapt < 5;i_adapt ++) {
						CAL_THREAD_TEST_CANCEL();
						GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
						if (iRef > 10)
							iAD = dBc2ad(iAD,10 - iRef);
						ddBc = ad2dBc(_0dBFS,iAD);
						if (ddBc < -8.0 && LNAAtt == SP1401::Rx_Att) {
							LNAAtt = SP1401::Rx_LNA;
							dAtt1 = 0.0;
							iAtt2 = 10;
							pSP1401->SetRxLNAAttSw(LNAAtt);
							pSP1401->SetRxAtt1(dAtt1);
							pSP1401->SetRxAtt2(iAtt2);
							Sleep(2);
							CCalBaseThread::GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
							break;
						}
						if (ddBc > -5 && LNAAtt == SP1401::Rx_LNA) {
							CCalBaseThread::GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
							break;
						}
						if (ddBc <= -5 && LNAAtt == SP1401::Rx_LNA) {
							iAtt2 = 0;
							pSP1401->SetRxAtt2(iAtt2);
							Sleep(2);
							GetADS5474(pSP1401,iAD,AVERAGE_TIMES);
							break;
						}
						if (abs(ddBc) > 0.5) {
							dAttRf += ad2dBc(_0dBFS,iAD);
							dAttRf = (dAttRf >= 0 ? dAttRf : 0);
							pSP1401->SetRxAtt(dAttRf,&dAtt1,&iAtt2);
							Sleep(2);
						}
						else
							break;	
					}

					Data.m_iIdx = i;
					Data.m_uiFreq = uiFreq[i];
					Data.m_StateIO[iRefIdx].m_iLNAAtt = LNAAtt;
					Data.m_StateIO[iRefIdx].m_dAtt1 = dAtt1;
					Data.m_StateIO[iRefIdx].m_iAtt2 = iAtt2;
					Data.m_StateIO[iRefIdx].m_iADOffset = _0dBFS - iAD;
					Data.m_iRefIdxIO = iRefIdx;
					pParent->m_Data.at(i) = *(RxRefTableR1A::DataF *)(&Data);
					::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_REF_SHOW,(WPARAM)&CallingMode,(LPARAM)&Data);
					SetPos(i + 1,m_nThreadID);
				}
				int32_t iOPRefIdx = 0;
				int64_t iOPAD_0dBFS = 0;
				int32_t iOPLNAAtt = 0;
				double  dOPAtt1 = 0.0;
				int32_t iOPAtt2 = 0;
				for (double dOPRef = R1A_RX_REF_OP_STAR;dOPRef >= R1A_RX_REF_OP_STOP;dOPRef += R1A_RX_REF_STEP) {
					((CalFileR1A *)(pSP1401->GetCalFile()))->m_pRxRef->GetOP(uiFreq[i],dOPRef,iOPAD_0dBFS,iOPLNAAtt,dOPAtt1,iOPAtt2);
					Data.m_StateOP[iOPRefIdx].m_iADOffset = _0dBFS - iOPAD_0dBFS;
					Data.m_StateOP[iOPRefIdx].m_iLNAAtt = iOPLNAAtt;
					Data.m_StateOP[iOPRefIdx].m_dAtt1 = dOPAtt1;
					Data.m_StateOP[iOPRefIdx].m_iAtt2 = iOPAtt2;
					iOPRefIdx ++;
				}
				pSP1401->GetCalFile()->Add(ICalFile::RxRef,(RxRefTableR1A::DataF *)(&Data));
			}
		}
		pSP1401->GetCalFile()->Write(ICalFile::RxRef);
		Instrument.Close();
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		AfxMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}

void CCalR1ARxRefThread::AjustSG(double dFreq,int32_t iPower)
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


IMPLEMENT_DYNCREATE(CExportR1ARxRefThread, CWinThread)

CExportR1ARxRefThread::CExportR1ARxRefThread()
{
}

CExportR1ARxRefThread::CExportR1ARxRefThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1ARxRefThread::~CExportR1ARxRefThread()
{
}

BOOL CExportR1ARxRefThread::InitInstance()
{
	return TRUE;
}

int CExportR1ARxRefThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1ARxRefThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1ARxRefThread::Run()
{
	CCalR1ARxRefDlg *pParent = (CCalR1ARxRefDlg *)m_pParent;

	uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	double dRef = 0.0;
	CSP1401R1A *pSP1401 = pParent->m_pSP1401;
	SP1401::RxLNAAtt LNAAtt = SP1401::Rx_Att;
	CalIOMode Mode = CAL_OP;
	CCalR1ARxRefThread::CalRxRefResult Data;
	Data.m_iIdx = 0;
	Data.m_iRefIdxOP = 0;
	Data.m_iRefIdxIO = 0;

	InitSinglePos("Exporting Rx Reference",iPts);

	for (int32_t i = 0;i < iPts;i ++) {
		CAL_THREAD_TEST_CANCEL();
		Data.m_uiFreq = uiFreq[i];
		for (int32_t j = 0;j < SERIE_SIZE(R1A_RX_REF_OP_STAR,R1A_RX_REF_OP_STOP,R1A_RX_REF_STEP);j ++) {
			dRef = R1A_RX_REF_OP_STAR + j * R1A_RX_REF_STEP;
			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pRxRef->Get(uiFreq[i],dRef,OutPut,Data.m_StateOP[j].m_iADOffset,LNAAtt,Data.m_StateOP[j].m_dAtt1,Data.m_StateOP[j].m_iAtt2);
			Data.m_StateOP[j].m_iLNAAtt = (int32_t)LNAAtt;
			Data.m_StateOP[j].m_iADOffset = _0dBFS - Data.m_StateOP[j].m_iADOffset;
			Data.m_iRefIdxOP = j;
		}
		for (int32_t j = 0;j < SERIE_SIZE(R1A_RX_REF_IO_STAR,R1A_RX_REF_IO_STOP,R1A_RX_REF_STEP);j ++) {
			dRef = R1A_RX_REF_IO_STAR + j * R1A_RX_REF_STEP;
			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pRxRef->Get(uiFreq[i],dRef,IO,Data.m_StateIO[j].m_iADOffset,LNAAtt,Data.m_StateIO[j].m_dAtt1,Data.m_StateIO[j].m_iAtt2);
			Data.m_StateIO[j].m_iLNAAtt = (int32_t)LNAAtt;
			Data.m_StateIO[j].m_iADOffset = _0dBFS - Data.m_StateIO[j].m_iADOffset;
			Data.m_iRefIdxIO = j;
		}
		pParent->m_Data.at(i) = *(RxRefTableR1A::DataF *)(&Data);
		Data.m_iIdx ++;
	}
	SetPos(iPts,m_nThreadID);
	CAL_THREAD_ABOART();
	return 0;
}