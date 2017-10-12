#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxFilterThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCalR1CRxFilterThread, CWinThread)

CCalR1CRxFilterThread::CCalR1CRxFilterThread()
{
}

CCalR1CRxFilterThread::CCalR1CRxFilterThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CCalR1CRxFilterThread::~CCalR1CRxFilterThread()
{
}

BOOL CCalR1CRxFilterThread::InitInstance()
{
	return TRUE;
}

int CCalR1CRxFilterThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CCalR1CRxFilterThread, CCalBaseThread)
END_MESSAGE_MAP()


int CCalR1CRxFilterThread::Run()
{
	return CalRxFilter();
}

int CCalR1CRxFilterThread::CalRxFilter()
{
	try {
		CAL_THREAD_TEST_CANCEL();
		CAL_THREAD_INSTR_CHK(ThreadCheckBox("Prepare Calibrating Rx Filter"));

		CCalR1CRxFilterDlg *pParent = (CCalR1CRxFilterDlg *)m_pParent;
		CSP1401R1C *pSP1401 = pParent->m_pSP1401;
		CSP2401R1A *pSP2401 = pParent->m_pSP2401;
		BOOL bFast = pParent->m_bFast;

		//80M : 100M-43M ~~~ 6G+43M,step 2M
		//160M: 100M-83M ~~~ 6G+83M,step 2M
		Result_80  Data_80M;
		Result_160 Data_160M;
		uint64_t uiRxFreq = 0;

		InitSinglePos("Calibrating Rx Filter",RF_RX_FILTER_PTS +(int32_t)(IF_RX_FILTER_160M_PTS));

		if (!bFast) {
			CAL_THREAD_INSTR_CHK(ThreadCheckBox("Rx<===>Z28<===>Signal Generator"));

			Instrument.Init();
			Instrument.HasPM();
			Instrument.HasSG();
			Instrument.PM_Reset();
			Instrument.SG_Reset();
			Instrument.SG_SetModEnable(false);
			Instrument.SG_SetPL(-60.0);
			Instrument.SG_SetOutputEnable(true);

			int32_t iRef = 0,iLNAAtt = 0,iAtt019 = 0;
			double dAtt1 = 0.0,dAtt2 = 0.0,dAtt3 = 0.0;
			RxRefOPTableR1C::GuessBase(1,iRef,iLNAAtt,iAtt019,dAtt1,dAtt2,dAtt3);
			iRef -= 15;
//			dAtt1 -= 5.0;
			pSP1401->SetIOMode(OutPut);
			pSP1401->SetRxLNAAttSw((SP1401::RxLNAAtt)(iLNAAtt));
			pSP1401->SetRxAtt019Sw((SP1401::RxAtt019)(iAtt019));
			pSP1401->SetRxAtt(dAtt1,dAtt2,dAtt3);
			pSP1401->SetPowerMeasSrc(CSP1401R1C::PowerMeasFreeRun,false);
			pSP1401->SetPowerMeasTimeout(6553600);
			pSP1401->SetPowerMeasSamples(102400);
			pSP1401->SetTxAtt(30.0,30.0,30.0,30.0);
			pSP1401->SetRxIFFilterSw(ISP1401::_160MHz);
			pSP2401->SetDDC(-92640000.0);
			pSP2401->SetRxPowerComp(0);
			pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
			pSP2401->SetRxFilter2Default();
			Sleep(10);

			for (int32_t i = 0;i < RF_RX_FILTER_PTS;i ++) {
				CAL_THREAD_TEST_CANCEL();
				RxRFFrTableR1C::DataF DataRFFr;
				DataRFFr.m_uiFreq = RF_RX_FILTER_FREQ_STAR + i * RF_RX_FILTER_FREQ_STEP;
				uiRxFreq = DataRFFr.m_uiFreq < RF_RX_FREQ_STAR ? RF_RX_FREQ_STAR : DataRFFr.m_uiFreq;
				uiRxFreq = uiRxFreq > RF_RX_FREQ_STOP ? RF_RX_FREQ_STOP : uiRxFreq;
				AjustSG((double)(uiRxFreq),iRef);
				pSP1401->SetRxFreq(uiRxFreq);
				Sleep(50);
				DataRFFr.m_dPower = GetRxPower(pSP1401);
				((CalFileR1C *)(pSP1401->GetCalFile()))->Add(ICalFile::RxRFFr,(RxRFFrTableR1C::DataF *)&DataRFFr);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_RFFR_SHOW,NULL,(LPARAM)&DataRFFr);
				SetPos(i + 1,m_nThreadID);
			}
			((CalFileR1C *)(pSP1401->GetCalFile()))->Write(ICalFile::RxRFFr);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRFFr->SaveAs("c:\\rx_filter_rf_fr.txt");

			pSP1401->SetRxFreq(2000000000);
			Sleep(10);
			for (int32_t i = 0;i < IF_RX_FILTER_160M_PTS;i ++) {
				CAL_THREAD_TEST_CANCEL();
				RxIFFrTableR1C::DataF DataIFFr;
				DataIFFr.m_dFreq = IF_RX_FILTER_160M_FREQ_STAR + i * IF_RX_FILTER_FREQ_STEP;
				AjustSG(2000000000 + DataIFFr.m_dFreq,iRef);
				Sleep(50);
				DataIFFr.m_dPower = GetRxPower(pSP1401);
				((CalFileR1C *)(pSP1401->GetCalFile()))->Add(ICalFile::RxIFFr,(RxIFFrTableR1C::DataF *)&DataIFFr);
				::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_IFFR_SHOW,NULL,(LPARAM)&DataIFFr);
				SetPos(RF_RX_FILTER_PTS + i + 1,m_nThreadID);
			}
			((CalFileR1C *)(pSP1401->GetCalFile()))->Write(ICalFile::RxIFFr);
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxIFFr->SaveAs("c:\\rx_filter_if_fr.txt");
		}
		
		//80M
		pSP1401->GetCalFile()->SetBW(_80M);
		FILE *fp_fr = NULL,*fp_real = NULL,*fp_imag = NULL;
		char szPathFr[MAX_PATH],szPathReal[MAX_PATH],szPathImag[MAX_PATH] ;
		sprintf(szPathFr,"C:\\CSECal\\rx_filter_80.txt");

		RxRFFrTableR1C::DataF DataRFFr_80[(uint32_t)(IF_RX_FILTER_80M_PTS)];	//(2G-43M)~~~(2G+43M),step 2M
		RxIFFrTableR1C::DataF DataIFFr_80[(uint32_t)(IF_RX_FILTER_80M_PTS)];	//-43M~~~43M,step 2M
		int32_t iIdx = 0;

		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRFFr);
		for (uint64_t uiFreq = (uint64_t)(2000000000.0 + IF_RX_FILTER_80M_FREQ_STAR);uiFreq <= (uint64_t)(2000000000.0 + IF_RX_FILTER_80M_FREQ_STAR);uiFreq += (uint64_t)IF_RX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRFFr->Get(uiFreq,&DataRFFr_80[iIdx]);
			iIdx ++;
		}
		iIdx = 0;
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxIFFr);
		for (double dFreq = IF_RX_FILTER_80M_FREQ_STAR;dFreq <= IF_RX_FILTER_80M_FREQ_STAR;dFreq += IF_RX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxIFFr->Get(dFreq,&DataIFFr_80[iIdx]);
			iIdx ++;
		}

		for (iIdx = 0;iIdx < IF_RX_FILTER_80M_PTS;iIdx ++)
			DataIFFr_80[iIdx].m_dPower -= DataRFFr_80[iIdx].m_dPower;		//real IF frequency response,-43M~~~43M

		fp_fr = fopen(szPathFr,"w");
		if (NULL == fp_fr) {
			CseMessageBox("could not open %s",szPathFr);
			CAL_THREAD_ABOART();
		}
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRFFr);
		uint64_t uiCWFreqCalled = 0;
		for (uint64_t uiCWFreq = RF_RX_FREQ_STAR;uiCWFreq <= RF_RX_FREQ_STOP;uiCWFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
			uiCWFreqCalled = uiCWFreq < 100000000 ? 100000000 : uiCWFreq;
			iIdx = 0;
			for (uint64_t uiFreq = (uint64_t)(uiCWFreqCalled + IF_RX_FILTER_80M_FREQ_STAR);uiFreq <= (uint64_t)(uiCWFreqCalled + IF_RX_FILTER_80M_FREQ_STOP);uiFreq += (uint64_t)IF_RX_FILTER_FREQ_STEP) {
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRFFr->Get(uiFreq,&DataRFFr_80[iIdx]);
				fprintf(fp_fr,"%.6f ",DataRFFr_80[iIdx].m_dPower + DataIFFr_80[iIdx].m_dPower);
				iIdx ++;
			}
			fprintf(fp_fr,"\n");
		}
		fclose(fp_fr);
		fp_fr = NULL;

		//160M
		pSP1401->GetCalFile()->SetBW(_160M);
		sprintf(szPathFr,"C:\\CSECal\\rx_filter_160.txt");

		RxRFFrTableR1C::DataF DataRFFr_160[(uint32_t)(IF_RX_FILTER_160M_PTS)];	//(2G-83M)~~~(2G+83M),step 2M
		RxIFFrTableR1C::DataF DataIFFr_160[(uint32_t)(IF_RX_FILTER_160M_PTS)];	//-83M~~~83M,step 2M

		iIdx = 0;
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRFFr);
		for (uint64_t uiFreq = (uint64_t)(2000000000.0 + IF_RX_FILTER_160M_FREQ_STAR);uiFreq <= (uint64_t)(2000000000.0 + IF_RX_FILTER_160M_FREQ_STOP);uiFreq += (uint64_t)IF_RX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRFFr->Get(uiFreq,&DataRFFr_160[iIdx]);
			iIdx ++;
		}
		iIdx = 0;
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxIFFr);
		for (double dFreq = IF_RX_FILTER_160M_FREQ_STAR;dFreq <= IF_RX_FILTER_160M_FREQ_STOP;dFreq += IF_RX_FILTER_FREQ_STEP) {
			((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxIFFr->Get(dFreq,&DataIFFr_160[iIdx]);
			iIdx ++;
		}

		for (iIdx = 0;iIdx < IF_RX_FILTER_160M_PTS;iIdx ++)
			DataIFFr_160[iIdx].m_dPower -= DataRFFr_160[iIdx].m_dPower;		//real IF frequency response,-83M~~~83M

		fp_fr = fopen(szPathFr,"w");
		if (NULL == fp_fr) {
			CseMessageBox("could not open %s",szPathFr);
			CAL_THREAD_ABOART();
		}
		((CalFileR1C *)(pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRFFr);
		uiCWFreqCalled = 0;
		for (uint64_t uiCWFreq = RF_RX_FREQ_STAR;uiCWFreq <= RF_RX_FREQ_STOP;uiCWFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
			uiCWFreqCalled = uiCWFreq < 100000000 ? 100000000 : uiCWFreq;
			iIdx = 0;
			for (uint64_t uiFreq = (uint64_t)(uiCWFreqCalled + IF_RX_FILTER_160M_FREQ_STAR);uiFreq <= (uint64_t)(uiCWFreqCalled + IF_RX_FILTER_160M_FREQ_STOP);uiFreq += (uint64_t)IF_RX_FILTER_FREQ_STEP) {
				((CalFileR1C *)(pSP1401->GetCalFile()))->m_pRxRFFr->Get(uiFreq,&DataRFFr_160[iIdx]);
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
		strcat(szFirExePaht,"\\rx_filter.exe");
		if (ExeFirProcess(szFirExePaht)) {
			DeleteFile(szPathFr);
			CseMessageBox("%s",Log->GetLastError());
			CAL_THREAD_ABOART();
		}

		//80M
		sprintf(szPathReal,"C:\\CSECal\\rx_filter_80_coef_real.txt");
		sprintf(szPathImag,"C:\\CSECal\\rx_filter_80_coef_imag.txt");
		fp_real = fopen(szPathReal,"r");
		fp_imag = fopen(szPathImag,"r");
		if (NULL == fp_real) {
			Log->SetLastError("could not open %s",szPathReal);
			CseMessageBox("%s",Log->GetLastError());
			CAL_THREAD_ABOART();
		}
		if (NULL == fp_imag) {
			Log->SetLastError("could not open %s",szPathImag);
			CseMessageBox("%s",Log->GetLastError());
			CAL_THREAD_ABOART();
		}
		iIdx = 0;
		for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
			memset(&Data_80M,0,sizeof(Data_80M));
			for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
				fscanf(fp_real,"%d,",&(Data_80M.m_iCoefReal[i]));
			fscanf(fp_real,"\n");

			for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
				fscanf(fp_imag,"%d,",&(Data_80M.m_iCoefImag[i]));
			fscanf(fp_imag,"\n");

			Data_80M.m_uiFreq = uiFreq;
			Data_80M.m_iIdx = iIdx;
			time_t Time;
			time(&Time);
			Data_80M.m_EndTime = *localtime(&Time);
			pParent->m_TabCtrl.m_Data_80.at(iIdx) = Data_80M;
			pParent->m_pSP1401->GetCalFile()->Add(ICalFile::RxFil_80,(RxFilter_80M_TableR1C::DataF *)&Data_80M);
			::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_80_SHOW,NULL,(LPARAM)&Data_80M);
			iIdx ++;
		}
		pParent->m_pSP1401->GetCalFile()->Write(ICalFile::RxFil_80);
		fclose(fp_real);
		fclose(fp_imag);
		fp_real = fp_imag = NULL;
		DeleteFile(szPathFr);
		DeleteFile(szPathReal);
		DeleteFile(szPathImag);

		//160M
		sprintf(szPathReal,"C:\\CSECal\\rx_filter_160_coef_real.txt");
		sprintf(szPathImag,"C:\\CSECal\\rx_filter_160_coef_imag.txt");
		fp_real = fopen(szPathReal,"r");
		fp_imag = fopen(szPathImag,"r");
		if (NULL == fp_real) {
			Log->SetLastError("could not open %s",szPathReal);
			CseMessageBox("%s",Log->GetLastError());
			CAL_THREAD_ABOART();
		}
		if (NULL == fp_imag) {
			Log->SetLastError("could not open %s",szPathImag);
			CseMessageBox("%s",Log->GetLastError());
			CAL_THREAD_ABOART();
		}
		iIdx = 0;
		for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
			memset(&Data_160M,0,sizeof(Data_160M));
			for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
				fscanf(fp_real,"%d,",&(Data_160M.m_iCoefReal[i]));
			fscanf(fp_real,"\n");

			for (int32_t i = 0;i < RX_FILTER_ORDER;i ++)
				fscanf(fp_imag,"%d,",&(Data_160M.m_iCoefImag[i]));
			fscanf(fp_imag,"\n");

			Data_160M.m_uiFreq = uiFreq;
			Data_160M.m_iIdx = iIdx;
			time_t Time;
			time(&Time);
			Data_160M.m_EndTime = *localtime(&Time);
			pParent->m_TabCtrl.m_Data_160.at(iIdx) = Data_160M;
			pParent->m_pSP1401->GetCalFile()->Add(ICalFile::RxFil_160,(RxFilter_160M_TableR1C::DataF *)&Data_160M);
			::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_160_SHOW,NULL,(LPARAM)&Data_160M);
			iIdx ++;
		}
		pParent->m_pSP1401->GetCalFile()->Write(ICalFile::RxFil_160);
		fclose(fp_real);
		fclose(fp_imag);
		fp_real = fp_imag = NULL;
		DeleteFile(szPathFr);
		DeleteFile(szPathReal);
		DeleteFile(szPathImag);

		if (bFast)
			SetPos(RF_RX_FILTER_PTS +(int32_t)(IF_RX_FILTER_160M_PTS),m_nThreadID);

		CAL_THREAD_ABOART();
	}catch (CInstrExpt &expt) {
		CseMessageBox(expt.GetExpt());
		CAL_THREAD_ABOART();
	}
}

