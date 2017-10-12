#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxAttDlg.h"
#include "CalR1CTxAttThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CTxAttDlg, CDialog)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CTxAttDlg)

CCalR1CTxAttDlg::CCalR1CTxAttDlg(CWnd* pParent /*=NULL*/)
	: CCalR1CBaseDlg(CCalR1CTxAttDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_strFreq(_T(""))
{

}

CCalR1CTxAttDlg::~CCalR1CTxAttDlg()
{
}

void CCalR1CTxAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Control(pDX, IDC_COMBO1, m_CalIOMode);
	DDX_Text(pDX, IDC_EDIT5, m_strFreq);
	DDX_Control(pDX, IDC_COMBO2, m_IOMode);
	DDX_Control(pDX, IDC_LIST1, m_ListAttOP);
	DDX_Control(pDX, IDC_LIST2, m_ListAttIO);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CCalR1CTxAttDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CTxAttDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CTxAttDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON174, &CCalR1CTxAttDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON15, &CCalR1CTxAttDlg::OnBnClickedContinue)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CTxAttDlg::Done)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_ATT_R1C_OP, &CCalR1CTxAttDlg::OnLvnGetdispinfoListTxR1cAttOp)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_ATT_R1C_IO, &CCalR1CTxAttDlg::OnLvnGetdispinfoListTxR1cAttIo)
	ON_MESSAGE(WM_CSE_CAL_TX_ATT_SHOW, &CCalR1CTxAttDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CTxAttDlg::OnBnClickedExportCalFile)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCalR1CTxAttDlg::OnLvnItemchangedListTxAttR1COP)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CCalR1CTxAttDlg::OnLvnItemchangedListTxAttR1CIO)
END_MESSAGE_MAP()


void CCalR1CTxAttDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());

	m_TabCtrl.ResetShowCtrl(Mode);

	if (CalOP(Mode)) {
		TxAttOPTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_TabCtrl.m_DataOP.push_back(Data);

		m_TabCtrl.m_pListOP->SetItemCount(iPts * ARRAY_SIZE(Data.m_fOffset));
		m_TabCtrl.m_pListOP->SetRedraw();

		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_fOffset);i ++) {
			CCalR1CTxAttTabCtrl::LineSerieVector OneFreqLine;
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
		TxAttIOTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_TabCtrl.m_DataIO.push_back(Data);

		m_TabCtrl.m_pListIO->SetItemCount(iPts * ARRAY_SIZE(Data.m_fOffset));
		m_TabCtrl.m_pListIO->SetRedraw();

		for (int32_t i = 0;i < ARRAY_SIZE(Data.m_fOffset);i ++) {
			CCalR1CTxAttTabCtrl::LineSerieVector OneFreqLine;
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

void CCalR1CTxAttDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CTxAttThread(this);
	m_pThread->CreateThread();
}

void CCalR1CTxAttDlg::OnBnClickedGetAndSet()
{

}

LRESULT CCalR1CTxAttDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CalIOMode Mode = *(CalIOMode *)wParam;
	if (CAL_OP == Mode) {
		CCalR1CTxAttThread::ResultOP Data = *(CCalR1CTxAttThread::ResultOP *)lParam;
		int iLastItemOP = (Data.m_iIdx) * ARRAY_SIZE(Data.m_fOffset) + Data.m_iIdxOffsetStop;
		m_TabCtrl.m_pListOP->EnsureVisible(iLastItemOP,FALSE);
		m_TabCtrl.m_pListOP->SetItemState(iLastItemOP,LVIS_SELECTED,LVIS_SELECTED);

		for (int32_t i = Data.m_iIdxOffsetStar;i < Data.m_iIdxOffsetStop;i ++)
			m_TabCtrl.m_OPLineSerie.at(i).at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,(double)(Data.m_fOffset[i]));
	}
	if (CAL_IO == Mode) {
		CCalR1CTxAttThread::ResultIO Data = *(CCalR1CTxAttThread::ResultIO *)lParam;
		int iLastItemIO = (Data.m_iIdx) * ARRAY_SIZE(Data.m_fOffset) + Data.m_iIdxOffsetStop;
		m_TabCtrl.m_pListIO->EnsureVisible(iLastItemIO,FALSE);
		m_TabCtrl.m_pListIO->SetItemState(iLastItemIO,LVIS_SELECTED,LVIS_SELECTED);

		for (int32_t i = Data.m_iIdxOffsetStar;i < Data.m_iIdxOffsetStop;i ++) {
			m_TabCtrl.m_IOLineSerie.at(i).at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,(double)(Data.m_fOffset[i]));
		}
	}
	return 0;
}

void CCalR1CTxAttDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	m_CalIOMode.SetCurSel(2);
	ResetShowCtrl();

	m_pThread = new CExportR1CTxAttThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CTxAttDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

