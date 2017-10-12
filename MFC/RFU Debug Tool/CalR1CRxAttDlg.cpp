#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CRxAttDlg.h"
#include "CalR1CRxAttThread.h"

IMPLEMENT_DYNAMIC(CCalR1CRxAttDlg, CDialog)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CRxAttDlg)

CCalR1CRxAttDlg::CCalR1CRxAttDlg(CWnd* pParent /*=NULL*/)
	: CCalR1CBaseDlg(CCalR1CRxAttDlg::IDD, _T("2G"),pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_strFreq(_T(""))
{

}

CCalR1CRxAttDlg::~CCalR1CRxAttDlg()
{
}

void CCalR1CRxAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT5, m_strFreq);
	DDX_Control(pDX, IDC_COMBO1, m_CalIOMode);
	DDX_Control(pDX, IDC_COMBO2, m_IOMode);
	DDX_Control(pDX, IDC_LIST1, m_ListAttOP);
	DDX_Control(pDX, IDC_LIST2, m_ListAttIO);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CCalR1CRxAttDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CRxAttDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CRxAttDlg::OnBnClickedStop)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1CRxAttDlg::OnBnClickedGetAndSet)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CRxAttDlg::Done)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RX_ATT_R1C_OP, &CCalR1CRxAttDlg::OnLvnGetdispinfoList1RxCalData)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_RX_ATT_R1C_IO, &CCalR1CRxAttDlg::OnLvnGetdispinfoList2RxCalData)
	ON_MESSAGE(WM_CSE_CAL_RX_ATT_SHOW, &CCalR1CRxAttDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CRxAttDlg::OnBnClickedExportCalFile)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1CRxAttDlg::SP3301Change)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1CRxAttDlg::UpdateDataFalse)
	ON_BN_CLICKED(IDC_BUTTON9, &CCalR1CRxAttDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1CRxAttDlg::OnBnClickedContinue)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCalR1CRxAttDlg::OnLvnItemchangedListRxAttR1COP)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CCalR1CRxAttDlg::OnLvnItemchangedListRxAttR1CIO)
END_MESSAGE_MAP()


void CCalR1CRxAttDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());

	m_TabCtrl.ResetShowCtrl(Mode);

	if (CalOP(Mode)) {
		RxAttOPTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_TabCtrl.m_DataOP.push_back(Data);

		m_TabCtrl.m_pListOP->SetItemCount(iPts * ARRAY_SIZE(Data.m_iOffset));
		m_TabCtrl.m_pListOP->SetRedraw();

		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_iOffset);i ++) {
			CCalR1CRxAttTabCtrl::LineSerieVector OneFreqLine;
			COLORREF clrLine;
			for (int32_t j = 0;j < iSec;j ++) {
				CChartLineSerie *pLineSerie = m_TabCtrl.m_pChartOP->CreateLineSerie();
				pLineSerie->SetSeriesOrdering(poNoOrdering);
				if (0 == j)
					clrLine = pLineSerie->GetColor();
				else
					pLineSerie->SetColor(clrLine);

				OneFreqLine.push_back(pLineSerie);
			}
			m_TabCtrl.m_OPLineSerie.push_back(OneFreqLine);
		}
		m_TabCtrl.m_pChartOP->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);
		m_ListAttOP.EnableWindow(TRUE);
	}
	else {
		m_ListAttOP.EnableWindow(FALSE);
	}

	if (CalIO(Mode)) {
		RxAttIOTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_TabCtrl.m_DataIO.push_back(Data);

		m_TabCtrl.m_pListIO->SetItemCount(iPts * ARRAY_SIZE(Data.m_iOffset));
		m_TabCtrl.m_pListIO->SetRedraw();

		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_iOffset);i ++) {
			CCalR1CRxAttTabCtrl::LineSerieVector OneFreqLine;
			for (int32_t j = 0;j < iSec;j ++) {
				CChartLineSerie *pLineSerie = m_TabCtrl.m_pChartIO->CreateLineSerie();
				pLineSerie->SetSeriesOrdering(poNoOrdering);
				OneFreqLine.push_back(pLineSerie);
			}
			m_TabCtrl.m_IOLineSerie.push_back(OneFreqLine);
		}
		m_TabCtrl.m_pChartIO->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);
		m_ListAttIO.EnableWindow(TRUE);
	}
	else {
		m_ListAttIO.EnableWindow(FALSE);
	}
}

void CCalR1CRxAttDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CRxAttThread(this);
	m_pThread->CreateThread();
}

