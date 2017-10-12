#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1ADDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRFR1ADDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CRFR1ADDlg)

CRFR1ADDlg::CRFR1ADDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CRFR1ADDlg::IDD, pParent)
	, m_strTxLO1(_T("3.8G"))
	, m_strTxLO2(_T("6.8G"))
	, m_strRxLO1(_T("9.9G"))
	, m_strRxLO2(_T("7.224G"))
	, m_strTxDet(_T("0"))
	, m_strRxDet(_T("0"))
	, m_bTxADL5375(TRUE)
	, m_bPowerOnOff(TRUE)
	, m_strTxTemp(_T("0.00"))
	, m_strRxTemp(_T("0.00"))
	, m_bTxLED(TRUE)
	, m_bRxLED(TRUE)
	, m_strTxFreq(_T("3.8G"))
	, m_strRxFreq(_T("2.4G"))
{
	m_pLTC2666Dlg = NULL;
	m_pEEPROMDlg = NULL;
}

CRFR1ADDlg::~CRFR1ADDlg()
{
	SAFE_DESTROY_DLG(m_pLTC2666Dlg);
	SAFE_DESTROY_DLG(m_pEEPROMDlg);
}

void CRFR1ADDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTxLO1);
	DDX_Text(pDX, IDC_EDIT2, m_strTxLO2);
	DDX_Text(pDX, IDC_EDIT4, m_strRxLO1);
	DDX_Text(pDX, IDC_EDIT18, m_strRxLO2);
	DDX_Text(pDX, IDC_EDIT19, m_strTxDet);
	DDX_Text(pDX, IDC_EDIT29, m_strRxDet);
	DDX_Check(pDX, IDC_CHECK3, m_bTxADL5375);
	DDX_Control(pDX, IDC_CHECK3, m_TxMOD);
	DDX_Control(pDX, IDC_COMBO1, m_TxBandSw);
	DDX_Control(pDX, IDC_COMBO8, m_TxPAAttSw);
	DDX_Control(pDX, IDC_CHECK25, m_Pow);
	DDX_Check(pDX, IDC_CHECK25, m_bPowerOnOff);
	DDX_Control(pDX, IDC_COMBO16, m_RxLNAAttSw);
	DDX_Control(pDX, IDC_COMBO19, m_RxIFFilterSw);
	DDX_Control(pDX, IDC_COMBO12, m_TxIOSw);
	DDX_Control(pDX, IDC_COMBO13, m_RxIOSw1);
	DDX_Control(pDX, IDC_COMBO14, m_RxIOSw2);
	DDX_Control(pDX, IDC_COMBO15, m_RxIOSw);
	DDX_Control(pDX, IDC_COMBO38, m_IOMode);
	DDX_Control(pDX, IDC_COMBO2, m_TxAtt1);
	DDX_Control(pDX, IDC_COMBO3, m_TxAtt2);
	DDX_Control(pDX, IDC_COMBO4, m_TxAtt3);
	DDX_Control(pDX, IDC_COMBO11, m_RxAtt1);
	DDX_Control(pDX, IDC_COMBO17, m_RxAtt2);
	DDX_Text(pDX, IDC_EDIT30, m_strTxTemp);
	DDX_Text(pDX, IDC_EDIT31, m_strRxTemp);
	DDX_Check(pDX, IDC_CHECK20, m_bTxLED);
	DDX_Check(pDX, IDC_CHECK26, m_bRxLED);
	DDX_Text(pDX, IDC_EDIT41, m_strTxFreq);
	DDX_Text(pDX, IDC_EDIT77, m_strRxFreq);
	DDX_Control(pDX, IDC_STATIC_LOCK_TX_LO2, m_LockTxLO2);
	DDX_Control(pDX, IDC_STATIC_LOCK_TX_LO1, m_LockTxLO1);
	DDX_Control(pDX, IDC_STATIC_LOCK_RX_LO1, m_LockRxLO1);
	DDX_Control(pDX, IDC_STATIC_LOCK_RX_LO2, m_LockRxLO2);
}


