#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CRxFRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1CRxFRDlg, CTestR1CBaseDlg)
IMPLEMENT_CSE_TEST_R1C_DLG(CTestR1CRxFRDlg)

CTestR1CRxFRDlg::CTestR1CRxFRDlg(CWnd* pParent /*=NULL*/)
	: CTestR1CBaseDlg(CTestR1CRxFRDlg::IDD, pParent)
	, m_strRFFreqStar(_T(""))
	, m_strRFFreqStop(_T(""))
	, m_strRFFreqStep(_T(""))
	, m_strRFFreq(_T("2.4G"))
	, m_strIFFreqStep(_T(""))
{
	m_pChart = NULL;
	m_pFRLineSerie.clear();
}

CTestR1CRxFRDlg::~CTestR1CRxFRDlg()
{
	for (int i = 0;i < m_pFRLineSerie.size();i ++)
		m_pFRLineSerie[i]->ClearSerie();
	m_pFRLineSerie.clear();
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CTestR1CRxFRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRFFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strRFFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strRFFreqStep);
	DDX_Text(pDX, IDC_EDIT4, m_strRFFreq);
	DDX_Text(pDX, IDC_EDIT68, m_strIFFreqStep);
}


BEGIN_MESSAGE_MAP(CTestR1CRxFRDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1CRxFRDlg::OnBnClickedStartTestRF)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestR1CRxFRDlg::OnBnClickedStartTestIF)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestR1CRxFRDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestR1CRxFRDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_TEST_RX_FR_SHOW, &CTestR1CRxFRDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestR1CRxFRDlg::Done)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestR1CRxFRDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestR1CRxFRDlg::OnBnClickedContinue)
END_MESSAGE_MAP()


void CTestR1CRxFRDlg::ResetShowCtrl(CTestR1CRxFRThread::TestRFFRInfo *pInfo)
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	freq2array(pInfo->m_strFreqStar,pInfo->m_strFreqStop,pInfo->m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	m_pChart->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);

	for (int i = 0;i < m_pFRLineSerie.size();i ++)
		m_pFRLineSerie[i]->ClearSerie();
	m_pFRLineSerie.clear();
	m_pChart->RemoveAllSeries();
	m_pChart->GetTitle()->RemoveAll();
	m_pChart->GetTitle()->AddString(_T("Rx RF Frequency Response Curve(X:Freq(MHz) Y:Power(dBm))"));

	int32_t iSec = strsections((LPSTR)(LPCTSTR)(pInfo->m_strFreqStar));
	for (int32_t i = 0;i < iSec;i ++) {
		CChartLineSerie *pFRLineSerie;
		m_pFRLineSerie.push_back(pFRLineSerie);
		m_pFRLineSerie[i] = m_pChart->CreateLineSerie();
		m_pFRLineSerie[i]->SetSeriesOrdering(poNoOrdering);
		m_pFRLineSerie[i]->SetColor(RED);
	}
}

void CTestR1CRxFRDlg::ResetShowCtrl(CTestR1CRxFRThread::TestIFFRInfo *pInfo)
{
	m_pChart->GetBottomAxis()->SetMinMax(-1.0 * MAX_BW / 1000000.0 / 2.0,MAX_BW / 1000000.0 / 2.0);

	for (int i = 0;i < m_pFRLineSerie.size();i ++)
		m_pFRLineSerie[i]->ClearSerie();
	m_pFRLineSerie.clear();
	m_pChart->RemoveAllSeries();
	m_pChart->GetTitle()->RemoveAll();
	m_pChart->GetTitle()->AddString(_T("Rx IF Filter Frequency Response Curve(X:Freq(MHz) Y:Power(dBm))"));

	CChartLineSerie *pFRLineSerie;
	m_pFRLineSerie.push_back(pFRLineSerie);
	m_pFRLineSerie[0] = m_pChart->CreateLineSerie();
	m_pFRLineSerie[0]->SetSeriesOrdering(poNoOrdering);
	m_pFRLineSerie[0]->SetColor(RED);
}