BOOL CCalR1CRxAttDlg::OnInitDialog()
{
	CStretchDlg::OnInitDialog();

	m_CalIOMode.SetCurSel(2);
	m_IOMode.SetCurSel(1);

	m_TabCtrl.InsertItem(0,_T("Sheet"));
	m_TabCtrl.InsertItem(1,_T("Chart"));
	m_TabCtrl.AttachChild();

	DWORD dwStyleList = m_ListAttOP.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList |= LVS_EX_CHECKBOXES;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_ListAttOP.SetExtendedStyle(dwStyleList);
	m_ListAttOP.InsertColumn(0,_T("Ref(Output)"),LVCFMT_RIGHT,100);
	m_ListAttOP.EnableWindow(FALSE);
	CString strPower = "";
	int32_t iIdxPower = 0;
	for (int32_t iPower = R1C_RX_REF_OP_STAR;iPower >= R1C_RX_REF_OP_STOP;iPower += R1C_RX_REF_STEP) {
		if (10 == iPower || -20 == iPower)
			continue;;
		strPower.Format("%d",iPower);
		m_ListAttOP.InsertItem(iIdxPower,strPower);
		m_ListAttOP.SetCheck(iIdxPower,TRUE);
		iIdxPower ++;
	}

	iIdxPower = 0;
	m_ListAttIO.SetExtendedStyle(dwStyleList);
	m_ListAttIO.InsertColumn(0,_T("Ref(IO)"),LVCFMT_RIGHT,100);
	m_ListAttIO.EnableWindow(FALSE);
	for (int32_t iPower = R1C_RX_REF_IO_STAR;iPower >= R1C_RX_REF_IO_STOP;iPower += R1C_RX_REF_STEP) {
		if (10 == iPower || -20 == iPower)
			continue;
		strPower.Format("%d",iPower);
		m_ListAttIO.InsertItem(iIdxPower,strPower);
		m_ListAttIO.SetCheck(iIdxPower,TRUE);
		iIdxPower ++;
	}

	m_strFreqStar.Format("%dM",RF_RX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_RX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_RX_FREQ_STEP_CALLED_R1C / 1000000);

	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_LIST1,ChgLT_xyStretch,1,0,0,2,0,1);
	SetControlInfo(IDC_LIST2,ChgLT_xyStretch,1,0,0,2,1,2);
	SetControlInfo(IDC_TAB1,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT5);
	AddThreadIdleCtrl(IDC_EDIT6);
	AddThreadIdleCtrl(IDC_COMBO1);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_COMBO2);
	AddThreadIdleCtrl(IDC_COMBO3);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON9);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

void CCalR1CRxAttDlg::OnBnClickedGetAndSet()
{

}