BEGIN_MESSAGE_MAP(CRFR1ADDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFR1ADDlg::OnBnClickedSetTxLO1)
	ON_BN_CLICKED(IDC_BUTTON21, &CRFR1ADDlg::OnBnClickedSetTxLO2)
	ON_BN_CLICKED(IDC_BUTTON29, &CRFR1ADDlg::OnBnClickedSetRxLO1)
	ON_BN_CLICKED(IDC_BUTTON31, &CRFR1ADDlg::OnBnClickedSetRxLO2)
	ON_BN_CLICKED(IDC_BUTTON38, &CRFR1ADDlg::OnBnClickedDebugTxLO1)
	ON_BN_CLICKED(IDC_BUTTON37, &CRFR1ADDlg::OnBnClickedDebugTxLO2)
	ON_BN_CLICKED(IDC_BUTTON39, &CRFR1ADDlg::OnBnClickedDebugRxLO1)
	ON_BN_CLICKED(IDC_BUTTON40, &CRFR1ADDlg::OnBnClickedDebugRxLO2)
	ON_BN_CLICKED(IDC_BUTTON41, &CRFR1ADDlg::OnBnClickedSetLTC2666)
	ON_BN_CLICKED(IDC_BUTTON8, &CRFR1ADDlg::OnBnClickedGetTxDet)
	ON_BN_CLICKED(IDC_BUTTON33, &CRFR1ADDlg::OnBnClickedGetRxDet)
	ON_BN_CLICKED(IDC_CHECK3, &CRFR1ADDlg::OnBnClickedSetTxADL5375)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFR1ADDlg::OnBnClickedSetTxBandSw)
	ON_BN_CLICKED(IDC_BUTTON9, &CRFR1ADDlg::OnBnClickedSetTxPAAttSw)
	ON_BN_CLICKED(IDC_CHECK25, &CRFR1ADDlg::OnBnClickedSetRFPower)
	ON_BN_CLICKED(IDC_BUTTON26, &CRFR1ADDlg::OnBnClickedSetRxLNAAttSw)
	ON_BN_CLICKED(IDC_BUTTON32, &CRFR1ADDlg::OnBnClickedSetRxIFFilterSw)
	ON_BN_CLICKED(IDC_BUTTON6, &CRFR1ADDlg::OnBnClickedSetTxIOSw)
	ON_BN_CLICKED(IDC_BUTTON23, &CRFR1ADDlg::OnBnClickedSetRxIOSw1)
	ON_BN_CLICKED(IDC_BUTTON24, &CRFR1ADDlg::OnBnClickedSetRxIOSw2)
	ON_BN_CLICKED(IDC_BUTTON25, &CRFR1ADDlg::OnBnClickedSetRxIOSw)
	ON_BN_CLICKED(IDC_BUTTON56, &CRFR1ADDlg::OnBnClickedSetIOMode)
	ON_BN_CLICKED(IDC_BUTTON4, &CRFR1ADDlg::OnBnClickedSetTxAtt1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFR1ADDlg::OnBnClickedSetTxAtt2)
	ON_BN_CLICKED(IDC_BUTTON22, &CRFR1ADDlg::OnBnClickedSetTxAtt3)
	ON_BN_CLICKED(IDC_BUTTON27, &CRFR1ADDlg::OnBnClickedSetRxAtt1)
	ON_BN_CLICKED(IDC_BUTTON28, &CRFR1ADDlg::OnBnClickedSetRxAtt2)
	ON_BN_CLICKED(IDC_BUTTON35, &CRFR1ADDlg::OnBnClickedGetTxTemp)
	ON_BN_CLICKED(IDC_BUTTON36, &CRFR1ADDlg::OnBnClickedGetRxTemp)
	ON_BN_CLICKED(IDC_BUTTON61, &CRFR1ADDlg::OnBnClickedDebugEEPROM)
	ON_BN_CLICKED(IDC_CHECK20, &CRFR1ADDlg::OnBnClickedSetTxLED)
	ON_BN_CLICKED(IDC_CHECK26, &CRFR1ADDlg::OnBnClickedSetRxLED)
	ON_BN_CLICKED(IDC_BUTTON66, &CRFR1ADDlg::OnBnClickedSetTxFreq)
	ON_BN_CLICKED(IDC_BUTTON67, &CRFR1ADDlg::OnBnClickedSetRxFreq)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXFREQ, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT1, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT2, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXPAATT, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT3, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXDET, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXTEMP, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXFREQ, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT1, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT2, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXDET, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXTEMP, &CRFR1ADDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_IOMODE, &CRFR1ADDlg::SyncWithRFDlg)
END_MESSAGE_MAP()


