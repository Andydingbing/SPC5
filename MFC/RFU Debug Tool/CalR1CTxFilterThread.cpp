#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxFilterThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CTxFilterThread, CCalBaseThread)

CCalR1CTxFilterThread::CCalR1CTxFilterThread()
{
}

CCalR1CTxFilterThread::CCalR1CTxFilterThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CTxFilterThread::~CCalR1CTxFilterThread()
{
}

BOOL CCalR1CTxFilterThread::InitInstance()
{
	return TRUE;
}

int CCalR1CTxFilterThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CTxFilterThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CTxFilterThread::Run()
{
	return CalTxFilter();
}

int CCalR1CTxFilterThread::CalTxFilter()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Tx Filter"));
		CCalR1CTxFilterDlg *pParent = (CCalR1CTxFilterDlg *)m_pParent;

		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		BOOL bFast = pParent->m_bFast;

		//80M : 100M-43M ~~~ 6G+43M,step 2M
		//160M: 100M-83M ~~~ 6G+83M,step 2M
		Result_80  Data_80M;
		Result_160 Data_160M;
		uint64_t uiTxFreq = 0;

		InitSinglePos("Calibrating Tx Filter",RF_TX_FILTER_PTS +(int32_t)(IF_TX_FILTER_160M_PTS));

		if (!bFast) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Tx<===>Power Meter"));

			Instrument.Init();
			Instrument.HasPM();
			Instrument.PM_Reset();

			TxLOLeakageTableR1C::DataM DataLOLeak;
			TxSidebandTableR1C::DataM DataSideband;
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxLOLeak->Get(2000000000,&DataLOLeak);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxSideband->Get(2000000000,&DataSideband);
			pSP2401->SetTxDCOffset((uint16_t)(DataLOLeak.m_iDCI),(uint16_t)(DataLOLeak.m_iDCQ));
			pSP2401->SetTxPhaseRotateI((double)(DataSideband.m_fTh));
			pSP2401->SetTxAmplitudeBalance(DataSideband.m_uiAmI,DataSideband.m_uiAmQ);
			pSP2401->SetDDSSrc(CSP2401R1A::Sin_Tone);
			pSP2401->SetDDS1(0.0);
			pSP2401->SetDUCDDS(0.0);
			pSP2401->SetTxFilterSw(false);

			double dAtt0 = 0.0,dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0,dDGain = -5.0;
			TxPowerOPTableR1C::GuessBase(dAtt0,dAtt1,dAtt2,dAtt3,dDGain);
			pSP1401->SetIOMode(OutPut);
			pSP1401->SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3);
			pSP2401->SetTxPowerComp(dDGain);
			pSP1401->SetRxLNAAttSw(SP1401::Rx_Att);
			pSP1401->SetRxAtt019Sw(SP1401::Rx_Att_19);
			pSP1401->SetRxAtt(31.0,31.0,31.0);
			Sleep(10);

			for (int32_t i = 0;i < RF_TX_FILTER_PTS_0;i ++) {
				CAL_THREAD_TEST_CANCEL();
				TxRFFrTable::DataF DataRFFr;
				DataRFFr.m_uiFreq = RF_TX_FILTER_FREQ_STAR_0 + i * RF_TX_FILTER_FREQ_STEP;
				uiTxFreq = DataRFFr.m_uiFreq < RF_TX_FREQ_STAR ? RF_TX_FREQ_STAR : DataRFFr.m_uiFreq;
				pSP1401->SetTxFreq(uiTxFreq);
				pSP1401->SetTxFilterSw(CSP1401R1C::LPF,CSP1401R1C::LPF);
 				Instrument.PM_SetFrequency((double)(uiTxFreq));
 				Sleep(10);
 				Instrument.PM_GetPwr(DataRFFr.m_dPower);
				((CalFileR1C *)(pSP1401->GetCalFile()))->Add(ICalFile::TxRFFr_0,(TxRFFrTable::DataF *)&DataRFFr);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_RFFR_0_SHOW,NULL,(LPARAM)&DataRFFr);
				SetPos(i + 1,m_nThreadID);
			}
			((CalFileR1C *)(pSP1401->GetCalFile()))->Write(ICalFile::TxRFFr_0);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr0->SaveAs("c:\\tx_filter_rf_fr_0.txt");

			for (int32_t i = 0;i < RF_TX_FILTER_PTS_1;i ++) {
				CAL_THREAD_TEST_CANCEL();
				TxRFFrTable::DataF DataRFFr;
				DataRFFr.m_uiFreq = RF_TX_FILTER_FREQ_STAR_1 + i * RF_TX_FILTER_FREQ_STEP;
				uiTxFreq = DataRFFr.m_uiFreq > RF_TX_FREQ_STOP ? RF_TX_FREQ_STOP : DataRFFr.m_uiFreq;
				pSP1401->SetTxFreq(uiTxFreq);
				pSP1401->SetTxFilterSw(CSP1401R1C::BPF,CSP1401R1C::BPF);
				Instrument.PM_SetFrequency((double)(uiTxFreq));
				Sleep(10);
				Instrument.PM_GetPwr(DataRFFr.m_dPower);
				((CalFileR1C *)(pSP1401->GetCalFile()))->Add(ICalFile::TxRFFr_1,(TxRFFrTable::DataF *)&DataRFFr);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_RFFR_1_SHOW,NULL,(LPARAM)&DataRFFr);
				SetPos(RF_TX_FILTER_PTS_0 + i + 1,m_nThreadID);
			}
			((CalFileR1C *)(pSP1401->GetCalFile()))->Write(ICalFile::TxRFFr_1);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr1->SaveAs("c:\\tx_filter_rf_fr_1.txt");

			pSP1401->SetTxFreq(2000000000);
			Sleep(10);
			for (int32_t i = 0;i < IF_TX_FILTER_160M_PTS;i ++) {
				CAL_THREAD_TEST_CANCEL();
				TxIFFrTableR1C::DataF DataIFFr;
				DataIFFr.m_dFreq = IF_TX_FILTER_160M_FREQ_STAR + i * IF_TX_FILTER_FREQ_STEP;
				pSP2401->SetDUCDDS(DataIFFr.m_dFreq);
				Instrument.PM_SetFrequency((double)2000000000 + DataIFFr.m_dFreq);
				Sleep(10);
				Instrument.PM_GetPwr(DataIFFr.m_dPower);
				((CalFileR1C *)(pSP1401->GetCalFile()))->Add(ICalFile::TxIFFr,(TxIFFrTableR1C::DataF *)&DataIFFr);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_IFFR_SHOW,NULL,(LPARAM)&DataIFFr);
				SetPos(RF_TX_FILTER_PTS + i + 1,m_nThreadID);
			}
			((CalFileR1C *)(pSP1401->GetCalFile()))->Write(ICalFile::TxIFFr);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxIFFr->SaveAs("c:\\tx_filter_if_fr.txt");
		}

		//80M
		pSP1401->GetCalFile()->SetBW(_80M);
		FILE *fp_fr = NULL,*fp_real = NULL,*fp_imag = NULL;
		char szPathFr[MAX_PATH],szPathReal[MAX_PATH],szPathImag[MAX_PATH] ;
		sprintf(szPathFr,"C:\\CSECal\\tx_filter_80.txt");

		TxRFFrTable::DataF	  DataRFFr_80[(uint32_t)(IF_TX_FILTER_80M_PTS)];	//(2G-43M)~~~(2G+43M),step 2M
		TxIFFrTableR1C::DataF DataIFFr_80[(uint32_t)(IF_TX_FILTER_80M_PTS)];	//-43M~~~43M,step 2M
		int32_t iIdx = 0;

		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_0);
		for (uint64_t uiFreq = (uint64_t)(2000000000.0 + IF_TX_FILTER_80M_FREQ_STAR);uiFreq <= (uint64_t)(2000000000.0 + IF_TX_FILTER_80M_FREQ_STOP);uiFreq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr0->Get(uiFreq,&DataRFFr_80[iIdx]);
			iIdx ++;
		}
		iIdx = 0;
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxIFFr);
		for (double dFreq = IF_TX_FILTER_80M_FREQ_STAR;dFreq <= IF_TX_FILTER_80M_FREQ_STOP;dFreq += IF_TX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxIFFr->Get(dFreq,&DataIFFr_80[iIdx]);
			iIdx ++;
		}

		for (iIdx = 0;iIdx < IF_TX_FILTER_80M_PTS;iIdx ++)
			DataIFFr_80[iIdx].m_dPower -= DataRFFr_80[iIdx].m_dPower;		//real IF frequency response,-43M~~~43M

		fp_fr = fopen(szPathFr,"w");
		if (NULL == fp_fr) {
			CseMessageBox("could not open %s",szPathFr);
			CAL_THREAD_ABOART();
		}
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_0);
		uint64_t uiCWFreqCalled = 0;
		for (uint64_t uiCWFreq = RF_TX_FREQ_STAR;uiCWFreq < 3100000000;uiCWFreq += RF_TX_FREQ_STEP_CALLED) {
			uiCWFreqCalled = uiCWFreq < 100000000 ? 100000000 : uiCWFreq;
			iIdx = 0;
			for (uint64_t uiFreq = (uint64_t)(uiCWFreqCalled + IF_TX_FILTER_80M_FREQ_STAR);uiFreq <= (uint64_t)(uiCWFreqCalled + IF_TX_FILTER_80M_FREQ_STOP);uiFreq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr0->Get(uiFreq,&DataRFFr_80[iIdx]);
				fprintf(fp_fr,"%.6f ",DataRFFr_80[iIdx].m_dPower + DataIFFr_80[iIdx].m_dPower);
				iIdx ++;
			}
			fprintf(fp_fr,"\n");
		}
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_1);
		for (uint64_t uiCWFreq = 3100000000;uiCWFreq <= RF_TX_FREQ_STOP;uiCWFreq += RF_TX_FREQ_STEP_CALLED) {
			iIdx = 0;
			for (uint64_t uiFreq = (uint64_t)(uiCWFreq + IF_TX_FILTER_80M_FREQ_STAR);uiFreq <= (uint64_t)(uiCWFreq + IF_TX_FILTER_80M_FREQ_STOP);uiFreq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr1->Get(uiFreq,&DataRFFr_80[iIdx]);
				fprintf(fp_fr,"%.6f ",DataRFFr_80[iIdx].m_dPower + DataIFFr_80[iIdx].m_dPower);
				iIdx ++;
			}
			fprintf(fp_fr,"\n");
		}
		fclose(fp_fr);
		fp_fr = NULL;

		//160M
		pSP1401->GetCalFile()->SetBW(_160M);
		sprintf(szPathFr,"C:\\CSECal\\tx_filter_160.txt");

		TxRFFrTable::DataF	  DataRFFr_160[(uint32_t)(IF_TX_FILTER_160M_PTS)];	//(2G-83M)~~~(2G+83M),step 2M
		TxIFFrTableR1C::DataF DataIFFr_160[(uint32_t)(IF_TX_FILTER_160M_PTS)];	//-83M~~~83M,step 2M

		iIdx = 0;
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_0);
		for (uint64_t uiFreq = (uint64_t)(2000000000.0 + IF_TX_FILTER_160M_FREQ_STAR);uiFreq <= (uint64_t)(2000000000.0 + IF_TX_FILTER_160M_FREQ_STOP);uiFreq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr0->Get(uiFreq,&DataRFFr_160[iIdx]);
			iIdx ++;
		}
		iIdx = 0;
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxIFFr);
		for (double dFreq = IF_TX_FILTER_160M_FREQ_STAR;dFreq <= IF_TX_FILTER_160M_FREQ_STOP;dFreq += IF_TX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxIFFr->Get(dFreq,&DataIFFr_160[iIdx]);
			iIdx ++;
		}

		for (iIdx = 0;iIdx < IF_TX_FILTER_160M_PTS;iIdx ++)
			DataIFFr_160[iIdx].m_dPower -= DataRFFr_160[iIdx].m_dPower;		//real IF frequency response,-83M~~~83M

		fp_fr = fopen(szPathFr,"w");
		if (NULL == fp_fr) {
			CseMessageBox("could not open %s",szPathFr);
			CAL_THREAD_ABOART();
		}
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_0);
		uiCWFreqCalled = 0;
		for (uint64_t uiCWFreq = RF_TX_FREQ_STAR;uiCWFreq < 3100000000;uiCWFreq += RF_TX_FREQ_STEP_CALLED) {
			uiCWFreqCalled = uiCWFreq < 100000000 ? 100000000 : uiCWFreq;
			iIdx = 0;
			for (uint64_t uiFreq = (uint64_t)(uiCWFreqCalled + IF_TX_FILTER_160M_FREQ_STAR);uiFreq <= (uint64_t)(uiCWFreqCalled + IF_TX_FILTER_160M_FREQ_STOP);uiFreq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr0->Get(uiFreq,&DataRFFr_160[iIdx]);
				fprintf(fp_fr,"%.6f ",DataRFFr_160[iIdx].m_dPower + DataIFFr_160[iIdx].m_dPower);
				iIdx ++;
			}
			fprintf(fp_fr,"\n");
		}
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_1);
		for (uint64_t uiCWFreq = 3100000000;uiCWFreq <= RF_TX_FREQ_STOP;uiCWFreq += RF_TX_FREQ_STEP_CALLED) {
			iIdx = 0;
			for (uint64_t uiFreq = (uint64_t)(uiCWFreq + IF_TX_FILTER_160M_FREQ_STAR);uiFreq <= (uint64_t)(uiCWFreq + IF_TX_FILTER_160M_FREQ_STOP);uiFreq += (uint64_t)IF_TX_FILTER_FREQ_STEP) {
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pTxRFFr1->Get(uiFreq,&DataRFFr_160[iIdx]);
				fprintf(fp_fr,"%.6f ",DataRFFr_160[iIdx].m_dPower + DataIFFr_160[iIdx].m_dPower);
				iIdx ++;
			}
			fprintf(fp_fr,"\n");
		}
		fclose(fp_fr);
		fp_fr = NULL;

		//build coef
		char szFirExePaht[MAX_PATH] = {0};
		memset(szFirExePaht,0,sizeof(szFirExePaht));
		::GetModuleFileName(NULL,szFirExePaht,MAX_PATH);
		*strrchr(szFirExePaht,'\\') = 0;
		strcat(szFirExePaht,"\\tx_filter.exe");
		if (ExeFirProcess(szFirExePaht)) {
			DeleteFile(szPathFr);
			CseMessageBox("%s",Log.GetLastError());
			CAL_THREAD_ABOART();
		}

		//80M
		sprintf(szPathReal,"C:\\CSECal\\tx_filter_80_coef_real.txt");
		sprintf(szPathImag,"C:\\CSECal\\tx_filter_80_coef_imag.txt");
		fp_real = fopen(szPathReal,"r");
		fp_imag = fopen(szPathImag,"r");
		if (NULL == fp_real) {
			Log.SetLastError("could not open %s",szPathReal);
			CseMessageBox("%s",Log.GetLastError());
			CAL_THREAD_ABOART();
		}
		if (NULL == fp_imag) {
			Log.SetLastError("could not open %s",szPathImag);
			CseMessageBox("%s",Log.GetLastError());
			CAL_THREAD_ABOART();
		}
		iIdx = 0;
		for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_CALLED) {
			memset(&Data_80M,0,sizeof(Data_80M));
			for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
				fscanf(fp_real,"%d,",&(Data_80M.m_iCoefReal[i]));
			fscanf(fp_real,"\n");
			
			for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
				fscanf(fp_imag,"%d,",&(Data_80M.m_iCoefImag[i]));
			fscanf(fp_imag,"\n");

			Data_80M.m_uiFreq = uiFreq;
			Data_80M.m_iIdx = iIdx;
			time_t Time;
			time(&Time);
			Data_80M.m_EndTime = *localtime(&Time);
			pParent->m_TabCtrl.m_Data_80.at(iIdx) = Data_80M;
			pParent->m_pSP1401->GetCalFile()->Add(ICalFile::TxFil_80,(TxFilter_80M_TableR1C::DataF *)&Data_80M);
			::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_80_SHOW,NULL,(LPARAM)&Data_80M);
			iIdx ++;
		}
		pParent->m_pSP1401->GetCalFile()->Write(ICalFile::TxFil_80);
		fclose(fp_real);
		fclose(fp_imag);
		fp_real = NULL;
		fp_imag = NULL;
		DeleteFile(szPathFr);
		DeleteFile(szPathReal);
		DeleteFile(szPathImag);

		//160M
		sprintf(szPathReal,"C:\\CSECal\\tx_filter_160_coef_real.txt");
		sprintf(szPathImag,"C:\\CSECal\\tx_filter_160_coef_imag.txt");
		fp_real = fopen(szPathReal,"r");
		fp_imag = fopen(szPathImag,"r");
		if (NULL == fp_real) {
			Log.SetLastError("could not open %s",szPathReal);
			CseMessageBox("%s",Log.GetLastError());
			CAL_THREAD_ABOART();
		}
		if (NULL == fp_imag) {
			Log.SetLastError("could not open %s",szPathImag);
			CseMessageBox("%s",Log.GetLastError());
			CAL_THREAD_ABOART();
		}
		iIdx = 0;
		for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_CALLED) {
			memset(&Data_160M,0,sizeof(Data_160M));
			for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
				fscanf(fp_real,"%d,",&(Data_160M.m_iCoefReal[i]));
			fscanf(fp_real,"\n");

			for (int32_t i = 0;i < TX_FILTER_ORDER_2I;i ++)
				fscanf(fp_imag,"%d,",&(Data_160M.m_iCoefImag[i]));
			fscanf(fp_imag,"\n");

			Data_160M.m_uiFreq = uiFreq;
			Data_160M.m_iIdx = iIdx;
			time_t Time;
			time(&Time);
			Data_160M.m_EndTime = *localtime(&Time);
			pParent->m_TabCtrl.m_Data_160.at(iIdx) = Data_160M;
			pParent->m_pSP1401->GetCalFile()->Add(ICalFile::TxFil_160,(TxFilter_160M_TableR1C::DataF *)&Data_160M);
			::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_160_SHOW,NULL,(LPARAM)&Data_160M);
			iIdx ++;
		}
		pParent->m_pSP1401->GetCalFile()->Write(ICalFile::TxFil_160);
		fclose(fp_real);
		fclose(fp_imag);
		fp_real = NULL;
		fp_imag = NULL;
		DeleteFile(szPathFr);
		DeleteFile(szPathReal);
		DeleteFile(szPathImag);

		if (bFast)
			SetPos(RF_TX_FILTER_PTS +(int32_t)(IF_TX_FILTER_160M_PTS),m_nThreadID);

		CAL_THREAD_ABOART();
	}catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
}


