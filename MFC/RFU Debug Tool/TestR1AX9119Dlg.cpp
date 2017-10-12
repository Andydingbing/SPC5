#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1AX9119Dlg.h"
#include "TestR1AX9119Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1AX9119Dlg, CTestR1ABaseDlg)
IMPLEMENT_CSE_TEST_R1A_DLG(CTestR1AX9119Dlg)

CTestR1AX9119Dlg::CTestR1AX9119Dlg(CWnd* pParent /*=NULL*/)
	: CTestR1ABaseDlg(CTestR1AX9119Dlg::IDD, pParent)
	, m_strFreq(_T("2.4G"))
{
	m_pChart = NULL;
}

CTestR1AX9119Dlg::~CTestR1AX9119Dlg()
{
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CTestR1AX9119Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreq);
}


BEGIN_MESSAGE_MAP(CTestR1AX9119Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1AX9119Dlg::OnBnClickedStartTest)
	ON_MESSAGE(WM_CSE_TEST_X9119_SHOW, &CTestR1AX9119Dlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestR1AX9119Dlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestR1AX9119Dlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestR1AX9119Dlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestR1AX9119Dlg::Done)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CTestR1AX9119Dlg::OnBnClickedStartTest()
{
	TEST_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();

	m_pLineSerie->ClearSerie();

	m_pThread = new CTestR1AX9119Thread(this);
	m_pThread->CreateThread();
}

BOOL CTestR1AX9119Dlg::OnInitDialog()
{
	CAttachThreadDlg::OnInitDialog();

	CRect crChart = TEST_CHART_RECT;
	crChart.top -= 60;

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,crChart,IDC_TEST_X9119_CHART);
	m_pChart->GetTitle()->AddString(_T("Voltage VS Frequency Offset Curve(X:Value Y:Offset(Hz))"));
	m_pChart->GetTitle()->SetColor(BLUE);
	CChartAxis *pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(0.0,1023.0);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_pLineSerie = m_pChart->CreateLineSerie();
	m_pLineSerie->SetSeriesOrdering(poNoOrdering);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_TEST_X9119_CHART,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON1);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

LRESULT CTestR1AX9119Dlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CTestR1AX9119Thread::Result Data = *(CTestR1AX9119Thread::Result *)lParam;
	m_pLineSerie->AddPoint((double)(Data.m_iValue),(Data.m_dFreq));
	return 0;
}