BOOL CCalR1CTxAttDlg::OnInitDialog()
{
	CCalR1CBaseDlg::OnInitDialog();

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
	m_ListAttOP.InsertColumn(0,_T("Power(Output)"),LVCFMT_RIGHT,100);
	m_ListAttOP.EnableWindow(FALSE);
	CString strPower = "";
	int32_t iIdxPower = 0;
	for (int32_t iPower = R1C_TX_ATT_OP_POWER_STAR;iPower >= R1C_TX_ATT_OP_POWER_STOP;iPower += R1C_TX_ATT_STEP) {
		if (R1C_TX_BASE_POWER_OP == iPower)
			continue;;
		strPower.Format("%d",iPower);
		m_ListAttOP.InsertItem(iIdxPower,strPower);
		m_ListAttOP.SetCheck(iIdxPower,TRUE);
		iIdxPower ++;
	}

	iIdxPower = 0;
	m_ListAttIO.SetExtendedStyle(dwStyleList);
	m_ListAttIO.InsertColumn(0,_T("Power(IO)"),LVCFMT_RIGHT,100);
	m_ListAttIO.EnableWindow(FALSE);
	for (int32_t iPower = R1C_TX_ATT_IO_POWER_STAR;iPower >= R1C_TX_ATT_IO_POWER_STOP;iPower += R1C_TX_ATT_STEP) {
		if (R1C_TX_BASE_POWER_OP == iPower)
			continue;
		strPower.Format("%d",iPower);
		m_ListAttIO.InsertItem(iIdxPower,strPower);
		m_ListAttIO.SetCheck(iIdxPower,TRUE);
		iIdxPower ++;
	}

	m_strFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_TX_FREQ_STEP_CALLED / 1000000);

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
	AddThreadIdleCtrl(IDC_COMBO1);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_COMBO2);
	AddThreadIdleCtrl(IDC_COMBO3);
	AddThreadIdleCtrl(IDC_BUTTON1);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON174);
	AddThreadToRunningCtrl(IDC_BUTTON15);
	CtrlResume();
	return TRUE;
}

void CCalR1CTxAttDlg::OnLvnGetdispinfoListTxR1cAttOp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		TxAttOPTableR1C::DataF Data;
		int iOffsetPts = ARRAY_SIZE(Data.m_fOffset);
		int iFreqIdx = (int)(iItemIdx / iOffsetPts);
		int iAttIdx = (int)(iItemIdx % iOffsetPts);

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_DataOP[iFreqIdx].m_uiFreq / 1000000.0);
		else if (7 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_fOffset[iAttIdx]);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[0]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[1]);
		else if (10 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[2]);
		else if (11 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP[iFreqIdx].m_dTemp[3]);
		else if (12 == pItem.iSubItem)
			tm2str(m_TabCtrl.m_DataOP[iFreqIdx].m_EndTime,strText);
		else {
			int32_t iPower = iAttIdx < (R1C_TX_ATT_OP_POWER_STAR - R1C_TX_BASE_POWER_OP) ? (R1C_TX_ATT_OP_POWER_STAR - iAttIdx) : (R1C_TX_ATT_OP_POWER_STAR - iAttIdx - 1);

			TxPowerOPTableR1C::DataM DataPower;
			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxPowerOP->Get(m_TabCtrl.m_DataOP[iFreqIdx].m_uiFreq,(double)iPower,&DataPower);

			if (1 == pItem.iSubItem)
				strText.Format("%d",iPower);
			else if (2 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt0 / 2.0);
			else if (3 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt1 / 2.0);
			else if (4 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt2 / 2.0);
			else if (5 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt3 / 2.0);
			else if (6 == pItem.iSubItem)
				strText.Format("%.3f",DataPower.m_fDGain);
		}
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1CTxAttDlg::OnLvnGetdispinfoListTxR1cAttIo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		TxAttIOTableR1C::DataF Data;
		int iOffsetPts = ARRAY_SIZE(Data.m_fOffset);
		int iFreqIdx = (int)(iItemIdx / iOffsetPts);
		int iAttIdx = (int)(iItemIdx % iOffsetPts);

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_DataIO[iFreqIdx].m_uiFreq / 1000000.0);
		else if (7 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_fOffset[iAttIdx]);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[0]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[1]);
		else if (10 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[2]);
		else if (11 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO[iFreqIdx].m_dTemp[3]);
		else if (12 == pItem.iSubItem)
			tm2str(m_TabCtrl.m_DataIO[iFreqIdx].m_EndTime,strText);
		else {
			int32_t iPower = iAttIdx < (R1C_TX_ATT_IO_POWER_STAR - R1C_TX_BASE_POWER_IO) ? (R1C_TX_ATT_IO_POWER_STAR - iAttIdx) : (R1C_TX_ATT_IO_POWER_STAR - iAttIdx - 1);

			TxPowerIOTableR1C::DataM DataPower;
			((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxPowerIO->Get(m_TabCtrl.m_DataIO[iFreqIdx].m_uiFreq,(double)iPower,&DataPower);

			if (1 == pItem.iSubItem)
				strText.Format("%d",iPower);
			else if (2 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt0 / 2.0);
			else if (3 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt1 / 2.0);
			else if (4 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt2 / 2.0);
			else if (5 == pItem.iSubItem)
				strText.Format("%.1f",DataPower.m_sAtt3 / 2.0);
			else if (6 == pItem.iSubItem)
				strText.Format("%.3f",DataPower.m_fDGain);
		}
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1CTxAttDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_TabCtrl.m_pListOP->RedrawItems(0,m_TabCtrl.m_pListOP->GetItemCount());
	m_TabCtrl.m_pListIO->RedrawItems(0,m_TabCtrl.m_pListIO->GetItemCount());

	DWORD dwStyleList = ::GetWindowLong(m_TabCtrl.m_pListOP->m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_TabCtrl.m_pListOP->m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_TabCtrl.m_pListIO->m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}

void CCalR1CTxAttDlg::OnLvnItemchangedListTxAttR1COP(NMHDR *pNMHDR, LRESULT *pResult)
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

void CCalR1CTxAttDlg::OnLvnItemchangedListTxAttR1CIO(NMHDR *pNMHDR, LRESULT *pResult)
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