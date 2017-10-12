#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1CTTBDlg.h"
#include "TestR1CTTBThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1CTTBDlg, CTestR1CBaseDlg)
IMPLEMENT_CSE_TEST_R1C_DLG(CTestR1CTTBDlg)

CTestR1CTTBDlg::CTestR1CTTBDlg(CWnd* pParent /*=NULL*/)
	: CTestR1CBaseDlg(CTestR1CTTBDlg::IDD, pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_iSpeedStep(0)
	, m_iKeepTime(0)
{
	m_pChart = NULL;
	m_PointsSerie.clear();
}

CTestR1CTTBDlg::~CTestR1CTTBDlg()
{
	for (int32_t i = 0;i < m_PointsSerie.size();i ++)
		m_PointsSerie.at(i)->ClearSerie();
	m_PointsSerie.clear();
	if (m_pChart) {
		m_pChart->RemoveAllSeries();
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CTestR1CTTBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT12, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT14, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT2, m_iSpeedStep);
	DDX_Text(pDX, IDC_EDIT3, m_iKeepTime);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CTestR1CTTBDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1CTTBDlg::OnBnClickedStart)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CSE_TEST_TTB_SHOW, &CTestR1CTTBDlg::ShowData)
END_MESSAGE_MAP()


void CTestR1CTTBDlg::ResetChart()
{
	uint64_t uiFreqStar = 0,uiFreqStop = 0;
	uint64_t uiFreq[RF_TX_FREQ_PTS_CALLED];
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,(int64_t *)uiFreq,NULL,NULL,(int64_t *)&uiFreqStop,(int64_t *)&uiFreqStar);

	for (int32_t i = 0;i < m_PointsSerie.size();i ++)
		m_PointsSerie.at(i)->ClearSerie();
	m_PointsSerie.clear();

	for (int32_t i = 0;i < iPts;i ++) {
		CChartPointsSerie *pPointsSerie = m_pChart->CreatePointsSerie();
		pPointsSerie->SetSeriesOrdering(poNoOrdering);
		m_PointsSerie.push_back(pPointsSerie);
	}

	CString strFreq = "";
	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	for (int32_t i = 0;i < iPts;i ++) {
		strFreq.Format("%llu",uiFreq[i]);
		m_List.InsertItem(i,strFreq);
		m_List.SetCheck(i,TRUE);
	}

	m_pChart->GetBottomAxis()->SetMinMax(uiFreqStar / 1000000.0,uiFreqStop / 1000000.0);
}

void CTestR1CTTBDlg::OnBnClickedStart()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing Tx BasePower vs RF Temperature"));
	STACHKV(ThreadCheckBox("Tx<===>Power Meter"));
	UpdateData(TRUE);
	CtrlPrepare();
	ResetChart();

	m_pThread = new CTestR1CTTBThread(this);
	m_pThread->CreateThread();
}

BOOL CTestR1CTTBDlg::OnInitDialog()
{
	CTestR1CBaseDlg::OnInitDialog();

	m_List.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_LEFT,70);

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,TEST_CHART_RECT,IDC_TEST_R1C_TTB_CHART);
	m_pChart->SetZoomRectColor(BLACK);
	m_pChart->GetTitle()->AddString(_T("Temperature VS BasePower Curve(X:Temp(¡æ) Y(Power(dBm))"));
	m_pChart->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_strFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strFreqStep = _T("100M");

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT12,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_LIST1,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_TEST_R1C_TTB_CHART,ChgLT_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT12);
	AddThreadIdleCtrl(IDC_EDIT14);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_EDIT4);
	AddThreadIdleCtrl(IDC_BUTTON1);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	return TRUE;
}

LRESULT CTestR1CTTBDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CTestR1CTTBThread::Result Data = *(CTestR1CTTBThread::Result *)lParam;
	m_PointsSerie.at(Data.m_iFreqIdx)->AddPoint(Data.m_dTemp,Data.m_dPower);
	return 0;
}