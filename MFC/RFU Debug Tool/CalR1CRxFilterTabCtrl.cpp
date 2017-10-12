#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxFilterTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CRxFilterTabCtrl, CCSETabCtrl)

CCalR1CRxFilterTabCtrl::CCalR1CRxFilterTabCtrl()
{
	m_pLineSerieRFFr = NULL;
	m_pLineSerieIFFr = NULL;
	m_pChartRFFr = NULL;
	m_pChartIFFr = NULL;
	m_pList_80  = NULL;
	m_pList_160 = NULL;
}

CCalR1CRxFilterTabCtrl::~CCalR1CRxFilterTabCtrl()
{
	if (m_pLineSerieRFFr)
		m_pLineSerieRFFr->ClearSerie();
	if (m_pChartRFFr) {
		m_pChartRFFr->RemoveAllSeries();
		delete m_pChartRFFr;
		m_pChartRFFr = NULL;
	}

	if (m_pLineSerieIFFr)
		m_pLineSerieIFFr->ClearSerie();
	if (m_pChartIFFr) {
		m_pChartIFFr->RemoveAllSeries();
		delete m_pChartIFFr;
		m_pChartIFFr = NULL;
	}

	SAFE_DESTROY_DLG(m_pList_80);
	SAFE_DESTROY_DLG(m_pList_160);
}


BEGIN_MESSAGE_MAP(CCalR1CRxFilterTabCtrl, CCSETabCtrl)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CCalR1CRxFilterTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()


void CCalR1CRxFilterTabCtrl::AttachChild()
{
	CRect crTabCtrl,crRFFr,crIFFr;
	this->GetClientRect(&crTabCtrl);
	crTabCtrl.top    += 30;
	crTabCtrl.bottom -= 8;
	crTabCtrl.left   += 8;
	crTabCtrl.right  -= 8;

	crRFFr.left = crTabCtrl.left;
	crRFFr.top = crTabCtrl.top;
	crRFFr.right = crTabCtrl.right;
	crRFFr.bottom = crTabCtrl.top + crTabCtrl.Height() / 2 - 1;

	crIFFr.left = crTabCtrl.left;
	crIFFr.top = crRFFr.bottom + 2;
	crIFFr.right = crTabCtrl.right;
	crIFFr.bottom = crTabCtrl.bottom;

	m_pChartRFFr = new CChartCtrl;
	m_pChartRFFr->Create(this,crRFFr,IDC_CAL_R1C_RX_FILTER_RF_CHART);
	m_pChartRFFr->ShowWindow(SW_NORMAL);
	m_pChartRFFr->SetZoomRectColor(BLACK);
	m_pChartRFFr->GetTitle()->AddString(_T("RF Freq Response Curve(X:Freq(MHz) Y:Power(dB))"));
	m_pChartRFFr->GetTitle()->SetColor(BLUE);

	CChartAxis *pAxis;
	pAxis = m_pChartRFFr->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(RF_RX_FILTER_FREQ_STAR / 1000000.0,RF_RX_FILTER_FREQ_STOP / 1000000.0);
	pAxis = m_pChartRFFr->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_pChartIFFr = new CChartCtrl;
	m_pChartIFFr->Create(this,crIFFr,IDC_CAL_R1C_RX_FILTER_IF_CHART);
	m_pChartIFFr->ShowWindow(SW_NORMAL);
	m_pChartIFFr->SetZoomRectColor(BLACK);
	m_pChartIFFr->GetTitle()->AddString(_T("IF Freq Response(@RF 2GHz) Curve(X:Freq(MHz) Y:Power(dB))"));
	m_pChartIFFr->GetTitle()->SetColor(BLUE);

	pAxis = m_pChartIFFr->CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(false);
	pAxis->SetMinMax(IF_RX_FILTER_160M_FREQ_STAR / 1000000.0,IF_RX_FILTER_160M_FREQ_STOP / 1000000.0);
	pAxis = m_pChartIFFr->CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	m_pList_80 = new CProtectEyeListCtrl;
	m_pList_80->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_OWNERDATA | LVS_ALIGNLEFT | LVS_REPORT,crTabCtrl,this,IDC_LIST_RX_FILTER_R1C_80);

	DWORD dwStyleList = 0;
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_pList_80->SetExtendedStyle(dwStyleList);
	m_pList_80->SetBkColor(PROTECT_EYE);
	m_pList_80->SetTextBkColor(PROTECT_EYE);
	m_pList_80->ShowWindow(SW_HIDE);

	m_pList_160 = new CProtectEyeListCtrl;
	m_pList_160->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_OWNERDATA | LVS_ALIGNLEFT | LVS_REPORT,crTabCtrl,this,IDC_LIST_RX_FILTER_R1C_160);
	m_pList_160->SetExtendedStyle(dwStyleList);
	m_pList_160->SetBkColor(PROTECT_EYE);
	m_pList_160->SetTextBkColor(PROTECT_EYE);
	m_pList_160->ShowWindow(SW_HIDE);
}

