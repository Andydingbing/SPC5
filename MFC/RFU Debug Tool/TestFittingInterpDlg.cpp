#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestFittingInterpDlg.h"

#define COLUMN_WIDTH 50

IMPLEMENT_DYNAMIC(CTestFittingInterpDlg, CDialog)
IMPLEMENT_CSE_STRETCH_DLG(CTestFittingInterpDlg)

CTestFittingInterpDlg::CTestFittingInterpDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CTestFittingInterpDlg::IDD, pParent)
{
	m_pChart = NULL;
}

CTestFittingInterpDlg::~CTestFittingInterpDlg()
{
	if (m_pChart) {
		m_pChart->RemoveAllSeries();
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CTestFittingInterpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Type);
	DDX_Control(pDX, IDC_STATIC_TFI_0, m_ConfigText);
	DDX_Control(pDX, IDC_EDIT1, m_ConfigEdit);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_STATIC_FX, m_Fx);
}


BEGIN_MESSAGE_MAP(CTestFittingInterpDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestFittingInterpDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestFittingInterpDlg::OnBnClickedDecreasePt)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestFittingInterpDlg::OnBnClickedAddPt)
END_MESSAGE_MAP()


BOOL CTestFittingInterpDlg::OnInitDialog()
{
	CStretchDlg::OnInitDialog();

	DWORD dwStyleList = m_List.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_List.SetExtendedStyle(dwStyleList);

	CString strHead = "";
	m_List.InsertColumn(0,_T(""),LVCFMT_LEFT,COLUMN_WIDTH);
	for (int32_t i = 1;i < 3;i ++) {
		strHead.Format("%d",i);
		m_List.InsertColumn(i,strHead,LVCFMT_LEFT,COLUMN_WIDTH);
	}
	m_List.InsertItem(0,_T("x"));
	m_List.SetItemText(0,1,_T("0"));
	m_List.SetItemText(0,2,_T("0"));
	m_List.InsertItem(1,_T("y"));
	m_List.SetItemText(1,1,_T("0"));
	m_List.SetItemText(1,2,_T("0"));

	m_Type.InsertString(0,_T("polynomial"));
	m_Type.InsertString(1,_T("linear"));
	m_Type.InsertString(2,_T("nearest neighbor"));
	m_Type.InsertString(3,_T("zero"));
	m_Type.InsertString(4,_T("cubic spline"));
	m_Type.InsertString(5,_T("lagrange"));
	m_Type.SetCurSel(0);
	m_ConfigText.SetWindowText(_T("degree"));
	m_ConfigText.ShowWindow(SW_NORMAL);
	m_ConfigEdit.SetWindowText(_T("3"));
	m_ConfigEdit.ShowWindow(SW_NORMAL);
	
	CRect crList,crWnd,crChart;
	m_List.GetWindowRect(&crList);
	this->GetClientRect(&crWnd);
	crChart.left = crList.left - 10;
	crChart.top = crList.bottom + 10;
	crChart.right = crList.right - 10;
	crChart.bottom = crWnd.bottom - 10;

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,crChart,IDC_TEST_FITTING_INTERP_CHART);
	m_pChart->SetZoomRectColor(BLACK);
	CChartStandardAxis *pAxis = NULL;
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);

	m_pLineSerie = m_pChart->CreateLineSerie();
	m_pLineSerie->SetColor(RED);
	m_pPointSerie = m_pChart->CreatePointsSerie();
	m_pPointSerie->SetColor(BLACK);
	m_pPointSerie->SetPointSize(8,8);
	m_pPointSerie->SetPointType(CChartPointsSerie::ptRectangle);

	return TRUE;
}

