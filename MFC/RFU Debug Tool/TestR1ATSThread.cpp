#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1ATSThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTestR1ATSThread, CWinThread)

CTestR1ATSThread::CTestR1ATSThread()
{
}

CTestR1ATSThread::CTestR1ATSThread(CWnd *pParent)
{
	m_SGPower.clear();
	m_pParent = pParent;
}

CTestR1ATSThread::~CTestR1ATSThread()
{
}

BOOL CTestR1ATSThread::InitInstance()
{
	return TRUE;
}

int CTestR1ATSThread::ExitInstance()
{
	m_SGPower.clear();
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CTestR1ATSThread, CWinThread)
END_MESSAGE_MAP()


int CTestR1ATSThread::Run()
{
#define POST_CLR		\
	if (fpTx)			\
		fclose(fpTx);	\
	if (fpRx)			\
		fclose(fpRx);

	try {
		CTestR1ATSDlg *pParent = (CTestR1ATSDlg *)m_pParent;
		CSP1401R1A *pSP1401 = pParent->m_pSP1401;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		CString strTxFreqStar = pParent->m_strTxFreqStar;
		CString strTxFreqStop = pParent->m_strTxFreqStop;
		CString strTxFreqStep = pParent->m_strTxFreqStep;
		CString strRxFreqStar = pParent->m_strRxFreqStar;
		CString strRxFreqStop = pParent->m_strRxFreqStop;
		CString strRxFreqStep = pParent->m_strRxFreqStep;
		FILE *fpTx = NULL,*fpRx = NULL;
		int32_t iTxPts = 0,iRxPts = 0;
		ISP1401::RFCh Ch = ISP1401::Tx;
		int32_t iCnt = 0;

		uint64_t uiTxFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE)];
		uint64_t uiRxFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A)];
		memset(uiTxFreq,0,sizeof(uiTxFreq));
		memset(uiRxFreq,0,sizeof(uiRxFreq));

		CTestR1ATSDlg::TxResult TxData;
		CTestR1ATSDlg::RxResult RxData;

		TEST_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Testing T/Rx Temp Stability"));
		Instrument.Init();

		if (pParent->m_bTestTx) {
			TEST_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Power Meter"));
			Instrument.HasPM();
			Instrument.PM_Reset();
			pSP1401->SetIOMode(OutPut);
			pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
			pSP2401->SetDDS1(20000000.0);
			pSP2401->SetDUCDDS(0.0);

			if (NULL == (fpTx = fopen((LPSTR)(LPCTSTR)pParent->m_strTxPath,"w"))) {
				CseMessageBox("can't open %s",pParent->m_strTxPath);
				TEST_THREAD_ABOART();
			}
			iTxPts = freq2array(strTxFreqStar,strTxFreqStop,strTxFreqStep,(int64_t *)uiTxFreq);
			for (int32_t i = 0;i < iTxPts;i ++)
				fprintf(fpTx,"%4lld:Power Temp  ",uiTxFreq[i] / 1000000);
			fprintf(fpTx,"\n");
		}
		if (pParent->m_bTestRx) {
			TEST_THREAD_INSTR_CHK(ThreadCheckBox("Rx<===>Signal Generator"));
			Instrument.HasSG();
			Instrument.SG_Reset();
			Instrument.SG_SetModEnable(false);
			Instrument.SG_SetOutputEnable(true);
			if (!pParent->m_bRxPM)
				Instrument.SG_SetPL(pParent->m_dRxPower);
			else {
				Instrument.HasPM();
				Instrument.PM_Reset();
			}
			pSP1401->SetPowerMeasSamples(10240);
			pSP1401->SetPowerMeasSrc(CSP1401R1A::PowerMeasFreeRun,false);
			pSP1401->SetPowerMeasTimeout(6000000);
			pSP2401->SetADSw(CSP2401R1A::FromRf);
			pSP2401->SetDDC(-92.64e6);

			if (NULL == (fpRx = fopen((LPSTR)(LPCTSTR)pParent->m_strRxPath,"w"))) {
				CseMessageBox("can't open %s",pParent->m_strRxPath);
				TEST_THREAD_ABOART();
			}
			iRxPts = freq2array(strRxFreqStar,strRxFreqStop,strRxFreqStep,(int64_t *)uiRxFreq);
			for (int32_t i = 0;i < iRxPts;i ++)
				fprintf(fpRx,"%4lld:Power Temp   ",uiRxFreq[i] / 1000000);
			fprintf(fpRx,"\n");
		}

		while (1) {
			TEST_THREAD_TEST_CANCEL_S(POST_CLR);
			if (pParent->m_bTestTx) {
				TEST_THREAD_TEST_CANCEL_S(POST_CLR);
				Ch = ISP1401::Tx;
				double dTxPower = 0.0,dTxTemp = 0.0;
				double dDGain = 0.0,dAttOffset = 0.0;
				int32_t iAtt1 = 0,iAtt2 = 0,iAtt3 = 0;
				SP1401::TxPAAtt PAAtt = SP1401::Tx_Att;
				int16_t iDCI = 0,iDCQ = 0;
				double dTh = 0.0;
				uint16_t uiAmI = 0,uiAmQ = 0;

				for (int32_t i = 0;i < iTxPts;i ++) {
					TEST_THREAD_TEST_CANCEL_S(POST_CLR);
					pSP1401->SetTxFreq(uiTxFreq[i]);
					((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiTxFreq[i],iDCI,iDCQ);
					pSP2401->SetTxDCOffset((uint16_t)iDCI,(uint16_t)iDCQ);
					((CalFileR1A *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(uiTxFreq[i],dTh,uiAmI,uiAmQ);
					pSP2401->SetTxPhaseRotateI(dTh);
					pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ);
					((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxPower(uiTxFreq[i],pParent->m_dTxPower,OutPut,dDGain,iAtt1,iAtt2,PAAtt,iAtt3);
					((CalFileR1A *)(pSP1401->GetCalFile()))->GetTxAtt(uiTxFreq[i],OutPut,iAtt1 + iAtt2 + iAtt3,dAttOffset);
					iAtt1 = abs(iAtt1);
					iAtt2 = abs(iAtt2);
					iAtt3 = abs(iAtt3);
					pSP1401->SetTxPAAttSw(PAAtt);
					pSP1401->SetTxAtt(iAtt1,iAtt2,iAtt3);
					pSP2401->SetTxPowerComp(dDGain + dAttOffset);

					Instrument.PM_GetPwr((double)uiTxFreq[i],dTxPower);
					pSP1401->GetTxTemp(dTxTemp);
					fprintf(fpTx,"%-7.3f %4.2f   ",dTxPower,dTxTemp);
					pParent->m_TxData.at(i).m_Power.push_back(dTxPower);
					pParent->m_TxData.at(i).m_Temp.push_back(dTxTemp);	
				}
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_TEST_TS_SHOW,(WPARAM)(&Ch),NULL);
				fprintf(fpTx,"\n");
			}
			if (pParent->m_bTestRx) {
				TEST_THREAD_TEST_CANCEL_S(POST_CLR);
				Ch = ISP1401::Rx;
				double dRxPower = 0.0,dRxTemp = 0.0;
				int64_t iAD_0dBFS = 0;
				SP1401::RxLNAAtt LNAAtt = SP1401::Rx_Att;
				double dAtt1 = 0.0;
				int32_t iAtt2 = 0;
				CSP1401R1A::PowerMeasState State = CSP1401R1A::IDLE;
				for (int32_t i = 0;i < iRxPts;i ++) {
					TEST_THREAD_TEST_CANCEL_S(POST_CLR);
					Instrument.SG_SetCW((double)uiRxFreq[i]);
					if (pParent->m_bRxPM) {
						if (0 == iCnt) {
							double dPMPower = 0.0,dSGPower = 0.0;
							Instrument.SG_SetCW((double)uiRxFreq[i]);
							dSGPower = pParent->m_dRxPower + PM_IL;
							Instrument.SG_SetPL(dSGPower);
							Instrument.PM_SetFrequency((double)uiRxFreq[i]);
							for (int32_t i = 0;i < 10;i ++) {
								TEST_THREAD_TEST_CANCEL_S(POST_CLR);
								Instrument.PM_GetPwr((double)uiRxFreq[i],dPMPower);
								if (abs(pParent->m_dRxPower - dPMPower) < 0.05)
									break;
								dSGPower += (pParent->m_dRxPower - dPMPower);
								Instrument.SG_SetPL(dSGPower);
							}
							m_SGPower.push_back(dSGPower);
						}
						else
							Instrument.SG_SetPL(m_SGPower[i]);
					}
					pSP1401->SetRxFreq(uiRxFreq[i]);
					((CalFileR1A *)(pSP1401->GetCalFile()))->m_pRxRef->Get(uiRxFreq[i],pParent->m_dRxPower,OutPut,iAD_0dBFS,LNAAtt,dAtt1,iAtt2);
					pSP1401->SetRxLNAAttSw(LNAAtt);
					pSP1401->SetRxAtt(dAtt1,iAtt2);
					pSP1401->PowerMeasAbort();
					pSP1401->PowerMeasStart();
					while (State != CSP1401R1A::DONE) {
						TEST_THREAD_TEST_CANCEL_S(POST_CLR);
						pSP1401->GetPowerMeasState(State);
						if (CSP1401R1A::TT == State)
						break;
					}
					if (CSP1401R1A::TT == State)
						dRxPower = -100.0;
					else
						pSP1401->GetPowerMeasPower(dRxPower);
					pSP1401->GetRxTemp(dRxTemp);
					fprintf(fpRx,"%-7.3f   %4.2f  ",dRxPower,dRxTemp);
					pParent->m_RxData.at(i).m_Power.push_back(dRxPower);
					pParent->m_RxData.at(i).m_Temp.push_back(dRxTemp);
				}
				::PostMessage(pParent->GetSafeHwnd(),WM_CSE_TEST_TS_SHOW,(WPARAM)(&Ch),NULL);
				fprintf(fpRx,"\n");
			}
			iCnt ++;
			if (pParent->m_dSpaceingInterval)
				Sleep((DWORD)(pParent->m_dSpaceingInterval * 1000));
		}
		TEST_THREAD_ABOART_S(POST_CLR);
	}
	catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		TEST_THREAD_ABOART();
	}
}
