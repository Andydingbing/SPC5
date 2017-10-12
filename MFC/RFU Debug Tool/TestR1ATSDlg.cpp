#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1ATSDlg.h"
#include "TestR1ATSThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1ATSDlg, CAttachThreadDlg)
IMPLEMENT_CSE_TEST_R1A_DLG(CTestR1ATSDlg)

CTestR1ATSDlg::TxResult::TxResult()
{
	m_Power.clear();
	m_Temp.clear();
}

CTestR1ATSDlg::RxResult::RxResult()
{
	m_Power.clear();
	m_Temp.clear();
}

CTestR1ATSDlg::CTestR1ATSDlg(CWnd* pParent /*=NULL*/)
	: CTestR1ABaseDlg(CTestR1ATSDlg::IDD, pParent)
	, m_strTxFreqStar(_T(""))
	, m_strTxFreqStop(_T(""))
	, m_strTxFreqStep(_T("200M"))
	, m_strRxFreqStar(_T(""))
	, m_strRxFreqStop(_T(""))
	, m_strRxFreqStep(_T("200M"))
	, m_bTestTx(TRUE)
	, m_bTestRx(TRUE)
	, m_dTxPower(0)
	, m_dRxPower(0)
	, m_dSpaceingInterval(5)
	, m_bRxPM(FALSE)
{
	m_TxData.clear();
	m_RxData.clear();
}

CTestR1ATSDlg::~CTestR1ATSDlg()
{
}

void CTestR1ATSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTxFreqStar);
	DDX_Text(pDX, IDC_EDIT2, m_strTxFreqStop);
	DDX_Text(pDX, IDC_EDIT3, m_strTxFreqStep);
	DDX_Text(pDX, IDC_EDIT11, m_strRxFreqStar);
	DDX_Text(pDX, IDC_EDIT4, m_strRxFreqStop);
	DDX_Text(pDX, IDC_EDIT14, m_strRxFreqStep);
	DDX_Check(pDX, IDC_CHECK1, m_bTestTx);
	DDX_Check(pDX, IDC_CHECK2, m_bTestRx);
	DDX_Text(pDX, IDC_EDIT7, m_dTxPower);
	DDX_Text(pDX, IDC_EDIT8, m_dRxPower);
	DDX_Text(pDX, IDC_EDIT5, m_dSpaceingInterval);
	DDX_Text(pDX, IDC_EDIT69, m_strTxPath);
	DDX_Text(pDX, IDC_EDIT89, m_strRxPath);
	DDX_Control(pDX, IDC_LIST_TEST_TX_TS, m_ListTx);
	DDX_Control(pDX, IDC_LIST_TEST_RX_TS, m_ListRx);
	DDX_Check(pDX, IDC_CHECK3, m_bRxPM);
}


BEGIN_MESSAGE_MAP(CTestR1ATSDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1ATSDlg::OnBnClickedStartTest)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON81, &CTestR1ATSDlg::OnBnClickedSetTxTestFile)
	ON_BN_CLICKED(IDC_BUTTON82, &CTestR1ATSDlg::OnBnClickedSetRxTestFile)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestR1ATSDlg::Done)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestR1ATSDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_TEST_TS_SHOW, &CTestR1ATSDlg::ShowData)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TEST_TX_TS, &CTestR1ATSDlg::OnLvnGetdispinfoListTestTxTempStability)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TEST_RX_TS, &CTestR1ATSDlg::OnLvnGetdispinfoListTestRxTempStability)
END_MESSAGE_MAP()