void CTestR1CRxFRDlg::OnBnClickedStartTestRF()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing Rx RF Frequency Response"));
	STACHKV(ThreadCheckBox("Rx<===>Signal Generator"));
	UpdateData(TRUE);
	CtrlPrepare();

	CTestR1CRxFRThread::TestRFFRInfo Info;
	Info.m_pSP1401 = m_pSP1401;
	Info.m_pSP2401 = m_pSP2401;
	Info.m_strFreqStar = m_strRFFreqStar;
	Info.m_strFreqStop = m_strRFFreqStop;
	Info.m_strFreqStep = m_strRFFreqStep;
	ResetShowCtrl(&Info);

	m_pThread = new CTestR1CRxFRThread(this,&Info);
	m_pThread->CreateThread();
}

BOOL CTestR1CRxFRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,TEST_CHART_RECT,IDC_TEST_R1C_RX_FR_CHART);
	m_pChart->SetZoomRectColor(BLACK);
	m_pChart->GetTitle()->AddString(_T("Rx RF Frequency Response Curve(X:Freq(MHz) Y:Power(dBm))"));
	m_pChart->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_RX_FREQ_STAR / 1000000.0,RF_RX_FREQ_STOP / 1000000.0);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_strRFFreqStar.Format("%dM",RF_RX_FREQ_STAR / 1000000);
	m_strRFFreqStop.Format("%dM",RF_RX_FREQ_STOP / 1000000);
	m_strRFFreqStep.Format("%dM",RF_RX_FREQ_STEP_CALLED_R1C / 1000000);
	m_strIFFreqStep.Format("%dM",(int32_t)(IF_RX_FILTER_FREQ_STEP / 1000000));
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_BUTTON2,UnChange);
	SetControlInfo(IDC_BUTTON3,UnChange);
	SetControlInfo(IDC_EDIT1,FixL_xStretch);
	SetControlInfo(IDC_EDIT2,FixL_xStretch);
	SetControlInfo(IDC_EDIT3,UnChange);
	SetControlInfo(IDC_BUTTON4,UnChange);
	SetControlInfo(IDC_BUTTON9,UnChange);
	SetControlInfo(IDC_EDIT4,UnChange);
	SetControlInfo(IDC_EDIT68,UnChange);
	SetControlInfo(IDC_TEST_R1C_RX_FR_CHART,FixL_xyStretch);

	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT4);
	AddThreadIdleCtrl(IDC_EDIT68);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadRunningCtrl(IDC_BUTTON9);
	AddThreadToPauseCtrl(IDC_BUTTON1);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

LRESULT CTestR1CRxFRDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CTestR1CRxFRThread::Result Data = *(CTestR1CRxFRThread::Result *)lParam;
	m_pFRLineSerie[Data.m_iSec]->AddPoint((double)(Data.m_iFreq / 1000000.0),Data.m_dRes);
	return 0;
}

void CTestR1CRxFRDlg::OnBnClickedStartTestIF()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing Rx IF Filter Frequency Response"));
	STACHKV(ThreadCheckBox("Rx<===>Signal Generator"));
	UpdateData(TRUE);
	CtrlPrepare();

	CTestR1CRxFRThread::TestIFFRInfo Info;
	Info.m_pSP1401 = m_pSP1401;
	Info.m_pSP2401 = m_pSP2401;
	str2freq(m_strRFFreq,Info.m_uiRFFreq);
	str2freq(m_strIFFreqStep,Info.m_uiFreqStep);
	ResetShowCtrl(&Info);

	m_pThread = new CTestR1CRxFRThread(this,&Info);
	m_pThread->CreateThread();
}

void CTestR1CRxFRDlg::OnBnClickedPause()
{
	m_pThread->SuspendThread();
	CtrlPausing();
}

void CTestR1CRxFRDlg::OnBnClickedContinue()
{
	m_pThread->ResumeThread();
	CtrlRunning();
}
