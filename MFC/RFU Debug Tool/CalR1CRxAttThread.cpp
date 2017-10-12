#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxAttThread.h"
#include "CheckDlg.h"
#include "algorithm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CRxAttThread, CCalBaseThread)

CCalR1CRxAttThread::CCalR1CRxAttThread()
{
}

CCalR1CRxAttThread::CCalR1CRxAttThread(CWnd *pParent) : m_iSGPow(-160)
													  , m_dPMIL(PM_IL)
{
	m_pParent = pParent;
}

CCalR1CRxAttThread::~CCalR1CRxAttThread()
{
}

BOOL CCalR1CRxAttThread::InitInstance()
{
	return TRUE;
}

int CCalR1CRxAttThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CRxAttThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CRxAttThread::Run()
{
	return CalRxAtt();
}

int CCalR1CRxAttThread::CalRxAtt()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Rx Att Offset"));
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Rx<===>Z28<===>Signal Generator"));
		Instrument.Init();
		Instrument.HasPM();
		Instrument.HasSG();
		Instrument.PM_Reset();
		Instrument.SG_Reset();
		Instrument.SG_SetModEnable(false);
		Instrument.SG_SetPL(-60.0);
		Instrument.SG_SetOutputEnable(true);

		CCalR1CRxAttDlg *pParent = (CCalR1CRxAttDlg *)m_pParent;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		CalIOMode CalMode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;

		int64_t iADBase = 0,iAD = 0,iOffsetBase = 0;
		int32_t iAttIdx = 0;
		int32_t iRef = 30;

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

		InitSinglePos("Calibrating Rx Att Offset",iPts);

		pSP1401->SetTxAtt(30.0,30.0,30.0,30.0);
		pSP1401->SetRxIFFilterSw(ISP1401::_160MHz);
		pSP2401->SetTxPowerComp(-5.0);
		pSP2401->SetRxPowerComp(0);
		pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
		pSP1401->GetCalFile()->SetBW(_160M);

		if (CalOP(CalMode)) {
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetIOMode(OutPut);
			CallingMode = CAL_OP;

			ResultOP DataOP;
			RxRefOPTableR1C::DataM DataRefOP;
			RxRefOPTableR1C::RxStateM State;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetRxFilter(dCoefReal,dCoefImag);
				pSP1401->SetRxFreq(uiFreq[i]);

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->GetBase(uiFreq[i],&DataRefOP);

				//	ref		att0	att1	att2	att3
				//	 30		 -20	 -25	 -15	 -10
				//	 20		 -20	 -15	 -15	 -10
				//	 10		 -20	  -5	 -15	 -10////m_State[0]
				iAttIdx = 0;
				iOffsetBase = 0;
				AjustSG((double)uiFreq[i],10);
				SetRxState(pSP1401,pSP2401,DataRefOP.m_State[0]);
				GetADS5474(pSP1401,iADBase);
				for (iRef = 30;iRef >= 11;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataOP.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,10.0 - iRef);
					iAttIdx ++;
				}

				//	ref		att0	att1	att2	att3
				//	 10		   0	 -25	 -15	 -10////m_State[1]
				//	  0		   0	 -15	 -15	 -10
				//	-10		   0	  -5	 -15	 -10
				//	-20		   0	  -5	  -5	 -10
				AjustSG((double)uiFreq[i],0);
				SetRxState(pSP1401,pSP2401,DataRefOP.m_State[1]);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0);
				for (iRef = 9;iRef >= 0;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataOP.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,0.0 - iRef);
					iAttIdx ++;
				}
				iOffsetBase = DataOP.m_iOffset[iAttIdx - 1];

				AjustSG((double)uiFreq[i],-10);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0) + iOffsetBase;
				for (iRef = -1;iRef >= -10;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataOP.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-10.0 - iRef);
					iAttIdx ++;
				}
				iOffsetBase = DataOP.m_iOffset[iAttIdx - 1];

				AjustSG((double)uiFreq[i],-19);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-9.0) + iOffsetBase;
				for (iRef = -11;iRef >= -19;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataOP.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-19.0 - iRef);
					iAttIdx ++;
				}

				//	ref		att0	att1	att2	att3
				//	-20		  14	 -10	 -15	 -10////m_State[2]
				//	-30		  14	   0	 -15	 -10
				//	-40		  14	   0	  -5	 -10
				AjustSG((double)uiFreq[i],-30);
				SetRxState(pSP1401,pSP2401,DataRefOP.m_State[2]);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0);
				for (iRef = -21;iRef >= -30;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataOP.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-30.0 - iRef);
					iAttIdx ++;
				}
				iOffsetBase = DataOP.m_iOffset[iAttIdx - 1];

				AjustSG((double)uiFreq[i],-40);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0) + iOffsetBase;
				for (iRef = -31;iRef >= -40;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefOP->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataOP.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-40.0 - iRef);
					iAttIdx ++;
				}
				DataOP.m_iIdx = i;
				DataOP.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				DataOP.m_uiFreq = uiFreq[i];
				pSP1401->GetTemp0RxLO2(&DataOP.m_dTemp[0]);
				pSP1401->GetTemp1RxLNA(&DataOP.m_dTemp[1]);
				pSP1401->GetTemp2RxLO1(&DataOP.m_dTemp[2]);
				pSP1401->GetTemp3RxPA1(&DataOP.m_dTemp[3]);
				time_t Time;
				time(&Time);
				DataOP.m_EndTime = *localtime(&Time);
				pParent->m_TabCtrl.m_DataOP.at(i) = *(RxAttOPTableR1C::DataF *)(&DataOP);
				pSP1401->GetCalFile()->Add(ICalFile::RxAttOP,(RxAttOPTableR1C::DataF *)&DataOP);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_ATT_SHOW,(WPARAM)&CallingMode,(LPARAM)&DataOP);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxAttOP);
		}
		if (CalIO(CalMode)) {
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetIOMode(IO);
			CallingMode = CAL_IO;

			ResultIO DataIO;
			RxRefIOTableR1C::DataM DataRefIO;
			RxRefOPTableR1C::RxStateM State;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();

				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq[i],&DataFilter);
				DataFilter._2Double(dCoefReal,dCoefImag);
				pSP2401->SetRxFilter(dCoefReal,dCoefImag);
				pSP1401->SetRxFreq(uiFreq[i]);
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->GetBase(uiFreq[i],&DataRefIO);

				//	ref		att0	att1	att2	att3
				//	 30		 -20	 -20	 -13	 -10
				//	 20		 -20	 -10	 -13	 -10
				//	 10		 -20	  -0	 -13	 -10////m_State[0]
				iAttIdx = 0;
				iOffsetBase = 0;
				AjustSG((double)uiFreq[i],10);
				SetRxState(pSP1401,pSP2401,DataRefIO.m_State[0]);
				GetADS5474(pSP1401,iADBase);
				for (iRef = 30;iRef >= 11;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataIO.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,10.0 - iRef);
					iAttIdx ++;
				}

				//	ref		att0	att1	att2	att3
				//	 10		   0	 -20	 -13	 -10////m_State[1]
				//	  0		   0	 -10	 -13	 -10
				//	-10		   0	   0	 -13	 -10
				//	-20		   0	   0	  -3	 -10
				AjustSG((double)uiFreq[i],0);
				SetRxState(pSP1401,pSP2401,DataRefIO.m_State[1]);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0);
				for (iRef = 9;iRef >= 0;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataIO.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,0.0 - iRef);
					iAttIdx ++;
				}
				iOffsetBase = DataIO.m_iOffset[iAttIdx - 1];

				AjustSG((double)uiFreq[i],-10);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0) + iOffsetBase;
				for (iRef = -1;iRef >= -10;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataIO.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-10.0 - iRef);
					iAttIdx ++;
				}
				iOffsetBase = DataIO.m_iOffset[iAttIdx - 1];

				AjustSG((double)uiFreq[i],-19);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-9.0) + iOffsetBase;
				for (iRef = -11;iRef >= -19;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataIO.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-19.0 - iRef);
					iAttIdx ++;
				}

				//	ref		att0	att1	att2	att3
				//	-20		  14	  -4	 -13	 -10////m_State[2]
				//	-24		  14	   0	 -13	 -10
				//	-30		  14	   0	  -7	 -10
				AjustSG((double)uiFreq[i],-30);
				SetRxState(pSP1401,pSP2401,DataRefIO.m_State[2]);
				GetADS5474(pSP1401,iADBase);
				iADBase = dBc2ad(iADBase,-10.0);
				for (iRef = -21;iRef >= -30;iRef --) {
					CAL_THREAD_TEST_CANCEL();
					((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRefIO->Get(uiFreq[i],(double)iRef,&State);
					SetRxState(pSP1401,pSP2401,State);
					GetADS5474(pSP1401,iAD);
					DataIO.m_iOffset[iAttIdx] = iADBase - dBc2ad(iAD,-30.0 - iRef);
					iAttIdx ++;
				}
				DataIO.m_iIdx = i;
				DataIO.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				DataIO.m_uiFreq = uiFreq[i];
				pSP1401->GetTemp0RxLO2(&DataIO.m_dTemp[0]);
				pSP1401->GetTemp1RxLNA(&DataIO.m_dTemp[1]);
				pSP1401->GetTemp2RxLO1(&DataIO.m_dTemp[2]);
				pSP1401->GetTemp3RxPA1(&DataIO.m_dTemp[3]);
				time_t Time;
				time(&Time);
				DataIO.m_EndTime = *localtime(&Time);
				pParent->m_TabCtrl.m_DataIO.at(i) = *(RxAttIOTableR1C::DataF *)(&DataIO);
				pSP1401->GetCalFile()->Add(ICalFile::RxAttIO,(RxAttIOTableR1C::DataF *)&DataIO);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_ATT_SHOW,(WPARAM)&CallingMode,(LPARAM)&DataIO);
				SetPos(i + 1,m_nThreadID);
			}
			pSP1401->GetCalFile()->Write(ICalFile::RxAttIO);
		}
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}

