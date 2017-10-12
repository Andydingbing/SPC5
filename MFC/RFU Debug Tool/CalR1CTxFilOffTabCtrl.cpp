#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxFilOffTabCtrl.h"

#define CHART_HEIGHT 150

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CTxFilOffTabCtrl, CCSETabCtrl)

CCalR1CTxFilOffTabCtrl::CCalR1CTxFilOffTabCtrl()
{
	m_pChartOP_80 = NULL;
	m_pChartIO_80 = NULL;
	m_pListOP_80 = NULL;
	m_pListIO_80 = NULL;
	m_OPLineSerie_80.clear();
	m_IOLineSerie_80.clear();
}

CCalR1CTxFilOffTabCtrl::~CCalR1CTxFilOffTabCtrl()
{
	ClearLineSerieOP();
	ClearLineSerieIO();
	SAFE_DESTROY_DLG(m_pChartOP_80);
	SAFE_DESTROY_DLG(m_pChartIO_80);
	SAFE_DESTROY_DLG(m_pListOP_80);
	SAFE_DESTROY_DLG(m_pListIO_80);
}


BEGIN_MESSAGE_MAP(CCalR1CTxFilOffTabCtrl, CCSETabCtrl)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CCalR1CTxFilOffTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()


void CCalR1CTxFilOffTabCtrl::AttachChild()
{
	CRect crTabCtrl,crChartL,crChartR,crListL,crListR;
	this->GetClientRect(&crTabCtrl);
	crTabCtrl.top    += 30;
	crTabCtrl.bottom -= 8;
	crTabCtrl.left   += 8;
	crTabCtrl.right  -= 8;

	crListL.top = crListR.top = crTabCtrl.top;
	crListL.bottom = crListR.bottom = crTabCtrl.bottom - CHART_HEIGHT;
	crListL.left = crTabCtrl.left;
	crListL.right = (crTabCtrl.right - crTabCtrl.left) / 2 + crTabCtrl.left - 1;
	crListR.left = crListL.right + 2;
	crListR.right = crTabCtrl.right;

	crChartL.top = crListL.bottom;
	crChartL.bottom = crTabCtrl.bottom;
	crChartL.left = crListL.left;
	crChartL.right = crListL.right;

	crChartR.top = crChartL.top;
	crChartR.bottom = crChartR.bottom;
	crChartR.left = crListR.left;
	crChartR.right = crListR.right;

	m_pListOP_80 = new CProtectEyeListCtrl;
	m_pListOP_80->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_OWNERDATA | LVS_ALIGNLEFT | LVS_REPORT,crListL,this,IDC_LIST_TX_FILOFF_R1C_OP);

	DWORD dwStyleList = 0;
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_pListOP_80->SetExtendedStyle(dwStyleList);
	m_pListOP_80->SetBkColor(PROTECT_EYE);
	m_pListOP_80->SetTextBkColor(PROTECT_EYE);
	m_pListOP_80->ShowWindow(SW_NORMAL);

	m_pListIO_80 = new CProtectEyeListCtrl;
	m_pListIO_80->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_OWNERDATA | LVS_ALIGNLEFT | LVS_REPORT,crListR,this,IDC_LIST_TX_FILOFF_R1C_IO);
	m_pListIO_80->SetExtendedStyle(dwStyleList);
	m_pListIO_80->SetBkColor(PROTECT_EYE);
	m_pListIO_80->SetTextBkColor(PROTECT_EYE);
	m_pListIO_80->ShowWindow(SW_NORMAL);

	m_pChartOP_80 = new CChartCtrl;
	m_pChartOP_80->Create(this,crChartL,IDC_CAL_R1C_TX_FILOFF_OP_CHART);
	m_pChartOP_80->ShowWindow(SW_NORMAL);
	m_pChartOP_80->SetZoomRectColor(BLACK);
	m_pChartOP_80->GetTitle()->AddString(_T("Filter Offset Curve(X:Freq(MHz) Y:Offset(dB))(Output)"));
	m_pChartOP_80->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChartOP_80->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChartOP_80->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	
	m_pChartIO_80 = new CChartCtrl;
	m_pChartIO_80->Create(this,crChartR,IDC_CAL_R1C_TX_FILOFF_IO_CHART);
	m_pChartIO_80->ShowWindow(SW_NORMAL);
	m_pChartIO_80->SetZoomRectColor(BLACK);
	m_pChartIO_80->GetTitle()->AddString(_T("Filter Offset Curve(X:Freq(MHz) Y:Offset(dB))(IO)"));
	m_pChartIO_80->GetTitle()->SetColor(BLUE);

	pAxis = m_pChartIO_80->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_TX_FREQ_STAR / 1000000.0,RF_TX_FREQ_STOP / 1000000.0);
	pAxis = m_pChartIO_80->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
}