void CCalR1CRxFilterThread::AjustSG(double dFreq,int32_t iPower)
{
	double dPMPower = 0.0;
	Instrument.SG_SetCW(dFreq);
	Instrument.SG_SetPL(iPower + PM_IL);
	Sleep(2);
	Instrument.PM_SetFrequency(dFreq);
	for (int32_t i = 0;i < 10;i ++) {
		Instrument.PM_GetPwr(dFreq,dPMPower);
		if (abs(iPower - dPMPower) < 0.05)
			break;
		Instrument.SG_SetPL(iPower + PM_IL + (iPower - dPMPower));
		Sleep(2);
	}
}

double CCalR1CRxFilterThread::GetRxPower(CSP1401R1C *pSP1401)
{
	double dPower = 0.0;
	CSP1401R1C::PowerMeasState State = CSP1401R1C::IDLE;
	pSP1401->PowerMeasAbort();
	pSP1401->PowerMeasStart();

	while (State != CSP1401R1C::DONE) {
		pSP1401->GetPowerMeasState(State);
		if (State == CSP1401R1C::TT)
			return -100.0;
	}
	pSP1401->GetPowerMeasPower(dPower);
	return dPower;
}


IMPLEMENT_DYNCREATE(CExportR1CRxFilterThread, CWinThread)

