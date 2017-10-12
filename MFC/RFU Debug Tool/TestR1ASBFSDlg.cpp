#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "TestR1ASBFSDlg.h"
#include "TestR1ASBFSThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTestR1ASBFSDlg, CTestR1ABaseDlg)
IMPLEMENT_CSE_TEST_R1A_DLG(CTestR1ASBFSDlg)

CTestR1ASBFSDlg::CTestR1ASBFSDlg(CWnd* pParent /*=NULL*/)
	: CTestR1ABaseDlg(CTestR1ASBFSDlg::IDD, pParent)
	, m_iRepeatTimes(10)
	, m_bDynDownK7_0(TRUE)
	, m_bDynDownK7_1(TRUE)
	, m_strSN(_T(""))
	, m_strTxLO1FreqStar(_T("3100M"))
	, m_strTxLO1FreqStop(_T("6000M"))
	, m_strTxLO1FreqStep(_T(""))
	, m_strTxLO2FreqStar(_T("4100M"))
	, m_strTxLO2FreqStop(_T("7200M"))
	, m_strTxLO2FreqStep(_T(""))
	, m_strRxLO1FreqStar(_T("7500M"))
	, m_strRxLO1FreqStop(_T("13.5G"))
	, m_strRxLO1FreqStep(_T(""))
	, m_strRxLO2FreqStar(_T("7224M"))
	, m_strRxLO2FreqStop(_T("7224M"))
	, m_strRxLO2FreqStep(_T(""))
	, m_bTestTxLO1(TRUE)
	, m_bTestTxLO2(TRUE)
	, m_bTestRxLO1(TRUE)
	, m_bTestRxLO2(TRUE)
	, m_iCurrentCount(0)
	, m_uiTxLO1Cur(0)
	, m_uiTxLO2Cur(0)
	, m_uiRxLO1Cur(0)
	, m_uiRxLO2Cur(0)
{

}

CTestR1ASBFSDlg::~CTestR1ASBFSDlg()
{
}

void CTestR1ASBFSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, m_BitFile_K7_0);
	DDX_Control(pDX, IDC_EDIT8, m_BitFile_K7_1);
	DDX_Text(pDX, IDC_EDIT1, m_iRepeatTimes);
	DDX_Check(pDX, IDC_CHECK_BIT_K7_0, m_bDynDownK7_0);
	DDX_Check(pDX, IDC_CHECK_BIT_K7_1, m_bDynDownK7_1);
	DDX_Text(pDX, IDC_EDIT2, m_strSN);
	DDX_Text(pDX, IDC_EDIT3, m_strTxLO1FreqStar);
	DDX_Text(pDX, IDC_EDIT4, m_strTxLO1FreqStop);
	DDX_Text(pDX, IDC_EDIT26, m_strTxLO1FreqStep);
	DDX_Text(pDX, IDC_EDIT14, m_strTxLO2FreqStar);
	DDX_Text(pDX, IDC_EDIT69, m_strTxLO2FreqStop);
	DDX_Text(pDX, IDC_EDIT74, m_strTxLO2FreqStep);
	DDX_Text(pDX, IDC_EDIT70, m_strRxLO1FreqStar);
	DDX_Text(pDX, IDC_EDIT79, m_strRxLO1FreqStop);
	DDX_Text(pDX, IDC_EDIT84, m_strRxLO1FreqStep);
	DDX_Text(pDX, IDC_EDIT85, m_strRxLO2FreqStar);
	DDX_Text(pDX, IDC_EDIT86, m_strRxLO2FreqStop);
	DDX_Text(pDX, IDC_EDIT87, m_strRxLO2FreqStep);
	DDX_Check(pDX, IDC_CHECK1, m_bTestTxLO1);
	DDX_Check(pDX, IDC_CHECK2, m_bTestTxLO2);
	DDX_Check(pDX, IDC_CHECK7, m_bTestRxLO1);
	DDX_Check(pDX, IDC_CHECK33, m_bTestRxLO2);
	DDX_Control(pDX, IDC_STATIC_TS_TXLO1, m_TxLO1Lock);
	DDX_Control(pDX, IDC_STATIC_TS_TXLO2, m_TxLO2Lock);
	DDX_Control(pDX, IDC_STATIC_TS_RXLO1, m_RxLO1Lock);
	DDX_Control(pDX, IDC_STATIC_TS_RXLO2, m_RxLO2Lock);
	DDX_Text(pDX, IDC_EDIT18, m_iCurrentCount);
	DDX_Text(pDX, IDC_EDIT19, m_uiTxLO1Cur);
	DDX_Text(pDX, IDC_EDIT20, m_uiTxLO2Cur);
	DDX_Text(pDX, IDC_EDIT25, m_uiRxLO1Cur);
	DDX_Text(pDX, IDC_EDIT28, m_uiRxLO2Cur);
}