void CTestR1ATSDlg::ResetShowCtrl()
{
	if (m_bTestTx) {
		TxResult Data;
		int32_t iTxPts = 0;
		CString strTxFreq = "";
		uint64_t uiTxFreq[SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE)];
		memset(uiTxFreq,0,sizeof(uiTxFreq));
		iTxPts = freq2array(m_strTxFreqStar,m_strTxFreqStop,m_strTxFreqStep,(int64_t *)uiTxFreq);

		m_ListTx.DeleteAllItems();
		while (m_ListTx.DeleteColumn(0));
		m_ListTx.InsertColumn(0,_T("NO."),LVCFMT_CENTER,60);

		m_TxData.clear();
		for (int32_t i = 0;i < iTxPts;i ++) {
			strTxFreq.Format("%dM Power",uiTxFreq[i] / 1000000);
			m_ListTx.InsertColumn(i * 2 + 1,strTxFreq,LVCFMT_LEFT,80);
			m_ListTx.InsertColumn(i * 2 + 2,_T("Temp"),LVCFMT_LEFT,50);
			m_TxData.push_back(Data);
		}
	}
	if (m_bTestRx) {
		RxResult Data;
		int32_t iRxPts = 0;
		CString strRxFreq = "";
		uint64_t uiRxFreq[SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A)];
		memset(uiRxFreq,0,ARRAY_SIZE(uiRxFreq));
		iRxPts = freq2array(m_strRxFreqStar,m_strRxFreqStop,m_strRxFreqStep,(int64_t *)uiRxFreq);

		m_ListRx.DeleteAllItems();
		while (m_ListRx.DeleteColumn(0));
		m_ListRx.InsertColumn(0,_T("NO."),LVCFMT_CENTER,60);

		m_RxData.clear();
		for (int32_t i = 0;i < iRxPts;i ++) {
			strRxFreq.Format("%dM Power",uiRxFreq[i] / 1000000);
			m_ListRx.InsertColumn(i * 2 + 1,strRxFreq,LVCFMT_CENTER,80);
			m_ListRx.InsertColumn(i * 2 + 2,_T("Temp"),LVCFMT_CENTER,50);
			m_RxData.push_back(Data);
		}
	}
}

void CTestR1ATSDlg::OnBnClickedStartTest()
{
	TEST_THREAD_EXISTED_CHKV();
	UpdateData(TRUE);
	CtrlPrepare();
	ResetShowCtrl();

	m_pThread = new CTestR1ATSThread(this);
	m_pThread->CreateThread();
}

BOOL CTestR1ATSDlg::OnInitDialog()
{
	CAttachThreadDlg::OnInitDialog();

	m_strTxFreqStar.Format("%dM",RF_TX_FREQ_STAR / 1000000);
	m_strTxFreqStop.Format("%dM",RF_TX_FREQ_STOP / 1000000);
	m_strRxFreqStar.Format("%dM",RF_RX_FREQ_STAR / 1000000);
	m_strRxFreqStop.Format("%dM",RF_RX_FREQ_STOP / 1000000);

	m_strTxPath.Format("C:\\CSETest\\rfu%drf%d_tx.txt",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	m_strRxPath.Format("C:\\CSETest\\rfu%drf%d_rx.txt",m_pSP1401->GetRfuIdx(),m_pSP1401->GetRfIdx());
	UpdateData(FALSE);

	DWORD dwStyleList = m_ListTx.GetExtendedStyle();
	dwStyleList |= LVS_EX_GRIDLINES;
	dwStyleList |= LVS_EX_FULLROWSELECT;
	dwStyleList &= ~LVS_EX_AUTOSIZECOLUMNS;
	m_ListTx.SetExtendedStyle(dwStyleList);
	m_ListTx.SetBkColor(PROTECT_EYE);
	m_ListTx.SetTextBkColor(PROTECT_EYE);
	m_ListRx.SetExtendedStyle(dwStyleList);
	m_ListRx.SetBkColor(PROTECT_EYE);
	m_ListRx.SetTextBkColor(PROTECT_EYE);

	dwStyleList = ::GetWindowLong(m_ListTx.m_hWnd,GWL_STYLE);
	dwStyleList |= LVS_SINGLESEL;
	::SetWindowLong(m_ListTx.m_hWnd,GWL_STYLE,dwStyleList);
	::SetWindowLong(m_ListRx.m_hWnd,GWL_STYLE,dwStyleList);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_EDIT69,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_BUTTON81,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_CHECK2,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT11,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT4,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT14,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT8,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT89,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_BUTTON82,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_CHECK3,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON2,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_LIST_TEST_TX_TS,FixL_xyStretch,2,0,1,1,0,1);
	SetControlInfo(IDC_LIST_TEST_RX_TS,ChgL_xyStretch,2,1,2,1,0,1);
	AddThreadIdleCtrl(IDC_CHECK1);
	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_EDIT2);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_EDIT7);
	AddThreadIdleCtrl(IDC_EDIT5);
	AddThreadIdleCtrl(IDC_CHECK3);
	AddThreadIdleCtrl(IDC_EDIT69);
	AddThreadIdleCtrl(IDC_BUTTON81);
	AddThreadIdleCtrl(IDC_CHECK2);
	AddThreadIdleCtrl(IDC_EDIT11);
	AddThreadIdleCtrl(IDC_EDIT4);
	AddThreadIdleCtrl(IDC_EDIT14);
	AddThreadIdleCtrl(IDC_EDIT8);
	AddThreadIdleCtrl(IDC_EDIT89);
	AddThreadIdleCtrl(IDC_BUTTON82);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadRunningCtrl(IDC_BUTTON3);
	CtrlResume();
	return TRUE;
}