void CCalR1CRxFilterTabCtrl::ResetShowCtrl()
{
	m_Data_80.clear();
	m_Data_160.clear();
	RxFilter_80M_TableR1C::DataF Data_80M;
	RxFilter_160M_TableR1C::DataF Data_160M;
	int32_t iFreqPts = (int32_t)(RF_RX_FREQ_PTS_CALLED_R1C);
	for (int32_t i = 0;i < iFreqPts;i ++) {
		m_Data_80.push_back(Data_80M);
		m_Data_160.push_back(Data_160M);
	}

	m_pList_80->DeleteAllItems();
	while(m_pList_80->DeleteColumn(0));
	m_pList_80->InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,80);
	m_pList_80->InsertColumn(1,_T("Real"),LVCFMT_LEFT,1000);
	m_pList_80->InsertColumn(2,_T("Imag"),LVCFMT_LEFT,1000);
	m_pList_80->InsertColumn(3,_T("Temp0_RxLO2"),LVCFMT_CENTER,100);
	m_pList_80->InsertColumn(4,_T("Temp1_RxLNA"),LVCFMT_CENTER,100);
	m_pList_80->InsertColumn(5,_T("Temp2_RxLO1"),LVCFMT_CENTER,100);
	m_pList_80->InsertColumn(6,_T("Temp3_RxPA1"),LVCFMT_CENTER,100);
	m_pList_80->InsertColumn(7,_T("End Time"),LVCFMT_RIGHT,140);
	m_pList_80->SetItemCount(iFreqPts);

	m_pList_160->DeleteAllItems();
	while(m_pList_160->DeleteColumn(0));
	m_pList_160->InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,80);
	m_pList_160->InsertColumn(1,_T("Real"),LVCFMT_LEFT,1000);
	m_pList_160->InsertColumn(2,_T("Imag"),LVCFMT_LEFT,1000);
	m_pList_160->InsertColumn(3,_T("Temp0_RxLO2"),LVCFMT_CENTER,100);
	m_pList_160->InsertColumn(4,_T("Temp1_RxLNA"),LVCFMT_CENTER,100);
	m_pList_160->InsertColumn(5,_T("Temp2_RxLO1"),LVCFMT_CENTER,100);
	m_pList_160->InsertColumn(6,_T("Temp3_RxPA1"),LVCFMT_CENTER,100);
	m_pList_160->InsertColumn(7,_T("End Time"),LVCFMT_RIGHT,140);
	m_pList_160->SetItemCount(iFreqPts);

	DWORD dwStyleList = ::GetWindowLong(m_pList_80->m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_pList_80->m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_pList_160->m_hWnd,GWL_STYLE,dwStyleList);

	if (m_pLineSerieRFFr)
		m_pLineSerieRFFr->ClearSerie();
	else {
		m_pLineSerieRFFr = m_pChartRFFr->CreateLineSerie();
		m_pLineSerieRFFr->SetSeriesOrdering(poNoOrdering);
	}

	if (m_pLineSerieIFFr)
		m_pLineSerieIFFr->ClearSerie();
	else {
		m_pLineSerieIFFr = m_pChartIFFr->CreateLineSerie();
		m_pLineSerieIFFr->SetSeriesOrdering(poNoOrdering);
	}
}

void CCalR1CRxFilterTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	m_pChartRFFr->MoveWindow(8,30,cx - 16,(cy - 38) / 2 - 1);
	m_pChartIFFr->MoveWindow(8,30 + (cy - 38) / 2 + 1,cx - 16,(cy - 38) / 2 - 1);
	m_pList_80->MoveWindow(8,30,cx - 16,cy - 38);
	m_pList_160->MoveWindow(8,30,cx - 16,cy - 38);
}

void CCalR1CRxFilterTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iTabIdx = GetCurSel();
	if (0 == iTabIdx) {
		if (::IsWindow(m_pChartRFFr->GetSafeHwnd()))
			m_pChartRFFr->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pChartIFFr->GetSafeHwnd()))
			m_pChartIFFr->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pList_80->GetSafeHwnd()))
			m_pList_80->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pList_160->GetSafeHwnd()))
			m_pList_160->ShowWindow(SW_HIDE);
	}
	else if (1 == iTabIdx) {
		if (::IsWindow(m_pChartRFFr->GetSafeHwnd()))
			m_pChartRFFr->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pChartIFFr->GetSafeHwnd()))
			m_pChartIFFr->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pList_80->GetSafeHwnd()))
			m_pList_80->ShowWindow(SW_NORMAL);
		if (::IsWindow(m_pList_160->GetSafeHwnd()))
			m_pList_160->ShowWindow(SW_HIDE);
	}
	else if (2 == iTabIdx) {
		if (::IsWindow(m_pChartRFFr->GetSafeHwnd()))
			m_pChartRFFr->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pChartIFFr->GetSafeHwnd()))
			m_pChartIFFr->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pList_80->GetSafeHwnd()))
			m_pList_80->ShowWindow(SW_HIDE);
		if (::IsWindow(m_pList_160->GetSafeHwnd()))
			m_pList_160->ShowWindow(SW_NORMAL);
	}
	*pResult = 0;
}