BEGIN_MESSAGE_MAP(CTestR1ASBFSDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestR1ASBFSDlg::OnBnClickedSelBitK7_0)
	ON_BN_CLICKED(IDC_BUTTON13, &CTestR1ASBFSDlg::OnBnClickedSelBitK7_1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestR1ASBFSDlg::OnBnClickedStartTestDynDown)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestR1ASBFSDlg::OnBnClickedStartTestSN)
	ON_BN_CLICKED(IDC_BUTTON62, &CTestR1ASBFSDlg::OnBnClickedStartTestLO)
	ON_MESSAGE(WM_CSE_TEST_SB_STABILITY_SN_SHOW, &CTestR1ASBFSDlg::ShowData)
	ON_MESSAGE(WM_CSE_TEST_SB_STABILITY_LO_SHOW, &CTestR1ASBFSDlg::ShowData)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CTestR1ASBFSDlg::Done)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON7, &CTestR1ASBFSDlg::OnBnClickedStop)
	ON_MESSAGE(WM_CSE_UPDATE_DATA_FALSE, &CTestR1ASBFSDlg::UpdateDataFalse)
	ON_BN_CLICKED(IDC_BUTTON18, &CTestR1ASBFSDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestR1ASBFSDlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_CHECK_BIT_K7_0, &CTestR1ASBFSDlg::OnBnClickedCheckBitK70)
	ON_BN_CLICKED(IDC_CHECK_BIT_K7_1, &CTestR1ASBFSDlg::OnBnClickedCheckBitK71)
END_MESSAGE_MAP()


void CTestR1ASBFSDlg::OnBnClickedSelBitK7_0()
{
	CFileDialog BitDlg(TRUE,_T("bit"),_T("*.bit"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("bit(*.bit)|*.bit|All Files (*.*)|*.*||"));
	if (BitDlg.DoModal() == IDOK) {
		for (int32_t i = 0;i < MAX_RF;i ++) {
			if (::IsWindow(ALL_DLG->m_pTestR1ASBFSDlg[i].GetSafeHwnd()))
				ALL_DLG->m_pTestR1ASBFSDlg[i].m_BitFile_K7_0.SetWindowText(BitDlg.GetPathName());
		}
	}
}

void CTestR1ASBFSDlg::OnBnClickedSelBitK7_1()
{
	CFileDialog BitDlg(TRUE,_T("bit"),_T("*.bit"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("bit(*.bit)|*.bit|All Files (*.*)|*.*||"));
	if (BitDlg.DoModal() == IDOK) {
		for (int32_t i = 0;i < MAX_RF;i ++)
			if (::IsWindow(ALL_DLG->m_pTestR1ASBFSDlg[i].GetSafeHwnd()))
				ALL_DLG->m_pTestR1ASBFSDlg[i].m_BitFile_K7_1.SetWindowText(BitDlg.GetPathName());
	}
}

void CTestR1ASBFSDlg::OnBnClickedStartTestDynDown()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing FPGA Program Bit Stability"));
	CtrlPrepare();
	UpdateData(TRUE);

	m_pThread = new CTestR1ASBFSThread(this,0);
	m_pThread->CreateThread();
}

void CTestR1ASBFSDlg::OnBnClickedStartTestSN()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing SN Read Stability"));
	CtrlPrepare();
	UpdateData(TRUE);

	m_pThread = new CTestR1ASBFSThread(this,1);
	m_pThread->CreateThread();
}

void CTestR1ASBFSDlg::OnBnClickedStartTestLO()
{
	TEST_THREAD_EXISTED_CHKV();
	STACHKV(ThreadCheckBox("Prepare Testing LO Lock Stability"));
	CtrlPrepare();
	UpdateData(TRUE);

	m_pThread = new CTestR1ASBFSThread(this,2);
	m_pThread->CreateThread();
}