BOOL CRFR1ADDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pLTC2666Dlg = new CLTC266616Dlg;
	m_pLTC2666Dlg->Create(CLTC266616Dlg::IDD,GetDesktopWindow(),m_pSP1401);
	m_pLTC2666Dlg->ShowWindow(SW_HIDE);

	m_pEEPROMDlg = new CAT24C256BDlg;
	m_pEEPROMDlg->Create(CAT24C256BDlg::IDD,GetDesktopWindow(),dynamic_cast<ISP1401 *>(m_pSP1401));
	m_pEEPROMDlg->ShowWindow(SW_HIDE);

	m_Pow.SetWindowText("Pow On");
	m_TxMOD.SetWindowText("ADL5375 En");
	m_TxBandSw.SetCurSel(1);
	m_TxPAAttSw.SetCurSel(1);
	m_TxIOSw.SetCurSel(2);
	m_TxAtt1.SetCurSel(1);
	m_TxAtt2.SetCurSel(2);
	m_TxAtt3.SetCurSel(2);
	m_RxLNAAttSw.SetCurSel(0);
	m_RxIFFilterSw.SetCurSel(0);
	m_RxIOSw1.SetCurSel(0);
	m_RxIOSw2.SetCurSel(0);
	m_RxIOSw.SetCurSel(3);
	m_RxAtt1.SetCurSel(40);
	m_RxAtt2.SetCurSel(2);
	m_IOMode.SetCurSel(1);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_STATIC_10,UnChange);
	SetControlInfo(IDC_STATIC_18,UnChange);
	SetControlInfo(IDC_COMBO12,FixL_xStretch,6,0,1,0,0);
	SetControlInfo(IDC_BUTTON6,FixL_xStretch,6,0,1,0,0);
	SetControlInfo(IDC_STATIC_9,ChgL_Fixx,6,1,1,0,0);
	SetControlInfo(IDC_STATIC_17,ChgL_Fixx,6,1,1,0,0);
	SetControlInfo(IDC_STATIC_34,ChgL_Fixx,6,1,1,0,0);
	SetControlInfo(IDC_STATIC_16,ChgL_Fixx,6,1,1,0,0);
	SetControlInfo(IDC_COMBO4,ChgL_xStretch,6,1,2,0,0);
	SetControlInfo(IDC_BUTTON22,ChgL_xStretch,6,1,2,0,0);
	SetControlInfo(IDC_STATIC_8,ChgL_Fixx,6,2,2,0,0);
	SetControlInfo(IDC_STATIC_15,ChgL_Fixx,6,2,2,0,0);
	SetControlInfo(IDC_COMBO8,ChgL_xStretch,6,2,3,0,0);
	SetControlInfo(IDC_BUTTON9,ChgL_xStretch,6,2,3,0,0);
	SetControlInfo(IDC_STATIC_30,ChgL_xStretch,6,1,3,0,0);
	SetControlInfo(IDC_STATIC_7,ChgL_Fixx,6,3,3,0,0);
	SetControlInfo(IDC_STATIC_14,ChgL_Fixx,6,3,3,0,0);
	SetControlInfo(IDC_COMBO3,ChgL_xStretch,6,3,4,0,0);
	SetControlInfo(IDC_BUTTON3,ChgL_xStretch,6,3,4,0,0);
	SetControlInfo(IDC_STATIC_20,ChgL_Fixx,6,4,4,0,0);
	SetControlInfo(IDC_STATIC_21,ChgL_Fixx,6,4,4,0,0);
	SetControlInfo(IDC_COMBO2,ChgL_xStretch,6,4,5,0,0);
	SetControlInfo(IDC_BUTTON4,ChgL_xStretch,6,4,5,0,0);
	SetControlInfo(IDC_STATIC_6,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_11,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_5,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_4,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_36,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_12,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_EDIT2,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_BUTTON21,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_BUTTON37,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_LOCK_TX_LO2,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_STATIC_19,ChgL_Fixx,6,5,5,0,0);
	SetControlInfo(IDC_COMBO1,ChgL_xStretch,6,5,6,0,0);
	SetControlInfo(IDC_BUTTON2,ChgL_xStretch,6,5,6,0,0);
	SetControlInfo(IDC_STATIC_3,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_CHECK3,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_37,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_38,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_13,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_EDIT1,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_BUTTON38,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_LOCK_TX_LO1,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_1,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_2,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_CHECK25,ChgL_Fixx,6,6,6,0,0);
	SetControlInfo(IDC_STATIC_39,ChgL_Fixx,6,5,5,0,0);

	SetControlInfo(IDC_STATIC_22,ChgT_Fixx,6,0,0,3,1,2);
	SetControlInfo(IDC_STATIC_114,ChgT_Fixx,6,0,0,3,1,2);
	SetControlInfo(IDC_STATIC_45,ChgT_Fixx,6,0,0,3,1,2);
	SetControlInfo(IDC_COMBO38,ChgT_xStretch,6,0,1,3,1,2);
	SetControlInfo(IDC_BUTTON56,ChgT_xStretch,6,0,1,3,1,2);
	SetControlInfo(IDC_CHECK20,ChgLT_Fixx,6,1,1,3,1,2);
	SetControlInfo(IDC_CHECK26,ChgLT_Fixx,6,1,1,3,1,2);
	SetControlInfo(IDC_EDIT41,ChgLT_xStretch,6,1,5,3,1,2);
	SetControlInfo(IDC_EDIT77,ChgLT_xStretch,6,1,5,3,1,2);
	SetControlInfo(IDC_BUTTON66,ChgLT_Fixx,6,5,5,3,1,2);
	SetControlInfo(IDC_BUTTON67,ChgLT_Fixx,6,5,5,3,1,2);
	SetControlInfo(IDC_STATIC_31,ChgLT_xStretch,6,1,5,3,1,2);
	SetControlInfo(IDC_STATIC_46,ChgLT_Fixx,6,5,5,3,1,2);
	SetControlInfo(IDC_EDIT19,ChgLT_xStretch,6,5,6,3,1,2);
	SetControlInfo(IDC_EDIT29,ChgLT_xStretch,6,5,6,3,1,2);
	SetControlInfo(IDC_BUTTON8,ChgLT_Fixx,6,6,6,3,1,2);
	SetControlInfo(IDC_BUTTON33,ChgLT_Fixx,6,6,6,3,1,2);
	SetControlInfo(IDC_EDIT30,ChgLT_xStretch,6,6,6,3,1,2);
	SetControlInfo(IDC_EDIT31,ChgLT_xStretch,6,6,6,3,1,2);
	SetControlInfo(IDC_BUTTON35,ChgLT_Fixx,6,6,6,3,1,2);
	SetControlInfo(IDC_BUTTON36,ChgLT_Fixx,6,6,6,3,1,2);
	SetControlInfo(IDC_BUTTON61,ChgLT_Fixx,6,6,6,3,1,2);
	SetControlInfo(IDC_STATIC_32,ChgLT_xStretch,6,5,6,3,1,2);

	SetControlInfo(IDC_STATIC_101,ChgT_Fixx,6,0,0,3,2,3);
	SetControlInfo(IDC_COMBO13,ChgT_xStretch,6,0,1,3,2,3);
	SetControlInfo(IDC_BUTTON23,ChgT_xStretch,6,0,1,3,2,3);
	SetControlInfo(IDC_STATIC_102,ChgT_Fixx,6,0,0,3,2,3);
	SetControlInfo(IDC_COMBO14,ChgT_xStretch,6,0,1,3,2,3);
	SetControlInfo(IDC_BUTTON24,ChgT_xStretch,6,0,1,3,2,3);
	SetControlInfo(IDC_STATIC_103,ChgLT_Fixx,6,1,1,3,2,3);
	SetControlInfo(IDC_STATIC_116,ChgLT_Fixx,6,1,1,3,2,3);
	SetControlInfo(IDC_COMBO15,ChgLT_xStretch,6,1,3,3,2,3);
	SetControlInfo(IDC_BUTTON25,ChgLT_xStretch,6,1,3,3,2,3);
	SetControlInfo(IDC_STATIC_104,ChgLT_Fixx,6,3,3,3,2,3);
	SetControlInfo(IDC_STATIC_117,ChgLT_Fixx,6,3,3,3,2,3);
	SetControlInfo(IDC_COMBO16,ChgLT_xStretch,6,3,4,3,2,3);
	SetControlInfo(IDC_BUTTON26,ChgLT_xStretch,6,3,4,3,2,3);
	SetControlInfo(IDC_STATIC_105,ChgLT_Fixx,6,4,4,3,2,3);
	SetControlInfo(IDC_STATIC_118,ChgLT_Fixx,6,4,4,3,2,3);
	SetControlInfo(IDC_COMBO11,ChgLT_xStretch,6,4,5,3,2,3);
	SetControlInfo(IDC_BUTTON27,ChgLT_xStretch,6,4,5,3,2,3);
	SetControlInfo(IDC_STATIC_124,ChgLT_Fixx,6,4,4,3,2,3);
	SetControlInfo(IDC_STATIC_35,ChgLT_xStretch,6,4,5,3,2,3);
	SetControlInfo(IDC_STATIC_33,ChgLT_xStretch,6,3,5,3,2,3);
	SetControlInfo(IDC_STATIC_106,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_47,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_119,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_COMBO17,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON28,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_107,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_112,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_108,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_115,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_125,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_120,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_EDIT4,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON29,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON39,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_LOCK_RX_LO1,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_121,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_COMBO18,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON30,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON41,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_109,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_113,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_110,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_126,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_127,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_122,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_EDIT18,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON31,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_BUTTON40,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_LOCK_RX_LO2,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_123,ChgLT_Fixx,6,5,5,3,2,3);
	SetControlInfo(IDC_COMBO19,ChgLT_xStretch,6,5,6,3,2,3);
	SetControlInfo(IDC_BUTTON32,ChgLT_xStretch,6,5,6,3,2,3);
	SetControlInfo(IDC_STATIC_111,ChgLT_Fixx,6,6,6,3,2,3);

	return TRUE;
}