void CCalR1CRxAttThread::SetRxState(CSP1401R1C *pSP1401,CSP2401R1A *pSP2401,RxRefOPTableR1C::RxStateM State)
{
	pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(State.m_sLNAAtt));
	pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(State.m_sAtt019));
	pSP1401->SetRxAtt1((double)(State.m_fAtt1));
	pSP1401->SetRxAtt2((double)(State.m_fAtt2));
	pSP1401->SetRxAtt3((double)(State.m_fAtt3));
	pSP2401->SetRxPowerComp(State.m_iADOffset);
	Sleep(2);
}

void CCalR1CRxAttThread::AjustSG(double dFreq,int32_t iPower)
{
	if (iPower == m_iSGPow)
		return;
	double dPMPower = 0.0;
	Instrument.SG_SetCW(dFreq);
	Instrument.SG_SetPL(iPower + m_dPMIL);
	Sleep(5);
	Instrument.PM_SetFrequency(dFreq);
	for (int32_t i = 0;i < 10;i ++) {
		Instrument.PM_GetPwr(dFreq,dPMPower);
		if (abs(iPower - dPMPower) < 0.5)
			break;
		Instrument.SG_SetPL(iPower + m_dPMIL + (iPower - dPMPower));
		Sleep(5);
		m_dPMIL += (iPower - dPMPower);
	}
	Sleep(5);
	m_iSGPow = iPower;
}