void CTestR1ATSDlg::OnBnClickedSetTxTestFile()
{
	UpdateData(TRUE);
	CFileDialog TestFile(TRUE);
	if (TestFile.DoModal() == IDOK) {
		m_strTxPath = TestFile.GetPathName();
		UpdateData(FALSE);
	}
}

void CTestR1ATSDlg::OnBnClickedSetRxTestFile()
{
	UpdateData(TRUE);
	CFileDialog TestFile(TRUE);
	if (TestFile.DoModal() == IDOK) {
		m_strRxPath = TestFile.GetPathName();
		UpdateData(FALSE);
	}
}

LRESULT CTestR1ATSDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	ISP1401::RFCh Ch = *(ISP1401::RFCh *)wParam;
	if (ISP1401::Tx == Ch)
		m_ListTx.SetItemCount((int)(m_TxData.at(0).m_Power.size() - 1));
	if (ISP1401::Rx == Ch)
		m_ListRx.SetItemCount((int)(m_RxData.at(0).m_Power.size() - 1));
	return 0;
}

void CTestR1ATSDlg::OnLvnGetdispinfoListTestTxTempStability(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem) {
			strText.Format("%d",iItemIdx);
		}
		else {
			if(0 == (pItem.iSubItem - 1) % 2)
				strText.Format("%7.3f",m_TxData.at((pItem.iSubItem - 1) / 2).m_Power.at(iItemIdx));
			else
				strText.Format("%4.2f",m_TxData.at((pItem.iSubItem - 1) / 2).m_Temp.at(iItemIdx));
		}
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}

void CTestR1ATSDlg::OnLvnGetdispinfoListTestRxTempStability(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	LVITEMA pItem= pDispInfo->item;
	int iItemIdx = pItem.iItem;

	if (pItem.mask & LVIF_TEXT)
	{
		CString strText;

		if (0 == pItem.iSubItem) {
			strText.Format("%d",iItemIdx);
		}
		else {
			if(0 == (pItem.iSubItem - 1) % 2)
				strText.Format("%7.3f",m_RxData.at((pItem.iSubItem - 1) / 2).m_Power.at(iItemIdx));
			else
				strText.Format("%4.2f",m_RxData.at((pItem.iSubItem - 1) / 2).m_Temp.at(iItemIdx));
		}
		lstrcpyn(pItem.pszText, strText, pItem.cchTextMax);
	}
	*pResult = 0;
}