#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxPowerThread.h"
#include "CheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1ATxPowerThread, CCalBaseThread)

CCalR1ATxPowerThread::CCalR1ATxPowerThread()
{
}

CCalR1ATxPowerThread::CCalR1ATxPowerThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1ATxPowerThread::~CCalR1ATxPowerThread()
{
}

BOOL CCalR1ATxPowerThread::InitInstance()
{
	return TRUE;
}

int CCalR1ATxPowerThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1ATxPowerThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1ATxPowerThread::Run()
{
	return CalTxPower();
}

int CCalR1ATxPowerThread::CalTxPower()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Power"));
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Power Meter"));
		Instrument.Init();
		Instrument.HasPM();
		Instrument.PM_Reset();

		CCalR1ATxPowerDlg *pParent = (CCalR1ATxPowerDlg *)m_pParent;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		CalIOMode Mode = CAL_OP;
		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		double dTxPower = 10.0,dPMPower = -100.0;
		SP1401::TxPAAtt PAAtt = SP1401::Tx_PA;
		int32_t _0_Att1 = 0,_0_Att2 = 0,_0_Att3 = 0;
		double _0_Att = 0.0;
		int32_t iAtt1 = 0,iAtt2 = 0,iAtt3 = 0;

		double dAtt = 0.0;		//att all
		double dAtt_Rf = 0.0;	//att from rf
		double dDGain = 0.0;	//digital gain

		double dTh = 0.0;
		uint16_t uiAmI = 0,uiAmQ = 0;
		int16_t iDCI = 0,iDCQ = 0;
		double dDGainOffset = 0.0;	//att offset

		int16_t iAdjust = 0;

		uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
		uint64_t uiFreqStar[16];
		uint64_t uiFreqStop[16];
		memset(uiFreq,0,ARRAY_SIZE(uiFreq));
		CalTxPowerResult Data;
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq,(int64_t *)uiFreqStar,(int64_t *)uiFreqStop);

		InitSinglePos("Calibrating Tx Power(Output)",iPts);

		pSP1401->SetIOMode(OutPut);
		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetDUCDDS(0.0);

		for (int32_t i = 0; i < iPts ;i ++) {
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetTxFreq(uiFreq[i]);
			Instrument.PM_SetFrequency((double)(uiFreq[i] + 20000000));

			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq[i],dTh,uiAmI,uiAmQ);
			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[i],iDCI,iDCQ);
			pSP2401->SetTxPhaseRotateI(dTh);
			pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ);
			pSP2401->SetTxDCOffset(iDCI,iDCQ);

			PAAtt = SP1401::Tx_PA;
			dDGain = 0;
			TxPowerTableR1A::GuessBaseOP(_0_Att1,_0_Att2,_0_Att3);
			dAtt_Rf = abs(_0_Att1) + abs(_0_Att2) + abs(_0_Att3);
			((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],OutPut,(int32_t)dAtt_Rf,dDGainOffset);	//just ignore the att offset when restricting the digital gain later
			pSP2401->SetTxPowerComp(dDGainOffset);
			pSP1401->SetTxPAAttSw(PAAtt);
			pSP1401->SetTxAtt(_0_Att1,_0_Att2,_0_Att3);
			Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
			do {
				if ((dAtt_Rf == 0    && PAAtt == SP1401::Tx_PA && ((0 - dPMPower + dDGain) < R1A_DGAIN_MIN)) ||	//if already max gain and the offset > 2dB,then switch to att chain
					(dAtt_Rf >= 20.0 && PAAtt == SP1401::Tx_PA && ((0 - dPMPower + dDGain) < 0))) {
						PAAtt = SP1401::Tx_Att;
						pSP1401->SetTxPAAttSw(PAAtt);
						Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
						continue;
				}
				dAtt = dAtt_Rf - dDGain + dPMPower - 0;	//don't worry about the att bound here
				TxPowerTableR1A::Split(dAtt,&dAtt_Rf,&dDGain);

				pSP1401->SetTxAtt(dAtt_Rf);
				((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],OutPut,(int32_t)dAtt_Rf,dDGainOffset);
				pSP2401->SetTxPowerComp(dDGainOffset + dDGain);
				Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
			}while (abs(dPMPower - 0) >= 0.5);

			_0_Att = dAtt_Rf;

			for (int32_t iPow = 0;iPow < 4;iPow ++) {
				dTxPower = 10 - 10 * iPow;
				if (dTxPower <= -10)
					dAtt_Rf -= 10;
				else
					dAtt_Rf = _0_Att + (0 - dTxPower);
				dAtt_Rf = dAtt_Rf <= 0 ? 0 : dAtt_Rf;	//if min att then keep the state first and test the power,ajust the chain later

				pSP1401->SetTxAtt(dAtt_Rf);
				((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],OutPut,(int32_t)dAtt_Rf,dDGainOffset);
				pSP2401->SetTxPowerComp(dDGainOffset + dDGain);	//keep the digital gain
				pSP1401->SetTxPAAttSw(PAAtt);
				Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);

				iAdjust = 0;
				do {	//adjust at least once in case that power is still correct when pa is on and rf att is switching from 10 to 20
					if (++iAdjust >= 10)
						break;
					if ((dAtt_Rf == 0	 && PAAtt == SP1401::Tx_PA && ((dTxPower - dPMPower + dDGain) < R1A_DGAIN_MIN)) || 
						(dAtt_Rf >= 10.0 && PAAtt == SP1401::Tx_PA && ((dTxPower - dPMPower + dDGain) < 0)) ||
						(dAtt_Rf >= 20.0 && PAAtt == SP1401::Tx_PA)) {
							PAAtt = SP1401::Tx_Att;	//only consider the min att condition,restrict the digital gain here
							pSP1401->SetTxPAAttSw(PAAtt);
							Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
							continue;
					}
					dAtt = dAtt_Rf - dDGain + dPMPower - dTxPower;
					if (dAtt < 0) {
						dDGain += (dTxPower - dPMPower);
						dDGain  = dDGain > 11.0 ? 10.0 : dDGain;
						dAtt_Rf = 0;
					}
					else
						TxPowerTableR1A::Split(dAtt,&dAtt_Rf,&dDGain);

					pSP1401->SetTxAtt(dAtt_Rf);
					((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],OutPut,(int32_t)dAtt_Rf,dDGainOffset);
					pSP2401->SetTxPowerComp(dDGainOffset + dDGain);
					Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
				}while(abs(dPMPower - dTxPower) >= 0.26);
				if (iAdjust >= 10) {
					do {
						dDGain += (dTxPower - dPMPower);
						pSP2401->SetTxPowerComp(dDGainOffset + dDGain);
						Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
					} while (abs(dPMPower - dTxPower) >= 0.26);
				}

				TxPowerTableR1A::Split((int32_t)dAtt_Rf,iAtt1,iAtt2,iAtt3);
				Data.m_State[iPow].m_uiFreq = uiFreq[i];
				Data.m_State[iPow].m_dDGain = dDGain;
				Data.m_State[iPow].m_iAtt1  = iAtt1;
				Data.m_State[iPow].m_iAtt2  = iAtt2;
				Data.m_State[iPow].m_iPAAtt = PAAtt;
				Data.m_State[iPow].m_iAtt3  = iAtt3;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
			}
			pSP1401->GetCalFile()->Add(ICalFile::TxPowerOP,(TxPowerTableR1A::DataF *)&Data);
			::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
			SetPos(i + 1,m_nThreadID);
		}
		pSP1401->GetCalFile()->Write(ICalFile::TxPowerOP);

		Mode = CAL_IO;
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Output Complete,IO<===>Power Meter"));
		InitSinglePos("Calibrating Tx Power(IO)",iPts);

		pSP1401->SetIOMode(IO);
		pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
		pSP2401->SetDDS1(20000000.0);
		pSP2401->SetDUCDDS(0.0);

		for (int32_t i = 0; i < iPts ;i ++) {
			CAL_THREAD_TEST_CANCEL();
			pSP1401->SetTxFreq(uiFreq[i]);
			Instrument.PM_SetFrequency((double)(uiFreq[i] + 20000000));

			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq[i],dTh,uiAmI,uiAmQ);
			((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq[i],iDCI,iDCQ);
			pSP2401->SetTxPhaseRotateI(dTh);
			pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ);
			pSP2401->SetTxDCOffset(iDCI,iDCQ);

			PAAtt = SP1401::Tx_PA;
			dDGain = 0;
			TxPowerTableR1A::GuessBaseIO(_0_Att1,_0_Att2,_0_Att3);
			dAtt_Rf = abs(_0_Att1) + abs(_0_Att2) + abs(_0_Att3);
			((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],IO,(int32_t)dAtt_Rf,dDGainOffset);	//just ignore the att offset when restricting the digital gain later
			pSP2401->SetTxPowerComp(dDGainOffset);
			pSP1401->SetTxPAAttSw(PAAtt);
			pSP1401->SetTxAtt(_0_Att1,_0_Att2,_0_Att3);
			Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
			do {
				if ((dAtt_Rf == 0    && ((0 - dPMPower + dDGain) < R1A_DGAIN_MIN)) ||	//if already max gain and the offset > 2dB,then switch to att chain
					(dAtt_Rf >= 10.0 && PAAtt == SP1401::Tx_PA && ((0 - dPMPower + dDGain) < 0))) {
						PAAtt = SP1401::Tx_Att;
						pSP1401->SetTxPAAttSw(PAAtt);
						Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
						continue;
				}
				dAtt = dAtt_Rf - dDGain + dPMPower - 0;
				if (dAtt < 0) {
					dDGain += (0 - dPMPower);
					dAtt_Rf = 0;
				}
				else
					TxPowerTableR1A::Split(dAtt,&dAtt_Rf,&dDGain);

				pSP1401->SetTxAtt(dAtt_Rf);
				((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],IO,(int32_t)dAtt_Rf,dDGainOffset);
				pSP2401->SetTxPowerComp(dDGainOffset + dDGain);
				Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
			}while (abs(dPMPower - 0) >= 0.5);

			_0_Att = dAtt_Rf;

			for (int32_t iPow = 0;iPow < 4;iPow ++) {
				dTxPower = 0 - 10 * iPow;
				if (dTxPower <= -10)
					dAtt_Rf -= 10;
				else
					dAtt_Rf = _0_Att + (0 - dTxPower);
				dAtt_Rf  = dAtt_Rf <= 0 ? 0 : dAtt_Rf;	//if min att then keep the state first and test the power,ajust the chain later
				dAtt_Rf += (PAAtt == SP1401::Tx_Att ? -10 : 0);	//in io mode,in order to prevent the lower power(<-30dBm),in which case the power-meter is fucking stupid slow 

				pSP1401->SetTxAtt(dAtt_Rf);
				((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],IO,(int32_t)dAtt_Rf,dDGainOffset);
				pSP2401->SetTxPowerComp(dDGainOffset + dDGain);	//keep the digital gain
				pSP1401->SetTxPAAttSw(PAAtt);
				Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);

				iAdjust = 0;
				do {
					if (++iAdjust >= 10)
						break;
					if ((dAtt_Rf == 0    && PAAtt == SP1401::Tx_PA && ((dTxPower - dPMPower + dDGain) < R1A_DGAIN_MIN)) || 
						(dAtt_Rf >= 10.0 && PAAtt == SP1401::Tx_PA && ((dTxPower - dPMPower + dDGain) < 0)) ||
						(dAtt_Rf >= 20.0 && PAAtt == SP1401::Tx_PA)) {
							PAAtt = SP1401::Tx_Att;	//only consider the min att condition,restrict the digital gain here
							pSP1401->SetTxPAAttSw(PAAtt);
							Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
							continue;
					}
					dAtt = dAtt_Rf - dDGain + dPMPower - dTxPower;
					if (dAtt < 0) {
						dDGain += (dTxPower - dPMPower);
						dDGain = dDGain > 11.0 ? 10 : dDGain;
						dAtt_Rf = 0;
					}
					else
						TxPowerTableR1A::Split(dAtt,&dAtt_Rf,&dDGain);

					pSP1401->SetTxAtt(dAtt_Rf);
					((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiFreq[i],IO,(int32_t)dAtt_Rf,dDGainOffset);
					pSP2401->SetTxPowerComp(dDGainOffset + dDGain);
					Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
				}while (abs(dPMPower - dTxPower) >= 0.26);
				if (iAdjust >= 10) {
					do {
						dDGain += (dTxPower - dPMPower);
						pSP2401->SetTxPowerComp(dDGainOffset + dDGain);
						Instrument.PM_GetPwr((double)uiFreq[i],dPMPower);
					} while (abs(dPMPower - dTxPower) >= 0.26);
				}

				TxPowerTableR1A::Split((int32_t)dAtt_Rf,iAtt1,iAtt2,iAtt3);
				Data.m_State[iPow].m_uiFreq = uiFreq[i];
				Data.m_State[iPow].m_dDGain = dDGain;
				Data.m_State[iPow].m_iAtt1  = iAtt1;
				Data.m_State[iPow].m_iAtt2  = iAtt2;
				Data.m_State[iPow].m_iPAAtt = PAAtt;
				Data.m_State[iPow].m_iAtt3  = iAtt3;
				Data.m_iSec = freqsection((int64_t *)uiFreqStar,(int64_t *)uiFreqStop,(int64_t)uiFreq[i]);
			}
			pSP1401->GetCalFile()->Add(ICalFile::TxPowerIO,(TxPowerTableR1A::DataF *)&Data);
			::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
			SetPos(i + 1,m_nThreadID);
		}
		pSP1401->GetCalFile()->Write(ICalFile::TxPowerIO);

		Instrument.Close();
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
	return 0;
}