CExportR1CRxFilterThread::CExportR1CRxFilterThread()
{
}

CExportR1CRxFilterThread::CExportR1CRxFilterThread(CWnd *pParent)
{
	m_pParent = pParent;
}

CExportR1CRxFilterThread::~CExportR1CRxFilterThread()
{
}

BOOL CExportR1CRxFilterThread::InitInstance()
{
	return TRUE;
}

int CExportR1CRxFilterThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CExportR1CRxFilterThread, CWinThread)
END_MESSAGE_MAP()


int CExportR1CRxFilterThread::Run()
{
	CCalR1CRxFilterDlg *pParent = (CCalR1CRxFilterDlg *)m_pParent;

	InitSinglePos("Exporting Rx Filter",(int32_t)(RF_RX_FILTER_PTS + IF_RX_FILTER_160M_PTS + RF_RX_FREQ_PTS_CALLED_R1C * 2));

	int32_t iIdx = 0;
	RxRFFrTableR1C::DataF DataRFFr;
	RxIFFrTableR1C::DataF DataIFFr;
	CCalR1CRxFilterThread::Result_80  Data_80M;
	CCalR1CRxFilterThread::Result_160 Data_160M;

	Data_80M.m_iIdx = 0;
	Data_160M.m_iIdx = 0;

	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxRFFr);
	for (uint64_t uiFreq = RF_RX_FILTER_FREQ_STAR;uiFreq <= RF_RX_FILTER_FREQ_STOP;uiFreq += RF_RX_FILTER_FREQ_STEP) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxRFFr->Get(uiFreq,&DataRFFr);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_RFFR_SHOW,NULL,(LPARAM)&DataRFFr);
		SetPos(iIdx + 1,m_nThreadID);
		iIdx ++;
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxIFFr);
	for (double dFreq = IF_RX_FILTER_160M_FREQ_STAR;dFreq <= IF_RX_FILTER_160M_FREQ_STOP;dFreq += IF_RX_FILTER_FREQ_STEP) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxIFFr->Get(dFreq,&DataIFFr);
		::SendMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_IFFR_SHOW,NULL,(LPARAM)&DataIFFr);
		SetPos(RF_TX_FILTER_PTS + iIdx + 1,m_nThreadID);
		iIdx ++;
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->SetBW(_80M);
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxFil_80);
	for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxFilter_80M->Get(uiFreq,(RxFilter_80M_TableR1C::DataF *)&Data_80M);
		Data_80M.m_iIdx = iIdx;
		pParent->m_TabCtrl.m_Data_80.at(iIdx) = Data_80M;
		::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_80_SHOW,NULL,(LPARAM)&Data_80M);
		SetPos(RF_RX_FILTER_PTS + IF_RX_FILTER_160M_PTS + iIdx + 1,m_nThreadID);
		iIdx ++;
	}

	iIdx = 0;
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->SetBW(_160M);
	((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->Map2Buf(ICalFile::RxFil_160);
	for (uint64_t uiFreq = RF_RX_FREQ_STAR;uiFreq <= RF_RX_FREQ_STOP;uiFreq += RF_RX_FREQ_STEP_CALLED_R1C) {
		((CalFileR1C *)(pParent->m_pSP1401->GetCalFile()))->m_pRxFilter_160M->Get(uiFreq,(RxFilter_160M_TableR1C::DataF *)&Data_160M);
		Data_160M.m_iIdx = iIdx;
		pParent->m_TabCtrl.m_Data_160.at(iIdx) = Data_160M;
		::PostMessage(pParent->GetSafeHwnd(),WM_CSE_CAL_RX_FILTER_160_SHOW,NULL,(LPARAM)&Data_160M);
		SetPos(RF_RX_FILTER_PTS + IF_RX_FILTER_160M_PTS + RF_RX_FREQ_PTS_CALLED_R1C + iIdx + 1,m_nThreadID);
		iIdx ++;
	}
	CAL_THREAD_ABOART();
	return 0;
}