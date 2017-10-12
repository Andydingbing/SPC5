#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalRxSgpmmThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define N_RETRY 5
#define PRECISION  0.05
#define RESOLUTION 0.01

IMPLEMENT_DYNCREATE(CCalRxSgpmmThread, CCalBaseThread)

CCalRxSgpmmThread::CCalRxSgpmmThread()
{
}

CCalRxSgpmmThread::CCalRxSgpmmThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalRxSgpmmThread::~CCalRxSgpmmThread()
{
}

BOOL CCalRxSgpmmThread::InitInstance()
{
	return TRUE;
}

int CCalRxSgpmmThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalRxSgpmmThread, CWinThread)
END_MESSAGE_MAP()


int CCalRxSgpmmThread::Run()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating SG---PM Map"));
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("SG<===>Power Meter<===>Load(50 Ohm)"));
		Instrument.Init();
		Instrument.HasSG();
		Instrument.HasPM();
		Instrument.SG_Reset();
		Instrument.SG_SetModEnable(false);
		Instrument.SG_SetOutputEnable(true);
		Instrument.PM_Reset();
	
		CCalRxSgpmmDlg *pParent = (CCalRxSgpmmDlg *)m_pParent;
		CSGPMMFile SgpmmFile((LPSTR)(LPCTSTR)(pParent->m_strPath));
		CSGPMMFile::Point Pt;
		CSGPMMFile::FreqSeg Pts;
		Pts.clear();

		Result Data;
		double dPMIL = PM_IL;
		double dPMPower = -150.0;
		double dSGPower = -150.0;

		CString strFreqStar = pParent->m_strFreqStar;
		CString strFreqStop = pParent->m_strFreqStop;
		CString strFreqStep = pParent->m_strFreqStep;
		float fPowerStar = 0.0,fPowerStop = 0.0,fPowerStep = 0.0;
		str2freq(pParent->m_strPowerStar,fPowerStar);
		str2freq(pParent->m_strPowerStop,fPowerStop);
		str2freq(pParent->m_strPowerStep,fPowerStep);

		uint64_t uiFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A)];
		int32_t iPts = freq2array(strFreqStar,strFreqStop,strFreqStep,(int64_t *)uiFreq);

		InitSinglePos("Calibrating SGPMM",iPts);

		for (int32_t i = 0;i < iPts;i ++) {
			CAL_THREAD_TEST_CANCEL();
			dPMIL = PM_IL;
			Pt.m_Key.m_uiFreq = uiFreq[i];
			Instrument.SG_SetCW(double(uiFreq[i]));
			Instrument.PM_SetFrequency(double(uiFreq[i]));
			int32_t j = 0;
			for (float fPow = fPowerStar;fPow >= fPowerStop;fPow += fPowerStep) {
				CAL_THREAD_TEST_CANCEL();
				Pt.m_Key.m_fPow = fPow;
				dSGPower = fPow + dPMIL;
				Instrument.SG_SetPL(dSGPower);
				Instrument.PM_GetPwr(dPMPower);
				int iRetry = 0;
				while (abs(fPow - dPMPower) > PRECISION && N_RETRY >= iRetry) {
					CAL_THREAD_TEST_CANCEL();
					dSGPower += (fPow - dPMPower);
					Instrument.SG_SetPL(dSGPower);
					Sleep(2);
					Instrument.PM_GetPwr(dPMPower);
					iRetry ++;
				}
				if (N_RETRY <= iRetry) {
					while (abs(fPow - dPMPower) > PRECISION) {
						CAL_THREAD_TEST_CANCEL();
						dSGPower += RESOLUTION * (fPow > dPMPower ? 1 : -1);
						Instrument.SG_SetPL(dSGPower);
						Sleep(2);
						Instrument.PM_GetPwr(dPMPower);
					}
				}
				dPMIL = dSGPower - dPMPower;
				Pt.m_fPow = (float)dSGPower;
				Pts.push_back(Pt);
				Data.m_iFreqIdx = i;
				Data.m_iPowIdx = j;
				Data.m_Pt = Pt;
				pParent->m_FreqSeg.at(Data.m_iFreqIdx).at(Data.m_iPowIdx) = Data.m_Pt;
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_SGPMM_SHOW,0,LPARAM(&Data));
				j ++;
			}
			SetPos(i + 1,m_nThreadID);
		}
		SgpmmFile.Fuse(Pts);
		SgpmmFile.Commit();
		CAL_THREAD_ABOART();
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
}