void CRFR1ADDlg::OnBnClickedSetTxLO1()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	CSP1401R1A::TxBand Band = (CSP1401R1A::TxBand)(m_TxBandSw.GetCurSel());

	str2freq(m_strTxLO1,uiFreqLO1);
	str2freq(m_strTxLO2,uiFreqLO2);
	m_pSP1401->TxLO2Freq(uiFreqLO1,uiFreqLO2,Band,uiFreq);
	freq2str(uiFreq,m_strTxFreq);
	STACHKV(m_pSP1401->SetLO(CSP1401R1A::Tx_LO_1,uiFreqLO1));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1A::Tx_LO_1,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO1.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_Band  = Band;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetTxLO2()
{
	UpdateData(TRUE);
	uint64_t uiFreq	 = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	CSP1401R1A::TxBand Band  = (CSP1401R1A::TxBand)(m_TxBandSw.GetCurSel());

	str2freq(m_strTxLO1,uiFreqLO1);
	str2freq(m_strTxLO2,uiFreqLO2);
	m_pSP1401->TxLO2Freq(uiFreqLO1,uiFreqLO2,Band,uiFreq);
	freq2str(uiFreq,m_strTxFreq);
	STACHKV(m_pSP1401->SetLO(CSP1401R1A::Tx_LO_2,uiFreqLO2));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1A::Tx_LO_2,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO2.SetBitmap((HBITMAP)(Bitmap.Detach()));

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_Band  = Band;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetRxLO1()
{
	UpdateData(TRUE);
	uint64_t uiFreq	   = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;

	str2freq(m_strRxLO1,uiFreqLO1);
	str2freq(m_strRxLO2,uiFreqLO2);
	m_pSP1401->RxLO2Freq(uiFreqLO1,uiFreqLO2,uiFreq);
	freq2str(uiFreq,m_strRxFreq);
	STACHKV(m_pSP1401->SetLO(CSP1401R1A::Rx_LO_1,uiFreqLO1));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1A::Rx_LO_1,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockRxLO1.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_uiLO1 = uiFreqLO1;
	RxState.m_uiLO2 = uiFreqLO2;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedSetRxLO2()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;

	str2freq(m_strRxLO1,uiFreqLO1);
	str2freq(m_strRxLO2,uiFreqLO2);
	m_pSP1401->RxLO2Freq(uiFreqLO1,uiFreqLO2,uiFreq);
	freq2str(uiFreq,m_strRxFreq);
	STACHKV(m_pSP1401->SetLO(CSP1401R1A::Rx_LO_2,uiFreqLO2));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1A::Rx_LO_2,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockRxLO2.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1A::TxChainStateAll RxState;
	RxState.m_uiLO1 = uiFreqLO1;
	RxState.m_uiLO2 = uiFreqLO2;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedDebugTxLO1()
{
	g_pADF5355Dlg->m_iLOIdx = 0;
	g_pADF5355Dlg->m_pSP1401R1A = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1C = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1ADDlg::OnBnClickedDebugTxLO2()
{
	g_pADF5355Dlg->m_iLOIdx = 1;
	g_pADF5355Dlg->m_pSP1401R1A = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1C = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1ADDlg::OnBnClickedDebugRxLO1()
{
	g_pADF5355Dlg->m_iLOIdx = 2;
	g_pADF5355Dlg->m_pSP1401R1A = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1C = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1ADDlg::OnBnClickedDebugRxLO2()
{
	g_pADF5355Dlg->m_iLOIdx = 3;
	g_pADF5355Dlg->m_pSP1401R1A = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1C = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1ADDlg::OnBnClickedSetLTC2666()
{
	UpdateData(TRUE);
	m_pLTC2666Dlg->m_pSP1401 = m_pSP1401;
	m_pLTC2666Dlg->ShowWindow(SW_NORMAL);
	m_pLTC2666Dlg->SetFocus();
}

void CRFR1ADDlg::OnBnClickedGetTxDet()
{
	STACHKV(m_pSP1401->SetDetSw(CSP1401R1A::Tx));
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680(uiDet));
	m_strTxDet.Format("%d",uiDet);
	UpdateData(FALSE);

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_uiDet = uiDet;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXDET,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedGetRxDet()
{
	STACHKV(m_pSP1401->SetDetSw(CSP1401R1A::Rx));
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680(uiDet));
	m_strRxDet.Format("%d",uiDet);
	UpdateData(FALSE);

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_uiDet = uiDet;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedSetTxADL5375()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetTxModulatorEn(m_bTxADL5375));
	m_TxMOD.SetWindowText(m_bTxADL5375 ? "ADL5375 En" : "ADL5375 Dis");
}

void CRFR1ADDlg::OnBnClickedSetTxBandSw()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;

	str2freq(m_strTxLO1,uiFreqLO1);
	str2freq(m_strTxLO2,uiFreqLO2);
	CSP1401R1A::TxBand Band = (CSP1401R1A::TxBand)m_TxBandSw.GetCurSel();
	m_pSP1401->TxLO2Freq(uiFreqLO1,uiFreqLO2,Band,uiFreq);
	STACHKV(m_pSP1401->SetTxBandSw(Band));
	freq2str(uiFreq,m_strTxFreq);
	UpdateData(FALSE);

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetTxPAAttSw()
{
	UpdateData(TRUE);
	SP1401::TxPAAtt PAAtt = (SP1401::TxPAAtt)m_TxPAAttSw.GetCurSel();
	STACHKV(m_pSP1401->SetTxPAAttSw(PAAtt));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_PAAtt = PAAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXPAATT,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetRFPower()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetPowEn(m_bPowerOnOff));
	m_Pow.SetWindowText(m_bPowerOnOff ? "Pow On" : "Pow Off");
}

void CRFR1ADDlg::OnBnClickedSetRxLNAAttSw()
{
	UpdateData(TRUE);
	SP1401::RxLNAAtt LNAAtt = (SP1401::RxLNAAtt)m_RxLNAAttSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxLNAAttSw(LNAAtt));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_LNAAtt = LNAAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedSetRxIFFilterSw()
{
	UpdateData(TRUE);
	CSP1401R1A::RxIFFilter Filter = (CSP1401R1A::RxIFFilter)m_RxIFFilterSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxIFFilterSw(Filter));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_IFFilter = Filter;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedSetTxIOSw()
{
	UpdateData(TRUE);
	CSP1401R1A::TxIOSw Mode = (CSP1401R1A::TxIOSw)m_TxIOSw.GetCurSel();
	STACHKV(m_pSP1401->SetTxIOSw(Mode));
}

void CRFR1ADDlg::OnBnClickedSetRxIOSw1()
{
	UpdateData(TRUE);
	CSP1401R1A::RxIOSw1 Mode = (CSP1401R1A::RxIOSw1)m_RxIOSw1.GetCurSel();
	STACHKV(m_pSP1401->SetRxIOSw1(Mode));
}

void CRFR1ADDlg::OnBnClickedSetRxIOSw2()
{
	UpdateData(TRUE);
	CSP1401R1A::RxIOSw2 Mode = (CSP1401R1A::RxIOSw2)m_RxIOSw2.GetCurSel();
	STACHKV(m_pSP1401->SetRxIOSw2(Mode));
}

void CRFR1ADDlg::OnBnClickedSetRxIOSw()
{
	UpdateData(TRUE);
	CSP1401R1A::RxIOSw Mode = (CSP1401R1A::RxIOSw)m_RxIOSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxIOSw(Mode));
}

void CRFR1ADDlg::OnBnClickedSetIOMode()
{
	UpdateData(TRUE);
	IOMode Mode = (IOMode)m_IOMode.GetCurSel();
	STACHKV(m_pSP1401->SetIOMode(Mode));

	CSP1401R1A::TxIOSw TxSw = CSP1401R1A::Tx_On;
	CSP1401R1A::RxIOSw RxSw = CSP1401R1A::Rx_On;
	CSP1401R1A::RxIOSw1 RxSw1 = CSP1401R1A::Rx_IO_On_1;
	CSP1401R1A::RxIOSw2 RxSw2 = CSP1401R1A::Rx_On_2;
	bool bTxLED = true,bRxLED = true;
	m_pSP1401->IOMode2IOSw(Mode,TxSw,RxSw,RxSw1,RxSw2,bTxLED,bRxLED);
	m_TxIOSw.SetCurSel(int(TxSw));
	m_RxIOSw.SetCurSel(int(RxSw));
	m_RxIOSw1.SetCurSel(int(RxSw1));
	m_RxIOSw2.SetCurSel(int(RxSw2));
	m_bTxLED = bTxLED;
	m_bRxLED = bRxLED;
	UpdateData(FALSE);

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_Mode = Mode;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_IOMODE,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedSetTxAtt1()
{
	UpdateData(TRUE);
	int32_t iAtt = m_TxAtt1.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetTxAtt1(iAtt));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt1 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT1,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetTxAtt2()
{
	UpdateData(TRUE);
	int32_t iAtt = m_TxAtt2.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetTxAtt2(iAtt));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt2 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT2,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetTxAtt3()
{
	UpdateData(TRUE);
	int32_t iAtt = m_TxAtt3.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetTxAtt3(iAtt));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt3 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT3,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetRxAtt1()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt1.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt1(dAtt));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_dAtt1 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT1,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedSetRxAtt2()
{
	UpdateData(TRUE);
	int32_t iAtt = m_RxAtt2.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetRxAtt2(iAtt));

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_iAtt2 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT2,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedGetTxTemp()
{
	double dTemp = 0.0;
	STACHKV(m_pSP1401->GetTxTemp(dTemp));
	m_strTxTemp.Format("%.2f¡æ",dTemp);
	UpdateData(FALSE);

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_dTemp = dTemp;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedGetRxTemp()
{
	double dTemp = 0.0;
	STACHKV(m_pSP1401->GetRxTemp(dTemp));
	m_strRxTemp.Format("%.2f¡æ",dTemp);
	UpdateData(FALSE);

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_dTemp = dTemp;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnBnClickedDebugEEPROM()
{
	m_pEEPROMDlg->m_pSP1401 = dynamic_cast<ISP1401 *>(m_pSP1401);
	m_pEEPROMDlg->ShowWindow(TRUE);
	m_pEEPROMDlg->SetFocus();
}

void CRFR1ADDlg::OnBnClickedSetTxLED()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetLED(m_bTxLED,m_bRxLED));
}

void CRFR1ADDlg::OnBnClickedSetRxLED()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetLED(m_bTxLED,m_bRxLED));
}

