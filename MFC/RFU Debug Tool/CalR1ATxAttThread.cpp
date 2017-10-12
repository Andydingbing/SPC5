#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxAttThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1ATxAttThread, CCalBaseThread)

double CCalR1ATxAttThread::CalSARef(double dPeak)
{
	double dReff = dPeak + 10.0,dX2 = 0.0;
	discretept(dReff,-10,dReff,dX2);
	return dReff;
}

BOOL CCalR1ATxAttThread::ShowLine(int32_t iLine,int32_t iMask)
{
	return (1 << iMask & iLine) == 0 ? FALSE : TRUE;
}

CCalR1ATxAttThread::CCalR1ATxAttThread()
{
}

CCalR1ATxAttThread::CCalR1ATxAttThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1ATxAttThread::~CCalR1ATxAttThread()
{
}

BOOL CCalR1ATxAttThread::InitInstance()
{
	return TRUE;
}

int CCalR1ATxAttThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1ATxAttThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1ATxAttThread::Run()
{
	return CalTxAtt();
}

int CCalR1ATxAttThread::CalTxAtt()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Att Offset"));
		CCalR1ATxAttDlg *pParent = (CCalR1ATxAttDlg *)m_pParent;
		CalIOMode Mode = (CalIOMode)(pParent->m_CalIOMode.GetCurSel());
		CalIOMode CallingMode = CAL_OP;

		if (CalOP(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<--->Spectrum"));
		}
		if (CalIO(Mode) && !CalOP(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("IO<--->Spectrum"));
		}
		Instrument.Init();
		Instrument.HasSA();
		Instrument.SA_Reset();
		Instrument.SA_SetSpan(50000.0);
		Instrument.SA_SetRBW(1000.0,false);
		Instrument.SA_SetVBW(1000.0,true);
		Instrument.SA_SetRefLevel(10.0);

		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;

		double dTh[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint16_t uiAmI[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint16_t uiAmQ[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		memset(dTh,0,ARRAY_SIZE(dTh));
		memset(uiAmI,0,ARRAY_SIZE(uiAmI));
		memset(uiAmQ,0,ARRAY_SIZE(uiAmQ));

		int16_t iDCI[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		int16_t iDCQ[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		memset(iDCI,0,ARRAY_SIZE(iDCI));
		memset(iDCQ,0,ARRAY_SIZE(iDCQ));

		double dSARef[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		for (int32_t i = 0;i < ARRAY_SIZE(dSARef);i ++)
			dSARef[i] = 10.0;

		double dPowerSA[2] = {0.0,0.0};
		double dRefAjusted = 0.0;

		Result Data[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(Data,0,sizeof(Data));
		memset(uiFreq,0,sizeof(uiFreq));
		memset(uiFreqStar,0,sizeof(uiFreqStar));
		memset(uiFreqStop,0,sizeof(uiFreqStop));
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);

		InitSinglePos("Calibrating Tx Att Offset",iPts);

		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetDUCDDS(0.0);
		pSP2401->SetTxPowerComp(0.0);
		pSP1401->SetTxPAAttSw(SP1401::Tx_Att);

		for (int32_t i = 0;i < iPts;i ++) {
			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq[i],dTh[i],uiAmI[i],uiAmQ[i]);
			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[i],iDCI[i],iDCQ[i]);
		}
		if (CalOP(Mode)) {
			pSP1401->SetIOMode(OutPut);
			CallingMode = CAL_OP;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP1401->SetTxAtt(0,0,0);
				pSP2401->SetTxPhaseRotateI(dTh[i]);
				pSP2401->SetTxAmplitudeBalance(uiAmI[i],uiAmQ[i]);
				pSP2401->SetTxDCOffset(iDCI[i],iDCQ[i]);

				Data[i].m_uiFreq = uiFreq[i];
				Instrument.SA_SetCF((double)uiFreq[i] + 20000000.0);
				Instrument.SA_SetRefLevel(dSARef[i]);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				if ((dRefAjusted = CCalR1ATxAttThread::CalSARef(dPowerSA[0])) != dSARef[i]) {	//get appropriate initial reference 
					dSARef[i] = dRefAjusted;
					Instrument.SA_SetRefLevel(dSARef[i]);
					Instrument.SA_SetPeakSearch(ISA::PeakHighest);
					Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				}
				pSP1401->SetTxAtt(10,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt10 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);
				pSP1401->SetTxAtt(20,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				Data[i].m_fAtt20 = (float)((dPowerSA[1] - dPowerSA[0]) - 10.0);
				pSP1401->SetTxAtt(30,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt30 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);

				Data[i].m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
				Data[i].m_iLine = 7;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)(&CallingMode),(LPARAM)(&Data[i]));
				SetPos(i + 1,m_nThreadID);
			}

			for (int32_t i = 0;i < iPts;i ++)
				dSARef[i] -= 30;
			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP2401->SetTxPhaseRotateI(dTh[i]);
				pSP2401->SetTxAmplitudeBalance(uiAmI[i],uiAmQ[i]);
				pSP2401->SetTxDCOffset(iDCI[i],iDCQ[i]);
				Instrument.SA_SetCF((double)uiFreq[i] + 20000000.0);

				if (0 == i)
					Instrument.SA_SetRefLevel(dSARef[i]);
				else if (dSARef[i] != dSARef[i - 1])
					Instrument.SA_SetRefLevel(dSARef[i]);

				pSP1401->SetTxAtt(30,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				pSP1401->SetTxAtt(30,10,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt40 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);
				pSP1401->SetTxAtt(30,20,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				Data[i].m_fAtt50 = (float)((dPowerSA[1] - dPowerSA[0]) - 10.0);
				pSP1401->SetTxAtt(30,30,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt60 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);

				Data[i].m_iLine = 7 << 3;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)(&CallingMode),(LPARAM)(&Data[i]));
				SetPos(i + 1,m_nThreadID);
			}

			for (int32_t i = 0;i < iPts;i ++)
				dSARef[i] -= 30;
			Instrument.SA_SetEnablePreamp(true);
			Instrument.SA_SetAverTrace(true,3);
			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP2401->SetTxPhaseRotateI(dTh[i]);
				pSP2401->SetTxAmplitudeBalance(uiAmI[i],uiAmQ[i]);
				pSP2401->SetTxDCOffset(iDCI[i],iDCQ[i]);
				Instrument.SA_SetCF((double)uiFreq[i] + 20000000.0);

				if (0 == i)
					Instrument.SA_SetRefLevel(dSARef[i]);
				else if (dSARef[i] != dSARef[i - 1])
					Instrument.SA_SetRefLevel(dSARef[i]);

				pSP1401->SetTxAtt(30,30,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				pSP1401->SetTxAtt(30,30,10);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt70 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);
				pSP1401->SetTxAtt(30,30,20);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				Data[i].m_fAtt80 = (float)((dPowerSA[1] - dPowerSA[0]) - 10.0);
				pSP1401->SetTxAtt(30,30,30);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt90 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);

				Data[i].m_iLine = (7 << 3) << 3;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)(&CallingMode),(LPARAM)(&Data[i]));
				SetPos(i + 1,m_nThreadID);
			}
			for (int32_t i = 0;i < iPts;i ++)
				pSP1401->GetCalFile()->Add(ICalFile::TxAttOP,(TxAttTableR1A::DataF *)(&Data[i]));
			pSP1401->GetCalFile()->Write(ICalFile::TxAttOP);
		}

		if (CalOP(Mode) && CalIO(Mode)) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Out Port Done,IO<--->Spectrum"));
		}

		for (int32_t i = 0;i < ARRAY_SIZE(dSARef);i ++)
			dSARef[i] = 0.0;

		if (CalIO(Mode)) {
			Instrument.SA_Reset();
			Instrument.SA_SetSpan(50000.0);
			Instrument.SA_SetRBW(1000.0,false);
			Instrument.SA_SetVBW(1000.0,true);
			Instrument.SA_SetRefLevel(0.0);
			Instrument.SA_SetEnablePreamp(false);
			Instrument.SA_SetAverTrace(false,0);
			pSP1401->SetIOMode(IO);
			CallingMode = CAL_IO;

			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP1401->SetTxAtt(0,0,0);
				pSP2401->SetTxPhaseRotateI(dTh[i]);
				pSP2401->SetTxAmplitudeBalance(uiAmI[i],uiAmQ[i]);
				pSP2401->SetTxDCOffset(iDCI[i],iDCQ[i]);

				Data[i].m_uiFreq = uiFreq[i];
				Instrument.SA_SetCF((double)uiFreq[i] + 20000000.0);
				Instrument.SA_SetRefLevel(dSARef[i]);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				if ((dRefAjusted = CCalR1ATxAttThread::CalSARef(dPowerSA[0])) != dSARef[i]) {	//get appropriate initial reference 
					dSARef[i] = dRefAjusted;
					Instrument.SA_SetRefLevel(dSARef[i]);
					Instrument.SA_SetPeakSearch(ISA::PeakHighest);
					Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				}
				pSP1401->SetTxAtt(10,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt10 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);
				pSP1401->SetTxAtt(20,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				Data[i].m_fAtt20 = (float)((dPowerSA[1] - dPowerSA[0]) - 10.0);
				pSP1401->SetTxAtt(30,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt30 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);

				Data[i].m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,int64_t(uiFreq[i]));
				Data[i].m_iLine = 7;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)(&CallingMode),(LPARAM)(&Data[i]));
				SetPos(i + 1,m_nThreadID);
			}

			for (int32_t i = 0;i < iPts;i ++)
				dSARef[i] -= 30;
			Instrument.SA_SetEnablePreamp(true);
			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP2401->SetTxPhaseRotateI(dTh[i]);
				pSP2401->SetTxAmplitudeBalance(uiAmI[i],uiAmQ[i]);
				pSP2401->SetTxDCOffset(iDCI[i],iDCQ[i]);
				Instrument.SA_SetCF((double)uiFreq[i] + 20000000.0);

				if (0 == i)
					Instrument.SA_SetRefLevel(dSARef[i]);
				else if (dSARef[i] != dSARef[i - 1])
					Instrument.SA_SetRefLevel(dSARef[i]);

				pSP1401->SetTxAtt(30,0,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				pSP1401->SetTxAtt(30,10,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt40 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);
				pSP1401->SetTxAtt(30,20,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				Data[i].m_fAtt50 = (float)((dPowerSA[1] - dPowerSA[0]) - 10.0);
				pSP1401->SetTxAtt(30,30,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt60 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);

				Data[i].m_iLine = 7 << 3;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)(&CallingMode),(LPARAM)(&Data[i]));
				SetPos(i + 1,m_nThreadID);
			}

			for (int32_t i = 0;i < iPts;i ++)
				dSARef[i] -= 30;
			Instrument.SA_SetEnablePreamp(true);
			Instrument.SA_SetAverTrace(true,6);
			pSP2401->SetTxPowerComp(5.0);
			for (int32_t i = 0;i < iPts;i ++) {
				CAL_THREAD_TEST_CANCEL();
				pSP1401->SetTxFreq(uiFreq[i]);
				pSP2401->SetTxPhaseRotateI(dTh[i]);
				pSP2401->SetTxAmplitudeBalance(uiAmI[i],uiAmQ[i]);
				pSP2401->SetTxDCOffset(iDCI[i],iDCQ[i]);
				Instrument.SA_SetCF((double)uiFreq[i] + 20000000.0);

				if (0 == i)
					Instrument.SA_SetRefLevel(dSARef[i]);
				else if (dSARef[i] != dSARef[i - 1])
					Instrument.SA_SetRefLevel(dSARef[i]);

				pSP1401->SetTxAtt(30,30,0);
				Instrument.SA_SweepOnce();
				Instrument.SA_SetPeakSearch(ISA::PeakHighest);
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				pSP1401->SetTxAtt(30,30,10);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt70 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);
				pSP1401->SetTxAtt(30,30,20);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[0]);
				Data[i].m_fAtt80 = (float)((dPowerSA[1] - dPowerSA[0]) - 10.0);
				pSP1401->SetTxAtt(30,30,30);
				Instrument.SA_SweepOnce();
				Instrument.SA_GetMarkerPwr(dPowerSA[1]);
				Data[i].m_fAtt90 = (float)((dPowerSA[0] - dPowerSA[1]) - 10.0);

				Data[i].m_iLine = (7 << 3) << 3;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)(&CallingMode),(LPARAM)(&Data[i]));
				SetPos(i + 1,m_nThreadID);
			}
			for (int32_t i = 0;i < iPts;i ++)
				pSP1401->GetCalFile()->Add(ICalFile::TxAttIO,(TxAttTableR1A::DataF *)(&Data[i]));
			pSP1401->GetCalFile()->Write(ICalFile::TxAttIO);
		}
		Instrument.Close();
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}


