#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1ATxFRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1ATxFRDlg, CTestR1ABaseDlg)
IMPLEMENT_CSE_TEST_R1A_DLG(CTestR1ATxFRDlg)

CTestR1ATxFRDlg::CTestR1ATxFRDlg(CWnd* pParent /*=NULL*/)
	: CTestR1ABaseDlg(CTestR1ATxFRDlg::IDD, pParent)
	, m_strRFFreqStar(_T(""))
	, m_strRFFreqStop(_T(""))
	, m_strRFFreqStep(_T(""))
	, m_strRFFreq(_T("2.4G"))
	, m_strBBFreqStep(_T(""))
{
	m_pChart = NULL;
	m_pFRLineSerie.clear();
}

CTestR1ATxFRDlg::~CTestR1ATxFRDlg()
{
	for (int i = 0;i < m_pFRLineSerie.size();i ++)
		m_pFRLineSerie[i]->ClearSerie();
	m_pFRLineSerie.clear();
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CTestR1ATxFRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRFFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strRFFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strRFFreqStep);
	DDX_Text(pDX, IDC_EDIT4, m_strRFFreq);
	DDX_Text(pDX, IDC_EDIT68, m_strBBFreqStep);
}


BEGIN_MESSAGE_MAP(CTestR1ATxFRDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1ATxFRDlg::OnBnClickedStartTestRF)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestR1ATxFRDlg::OnBnClickedStartTestBB)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestR1ATxFRDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestR1ATxFRDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_TEST_TX_FR_SHOW, &CTestR1ATxFRDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestR1ATxFRDlg::Done)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestR1ATxFRDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestR1ATxFRDlg::OnBnClickedContinue)
END_MESSAGE_MAP()


void CTestR1ATxFRDlg::ResetShowCtrl(CTestR1ATxFRThread::TestRFFRInfo *pInfo)
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	freq2array(pInfo->m_strFreqStar,pInfo->m_strFreqStop,pInfo->m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	m_pChart->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);

	for (int i = 0;i < m_pFRLineSerie.size();i ++)
		m_pFRLineSerie[i]->ClearSerie();
	m_pFRLineSerie.clear();
	m_pChart->RemoveAllSeries();
	m_pChart->GetTitle()->RemoveAll();
	m_pChart->GetTitle()->AddString(_T("Tx RF Frequency Response Curve(X:Freq(MHz) Y:Power(dBm))"));

	int32_t iSec = strsections((LPSTR)(LPCTSTR)(pInfo->m_strFreqStar));
	for (int32_t i = 0;i < iSec;i ++) {
		CChartLineSerie *pFRLineSerie;
		m_pFRLineSerie.push_back(pFRLineSerie);
		m_pFRLineSerie[i] = m_pChart->CreateLineSerie();
		m_pFRLineSerie[i]->SetSeriesOrdering(poNoOrdering);
		m_pFRLineSerie[i]->SetColor(RED);
	}
}

void CTestR1ATxFRDlg::ResetShowCtrl(CTestR1ATxFRThread::TestBBFRInfo *pInfo)
{
	m_pChart->GetBottomAxis()->SetMinMax(-1.0 * MAX_BW / 1000000.0 / 2.0,MAX_BW / 1000000.0 / 2.0);

	for (int i = 0;i < m_pFRLineSerie.size();i ++)
		m_pFRLineSerie[i]->ClearSerie();
	m_pFRLineSerie.clear();
	m_pChart->RemoveAllSeries();
	m_pChart->GetTitle()->RemoveAll();
	m_pChart->GetTitle()->AddString(_T("Tx Baseband Filter Frequency Response Curve(X:Freq(MHz) Y:Power(dBm))"));

	CChartLineSerie *pFRLineSerie;
	m_pFRLineSerie.push_back(pFRLineSerie);
	m_pFRLineSerie[0] = m_pChart->CreateLineSerie();
	m_pFRLineSerie[0]->SetSeriesOrdering(poNoOrdering);
	m_pFRLineSerie[0]->SetColor(RED);
}

void CTestR1ATxFRDlg::OnBnClickedStartTestRF()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing Tx RF Frequency Response"));
	STACHKV(ThreadCheckBox("Tx<===>Spectrum"));
	UpdateData(TRUE);
	CtrlPrepare();

	CTestR1ATxFRThread::TestRFFRInfo Info;
	Info.m_pSP1401 = m_pSP1401;
	Info.m_pSP2401 = m_pSP2401;
	Info.m_strFreqStar = m_strRFFreqStar;
	Info.m_strFreqStop = m_strRFFreqStop;
	Info.m_strFreqStep = m_strRFFreqStep;
	ResetShowCtrl(&Info);

	m_pThread = new CTestR1ATxFRThread(this,&Info);
	m_pThread->CreateThread();
}

BOOL CTestR1ATxFRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,TEST_CHART_RECT,IDC_TEST_R1A_TX_FR_CHART);
	m_pChart->GetTitle()->AddString(_T("Tx RF Frequency Response Curve(X:Freq(MHz) Y:Power(dBm))"));
	m_pChart->SetZoomRectColor(BLACK);
	m_pChart->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_strRFFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strRFFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strRFFreqStep.Format("%dM",RF_TX_FREQ_STEP_INFILE / 1000000);
	m_strBBFreqStep.Format("%dM",(int32_t)(IF_TX_FILTER_FREQ_STEP / 1000000));
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
	SetControlInfo(IDC_TEST_R1A_TX_FR_CHART,FixL_xyStretch);

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

LRESULT CTestR1ATxFRDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CTestR1ATxFRThread::Result Data = *(CTestR1ATxFRThread::Result *)lParam;
	m_pFRLineSerie[Data.m_iSec]->AddPoint((double)(Data.m_iFreq / 1000000.0),Data.m_dRes);
	return 0;
}

void CTestR1ATxFRDlg::OnBnClickedStartTestBB()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing Tx Baseband Filter Frequency Response"));
	STACHKV(ThreadCheckBox("Tx<===>Spectrum"));
	UpdateData(TRUE);
	CtrlPrepare();

	CTestR1ATxFRThread::TestBBFRInfo Info;
	Info.m_pSP1401 = m_pSP1401;
	Info.m_pSP2401 = m_pSP2401;
	str2freq(m_strRFFreq,Info.m_uiRFFreq);
	str2freq(m_strBBFreqStep,Info.m_uiFreqStep);
	ResetShowCtrl(&Info);

	m_pThread = new CTestR1ATxFRThread(this,&Info);
	m_pThread->CreateThread();
}

void CTestR1ATxFRDlg::OnBnClickedPause()
{
	m_pThread->SuspendThread();
	CtrlPausing();
}

void CTestR1ATxFRDlg::OnBnClickedContinue()
{
	m_pThread->ResumeThread();
	CtrlRunning();
}
