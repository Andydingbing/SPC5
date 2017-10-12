#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxFilOffDlg.h"
#include "CalR1CTxFilOffThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CTxFilOffDlg, CCalR1CBaseDlg)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CTxFilOffDlg)

CCalR1CTxFilOffDlg::CCalR1CTxFilOffDlg(CWnd* pParent /*=NULL*/)
	: CCalR1CBaseDlg(CCalR1CTxFilOffDlg::IDD, _T("2G"), pParent)
{

}

CCalR1CTxFilOffDlg::~CCalR1CTxFilOffDlg()
{
}

void CCalR1CTxFilOffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_CalIOMode);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
}


BEGIN_MESSAGE_MAP(CCalR1CTxFilOffDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CTxFilOffDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CTxFilOffDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON174, &CCalR1CTxFilOffDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON15, &CCalR1CTxFilOffDlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CTxFilOffDlg::OnBnClickedExportCalFile)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_FILOFF_R1C_OP, &CCalR1CTxFilOffDlg::OnLvnGetdispinfoListTxR1cFilOffOp)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_FILOFF_R1C_IO, &CCalR1CTxFilOffDlg::OnLvnGetdispinfoListTxR1cFilOffIo)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CTxFilOffDlg::Done)
	ON_MESSAGE(WM_CSE_CAL_TX_FILTER_OFF_80_SHOW, &CCalR1CTxFilOffDlg::ShowData)
END_MESSAGE_MAP()


BOOL CCalR1CTxFilOffDlg::OnInitDialog()
{
	CCalR1CBaseDlg::OnInitDialog();

	m_CalIOMode.SetCurSel(2);

	m_TabCtrl.InsertItem(0,_T("80M"));
	m_TabCtrl.AttachChild();

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
	SetControlInfo(IDC_TAB1,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_COMBO1);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON174);
	AddThreadToRunningCtrl(IDC_BUTTON15);
	CtrlResume();
	return TRUE;
}

void CCalR1CTxFilOffDlg::ResetShowCtrl()
{
	int64_t iFreqStar = 0,iFreqStop = 0;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep,NULL,NULL,NULL,&iFreqStop,&iFreqStar);
	int32_t iSec = strsections((LPSTR)(LPCTSTR)m_strFreqStar);
	CalIOMode Mode = (CalIOMode)(m_CalIOMode.GetCurSel());

	m_TabCtrl.ResetShowCtrl(Mode);

	if (CalOP(Mode)) {
		TxFilterOffTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_TabCtrl.m_DataOP_80.push_back(Data);

		m_TabCtrl.m_pListOP_80->SetItemCount(iPts);
		m_TabCtrl.m_pListOP_80->SetRedraw();

		for (int32_t i = 0;i < iSec;i ++) {
			CChartLineSerie *pLineSerie = m_TabCtrl.m_pChartOP_80->CreateLineSerie();
			pLineSerie->SetSeriesOrdering(poNoOrdering);
			m_TabCtrl.m_OPLineSerie_80.push_back(pLineSerie);
		}
		m_TabCtrl.m_pChartOP_80->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);
	}

	if (CalIO(Mode)) {
		TxFilterOffTableR1C::DataF Data;
		for (int32_t i = 0;i < iPts;i ++)
			m_TabCtrl.m_DataIO_80.push_back(Data);

		m_TabCtrl.m_pListIO_80->SetItemCount(iPts);
		m_TabCtrl.m_pListIO_80->SetRedraw();

		for (int32_t i = 0;i < iSec;i ++) {
			CChartLineSerie *pLineSerie = m_TabCtrl.m_pChartIO_80->CreateLineSerie();
			pLineSerie->SetSeriesOrdering(poNoOrdering);
			m_TabCtrl.m_IOLineSerie_80.push_back(pLineSerie);
		}
		m_TabCtrl.m_pChartIO_80->GetBottomAxis()->SetMinMax(iFreqStar / 1000000.0,iFreqStop / 1000000.0);
	}
}

void CCalR1CTxFilOffDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CTxFilOffThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CTxFilOffDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CalIOMode Mode = *(CalIOMode *)wParam;
	if (CAL_OP == Mode) {
		CCalR1CTxFilOffThread::Result Data = *(CCalR1CTxFilOffThread::Result *)lParam;
		m_TabCtrl.m_pListOP_80->EnsureVisible(Data.m_iIdx,FALSE);
		m_TabCtrl.m_pListOP_80->SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
		m_TabCtrl.m_OPLineSerie_80.at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,(double)(Data.m_dOffset));
	}
	if (CAL_IO == Mode) {
		CCalR1CTxFilOffThread::Result Data = *(CCalR1CTxFilOffThread::Result *)lParam;
		m_TabCtrl.m_pListIO_80->EnsureVisible(Data.m_iIdx,FALSE);
		m_TabCtrl.m_pListIO_80->SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
		m_TabCtrl.m_IOLineSerie_80.at(Data.m_iSec)->AddPoint(Data.m_uiFreq / 1000000.0,(double)(Data.m_dOffset));
	}
	return 0;
}

void CCalR1CTxFilOffDlg::OnBnClickedGetAndSet()
{

}

LRESULT CCalR1CTxFilOffDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_TabCtrl.m_pListOP_80->RedrawItems(0,(int)m_TabCtrl.m_DataOP_80.size());
	m_TabCtrl.m_pListIO_80->RedrawItems(0,(int)m_TabCtrl.m_DataIO_80.size());

	DWORD dwStyleList = ::GetWindowLong(m_TabCtrl.m_pListOP_80->m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_TabCtrl.m_pListOP_80->m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_TabCtrl.m_pListIO_80->m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}

void CCalR1CTxFilOffDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	m_CalIOMode.SetCurSel(2);
	ResetShowCtrl();

	m_pThread = new CExportR1CTxFilOffThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CTxFilOffDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}

void CCalR1CTxFilOffDlg::OnLvnGetdispinfoListTxR1cFilOffOp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_DataOP_80[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP_80[iItemIdx].m_dOffset);
		else if (2 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP_80[iItemIdx].m_dTemp[0]);
		else if (3 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP_80[iItemIdx].m_dTemp[1]);
		else if (4 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP_80[iItemIdx].m_dTemp[2]);
		else if (5 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataOP_80[iItemIdx].m_dTemp[3]);
		else if (6 == pItem.iSubItem)
			tm2str(m_TabCtrl.m_DataOP_80[iItemIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CCalR1CTxFilOffDlg::OnLvnGetdispinfoListTxR1cFilOffIo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_TabCtrl.m_DataIO_80[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO_80[iItemIdx].m_dOffset);
		else if (2 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO_80[iItemIdx].m_dTemp[0]);
		else if (3 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO_80[iItemIdx].m_dTemp[1]);
		else if (4 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO_80[iItemIdx].m_dTemp[2]);
		else if (5 == pItem.iSubItem)
			strText.Format("%.3f",m_TabCtrl.m_DataIO_80[iItemIdx].m_dTemp[3]);
		else if (6 == pItem.iSubItem)
			tm2str(m_TabCtrl.m_DataIO_80[iItemIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}