void CTestFittingInterpDlg::UpdateListToChart()
{
	char *pFunc = NULL;
	CString strType = "";
	double dX = 0.0;
	double dY = 0.0;
	int iColumn = GetListColumn();
	double *pX = new double[iColumn - 1];
	double *pY = new double[iColumn - 1];
	iColumn = 1;
	CString strTextX = m_List.GetItemText(0,iColumn);
	CString strTextY = m_List.GetItemText(1,iColumn);

	m_pPointSerie->ClearSerie();
	m_pLineSerie->ClearSerie();
	while (strTextX != "" && strTextY != "") {
		dX = atof(strTextX.GetBuffer(0));
		dY = atof(strTextY.GetBuffer(0));
		pX[iColumn - 1] = dX;
		pY[iColumn - 1] = dY;
		m_pPointSerie->AddPoint(dX,dY);
		iColumn ++;
		strTextX = m_List.GetItemText(0,iColumn);
		strTextY = m_List.GetItemText(1,iColumn);
	}
	double dXMin = 0.0,dXMax = 0.0;
	double dYMin = 0.0,dYMax = 0.0;
	m_pPointSerie->GetSerieXMinMax(dXMin,dXMax);
	m_pPointSerie->GetSerieYMinMax(dYMin,dYMax);
	dXMin -= 2.0;
	dXMax += 2.0;
	dYMin -= 2.0;
	dYMax += 2.0;
	m_pChart->GetBottomAxis()->SetMinMax(dXMin,dXMax);
	m_pChart->GetLeftAxis()->SetMinMax(dYMin,dYMax);

	iColumn -= 1;
	sort_bubble(pX,pY,iColumn);

	m_Type.GetWindowText(strType);
	if (_T("polynomial") == strType) {
		CString strDegree = "";
		m_ConfigEdit.GetWindowText(strDegree);
		int32_t iDegree = atoi(strDegree);
		double *pCoef = new double[iDegree + 1];
		polyfit(pX,pY,iColumn,iDegree,pCoef);
		pFunc = new char[(iDegree + 1) * 16];
		coef2polynomial(pCoef,iDegree,pFunc);
		m_Fx.SetWindowText(pFunc);
		
		for (dX = dXMin;dX <= dXMax;dX += 1.0) {
			dY = polynomial(pCoef,dX,iDegree);
			m_pLineSerie->AddPoint(dX,dY);
		}
		delete []pCoef;
	}
	else if (_T("linear") == strType || _T("nearest neighbor") == strType || _T("zero") == strType || _T("cubic spline") == strType) {
		CString strInterFactor = "";
		m_ConfigEdit.GetWindowText(strInterFactor);
		int32_t iInterFactor = atoi(strInterFactor);
		double *pXOut = new double[iColumn * iInterFactor - 1];
		double *pYOut = new double[iColumn * iInterFactor - 1];

		interp_method iterMethod = m_linear;
		if (_T("linear") == strType)
			iterMethod = m_linear;
		else if (_T("nearest neighbor") == strType)
			iterMethod = m_nearest;
		else if (_T("zero") == strType)
			iterMethod = m_zero;
		else
			iterMethod = m_spline;
		interp1(pX,pY,iColumn,pXOut,pYOut,iterMethod,iInterFactor);
		m_Fx.SetWindowText(_T(""));
		m_pLineSerie->AddPoints(pXOut,pYOut,iColumn * iInterFactor - 1);

		delete []pXOut;
		delete []pYOut;
	}

	delete []pX;
	delete []pY;
	if (pFunc)
		delete []pFunc;
}

int CTestFittingInterpDlg::GetListColumn()
{
	int iColumn = 1;
	while (m_List.GetItemText(0,iColumn) != "")
		iColumn ++;
	return iColumn;
}

void CTestFittingInterpDlg::OnBnClickedApply()
{
	UpdateListToChart();
}

void CTestFittingInterpDlg::OnBnClickedDecreasePt()
{
	m_List.DeleteColumn(GetListColumn() - 1);
	m_List.SetRedraw();
}

void CTestFittingInterpDlg::OnBnClickedAddPt()
{
	int iColumn = GetListColumn();
	CString strHead = "";
	strHead.Format("%d",iColumn);
	for (int i = 0;i < 2;i ++) {
		m_List.InsertColumn(iColumn + i,strHead,LVCFMT_LEFT,COLUMN_WIDTH);
		m_List.SetItemText(0,iColumn + i,_T("0"));
		m_List.SetItemText(1,iColumn + i,_T("0"));
	}
	OnBnClickedDecreasePt();
}