IMPLEMENT_DYNCREATE(CExportR1ATxAttThread, CCSEWinThread)

CExportR1ATxAttThread::CExportR1ATxAttThread()
{
}

CExportR1ATxAttThread::CExportR1ATxAttThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1ATxAttThread::~CExportR1ATxAttThread()
{
}

BOOL CExportR1ATxAttThread::InitInstance()
{
	return TRUE;
}

int CExportR1ATxAttThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1ATxAttThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1ATxAttThread::Run()
{
	CCalR1ATxAttDlg *pParent = (CCalR1ATxAttDlg *)m_pParent;
	CCalR1ATxAttThread::Result Data;
	CalIOMode Mode = CAL_OP;
	int iFreqIdx = 0;
	float fAttOffset[9];
	for (int32_t i = 0;i < ARRAY_SIZE(fAttOffset);i ++)
		fAttOffset[i] = 0.0;

	InitSinglePos("Exporting Tx Att Offset",SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE));

	Data.m_iSec = 0;
	Data.m_iLine = (7 | (7 << 3) | (7 << 6));
	for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		((CalFileR1A *)(pParent->m_pSP1401->GetCalFile()))->GetTxAtt(uiFreq,OutPut,fAttOffset);
		Data.m_uiFreq = uiFreq;
		Data.m_fAtt10 = fAttOffset[0];
		Data.m_fAtt20 = fAttOffset[1];
		Data.m_fAtt30 = fAttOffset[2];
		Data.m_fAtt40 = fAttOffset[3];
		Data.m_fAtt50 = fAttOffset[4];
		Data.m_fAtt60 = fAttOffset[5];
		Data.m_fAtt70 = fAttOffset[6];
		Data.m_fAtt80 = fAttOffset[7];
		Data.m_fAtt90 = fAttOffset[8];
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&Mode,(LPARAM)(&Data));
		iFreqIdx ++;
		SetPos(iFreqIdx,m_nThreadID);
	}
	Mode = CAL_IO;
	iFreqIdx = 0;
	for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_INFILE) {
		((CalFileR1A *)(pParent->m_pSP1401->GetCalFile()))->GetTxAtt(uiFreq,IO,fAttOffset);
		Data.m_uiFreq = uiFreq;
		Data.m_fAtt10 = fAttOffset[0];
		Data.m_fAtt20 = fAttOffset[1];
		Data.m_fAtt30 = fAttOffset[2];
		Data.m_fAtt40 = fAttOffset[3];
		Data.m_fAtt50 = fAttOffset[4];
		Data.m_fAtt60 = fAttOffset[5];
		Data.m_fAtt70 = fAttOffset[6];
		Data.m_fAtt80 = fAttOffset[7];
		Data.m_fAtt90 = fAttOffset[8];
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_ATT_SHOW,(WPARAM)&Mode,(LPARAM)(&Data));
		iFreqIdx ++;
		SetPos(iFreqIdx,m_nThreadID);
	}
	CAL_THREAD_ABOART();
	return 0;
}