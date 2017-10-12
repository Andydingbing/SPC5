#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "DMADlg.h"
#include "DMAThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDMADlg, CStretchDlg)
IMPLEMENT_CSE_ATTACHTHREAD_DLG(CDMADlg)

CDMADlg::CDMADlg(CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(CDMADlg::IDD, pParent)
	, m_uiRTlpSize(16)
	, m_uiRSamples(614400)
	, m_uiWTlpSize(32)
	, m_uiWSamples(614400)
	, m_uiTestSamples(614400)
	, m_uiTestTimes(10)
	, m_uiTestCurTimes(0)
	, m_bTestR(FALSE)
	, m_bTestW(FALSE)
{
	m_pChart = NULL;
}

CDMADlg::~CDMADlg()
{
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CDMADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_uiRTlpSize);
	DDX_Text(pDX, IDC_EDIT12, m_uiRSamples);
	DDX_Text(pDX, IDC_EDIT2, m_uiWTlpSize);
	DDX_Text(pDX, IDC_EDIT32, m_uiWSamples);
	DDX_Control(pDX, IDC_EDIT5, m_WPath);
	DDX_Text(pDX, IDC_EDIT69, m_uiTestSamples);
	DDX_Text(pDX, IDC_EDIT74, m_uiTestTimes);
	DDX_Text(pDX, IDC_EDIT75, m_uiTestCurTimes);
	DDX_Check(pDX, IDC_CHECK3, m_bTestR);
	DDX_Check(pDX, IDC_CHECK4, m_bTestW);
}


BEGIN_MESSAGE_MAP(CDMADlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CDMADlg::OnBnClickedRStart)
	ON_BN_CLICKED(IDC_BUTTON3, &CDMADlg::OnBnClickedWStart)
	ON_BN_CLICKED(IDC_BUTTON4, &CDMADlg::OnBnClickedSetWPath)
	ON_BN_CLICKED(IDC_BUTTON22, &CDMADlg::OnBnClickedOpenDataFile)
	ON_BN_CLICKED(IDC_BUTTON9, &CDMADlg::OnBnClickedShowWV)
	ON_BN_CLICKED(IDC_BUTTON5, &CDMADlg::OnBnClickedStartWRTest)
	ON_BN_CLICKED(IDC_BUTTON12, &CDMADlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CDMADlg::Done)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CDMADlg::UpdateDataFalse)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDMADlg::OnBnClickedRStart()
{
	UpdateData(TRUE);
	uint32_t uiSamples = uint32_t(m_uiRSamples);
	m_uiWSamples = m_uiRSamples;
	m_uiTestSamples = m_uiRSamples;
	UpdateData(FALSE);

	uint32_t *pData = new uint32_t[uiSamples];
	for (uint32_t i = 0;i < uiSamples;i ++)
		pData[i] = i;
	DDR->FpgaReadAll(m_pSP1401->GetK7(),pData,uiSamples);
	delete []pData;
}

void CDMADlg::OnBnClickedWStart()
{
	UpdateData(TRUE);
	uint32_t uiSamples = uint32_t(m_uiWSamples);
	CString strPath;
	m_WPath.GetWindowText(strPath);
	STACHKV(m_pSP1401->SetIQCapSamples(uiSamples));
//	STACHKV(m_pSP1401->SetIQCapSrc(CSP1401::IQCapsrcFreeRun,false));
	STACHKV(m_pSP1401->IQCap());
	STACHKV(DDR->WDumpToFileCh(m_pSP1401->GetRfIdx(),(LPSTR)(LPCTSTR)strPath,uiSamples));
}

void CDMADlg::OnBnClickedSetWPath()
{
	CFileDialog FileDlg(TRUE);
	if (FileDlg.DoModal() == IDOK)
		m_WPath.SetWindowText(FileDlg.GetPathName());
}

void CDMADlg::OnBnClickedOpenDataFile()
{
	UpdateData(TRUE);
	CString strPath;
	m_WPath.GetWindowText(strPath);
	if (strPath == "") {
		Log->AddMsgList(-1,"Invalid Path!");
		return;
	}
	char szCmd[128] = "start /b ";
	strcat(szCmd,(LPSTR)(LPCTSTR)strPath);
	system(szCmd);
}

BOOL CDMADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_WPath.SetWindowText("C:\\iq.txt");

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,IQ_CAPTURE_CHART_RECT,IDC_DMAW_IQ);
	CChartAxis *pAxis = NULL;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_pLineSerie = m_pChart->CreateLineSerie();
	m_pLineSerie->SetSeriesOrdering(poNoOrdering);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_DMAW_IQ,FixL_xyStretch,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT12);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT32);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_BUTTON3);
	AddThreadIdleCtrl(IDC_EDIT69);
	AddThreadIdleCtrl(IDC_EDIT74);
	AddThreadIdleCtrl(IDC_CHECK3);
	AddThreadIdleCtrl(IDC_CHECK4);
	AddThreadIdleCtrl(IDC_BUTTON5);
	AddThreadRunningCtrl(IDC_BUTTON12);
	CtrlResume();
	return TRUE;
}

void CDMADlg::OnBnClickedShowWV()
{
	UpdateData(TRUE);

	double *pX = new double[m_uiWSamples];
	for (UINT i = 0;i < m_uiWSamples;i ++)
		pX[i] = (double)i;

	int32_t *pData = new int32_t[m_uiWSamples];
	STACHKV(DDR->WDumpToBuf(pData,(uint32_t)m_uiWSamples));

	m_pLineSerie->ClearSerie();
	m_pLineSerie->AddPoints(pX,pData,m_uiWSamples);

	delete []pData;
	delete []pX;
}

void CDMADlg::OnBnClickedStartWRTest()
{
	TEST_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();

	CDMAThread *pThread = new CDMAThread(this);
	pThread->CreateThread();
}