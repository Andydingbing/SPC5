#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxAttTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CRxAttTabCtrl, CCSETabCtrl)

CCalR1CRxAttTabCtrl::CCalR1CRxAttTabCtrl()
{
	m_pChartOP = NULL;
	m_pChartIO = NULL;
	m_pListOP = NULL;
	m_pListIO = NULL;
	m_OPLineSerie.clear();
	m_IOLineSerie.clear();
}

CCalR1CRxAttTabCtrl::~CCalR1CRxAttTabCtrl()
{
	ClearLineSerieOP();
	ClearLineSerieIO();
	SAFE_DESTROY_DLG(m_pChartOP);
	SAFE_DESTROY_DLG(m_pChartIO);
	SAFE_DESTROY_DLG(m_pListOP);
	SAFE_DESTROY_DLG(m_pListIO);
}


BEGIN_MESSAGE_MAP(CCalR1CRxAttTabCtrl, CCSETabCtrl)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CCalR1CRxAttTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()


void CCalR1CRxAttTabCtrl::AttachChild()
{
	CRect crTabCtrl,crCtrlL,crCtrlR;
	this->GetClientRect(&crTabCtrl);
	crTabCtrl.top    += 30;
	crTabCtrl.bottom -= 8;
	crTabCtrl.left   += 8;
	crTabCtrl.right  -= 8;

	crCtrlL.top = crCtrlR.top = crTabCtrl.top;
	crCtrlL.bottom = crCtrlR.bottom = crTabCtrl.bottom;
	crCtrlL.left = crTabCtrl.left;
	crCtrlL.right = (crTabCtrl.right - crTabCtrl.left) / 2 + crTabCtrl.left - 1;
	crCtrlR.left = crCtrlL.right + 2;
	crCtrlR.right = crTabCtrl.right;

	m_pListOP = new CProtectEyeListCtrl;
	m_pListOP->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_OWNERDATA | LVS_ALIGNLEFT | LVS_REPORT,crCtrlL,this,IDC_LIST_RX_ATT_R1C_OP);

	DWORD dwStyleList = 0;
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_pListOP->SetExtendedStyle(dwStyleList);
	m_pListOP->SetBkColor(PROTECT_EYE);
	m_pListOP->SetTextBkColor(PROTECT_EYE);
	m_pListOP->ShowWindow(SW_NORMAL);

	m_pListIO = new CProtectEyeListCtrl;
	m_pListIO->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_OWNERDATA | LVS_ALIGNLEFT | LVS_REPORT,crCtrlR,this,IDC_LIST_RX_ATT_R1C_IO);
	m_pListIO->SetExtendedStyle(dwStyleList);
	m_pListIO->SetBkColor(PROTECT_EYE);
	m_pListIO->SetTextBkColor(PROTECT_EYE);
	m_pListIO->ShowWindow(SW_NORMAL);

	m_pChartOP = new CChartCtrl;
	m_pChartOP->Create(this,crCtrlL,IDC_CAL_R1C_TX_ATT_OP_CHART);
	m_pChartOP->ShowWindow(SW_HIDE);
	m_pChartOP->SetZoomRectColor(BLACK);
	m_pChartOP->GetTitle()->AddString(_T("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))(Output)"));
	m_pChartOP->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChartOP->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChartOP->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(-2.0,1.5);
	
	m_pChartIO = new CChartCtrl;
	m_pChartIO->Create(this,crCtrlR,IDC_CAL_R1C_TX_ATT_IO_CHART);
	m_pChartIO->ShowWindow(SW_HIDE);
	m_pChartIO->SetZoomRectColor(BLACK);
	m_pChartIO->GetTitle()->AddString(_T("Att Offset Curve(X:Freq(MHz) Y:Offset(dB))(IO)"));
	m_pChartIO->GetTitle()->SetColor(BLUE);

	pAxis = m_pChartIO->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChartIO->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(-2.0,1.5);
}