IMPLEMENT_DYNCREATE(CExportR1CTxFilterThread, CWinThread)

CExportR1CTxFilterThread::CExportR1CTxFilterThread()
{
}

CExportR1CTxFilterThread::CExportR1CTxFilterThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CTxFilterThread::~CExportR1CTxFilterThread()
{
}

BOOL CExportR1CTxFilterThread::InitInstance()
{
	return TRUE;
}

int CExportR1CTxFilterThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CTxFilterThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CTxFilterThread::Run()
{
	CCalR1CTxFilterDlg *pParent = (CCalR1CTxFilterDlg *)m_pParent;

	InitSinglePos("Exporting Tx Filter",(int32_t)(RF_TX_FILTER_PTS + IF_TX_FILTER_160M_PTS + RF_TX_FREQ_PTS_CALLED * 2));

	int32_t iIdx = 0;
	TxRFFrTable::DataF    DataRFFr;
	TxIFFrTableR1C::DataF DataIFFr;
	CCalR1CTxFilterThread::Result_80  Data_80M;
	CCalR1CTxFilterThread::Result_160 Data_160M;

	Data_80M.m_iIdx = 0;
	Data_160M.m_iIdx = 0;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_0);
	for (uint64_t uiFreq = RF_TX_FILTER_FREQ_STAR_0;uiFreq <= RF_TX_FILTER_FREQ_STOP_0;uiFreq += RF_TX_FILTER_FREQ_STEP) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxRFFr0->Get(uiFreq,&DataRFFr);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_RFFR_0_SHOW,NULL,(LPARAM)&DataRFFr);
		SetPos(iIdx + 1,m_nThreadID);
		iIdx ++;
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxRFFr_1);
	for (uint64_t uiFreq = RF_TX_FILTER_FREQ_STAR_1;uiFreq <= RF_TX_FILTER_FREQ_STOP_1;uiFreq += RF_TX_FILTER_FREQ_STEP) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxRFFr1->Get(uiFreq,&DataRFFr);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_RFFR_1_SHOW,NULL,(LPARAM)&DataRFFr);
		SetPos(RF_TX_FILTER_PTS_0 + iIdx + 1,m_nThreadID);
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxIFFr);
	for (double dFreq = IF_TX_FILTER_160M_FREQ_STAR;dFreq <= IF_TX_FILTER_160M_FREQ_STOP;dFreq += IF_TX_FILTER_FREQ_STEP) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxIFFr->Get(dFreq,&DataIFFr);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_IFFR_SHOW,NULL,(LPARAM)&DataIFFr);
		SetPos(RF_TX_FILTER_PTS + iIdx + 1,m_nThreadID);
		iIdx ++;
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->SetBW(_80M);
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxFil_80);
	for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_CALLED) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxFilter_80M->Get(uiFreq,(TxFilter_80M_TableR1C::DataF *)&Data_80M);
		Data_80M.m_iIdx = iIdx;
		pParent->m_TabCtrl.m_Data_80.at(iIdx) = Data_80M;
		::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_80_SHOW,NULL,(LPARAM)&Data_80M);
		SetPos(RF_TX_FILTER_PTS + IF_TX_FILTER_160M_PTS + iIdx + 1,m_nThreadID);
		iIdx ++;
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->SetBW(_160M);
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::TxFil_160);
	for (uint64_t uiFreq = RF_TX_FREQ_STAR;uiFreq <= RF_TX_FREQ_STOP;uiFreq += RF_TX_FREQ_STEP_CALLED) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pTxFilter_160M->Get(uiFreq,(TxFilter_160M_TableR1C::DataF *)&Data_160M);
		Data_160M.m_iIdx = iIdx;
		pParent->m_TabCtrl.m_Data_160.at(iIdx) = Data_160M;
		::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_TX_FILTER_160_SHOW,NULL,(LPARAM)&Data_160M);
		SetPos(RF_TX_FILTER_PTS + IF_TX_FILTER_160M_PTS + RF_TX_FREQ_PTS_CALLED + iIdx + 1,m_nThreadID);
		iIdx ++;
	}
	CAL_THREAD_ABOART();
	return 0;
}