#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestAlgo5355Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestAlgo5355Dlg, CAttachThreadDlg)
IMPLEMENT_CSE_ATTACHTHREAD_DLG(CTestAlgo5355Dlg)

CTestAlgo5355Dlg::CTestAlgo5355Dlg(CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(CTestAlgo5355Dlg::IDD, pParent)
	, m_strFreqStar(_T("3.4G"))
	, m_strFreqStop(_T("6.8G"))
	, m_strFreqStep(_T(""))
{
	ResetData();
}

CTestAlgo5355Dlg::~CTestAlgo5355Dlg()
{
	ResetData();
}

void CTestAlgo5355Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALGO_5355, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqStar);
	DDX_Text(pDX, IDC_EDIT11, m_strFreqStop);
	DDX_Text(pDX, IDC_EDIT12, m_strFreqStep);
}


BEGIN_MESSAGE_MAP(CTestAlgo5355Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestAlgo5355Dlg::OnBnClickedStart)
	ON_MESSAGE(WM_CSE_TEST_ALGORITHM_VCO5355_SHOW, &CTestAlgo5355Dlg::ShowData)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ALGO_5355, &CTestAlgo5355Dlg::OnLvnGetdispinfoList)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestAlgo5355Dlg::Done)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON175, &CTestAlgo5355Dlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON22, &CTestAlgo5355Dlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestAlgo5355Dlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_CHECK1, &CTestAlgo5355Dlg::OnBnClickedShowErrData)
END_MESSAGE_MAP()


void CTestAlgo5355Dlg::OnBnClickedStart()
{
	TEST_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	ResetData();
	ResetList();
	CtrlPrepare();
	
	m_pThread = new CTestAlgo5355Thread(this);
	m_pThread->CreateThread();
}

LRESULT CTestAlgo5355Dlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CTestAlgo5355Thread::Result Data = *(CTestAlgo5355Thread::Result *)lParam;
	m_List.EnsureVisible(Data.m_iFreqIdx,FALSE);
	m_List.SetItemState(Data.m_iFreqIdx,LVIS_SELECTED,LVIS_SELECTED);
	if (FALSE == Data.m_bResult) {
		CString strErrPts;
		((CStatic *)(GetDlgItem(IDC_STATIC_ERROR_POINTS)))->GetWindowText(strErrPts);
		strErrPts.Format("%d",_ttoi(strErrPts) + 1);
		((CStatic *)(GetDlgItem(IDC_STATIC_ERROR_POINTS)))->SetWindowText(strErrPts);
	}
	return 0;
}

void CTestAlgo5355Dlg::OnLvnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;
		BOOL bShowErr = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck());
		vector<CTestAlgo5355Thread::Result> *Data = bShowErr ? &m_ErrData : &m_Data;
		if (0 == pItem.iSubItem)
			strText.Format("%llu",Data->at(iItemIdx).m_uiFreq);
		else if (1 == pItem.iSubItem)
			strText.Format("%u",Data->at(iItemIdx).m_uiINT);
		else if (2 == pItem.iSubItem)
			strText.Format("%u",Data->at(iItemIdx).m_uiFRAC1);
		else if (3 == pItem.iSubItem)
			strText.Format("%u",Data->at(iItemIdx).m_uiFRAC2);
		else if (4 == pItem.iSubItem)
			strText.Format("%u",Data->at(iItemIdx).m_uiMOD1);
		else if (5 == pItem.iSubItem)
			strText.Format("%u",Data->at(iItemIdx).m_uiMOD2);
		else if (6 == pItem.iSubItem)
			strText.Format("%.15f",Data->at(iItemIdx).m_dDeviation);
		else if (7 == pItem.iSubItem)
			strText = Data->at(iItemIdx).m_bResult ? _T("¡Ì") : _T("¨w");
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CTestAlgo5355Dlg::ResetData()
{
	m_Data.clear();
	m_ErrData.clear();
}

void CTestAlgo5355Dlg::ResetList()
{
	m_List.DeleteAllItems();
	while (m_List.DeleteColumn(0));
	m_List.InsertColumn(0,_T("VCO Freq(Hz)"),LVCFMT_CENTER,90);
	m_List.InsertColumn(1,_T("_INT"),LVCFMT_CENTER,60);
	m_List.InsertColumn(2,_T("_FRAC1"),LVCFMT_CENTER,60);
	m_List.InsertColumn(3,_T("_FRAC2"),LVCFMT_CENTER,60);
	m_List.InsertColumn(4,_T("_MOD1"),LVCFMT_CENTER,60);
	m_List.InsertColumn(5,_T("_MOD2"),LVCFMT_CENTER,60);
	m_List.InsertColumn(6,_T("Deviation"),LVCFMT_CENTER,130);
	m_List.InsertColumn(7,_T("Result"),LVCFMT_CENTER,50);
	((CStatic *)(GetDlgItem(IDC_STATIC_ERROR_POINTS)))->SetWindowText(_T("0"));

	int32_t iPts = freq2array(m_strFreqStar,m_strFreqStop,m_strFreqStep);
	for (int32_t i = 0;i < iPts;i ++) {
		CTestAlgo5355Thread::Result Data;
		m_Data.push_back(Data);
	}
	BOOL bShowErr = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck());
	m_List.SetItemCount(bShowErr ? 0 : iPts);
}

BOOL CTestAlgo5355Dlg::OnInitDialog()
{
	CAttachThreadDlg::OnInitDialog();

	DWORD dwStyleList = m_List.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_List.SetExtendedStyle(dwStyleList);
	m_List.SetBkColor(PROTECT_EYE);
	m_List.SetTextBkColor(PROTECT_EYE);

	dwStyleList = ::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwStyleList);

	m_strFreqStep.Format("%d",ADF5355_FREQ_SPACE);
	((CButton *)(GetDlgItem(IDC_CHECK1)))->SetWindowText(_T(""));
	((CStatic *)(GetDlgItem(IDC_STATIC_ERROR_POINTS)))->SetWindowText(_T("0"));
	UpdateData(FALSE);

	this->GetClientRect(&m_crWnd);
	SetControlInfo(IDC_LIST_ALGO_5355,FixL_xyStretch,1,0,1,1,0,1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT11);
	AddThreadIdleCtrl(IDC_EDIT12);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON4);
	AddThreadToPauseCtrl(IDC_BUTTON175);
	AddThreadToRunningCtrl(IDC_BUTTON22);
	CtrlResume();
	return TRUE;
}

void CTestAlgo5355Dlg::OnBnClickedShowErrData()
{
	BOOL bShowErr = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK1)))->GetCheck());
	m_List.SetItemCount(bShowErr ? (int)(m_ErrData.size()) : (int)(m_Data.size()));
}