void CCalR1CRxAttTabCtrl::ResetShowCtrl(CalIOMode Mode)
{
	if (CalOP(Mode)) {
		m_DataOP.clear();
		m_pListOP->DeleteAllItems();
		while(m_pListOP->DeleteColumn(0));
		m_pListOP->InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,80);
		m_pListOP->InsertColumn(1,_T("Reference"),LVCFMT_RIGHT,70);
		m_pListOP->InsertColumn(2,_T("LNA/Att"),LVCFMT_CENTER,60);
		m_pListOP->InsertColumn(3,_T("Att0/19"),LVCFMT_CENTER,60);
		m_pListOP->InsertColumn(4,_T("Att 1"),LVCFMT_RIGHT,40);
		m_pListOP->InsertColumn(5,_T("Att 2"),LVCFMT_RIGHT,40);
		m_pListOP->InsertColumn(6,_T("Att 3"),LVCFMT_RIGHT,40);
		m_pListOP->InsertColumn(7,_T("DGain"),LVCFMT_RIGHT,70);
		m_pListOP->InsertColumn(8,_T("Offset"),LVCFMT_RIGHT,100);
		m_pListOP->InsertColumn(9,_T("Temp0_RxLO2"),LVCFMT_CENTER,100);
		m_pListOP->InsertColumn(10,_T("Temp1_RxLNA"),LVCFMT_CENTER,100);
		m_pListOP->InsertColumn(11,_T("Temp2_RxLO1"),LVCFMT_CENTER,100);
		m_pListOP->InsertColumn(12,_T("Temp3_RxPA1"),LVCFMT_CENTER,100);
		m_pListOP->InsertColumn(13,_T("End Time"),LVCFMT_RIGHT,140);

		DWORD dwStyleList = ::GetWindowLong(m_pListOP->m_hWnd,GWL_STYLE);
		dwStyleList |= LVS_SINGLESEL;
		::SetWindowLong(m_pListOP->m_hWnd,GWL_STYLE,dwStyleList);

		ClearLineSerieOP();
		m_pChartOP->RemoveAllSeries();
	}
	
	if (CalIO(Mode)) {
		m_DataIO.clear();
		m_pListIO->DeleteAllItems();
		while(m_pListIO->DeleteColumn(0));
		m_pListIO->InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,80);
		m_pListIO->InsertColumn(1,_T("Reference"),LVCFMT_RIGHT,70);
		m_pListIO->InsertColumn(2,_T("LNA/Att"),LVCFMT_CENTER,60);
		m_pListIO->InsertColumn(3,_T("Att0/19"),LVCFMT_CENTER,60);
		m_pListIO->InsertColumn(4,_T("Att 1"),LVCFMT_RIGHT,40);
		m_pListIO->InsertColumn(5,_T("Att 2"),LVCFMT_RIGHT,40);
		m_pListIO->InsertColumn(6,_T("Att 3"),LVCFMT_RIGHT,40);
		m_pListIO->InsertColumn(7,_T("DGain"),LVCFMT_RIGHT,70);
		m_pListIO->InsertColumn(8,_T("Offset"),LVCFMT_RIGHT,100);
		m_pListIO->InsertColumn(9,_T("Temp0_RxLO2"),LVCFMT_CENTER,100);
		m_pListIO->InsertColumn(10,_T("Temp1_RxLNA"),LVCFMT_CENTER,100);
		m_pListIO->InsertColumn(11,_T("Temp2_RxLO1"),LVCFMT_CENTER,100);
		m_pListIO->InsertColumn(12,_T("Temp3_RxPA1"),LVCFMT_CENTER,100);
		m_pListIO->InsertColumn(13,_T("End Time"),LVCFMT_RIGHT,140);

		DWORD dwStyleList = ::GetWindowLong(m_pListIO->m_hWnd,GWL_STYLE);
		dwStyleList |= LVS_SINGLESEL;
		::SetWindowLong(m_pListIO->m_hWnd,GWL_STYLE,dwStyleList);

		ClearLineSerieIO();
		m_pChartIO->RemoveAllSeries();
	}
}

void CCalR1CRxAttTabCtrl::ClearLineSerieOP()
{
	for (int32_t i = 0;i < m_OPLineSerie.size();i ++) {
		for (int32_t j = 0;j < m_OPLineSerie.at(i).size();j ++)
			m_OPLineSerie.at(i).at(j)->ClearSerie();
		m_OPLineSerie.at(i).clear();
	}
	m_OPLineSerie.clear();
}

void CCalR1CRxAttTabCtrl::ClearLineSerieIO()
{
	for (int32_t i = 0;i < m_IOLineSerie.size();i ++) {
		for (int32_t j = 0;j < m_IOLineSerie.at(i).size();j ++)
			m_IOLineSerie.at(i).at(j)->ClearSerie();
		m_IOLineSerie.at(i).clear();
	}
	m_IOLineSerie.clear();
}

void CCalR1CRxAttTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	m_pListOP->MoveWindow(8,30,(cx - 16) / 2 - 1,cy - 38);
	m_pListIO->MoveWindow((cx - 16) / 2 + 8 + 2,30,(cx - 16) / 2,cy - 38);
	m_pChartOP->MoveWindow(8,30,(cx - 16) / 2 - 1,cy - 38);
	m_pChartIO->MoveWindow((cx - 16) / 2 + 8 + 2,30,(cx - 16) / 2,cy - 38);
}

void CCalR1CRxAttTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iTabIdx = GetCurSel();
	if (0 == iTabIdx) {
		if (::IsWindow(m_pListOP->GetSafeHwnd()))
			m_pListOP->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pListIO->GetSafeHwnd()))
			m_pListIO->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pChartOP->GetSafeHwnd()))
			m_pChartOP->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pChartIO->GetSafeHwnd()))
			m_pChartIO->ShowWindow(SW_HIDE);
	}
	else if (1 == iTabIdx) {
		if (::IsWindow(m_pListOP->GetSafeHwnd()))
			m_pListOP->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pListIO->GetSafeHwnd()))
			m_pListIO->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pChartOP->GetSafeHwnd()))
			m_pChartOP->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pChartIO->GetSafeHwnd()))
			m_pChartIO->ShowWindow(SW_NORMAL);
	}
	*pResult = 0;
}