void CCalR1CRxAttDlg::OnLvnGetdispinfoList1RxCalData(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		RxAttOPTableR1C::DataF Data;
		int iRefPts = ARRAY_SIZE(Data.m_iOffset);
		int iFreqIdx = (int)(pItem.iItem / iRefPts);
		int iAttIdx = (int)(pItem.iItem % iRefPts);

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_DataOP[iFreqIdx].m_uiFreq / 1000000.0);
		else if (8 == pItem.iSubItem)
			strText.Format("%lld",m_TabCtrl.m_DataOP[iFreqIdx].m_iOffset[iAttIdx]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[0]);
		else if (10 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[1]);
		else if (11 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[2]);
		else if (12 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[3]);
		else if (13 == pItem.iSubItem)
			tm2str(m_TabCtrl.m_DataOP[iFreqIdx].m_EndTime,strText);
		else {
			int32_t iRef = 0;
			RxRefOPTableR1C::RxStateM DataRefOP;

			if (iAttIdx < 20)
				iRef = 30 - iAttIdx;
			else if (iAttIdx < 49)
				iRef = 29 - iAttIdx;
			else
				iRef = 28 - iAttIdx;

			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxRefOP->Get(m_TabCtrl.m_DataOP[iFreqIdx].m_uiFreq,(double)iRef,&DataRefOP);

			if (1 == pItem.iSubItem)
				strText.Format("%d",iRef);
			else if (2 == pItem.iSubItem)
				strText = DataRefOP.m_sLNAAtt ? _T("LNA") : _T("Att");
			else if (3 == pItem.iSubItem)
				strText = DataRefOP.m_sAtt019 ? _T("Att 0") : _T("Att 19");
			else if (4 == pItem.iSubItem)
				strText.Format("%.1f",DataRefOP.m_fAtt1);
			else if (5 == pItem.iSubItem)
				strText.Format("%.1f",DataRefOP.m_fAtt2);
			else if (6 == pItem.iSubItem)
				strText.Format("%.1f",DataRefOP.m_fAtt3);
			else if (7 == pItem.iSubItem)
				strText.Format("%d",DataRefOP.m_iADOffset);
		}
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1CRxAttDlg::OnLvnGetdispinfoList2RxCalData(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		RxAttIOTableR1C::DataF Data;
		int iRefPts = ARRAY_SIZE(Data.m_iOffset);
		int iFreqIdx = (int)(pItem.iItem / iRefPts);
		int iAttIdx = (int)(pItem.iItem % iRefPts);

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_DataIO[iFreqIdx].m_uiFreq / 1000000.0);
		else if (8 == pItem.iSubItem)
			strText.Format("%lld",m_TabCtrl.m_DataIO[iFreqIdx].m_iOffset[iAttIdx]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[0]);
		else if (10 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[1]);
		else if (11 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[2]);
		else if (12 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[3]);
		else if (13 == pItem.iSubItem) {
			tm2str(m_TabCtrl.m_DataIO[iFreqIdx].m_EndTime,strText);
		}
		else {
			int32_t iRef = 0;
			RxRefOPTableR1C::RxStateM DataRefIO;

			if (iAttIdx < 20)
				iRef = 30 - iAttIdx;
			else if (iAttIdx < 49)
				iRef = 29 - iAttIdx;
			else
				iRef = 28 - iAttIdx;

			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pRxRefIO->Get(m_TabCtrl.m_DataIO[iFreqIdx].m_uiFreq,(double)iRef,&DataRefIO);

			if (1 == pItem.iSubItem)
				strText.Format("%d",iRef);
			else if (2 == pItem.iSubItem)
				strText = DataRefIO.m_sLNAAtt ? _T("LNA") : _T("Att");
			else if (3 == pItem.iSubItem)
				strText = DataRefIO.m_sAtt019 ? _T("Att 0") : _T("Att 19");
			else if (4 == pItem.iSubItem)
				strText.Format("%.1f",DataRefIO.m_fAtt1);
			else if (5 == pItem.iSubItem)
				strText.Format("%.1f",DataRefIO.m_fAtt2);
			else if (6 == pItem.iSubItem)
				strText.Format("%.1f",DataRefIO.m_fAtt3);
			else if (7 == pItem.iSubItem)
				strText.Format("%d",DataRefIO.m_iADOffset);
		}
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1CRxAttDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CalIOMode Mode = *(CalIOMode *)wParam;
	if (CAL_OP == Mode) {
		CCalR1CRxAttThread::ResultOP Data = *(CCalR1CRxAttThread::ResultOP *)lParam;
		int iLastItemOP = (Data.m_iIdx) * ARRAY_SIZE(Data.m_iOffset);
		m_TabCtrl.m_pListOP->EnsureVisible(iLastItemOP,FALSE);
		m_TabCtrl.m_pListOP->SetItemState(iLastItemOP,LVIS_SELECTED,LVIS_SELECTED);

		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_iOffset);i ++) {
			double dOffset = ad2dBc(_0dBFS,_0dBFS - Data.m_iOffset[i]);
			m_TabCtrl.m_OPLineSerie.at(i).at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,dOffset);
		}
	}
	if (CAL_IO == Mode) {
		CCalR1CRxAttThread::ResultIO Data = *(CCalR1CRxAttThread::ResultIO *)lParam;
		int iLastItemIO = (Data.m_iIdx) * ARRAY_SIZE(Data.m_iOffset);
		m_TabCtrl.m_pListIO->EnsureVisible(iLastItemIO,FALSE);
		m_TabCtrl.m_pListIO->SetItemState(iLastItemIO,LVIS_SELECTED,LVIS_SELECTED);

		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_iOffset);i ++) {
			double dOffset = ad2dBc(_0dBFS,_0dBFS - Data.m_iOffset[i]);
			m_TabCtrl.m_IOLineSerie.at(i).at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,dOffset);
		}
	}
	return 0;
}

void CCalR1CRxAttDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	m_CalIOMode.SetCurSel(2);
	ResetShowCtrl();

	m_pThread = new CExportR1CRxAttThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CRxAttDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}

LRESULT CCalR1CRxAttDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_TabCtrl.m_pListOP->RedrawItems(0,m_TabCtrl.m_pListOP->GetItemCount());
	m_TabCtrl.m_pListIO->RedrawItems(0,m_TabCtrl.m_pListIO->GetItemCount());

	DWORD dwStyleList = ::GetWindowLong(m_TabCtrl.m_pListOP->m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_TabCtrl.m_pListOP->m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_TabCtrl.m_pListIO->m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}

void CCalR1CRxAttDlg::OnLvnItemchangedListRxAttR1COP(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iItem = pNMLV->iItem;

	if (iItem != -1 && m_TabCtrl.m_OPLineSerie.size() > 0) {
		BOOL bCheck = m_ListAttOP.GetCheck(iItem);
		for (int32_t i = 0;i < m_TabCtrl.m_OPLineSerie.at(iItem).size();i ++)
			m_TabCtrl.m_OPLineSerie.at(iItem).at(i)->SetVisible(bCheck);
	}
	*pResult = 0;
}

void CCalR1CRxAttDlg::OnLvnItemchangedListRxAttR1CIO(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int iItem = pNMLV->iItem;

	if (iItem != -1 && m_TabCtrl.m_IOLineSerie.size() > 0) {
		BOOL bCheck = m_ListAttIO.GetCheck(iItem);
		for (int32_t i = 0;i < m_TabCtrl.m_IOLineSerie.at(iItem).size();i ++)
			m_TabCtrl.m_IOLineSerie.at(iItem).at(i)->SetVisible(bCheck);
	}
	*pResult = 0;
}