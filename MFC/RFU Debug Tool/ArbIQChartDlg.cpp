#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "ArbIQChartDlg.h"
#include <complex>
#include "fftw3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CLR_I RGB(255,0,0)
#define CLR_Q RGB(0,255,0)

IMPLEMENT_DYNAMIC(CArbIQChartDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CArbIQChartDlg)

CArbIQChartDlg::CArbIQChartDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CArbIQChartDlg::IDD, pParent)
	, m_bI(TRUE)
	, m_bQ(TRUE)
	, m_bShowingDFT(FALSE)
{
	m_pChart = NULL;
}

CArbIQChartDlg::~CArbIQChartDlg()
{
	if (m_pChart) {
		m_pChart->RemoveAllSeries();
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CArbIQChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ARB_I, m_bI);
	DDX_Check(pDX, IDC_CHECK_ARB_Q, m_bQ);
	DDX_Control(pDX, IDC_EDIT1, m_FilePath);
	DDX_Control(pDX, IDC_COMBO1, m_Seg);
}


BEGIN_MESSAGE_MAP(CArbIQChartDlg, CDialog)
	ON_MESSAGE(WM_PASTE,&CArbIQChartDlg::OnDrawIQWav)
	ON_BN_CLICKED(IDC_CHECK_ARB_I, &CArbIQChartDlg::OnBnClickedCheckI)
	ON_BN_CLICKED(IDC_CHECK_ARB_Q, &CArbIQChartDlg::OnBnClickedCheckQ)
	ON_BN_CLICKED(IDC_BUTTON1, &CArbIQChartDlg::OnBnClickedSelArbFile)
	ON_BN_CLICKED(IDC_BUTTON3, &CArbIQChartDlg::OnBnClickedShowTD)
	ON_BN_CLICKED(IDC_BUTTON4, &CArbIQChartDlg::OnBnClickedShowDFT)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CArbIQChartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->GetClientRect(&m_crWnd);

	CRect crChart = m_crWnd;
	crChart.top += 30;
	m_pChart = new CChartCtrl;
	m_pChart->Create(this,crChart,IDC_ARB_IQ_CHART);
	m_pChart->SetZoomEnabled(true);
	m_pChart->SetZoomRectColor(BLACK);

	CChartAxis *pAxis = NULL;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_pILineSerie = m_pChart->CreateLineSerie();
	m_pILineSerie->SetColor(CLR_I);
	m_pILineSerie->SetSeriesOrdering(poNoOrdering);
	m_pILineSerie->SetVisible(m_bI);
	m_pQLineSerie = m_pChart->CreateLineSerie();
	m_pQLineSerie->SetColor(CLR_Q);
	m_pQLineSerie->SetSeriesOrdering(poNoOrdering);
	m_pQLineSerie->SetVisible(m_bQ);
	m_pDFTLineSerie = m_pChart->CreateLineSerie();
	m_pDFTLineSerie->SetSeriesOrdering(poNoOrdering);
	m_pDFTLineSerie->SetVisible(false);
	SetControlInfo(IDC_ARB_IQ_CHART,FixL_xyStretch,1,0,1,1,0,1);
	return TRUE;
}

void CArbIQChartDlg::OnCancel()
{
	ShowWindow(FALSE);
	CDialog::OnCancel();
}

void CArbIQChartDlg::DrawSeg(int32_t iSeg)
{
	UpdateData(TRUE);

	CString strPath = "";
	m_FilePath.GetWindowText(strPath);

	ArbReader File;
	ArbReader::Param_t Param;
	vector<ArbReader::DataVector> Data;
	File.Load((LPSTR)(LPCTSTR)strPath);
	File.Load(&Param,&Data);
	
	const ArbReader::Data_t *pData/* = File.GetData(iSeg)*/;
	const ArbReader::SegHeader_t *pSeg/* = File.GetSegInfo(iSeg)*/;
	if (m_bShowingDFT) {
		uint32_t uiSpls = (uint32_t)(pSeg->Samples);
		uiSpls = uiSpls > 409600 ? 409600 : uiSpls;
		DFT((int32_t)(pSeg->SampleRate),uiSpls,pData,m_pDFTLineSerie);
	}
	else
		TimeDomain((uint32_t)(pSeg->Samples),pData,m_pILineSerie,m_pQLineSerie);
}

LRESULT CArbIQChartDlg::OnDrawIQWav(WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);
	ShowWindow(TRUE);

	CString strPath = "";
	UpdateData(FALSE);
	return 0;
}

void CArbIQChartDlg::OnBnClickedCheckI()
{
	UpdateData(TRUE);
	m_pILineSerie->SetVisible(m_bI);
}

void CArbIQChartDlg::OnBnClickedCheckQ()
{
	UpdateData(TRUE);
	m_pQLineSerie->SetVisible(m_bQ);
}

HBRUSH CArbIQChartDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case (IDC_STATIC_1000): {
		pDC->SetTextColor(CLR_I);
		return (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
		break;
							}
	case (IDC_STATIC_1001): {
		pDC->SetTextColor(CLR_Q);
		return (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
		break;
							}
	default:
		return hbr;
	}
	return hbr;
}

void CArbIQChartDlg::OnBnClickedSelArbFile()
{
	CFileDialog ArbFile(TRUE);
	if (ArbFile.DoModal() == IDOK)
		m_FilePath.SetWindowText(ArbFile.GetPathName());
	else
		return;

	ArbReader File;
	ArbReader::Param_t Param;
	File.Load((LPSTR)(LPCTSTR)ArbFile.GetPathName());
	File.Load(&Param);

	CString strItem = "";
	m_Seg.Clear();
	for (int i = 0;i < Param.FileHeader.Segments;i ++) {
		strItem.Format("%d",i);
		m_Seg.AddString(strItem);
	}
	m_Seg.SetCurSel(0);

	DrawSeg(0);
}

void CArbIQChartDlg::OnBnClickedShowTD()
{
	m_bShowingDFT = FALSE;
	m_pILineSerie->SetVisible(true);
	m_pQLineSerie->SetVisible(true);
	m_pDFTLineSerie->SetVisible(false);
	DrawSeg(m_Seg.GetCurSel());
}

void CArbIQChartDlg::OnBnClickedShowDFT()
{
	m_bShowingDFT = TRUE;
	m_pILineSerie->SetVisible(false);
	m_pQLineSerie->SetVisible(false);
	m_pDFTLineSerie->SetVisible(true);
	DrawSeg(m_Seg.GetCurSel());
}