IMPLEMENT_DYNCREATE(CExportR1ATxPowerThread, CWinThread)

CExportR1ATxPowerThread::CExportR1ATxPowerThread()
{
}

CExportR1ATxPowerThread::CExportR1ATxPowerThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1ATxPowerThread::~CExportR1ATxPowerThread()
{
}

BOOL CExportR1ATxPowerThread::InitInstance()
{
	return TRUE;
}

int CExportR1ATxPowerThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1ATxPowerThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1ATxPowerThread::Run()
{
	CCalR1ATxPowerDlg *pParent = (CCalR1ATxPowerDlg *)m_pParent;
	CCalR1ATxPowerThread::CalTxPowerResult Data;
	CalIOMode Mode = CAL_OP;
	double dPower = 10.0,dDGain = 0.0;
	SP1401::TxPAAtt PAAtt = SP1401::Tx_Att;
	int32_t iAtt1 = 0,iAtt2 = 0,iAtt3 = 0;

	uint64_t uiFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED)];
	memset(uiFreq,0,sizeof(uiFreq));
	int32_t iPts = freq2array(pParent->m_strFreqStar,pParent->m_strFreqStop,pParent->m_strFreqStep,(int64_t *)uiFreq);

	InitSinglePos("Exporting Tx Power",SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE));
	
	for (int i = 0;i < ARRAY_SIZE(Data.m_State);i ++) {
		Data.m_State[i].m_iAtt1  = 0;
		Data.m_State[i].m_iAtt2  = 0;
		Data.m_State[i].m_iPAAtt = 0;
		Data.m_State[i].m_iAtt3  = 0;
		Data.m_State[i].m_dDGain = 0.0;
	}
	Data.m_iSec = 0;
	if (CalOP((CalIOMode)(pParent->m_CalIOMode.GetCurSel()))) {
		Mode = CAL_OP;
		for (int32_t iFreq = 0;iFreq < iPts;iFreq ++) {
			dPower = 10.0;
			for (int i = 0;i < 4;i ++) {
				((CalFileR1A *)(pParent->m_pSP1401->GetCalFile()))->GetTxPower(uiFreq[iFreq],dPower,OutPut,dDGain,iAtt1,iAtt3,PAAtt,iAtt3);
				Data.m_State[i].m_uiFreq = uiFreq[iFreq];
				Data.m_State[i].m_iAtt1 = iAtt1;
				Data.m_State[i].m_iAtt2 = iAtt2;
				Data.m_State[i].m_iPAAtt = (int32_t)PAAtt;
				Data.m_State[i].m_iAtt3 = iAtt3;
				Data.m_State[i].m_dDGain = dDGain;
				dPower -= 10;
			}
			::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
			SetPos(SERIE_INDEX(uiFreq[iFreq],RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE) + 1,m_nThreadID);
		}
	}
	
	if (CalIO((CalIOMode)(pParent->m_CalIOMode.GetCurSel()))) {
		Mode = CAL_IO;
		for (int32_t iFreq = 0;iFreq < iPts;iFreq ++) {
			dPower = 0.0;
			for (int i = 0;i < 4;i ++) {
				((CalFileR1A *)(pParent->m_pSP1401->GetCalFile()))->GetTxPower(uiFreq[iFreq],dPower,IO,dDGain,iAtt1,iAtt3,PAAtt,iAtt3);
				Data.m_State[i].m_uiFreq = uiFreq[iFreq];
				Data.m_State[i].m_iAtt1 = iAtt1;
				Data.m_State[i].m_iAtt2 = iAtt2;
				Data.m_State[i].m_iPAAtt = (int32_t)PAAtt;
				Data.m_State[i].m_iAtt3 = iAtt3;
				Data.m_State[i].m_dDGain = dDGain;
				dPower -= 10;
			}
			::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_POWER_SHOW,(WPARAM)&Mode,(LPARAM)&Data);
			SetPos(SERIE_INDEX(uiFreq[iFreq],RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE) + 1,m_nThreadID);
		}
	}

	CAL_THREAD_ABOART();
	return 0;
}