LRESULT CTestR1ASBFSDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	UINT uiMsg = GetCurrentMessage()->message;
	if (WM_CSE_TEST_SB_STABILITY_SN_SHOW == uiMsg) {
		m_strSN.Format("%s",(char *)lParam);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_TEST_SB_STABILITY_LO_SHOW == uiMsg) {
		CBitmap bitMap;
		CTestR1ASBFSThread::IsLOLock Lock = *(CTestR1ASBFSThread::IsLOLock *)lParam;

		if (m_bTestTxLO1) {
			bitMap.LoadBitmap(Lock.m_bTxLO1 ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
			m_TxLO1Lock.SetBitmap((HBITMAP)(bitMap.Detach()));
		}
		if (m_bTestTxLO2) {
			bitMap.LoadBitmap(Lock.m_bTxLO2 ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
			m_TxLO2Lock.SetBitmap((HBITMAP)(bitMap.Detach()));
		}
		if (m_bTestRxLO1) {
			bitMap.LoadBitmap(Lock.m_bRxLO1 ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
			m_RxLO1Lock.SetBitmap((HBITMAP)(bitMap.Detach()));
		}
		if (m_bTestRxLO2) {
			bitMap.LoadBitmap(Lock.m_bRxLO2 ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
			m_RxLO2Lock.SetBitmap((HBITMAP)(bitMap.Detach()));
		}
		return 0;
	}
	return 0;
}

BOOL CTestR1ASBFSDlg::OnInitDialog()
{
	CStretchDlg::OnInitDialog();

	m_strTxLO1FreqStep.Format("%d",ADF5355_FREQ_SPACE);
	m_strTxLO2FreqStep.Format("%d",ADF5355_FREQ_SPACE);
	m_strRxLO1FreqStep.Format("%d",ADF5355_FREQ_SPACE);
	m_strRxLO2FreqStep.Format("%d",ADF5355_FREQ_SPACE);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_BUTTON18,FixL_xStretch,3,0,1);
	SetControlInfo(IDC_BUTTON8,ChgL_xStretch,3,1,2);
	SetControlInfo(IDC_BUTTON7,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_CHECK1,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_CHECK2,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_CHECK7,ChgLT_Fixx,3,1,1,8,4,4);
	SetControlInfo(IDC_CHECK33,ChgLT_Fixx,3,2,2,8,4,4);
	SetControlInfo(IDC_STATIC_TS_TXLO1,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_STATIC_TS_TXLO2,ChgL_Fixx,3,2,1);
	SetControlInfo(IDC_STATIC_TS_RXLO1,ChgLT_Fixx,3,1,1,8,4,4);
	SetControlInfo(IDC_STATIC_TS_RXLO2,ChgLT_Fixx,3,2,1,8,4,4);
	SetControlInfo(IDC_STATIC_TX_LO1_FREQ_STOP,ChgT_Fixx,3,0,0,8,1,2);
	SetControlInfo(IDC_STATIC_TX_LO1_FREQ_STEP,ChgT_Fixx,3,0,0,8,2,3);
	SetControlInfo(IDC_STATIC_TX_LO1_CURRENT,ChgT_Fixx,3,0,0,8,3,4);
	SetControlInfo(IDC_STATIC_TX_LO2_FREQ_STAR,ChgL_Fixx,3,1,1);
	SetControlInfo(IDC_STATIC_TX_LO2_FREQ_STOP,ChgLT_Fixx,3,1,1,8,1,2);
	SetControlInfo(IDC_STATIC_TX_LO2_FREQ_STEP,ChgLT_Fixx,3,1,1,8,2,3);
	SetControlInfo(IDC_STATIC_TX_LO2_CURRENT,ChgLT_Fixx,3,1,1,8,3,4);
	SetControlInfo(IDC_STATIC_RX_LO1_FREQ_STAR,ChgT_Fixx,3,0,0,8,4,5);
	SetControlInfo(IDC_STATIC_RX_LO1_FREQ_STOP,ChgT_Fixx,3,0,0,8,5,6);
	SetControlInfo(IDC_STATIC_RX_LO1_FREQ_STEP,ChgT_Fixx,3,0,0,8,6,7);
	SetControlInfo(IDC_STATIC_RX_LO1_CURRENT,ChgT_Fixx,3,0,0,8,7,8);
	SetControlInfo(IDC_STATIC_RX_LO2_FREQ_STAR,ChgLT_Fixx,3,1,1,8,4,5);
	SetControlInfo(IDC_STATIC_RX_LO2_FREQ_STOP,ChgLT_Fixx,3,1,1,8,5,6);
	SetControlInfo(IDC_STATIC_RX_LO2_FREQ_STEP,ChgLT_Fixx,3,1,1,8,6,7);
	SetControlInfo(IDC_STATIC_RX_LO2_CURRENT,ChgLT_Fixx,3,1,1,8,7,8);
	SetControlInfo(IDC_EDIT3,ChgLT_xyStretch,3,0,1,8,0,1);
	SetControlInfo(IDC_EDIT4,ChgLT_xyStretch,3,0,1,8,1,2);
	SetControlInfo(IDC_EDIT26,ChgLT_xyStretch,3,0,1,8,2,3);
	SetControlInfo(IDC_EDIT19,ChgLT_xyStretch,3,0,1,8,3,4);
	SetControlInfo(IDC_EDIT14,ChgLT_xyStretch,3,1,2,8,0,1);
	SetControlInfo(IDC_EDIT69,ChgLT_xyStretch,3,1,2,8,1,2);
	SetControlInfo(IDC_EDIT74,ChgLT_xyStretch,3,1,2,8,2,3);
	SetControlInfo(IDC_EDIT20,ChgLT_xyStretch,3,1,2,8,3,4);
	SetControlInfo(IDC_EDIT70,ChgLT_xyStretch,3,0,1,8,4,5);
	SetControlInfo(IDC_EDIT79,ChgLT_xyStretch,3,0,1,8,5,6);
	SetControlInfo(IDC_EDIT84,ChgLT_xyStretch,3,0,1,8,6,7);
	SetControlInfo(IDC_EDIT25,ChgLT_xyStretch,3,0,1,8,7,8);
	SetControlInfo(IDC_EDIT85,ChgLT_xyStretch,3,1,2,8,4,5);
	SetControlInfo(IDC_EDIT86,ChgLT_xyStretch,3,1,2,8,5,6);
	SetControlInfo(IDC_EDIT87,ChgLT_xyStretch,3,1,2,8,6,7);
	SetControlInfo(IDC_EDIT28,ChgLT_xyStretch,3,1,2,8,7,8);
	SetControlInfo(IDC_BUTTON62,ChgLT_xyStretch,3,2,2,8,0,8);
	SetControlInfo(IDC_STATIC_LO_LOCK,FixL_xyStretch,3,0,2,8,0,8);
	SetControlInfo(IDC_CHECK_BIT_K7_0,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_CHECK_BIT_K7_1,ChgL_Fixx,3,2,2);
	SetControlInfo(IDC_EDIT6,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_EDIT8,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_BUTTON5,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_BUTTON13,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_BUTTON2,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_STATIC_DYN_DOWN,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_EDIT2,ChgL_xStretch,3,2,3);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,3,3,3);
	SetControlInfo(IDC_STATIC_SN_READ,ChgL_xStretch,3,2,3);

	AddThreadIdleCtrl(IDC_EDIT1);
	AddThreadIdleCtrl(IDC_CHECK1);
	AddThreadIdleCtrl(IDC_CHECK2);
	AddThreadIdleCtrl(IDC_CHECK7);
	AddThreadIdleCtrl(IDC_CHECK33);
	AddThreadIdleCtrl(IDC_EDIT3);
	AddThreadIdleCtrl(IDC_EDIT4);
	AddThreadIdleCtrl(IDC_EDIT26);
	AddThreadIdleCtrl(IDC_EDIT14);
	AddThreadIdleCtrl(IDC_EDIT69);
	AddThreadIdleCtrl(IDC_EDIT74);
	AddThreadIdleCtrl(IDC_EDIT70);
	AddThreadIdleCtrl(IDC_EDIT79);
	AddThreadIdleCtrl(IDC_EDIT84);
	AddThreadIdleCtrl(IDC_EDIT85);
	AddThreadIdleCtrl(IDC_EDIT86);
	AddThreadIdleCtrl(IDC_EDIT87);
	AddThreadIdleCtrl(IDC_BUTTON62);
	AddThreadIdleCtrl(IDC_CHECK_BIT_K7_0);
	AddThreadIdleCtrl(IDC_CHECK_BIT_K7_1);
	AddThreadIdleCtrl(IDC_BUTTON5);
	AddThreadIdleCtrl(IDC_BUTTON13);
	AddThreadIdleCtrl(IDC_BUTTON2);
	AddThreadIdleCtrl(IDC_BUTTON1);
	AddThreadRunningCtrl(IDC_BUTTON7);
	AddThreadToPauseCtrl(IDC_BUTTON18);
	AddThreadToRunningCtrl(IDC_BUTTON8);
	CtrlResume();
	return TRUE;
}

void CTestR1ASBFSDlg::OnBnClickedPause()
{
	m_pThread->SuspendThread();
	CtrlPausing();
}

void CTestR1ASBFSDlg::OnBnClickedContinue()
{
	m_pThread->ResumeThread();
	CtrlRunning();
}

void CTestR1ASBFSDlg::OnBnClickedCheckBitK70()
{
	UpdateData(TRUE);
	for (int32_t i = 0;i < MAX_RF;i ++) {
		ALL_DLG->m_pTestR1ASBFSDlg[i].m_bDynDownK7_0 = this->m_bDynDownK7_0;
		::PostMessage(ALL_DLG->m_pTestR1ASBFSDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
}

void CTestR1ASBFSDlg::OnBnClickedCheckBitK71()
{
	UpdateData(TRUE);
	for (int32_t i = 0;i < MAX_RF;i ++) {
		ALL_DLG->m_pTestR1ASBFSDlg[i].m_bDynDownK7_1 = this->m_bDynDownK7_1;
		::PostMessage(ALL_DLG->m_pTestR1ASBFSDlg[i].GetSafeHwnd(),WM_CSE_UPDATE_DATA_FALSE,0,0);
	}
}
