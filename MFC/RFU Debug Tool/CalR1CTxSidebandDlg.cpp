#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1CTxSideBandDlg.h"
#include "CalR1CTxSideBandThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1CTxSidebandDlg, CCalR1CBaseDlg)
IMPLEMENT_CSE_CAL_R1C_DLG(CCalR1CTxSidebandDlg)

CCalR1CTxSidebandDlg::CCalR1CTxSidebandDlg(CWnd* pParent /*=NULL*/)
	: CCalR1CBaseDlg(CCalR1CTxSidebandDlg::IDD, _T("2G"), pParent)
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

CCalR1CTxSidebandDlg::~CCalR1CTxSidebandDlg()
{
}

void CCalR1CTxSidebandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT7, m_dTh);
	DDX_Text(pDX, IDC_EDIT8, m_uAmI);
	DDX_Text(pDX, IDC_EDIT9, m_uAmQ);
	DDX_Control(pDX, IDC_LIST_TX_R1C_SIDEBAND, m_List);
	DDX_Radio(pDX, IDC_RADIO1, m_bUseSA);
}


BEGIN_MESSAGE_MAP(CCalR1CTxSidebandDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1CTxSidebandDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1CTxSidebandDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1CTxSidebandDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1CTxSidebandDlg::OnBnClickedContinue)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1CTxSidebandDlg::Done)
	ON_MESSAGE(WM_CSE_CAL_TX_SIDEBAND_SHOW_CALLING, &CCalR1CTxSidebandDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1CTxSidebandDlg::OnBnClickedGetAndSet)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1CTxSidebandDlg::OnBnClickedExportCalFile)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_R1C_SIDEBAND, &CCalR1CTxSidebandDlg::OnLvnGetdispinfoListTxR1aSideband)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1CTxSidebandDlg::SP3301Change)
	ON_EN_CHANGE(IDC_EDIT6, &CCalR1CTxSidebandDlg::OnEnChangeFreqEdit)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1CTxSidebandDlg::UpdateDataFalse)
END_MESSAGE_MAP()


void CCalR1CTxSidebandDlg::ResetShowCtrl()
{
	m_Data.clear();
	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	m_List.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
	m_List.InsertColumn(1,_T("Angle(бу)"),LVCFMT_CENTER,100);
	m_List.InsertColumn(2,_T("Am Offset I"),LVCFMT_CENTER,100);
	m_List.InsertColumn(3,_T("Am Offset Q"),LVCFMT_CENTER,100);
	m_List.InsertColumn(4,_T("Method"),LVCFMT_RIGHT,80);
	m_List.InsertColumn(5,_T("Sideband(dBm)"),LVCFMT_CENTER,140);
	m_List.InsertColumn(6,_T("Temp4_TxLO2"),LVCFMT_CENTER,90);
	m_List.InsertColumn(7,_T("Temp5_TxLO1"),LVCFMT_CENTER,90);
	m_List.InsertColumn(8,_T("Temp6_TxPA4"),LVCFMT_CENTER,90);
	m_List.InsertColumn(9,_T("Temp7_TxPA3"),LVCFMT_CENTER,90);
	m_List.InsertColumn(10,_T("End Time"),LVCFMT_RIGHT,120);

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	TxSidebandTableR1C::DataF Data;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep);
	for (int32_t i = 0;i < iPts;i ++)
		m_Data.push_back(Data);
	m_List.SetItemCount(iPts);
	m_List.SetRedraw();
}

void CCalR1CTxSidebandDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1CTxSidebandThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1CTxSidebandDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

BOOL CCalR1CTxSidebandDlg::OnInitDialog()
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
	SetControlInfo(IDC_LIST_TX_R1C_SIDEBAND,FixL_xyStretch,1,0,1,1,0,1);
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

void CCalR1CTxSidebandDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	TxSidebandTableR1C::DataM Data;

	str2freq(m_strFreq,uiFreq);

	((CalFileR1C *)(m_pSP1401->GetCalFile()))->m_pTxSideband->Get(uiFreq,&Data);
	m_dTh = (double)(Data.m_fTh);
	m_uAmI = (UINT)(Data.m_uiAmI);
	m_uAmQ = (UINT)(Data.m_uiAmQ);
	UpdateData(FALSE);

	m_pSP1401->SetTxFreq(uiFreq);
	m_pSP2401->SetTxPhaseRotateI((double)(Data.m_fTh));
	m_pSP2401->SetTxAmplitudeBalance(Data.m_uiAmI,Data.m_uiAmQ);
}

void CCalR1CTxSidebandDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1CTxSidebandThread(this);
	m_pThread->CreateThread();
}

void CCalR1CTxSidebandDlg::OnLvnGetdispinfoListTxR1aSideband(NMHDR *pNMHDR, LRESULT *pResult)
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
			strText = m_Data[iItemIdx].m_bUseSA ? "Spectrum" : "Loopback";
		else if (5 == pItem.iSubItem)
			strText.Format("%.3f",m_Data[iItemIdx].m_dPower);
		else if (6 == pItem.iSubItem)
			strText.Format("%.3f",m_Data[iItemIdx].m_dTemp[0]);
		else if (7 == pItem.iSubItem)
			strText.Format("%.3f",m_Data[iItemIdx].m_dTemp[1]);
		else if (8 == pItem.iSubItem)
			strText.Format("%.3f",m_Data[iItemIdx].m_dTemp[2]);
		else if (9 == pItem.iSubItem)
			strText.Format("%.3f",m_Data[iItemIdx].m_dTemp[3]);
		else if (10 == pItem.iSubItem)
			tm2str(m_Data[iItemIdx].m_EndTime,strText);

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1CTxSidebandDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_List.RedrawItems(0,m_List.GetItemCount());

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1CBaseDlg::Done(wParam,lParam);
}

LRESULT CCalR1CTxSidebandDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}