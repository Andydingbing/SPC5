#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxSideBandDlg.h"
#include "CalR1ATxSideBandThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1ATxSideBandDlg, CCalR1ABaseDlg)
IMPLEMENT_CSE_CAL_R1A_DLG(CCalR1ATxSideBandDlg)

CCalR1ATxSideBandDlg::CCalR1ATxSideBandDlg(CWnd* pParent /*=NULL*/)
	: CCalR1ABaseDlg(CCalR1ATxSideBandDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_dTh(0.0)
	, m_uAmI(0)
	, m_uAmQ(0)
	, m_bUseSA(FALSE)
{
	m_Data.clear();
}

CCalR1ATxSideBandDlg::~CCalR1ATxSideBandDlg()
{
}

void CCalR1ATxSideBandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT7, m_dTh);
	DDX_Text(pDX, IDC_EDIT8, m_uAmI);
	DDX_Text(pDX, IDC_EDIT9, m_uAmQ);
	DDX_Control(pDX, IDC_LIST_TX_R1A_SIDEBAND, m_List);
	DDX_Radio(pDX, IDC_RADIO1, m_bUseSA);
}


BEGIN_MESSAGE_MAP(CCalR1ATxSideBandDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1ATxSideBandDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1ATxSideBandDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1ATxSideBandDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1ATxSideBandDlg::OnBnClickedContinue)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1ATxSideBandDlg::Done)
	ON_MESSAGE(WM_CSE_CAL_TX_SIDEBAND_SHOW_CALLING, &CCalR1ATxSideBandDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1ATxSideBandDlg::OnBnClickedGetAndSet)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1ATxSideBandDlg::OnBnClickedExportCalFile)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_R1A_SIDEBAND, &CCalR1ATxSideBandDlg::OnLvnGetdispinfoListTxR1aSideband)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1ATxSideBandDlg::SP3301Change)
	ON_EN_CHANGE(IDC_EDIT6, &CCalR1ATxSideBandDlg::OnEnChangeFreqEdit)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1ATxSideBandDlg::UpdateDataFalse)
END_MESSAGE_MAP()


void CCalR1ATxSideBandDlg::ResetShowCtrl()
{
	m_Data.clear();
	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	m_List.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
	m_List.InsertColumn(1,_T("Angle(бу)"),LVCFMT_CENTER,100);
	m_List.InsertColumn(2,_T("Am Offset I"),LVCFMT_CENTER,100);
	m_List.InsertColumn(3,_T("Am Offset Q"),LVCFMT_CENTER,100);
	m_List.InsertColumn(4,_T("Sideband(dBm)"),LVCFMT_CENTER,140);

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	TxSidebandTableR1A::DataF Data;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep);
	for (int32_t i = 0;i < iPts;i ++)
		m_Data.push_back(Data);
	m_List.SetItemCount(iPts);
	m_List.SetRedraw();
}

void CCalR1ATxSideBandDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1ATxSideBandThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1ATxSideBandDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CCalR1ATxSideBandThread::Result Data = *(CCalR1ATxSideBandThread::Result *)lParam;
	m_List.EnsureVisible(Data.m_iIdx,FALSE);
	m_List.SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
	return 0;
}

BOOL CCalR1ATxSideBandDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strFreqStep.Format("%dM",RF_TX_FREQ_STEP_CALLED / 1000000);

	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	UpdateData(FALSE);

	DWORD dwStyleList = m_List.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_List.SetExtendedStyle(dwStyleList);
	m_List.SetBkColor(PROTECT_EYE);
	m_List.SetTextBkColor(PROTECT_EYE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,1,0,1);
	SetControlInfo(IDC_EDIT4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_BUTTON4,ChgL_Fixx,1,1,1);
	SetControlInfo(IDC_LIST_TX_R1A_SIDEBAND,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT6);
	AddThreadIdleCtrl(IDC_EDIT7);
	AddThreadIdleCtrl(IDC_EDIT8);
	AddThreadIdleCtrl(IDC_EDIT9);
	AddThreadIdleCtrl(IDC_BUTTON6);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_RADIO1);
	AddThreadIdleCtrl(IDC_RADIO2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON1);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

void CCalR1ATxSideBandDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	double dTh = 0.0;
	uint16_t uiAmI = 0,uiAmQ = 0;

	str2freq(m_strFreq,uiFreq);

	((CalFileR1A *)(m_pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq,dTh,uiAmI,uiAmQ);
	m_dTh = dTh;
	m_uAmI = (UINT)uiAmI;
	m_uAmQ = (UINT)uiAmQ;
	UpdateData(FALSE);

	m_pSP1401->SetTxFreq(uiFreq);
	m_pSP2401->SetTxPhaseRotateI(dTh);
	m_pSP2401->SetTxAmplitudeBalance(uiAmI,uiAmQ);
}

void CCalR1ATxSideBandDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1ATxSideBandThread(this);
	m_pThread->CreateThread();
}

void CCalR1ATxSideBandDlg::OnLvnGetdispinfoListTxR1aSideband(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem)
			strText.Format("%.0f",m_Data[iItemIdx].m_uiFreq / 1000000.0);
		else if (1 == pItem.iSubItem)
			strText.Format("%.2f",m_Data[iItemIdx].m_dTh);
		else if (2 == pItem.iSubItem)
			strText.Format("%d",m_Data[iItemIdx].m_uiAmI);
		else if (3 == pItem.iSubItem)
			strText.Format("%d",m_Data[iItemIdx].m_uiAmQ);
		else if (4 == pItem.iSubItem)
			strText = _T("---");

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1ATxSideBandDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_List.RedrawItems(0,m_List.GetItemCount());

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1ABaseDlg::Done(wParam,lParam);
}

LRESULT CCalR1ATxSideBandDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}