IMPLEMENT_DYNCREATE(CExportR1CRxAttThread, CCSEWinThread)

CExportR1CRxAttThread::CExportR1CRxAttThread()
{
}

CExportR1CRxAttThread::CExportR1CRxAttThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CRxAttThread::~CExportR1CRxAttThread()
{
}

BOOL CExportR1CRxAttThread::InitInstance()
{
	return TRUE;
}

int CExportR1CRxAttThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CRxAttThread, CCSEWinThread)
END_MESSAGE_MAP()


int CExportR1CRxAttThread::Run()
{
	CCalR1CRxAttDlg *pParent = (CCalR1CRxAttDlg *)m_pParent;
	CalIOMode Mode = CAL_OP;

	uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	InitSinglePos("Exporting Rx Att",iPts);

	CCalR1CRxAttThread::ResultOP DataOP;
	CCalR1CRxAttThread::ResultIO DataIO;
	DataOP.m_iIdx = 0;
	DataOP.m_iSec = 0;
	DataIO.m_iIdx = 0;
	DataIO.m_iSec = 0;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxAttOP);
	for (int32_t i = 0;i < iPts;i ++) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxAttOP->Get(uiFreq[i],(RxAttOPTableR1C::DataF *)&DataOP);
		pParent->m_TabCtrl.m_DataOP.at(DataOP.m_iIdx) = *(RxAttOPTableR1C::DataF *)(&DataOP);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_ATT_SHOW,(WPARAM)&Mode,(LPARAM)&DataOP);
		SetPos(i + 1,m_nThreadID);
		DataOP.m_iIdx ++;
	}

	Mode = CAL_IO;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxAttIO);
	for (int32_t i = 0;i < iPts;i ++) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxAttIO->Get(uiFreq[i],(RxAttIOTableR1C::DataF *)&DataIO);
		pParent->m_TabCtrl.m_DataIO.at(DataIO.m_iIdx) = *(RxAttIOTableR1C::DataF *)(&DataIO);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_ATT_SHOW,(WPARAM)&Mode,(LPARAM)&DataIO);
		SetPos(i + 1,m_nThreadID);
		DataIO.m_iIdx ++;
	}
	CAL_THREAD_ABOART();
	return 0;
}