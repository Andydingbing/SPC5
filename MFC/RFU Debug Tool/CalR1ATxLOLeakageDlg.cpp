#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "CalR1ATxLOLeakageDlg.h"
#include "CalR1ATxLOLeakageThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCalR1ATxLOLeakageDlg, CCalR1ABaseDlg)
IMPLEMENT_CSE_CAL_R1A_DLG(CCalR1ATxLOLeakageDlg);

CCalR1ATxLOLeakageDlg::CCalR1ATxLOLeakageDlg(CWnd* pParent /*=NULL*/)
	: CCalR1ABaseDlg(CCalR1ATxLOLeakageDlg::IDD, _T("2G"), pParent)
	, m_strFreqStar(_T(""))
	, m_strFreqStop(_T(""))
	, m_strFreqStep(_T(""))
	, m_iDCOffsetI(0)
	, m_iDCOffsetQ(0)
	, m_bCalX9119(FALSE)
	, m_bUseSA(FALSE)
{
	m_Data.clear();
}

CCalR1ATxLOLeakageDlg::~CCalR1ATxLOLeakageDlg()
{
}

void CCalR1ATxLOLeakageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strFreqStep);
	DDX_Text(pDX, IDC_EDIT6, m_strFreq);
	DDX_Text(pDX, IDC_EDIT8, m_iDCOffsetI);
	DDX_Text(pDX, IDC_EDIT9, m_iDCOffsetQ);
	DDX_Check(pDX, IDC_CHECK1, m_bCalX9119);
	DDX_Control(pDX, IDC_LIST_TX_R1A_LOLEAKAGE, m_List);
	DDX_Radio(pDX, IDC_RADIO1, m_bUseSA);
}


BEGIN_MESSAGE_MAP(CCalR1ATxLOLeakageDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalR1ATxLOLeakageDlg::OnBnClickedStartCal)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalR1ATxLOLeakageDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalR1ATxLOLeakageDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalR1ATxLOLeakageDlg::OnBnClickedContinue)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CCalR1ATxLOLeakageDlg::Done)
	ON_MESSAGE(WM_CSE_CAL_TX_LOLEAKAGE_SHOW, &CCalR1ATxLOLeakageDlg::ShowData)
	ON_BN_CLICKED(IDC_BUTTON6, &CCalR1ATxLOLeakageDlg::OnBnClickedGetAndSet)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalR1ATxLOLeakageDlg::OnBnClickedExportCalFile)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TX_R1A_LOLEAKAGE, &CCalR1ATxLOLeakageDlg::OnLvnGetdispinfoListTxR1aLoleakage)
	ON_MESSAGE(WM_CSE_SP3301_CHANGE, &CCalR1ATxLOLeakageDlg::SP3301Change)
	ON_EN_CHANGE(IDC_EDIT6, &CCalR1ATxLOLeakageDlg::OnEnChangeFreqEdit)
	ON_BN_CLICKED(IDC_CHECK1, &CCalR1ATxLOLeakageDlg::OnBnClickedCalRef)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CCalR1ATxLOLeakageDlg::UpdateDataFalse)
END_MESSAGE_MAP()


void CCalR1ATxLOLeakageDlg::ResetShowCtrl()
{
	m_Data.clear();
	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	m_List.InsertColumn(0,_T("Freq(MHz)"),LVCFMT_CENTER,70);
	m_List.InsertColumn(1,_T("DC Offset I"),LVCFMT_CENTER,100);
	m_List.InsertColumn(2,_T("DC Offset Q"),LVCFMT_CENTER,100);
	m_List.InsertColumn(3,_T("LO Leakage(dBm)"),LVCFMT_CENTER,120);

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	TxLOLeakageTableR1A::DataF Data;
	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep);
	for (int32_t i = 0;i < iPts;i ++)
		m_Data.push_back(Data);
	m_List.SetItemCount(iPts);
	m_List.SetRedraw();
}