void CCalR1CTxFilOffTabCtrl::ResetShowCtrl(CalIOMode Mode)
{
	if (CalOP(Mode)) {
		m_DataOP_80.clear();
		m_pListOP_80->DeleteAllItems();
		while(m_pListOP_80->DeleteColumn(0));
		m_pListOP_80->InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,80);
		m_pListOP_80->InsertColumn(1,_T("Offset"),LVCFMT_RIGHT,70);
		m_pListOP_80->InsertColumn(2,_T("Temp4_TxLO2"),LVCFMT_CENTER,100);
		m_pListOP_80->InsertColumn(3,_T("Temp5_TxLO1"),LVCFMT_CENTER,100);
		m_pListOP_80->InsertColumn(4,_T("Temp6_TxPA4"),LVCFMT_CENTER,100);
		m_pListOP_80->InsertColumn(5,_T("Temp7_TxPA3"),LVCFMT_CENTER,100);
		m_pListOP_80->InsertColumn(6,_T("End Time"),LVCFMT_RIGHT,140);

		DWORD dwStyleList = ::GetWindowLong(m_pListOP_80->m_hWnd,GWL_STYLE);
		dwStyleList |= LVS_SINGLESEL;
		::SetWindowLong(m_pListOP_80->m_hWnd,GWL_STYLE,dwStyleList);

		ClearLineSerieOP();
		m_pChartOP_80->RemoveAllSeries();
	}
	
	if (CalIO(Mode)) {
		m_DataIO_80.clear();
		m_pListIO_80->DeleteAllItems();
		while(m_pListIO_80->DeleteColumn(0));
		m_pListIO_80->InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,80);
		m_pListIO_80->InsertColumn(1,_T("Offset"),LVCFMT_RIGHT,70);
		m_pListIO_80->InsertColumn(2,_T("Temp4_TxLO2"),LVCFMT_CENTER,100);
		m_pListIO_80->InsertColumn(3,_T("Temp5_TxLO1"),LVCFMT_CENTER,100);
		m_pListIO_80->InsertColumn(4,_T("Temp6_TxPA4"),LVCFMT_CENTER,100);
		m_pListIO_80->InsertColumn(5,_T("Temp7_TxPA3"),LVCFMT_CENTER,100);
		m_pListIO_80->InsertColumn(6,_T("End Time"),LVCFMT_RIGHT,140);

		DWORD dwStyleList = ::GetWindowLong(m_pListIO_80->m_hWnd,GWL_STYLE);
		dwStyleList |= LVS_SINGLESEL;
		::SetWindowLong(m_pListIO_80->m_hWnd,GWL_STYLE,dwStyleList);

		ClearLineSerieIO();
		m_pChartIO_80->RemoveAllSeries();
	}
}

void CCalR1CTxFilOffTabCtrl::ClearLineSerieOP()
{
	for (int32_t i = 0;i < m_OPLineSerie_80.size();i ++)
		m_OPLineSerie_80.at(i)->ClearSerie();
	m_OPLineSerie_80.clear();
}

void CCalR1CTxFilOffTabCtrl::ClearLineSerieIO()
{
	for (int32_t i = 0;i < m_IOLineSerie_80.size();i ++)
		m_IOLineSerie_80.at(i)->ClearSerie();
	m_IOLineSerie_80.clear();
}

void CCalR1CTxFilOffTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	m_pListOP_80->MoveWindow(8,30,(cx - 16) / 2 - 1,cy - 38 - CHART_HEIGHT);
	m_pListIO_80->MoveWindow((cx - 16) / 2 + 8 + 2,30,(cx - 16) / 2,cy - 38 - CHART_HEIGHT);
	m_pChartOP_80->MoveWindow(8,30 + cy - 38 - CHART_HEIGHT,(cx - 16) / 2 - 1,CHART_HEIGHT);
	m_pChartIO_80->MoveWindow((cx - 16) / 2 + 8 + 2,30 + cy - 38 - CHART_HEIGHT,(cx - 16) / 2,CHART_HEIGHT);
}

void CCalR1CTxFilOffTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iTabIdx = GetCurSel();
	if (0 == iTabIdx) {
		if (::IsWindow(m_pListOP_80->GetSafeHwnd()))
			m_pListOP_80->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pListIO_80->GetSafeHwnd()))
			m_pListIO_80->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pChartOP_80->GetSafeHwnd()))
			m_pChartOP_80->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pChartIO_80->GetSafeHwnd()))
			m_pChartIO_80->ShowWindow(SW_NORMAL);
	}
	*pResult = 0;
}