void CRFR1ADDlg::OnBnClickedSetTxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	CSP1401R1A::TxBand Band  = CSP1401R1A::Low;

	str2freq(m_strTxFreq,uiFreq);
	m_pSP1401->TxFreq2LO(uiFreq,uiFreqLO1,uiFreqLO2,Band);
	STACHKV(m_pSP1401->SetTxFreq(uiFreq));

	freq2str(uiFreqLO1,m_strTxLO1);
	freq2str(uiFreqLO2,m_strTxLO2);
	m_TxBandSw.SetCurSel((int)Band);
	UpdateData(FALSE);

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_Band  = Band;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1ADDlg::OnBnClickedSetRxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;

	str2freq(m_strRxFreq,uiFreq);
	m_pSP1401->RxFreq2LO(uiFreq,uiFreqLO1,uiFreqLO2);
	STACHKV(m_pSP1401->SetRxFreq(uiFreq));

	freq2str(uiFreqLO1,m_strRxLO1);
	freq2str(uiFreqLO2,m_strRxLO2);
	UpdateData(FALSE);

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_uiLO1 = uiFreqLO1;
	RxState.m_uiLO2 = uiFreqLO2;
	::SendMessage(ALL_DLG->m_pRFR1ADlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}

void CRFR1ADDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (!::IsWindowVisible(m_hWnd))
		return;

	bool bLock[4] = {false,false,false,false};
	CSP1401R1A::LO Lo[4] = {CSP1401R1A::Tx_LO_1,CSP1401R1A::Tx_LO_2,CSP1401R1A::Rx_LO_1,CSP1401R1A::Rx_LO_2};
	CBitmap Bitmap;
	for (int32_t i = 0;i < 4;i ++)
		m_pSP1401->DetLO(Lo[i],bLock[i]);
	
	Bitmap.LoadBitmap(bLock[0] ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO1.SetBitmap((HBITMAP)Bitmap.Detach());
	Bitmap.LoadBitmap(bLock[1] ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO2.SetBitmap((HBITMAP)Bitmap.Detach());
	Bitmap.LoadBitmap(bLock[2] ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockRxLO1.SetBitmap((HBITMAP)Bitmap.Detach());
	Bitmap.LoadBitmap(bLock[3] ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockRxLO2.SetBitmap((HBITMAP)Bitmap.Detach());

	CStretchDlg::OnTimer(nIDEvent);
}

int CRFR1ADDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStretchDlg::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_LO_LOCK_DET,5 * 60 * 1000,NULL);
	return 0;
}

void CRFR1ADDlg::OnDestroy()
{
	CStretchDlg::OnDestroy();
	KillTimer(TIMER_LO_LOCK_DET);
}

LRESULT CRFR1ADDlg::SyncWithRFDlg(WPARAM wParam,LPARAM lParam)
{
	UINT uiMsg = GetCurrentMessage()->message;
	CSP1401R1A::TxChainStateAll TxState = *(CSP1401R1A::TxChainStateAll *)lParam;
	CSP1401R1A::RxChainStateAll RxState = *(CSP1401R1A::RxChainStateAll *)lParam;

	if (WM_CSE_SYNCRFANDRFD_DLG_TXFREQ == uiMsg) {
		uint64_t uiFreq = 0;
		m_pSP1401->TxLO2Freq(TxState.m_uiLO1,TxState.m_uiLO2,TxState.m_Band,uiFreq);
		freq2str(uiFreq,m_strTxFreq);
		freq2str(TxState.m_uiLO1,m_strTxLO1);
		freq2str(TxState.m_uiLO2,m_strTxLO2);
		m_TxBandSw.SetCurSel((int)TxState.m_Band);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT1 == uiMsg) {
		m_TxAtt1.SetCurSel(TxState.m_iAtt1 / 10);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT2 == uiMsg) {
		m_TxAtt2.SetCurSel(TxState.m_iAtt2 / 10);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXPAATT == uiMsg) {
		m_TxPAAttSw.SetCurSel((int)TxState.m_PAAtt);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT3 == uiMsg) {
		m_TxAtt3.SetCurSel(TxState.m_iAtt3 / 10);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXDET == uiMsg) {
		m_strTxDet.Format("%u",TxState.m_uiDet);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXTEMP == uiMsg) {
		m_strTxTemp.Format("%.2f¡æ",TxState.m_dTemp);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXFREQ == uiMsg) {
		uint64_t uiFreq = 0;
		m_pSP1401->RxLO2Freq(RxState.m_uiLO1,RxState.m_uiLO2,uiFreq);
		freq2str(uiFreq,m_strRxFreq);
		freq2str(RxState.m_uiLO1,m_strRxLO1);
		freq2str(RxState.m_uiLO2,m_strRxLO2);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT == uiMsg) {
		m_RxLNAAttSw.SetCurSel((int)RxState.m_LNAAtt);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXATT1 == uiMsg) {
		m_RxAtt1.SetCurSel(int(RxState.m_dAtt1 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXATT2 == uiMsg) {
		m_RxAtt2.SetCurSel(RxState.m_iAtt2 / 10);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER == uiMsg) {
		m_RxIFFilterSw.SetCurSel(int(RxState.m_IFFilter));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXDET == uiMsg) {
		m_strRxDet.Format("%u",RxState.m_uiDet);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXTEMP == uiMsg) {
		m_strRxTemp.Format("%.2f¡æ",RxState.m_dTemp);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_IOMODE == uiMsg) {
		CSP1401R1A::TxIOSw TxSw = CSP1401R1A::Tx_On;
		CSP1401R1A::RxIOSw RxSw = CSP1401R1A::Rx_On;
		CSP1401R1A::RxIOSw1 RxSw1 = CSP1401R1A::Rx_IO_On_1;
		CSP1401R1A::RxIOSw2 RxSw2 = CSP1401R1A::Rx_On_2;
		bool bTxLED = true,bRxLED = true;

		m_IOMode.SetCurSel((int)RxState.m_Mode);
		m_pSP1401->IOMode2IOSw(RxState.m_Mode,TxSw,RxSw,RxSw1,RxSw2,bTxLED,bRxLED);
		m_TxIOSw.SetCurSel(int(TxSw));
		m_RxIOSw.SetCurSel(int(RxSw));
		m_RxIOSw1.SetCurSel(int(RxSw1));
		m_RxIOSw2.SetCurSel(int(RxSw2));
		m_bTxLED = bTxLED;
		m_bRxLED = bRxLED;
		UpdateData(FALSE);
		return 0;
	}
	return 0;
}