void CCalR1ATxLOLeakageDlg::OnBnClickedStartCal()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CCalR1ATxLOLeakageThread(this);
	m_pThread->CreateThread();
}

LRESULT CCalR1ATxLOLeakageDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CCalR1ATxLOLeakageThread::Result Data = *(CCalR1ATxLOLeakageThread::Result *)lParam;
	m_List.EnsureVisible(Data.m_iIdx,FALSE);
	m_List.SetItemState(Data.m_iIdx,LVIS_SELECTED,LVIS_SELECTED);
	return 0;
}

BOOL CCalR1ATxLOLeakageDlg::OnInitDialog()
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
	SetControlInfo(IDC_LIST_TX_R1A_LOLEAKAGE,FixL_xyStretch,1,0,1,1,0,1);

	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_EDIT6);
	AddThreadIdleCtrl(IDC_EDIT8);
	AddThreadIdleCtrl(IDC_EDIT9);
	AddThreadIdleCtrl(IDC_BUTTON6);
	AddThreadIdleCtrl(IDC_CHECK1);
	AddThreadIdleCtrl(IDC_BUTTON4);
	AddThreadIdleCtrl(IDC_RADIO1);
	AddThreadIdleCtrl(IDC_RADIO2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	AddThreadToPauseCtrl(IDC_BUTTON1);
	AddThreadToRunningCtrl(IDC_BUTTON5);
	CtrlResume();
	return TRUE;
}

void CCalR1ATxLOLeakageDlg::OnBnClickedGetAndSet()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	int16_t iDCI = 0,iDCQ = 0;

	str2freq(m_strFreq,uiFreq);
	((CalFileR1A *)(m_pSP1401->GetCalFile()))->m_pTxLOLeak->Get(uiFreq,iDCI,iDCQ);
	m_iDCOffsetI = (int)iDCI;
	m_iDCOffsetQ = (int)iDCQ;
	UpdateData(FALSE);

	m_pSP1401->SetTxFreq(uiFreq);
	m_pSP2401->SetTxDCOffset((uint16_t)iDCI,(uint16_t)iDCQ);
}

void CCalR1ATxLOLeakageDlg::OnBnClickedExportCalFile()
{
	CAL_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetShowCtrl();

	m_pThread = new CExportR1ATxLoLeakageThread(this);
	m_pThread->CreateThread();
}

void CCalR1ATxLOLeakageDlg::OnLvnGetdispinfoListTxR1aLoleakage(NMHDR *pNMHDR, LRESULT *pResult)
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
			strText.Format("%d",m_Data[iItemIdx].m_iDCI);
		else if (2 == pItem.iSubItem)
			strText.Format("%d",m_Data[iItemIdx].m_iDCQ);
		else if (3 == pItem.iSubItem)
			strText = _T("---");

		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

LRESULT CCalR1ATxLOLeakageDlg::Done(WPARAM wParam,LPARAM lParam)
{
	m_List.RedrawItems(0,m_List.GetItemCount());

	DWORD dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList &= ~(LVS_SINGLESEL);
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	return CCalR1ABaseDlg::Done(wParam,lParam);
}

LRESULT CCalR1ATxLOLeakageDlg::SP3301Change(WPARAM wParam,LPARAM lParam)
{
	CString strPath;
	strPath.Format("c:\\CSECal\\rfu%drf%d.cal",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	((CEdit *)(GetDlgItem(IDC_EDIT4)))->SetWindowText(strPath);
	return 0;
}

void CCalR1ATxLOLeakageDlg::OnBnClickedCalRef()
{
	UpdateData(TRUE);
	uint32_t uiRfIdx = m_pSP1401->GetRfIdx();
	if (m_bCalX9119) {
		for (uint32_t i = 0;i < MAX_RF;i ++) {
			if (uiRfIdx != i) {
				ALL_DLG->m_pCalR1ATxLOLeakageDlg[i].m_bCalX9119 = FALSE;
				::PostMessage(ALL_DLG->m_pCalR1ATxLOLeakageDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
			}
		}
	}
}
