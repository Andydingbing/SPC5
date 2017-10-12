#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1CDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRFR1CDDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CRFR1CDDlg)

CRFR1CDDlg::CRFR1CDDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CRFR1CDDlg::IDD, pParent)
	, m_bPowerOnOff(TRUE)
	, m_bTxMod(TRUE)
	, m_dTemp0(0.0)
	, m_dTemp1(0.0)
	, m_dTemp2(0.0)
	, m_dTemp3(0.0)
	, m_dTemp4(0.0)
	, m_dTemp5(0.0)
	, m_dTemp6(0.0)
	, m_dTemp7(0.0)
	, m_uiDetRxComb(0)
	, m_uiDetRxIF1(0)
	, m_uiDetRxIF2(0)
	, m_uiDetTxIF2(0)
	, m_strTxLO1(_T("925M"))
	, m_strTxLO2RxLO2(_T("6574M"))
	, m_strTxLO3(_T("11.299G"))
	, m_strTxFreq(_T("3.8G"))
	, m_strRxFreq(_T("2.4G"))
	, m_strRxLO1(_T("9.25G"))
	, m_bTxLED(TRUE)
	, m_bRxLED(TRUE)
{
	m_pSP1401 = NULL;
	m_pEEPROMDlg = NULL;
	m_pMCP23S17Dlg = NULL;
}

CRFR1CDDlg::~CRFR1CDDlg()
{
	SAFE_DESTROY_DLG(m_pEEPROMDlg);
	SAFE_DESTROY_DLG(m_pMCP23S17Dlg);
}

void CRFR1CDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK25, m_bPowerOnOff);
	DDX_Control(pDX, IDC_CHECK25, m_Pow);
	DDX_Check(pDX, IDC_CHECK3, m_bTxMod);
	DDX_Control(pDX, IDC_CHECK3, m_TxMOD);
	DDX_Control(pDX, IDC_COMBO59, m_TxAtt0);
	DDX_Control(pDX, IDC_COMBO8, m_TxFilter1Sw);
	DDX_Control(pDX, IDC_COMBO35, m_TxFilter2Sw);
	DDX_Control(pDX, IDC_COMBO2, m_TxAtt1);
	DDX_Control(pDX, IDC_COMBO3, m_TxAtt2);
	DDX_Control(pDX, IDC_COMBO4, m_TxAtt3);
	DDX_Control(pDX, IDC_COMBO12, m_TxIOSw1);
	DDX_Control(pDX, IDC_COMBO60, m_TxIOSw2);
	DDX_Control(pDX, IDC_COMBO64, m_LoopSw);
	DDX_Control(pDX, IDC_COMBO38, m_IOMode);
	DDX_Control(pDX, IDC_COMBO13, m_RxIOSw1);
	DDX_Control(pDX, IDC_COMBO14, m_RxIOSw2);
	DDX_Control(pDX, IDC_COMBO15, m_RxIOSw3);
	DDX_Control(pDX, IDC_COMBO16, m_RxLNAAttSw);
	DDX_Control(pDX, IDC_COMBO32, m_RxAtt019Sw);
	DDX_Control(pDX, IDC_COMBO11, m_RxAtt1);
	DDX_Control(pDX, IDC_COMBO17, m_RxAtt2);
	DDX_Control(pDX, IDC_COMBO19, m_RxIFFilterSw);
	DDX_Control(pDX, IDC_COMBO18, m_RxAtt3);
	DDX_Text(pDX, IDC_EDIT12, m_dTemp0);
	DDX_Text(pDX, IDC_EDIT114, m_dTemp1);
	DDX_Text(pDX, IDC_EDIT113, m_dTemp2);
	DDX_Text(pDX, IDC_EDIT31, m_dTemp3);
	DDX_Text(pDX, IDC_EDIT60, m_dTemp4);
	DDX_Text(pDX, IDC_EDIT115, m_dTemp5);
	DDX_Text(pDX, IDC_EDIT116, m_dTemp6);
	DDX_Text(pDX, IDC_EDIT117, m_dTemp7);
	DDX_Text(pDX, IDC_EDIT119, m_uiDetRxComb);
	DDX_Text(pDX, IDC_EDIT120, m_uiDetRxIF1);
	DDX_Text(pDX, IDC_EDIT121, m_uiDetRxIF2);
	DDX_Text(pDX, IDC_EDIT118, m_uiDetTxIF2);
	DDX_Text(pDX, IDC_EDIT1, m_strTxLO1);
	DDX_Text(pDX, IDC_EDIT2, m_strTxLO2RxLO2);
	DDX_Text(pDX, IDC_EDIT66, m_strTxLO3);
	DDX_Text(pDX, IDC_EDIT41, m_strTxFreq);
	DDX_Text(pDX, IDC_EDIT77, m_strRxFreq);
	DDX_Control(pDX, IDC_STATIC_LOCK_TX_LO1, m_LockTxLO1);
	DDX_Control(pDX, IDC_STATIC_LOCK_TX_LO2, m_LockTxLO2RxLO2);
	DDX_Control(pDX, IDC_STATIC_LOCK_TX_LO3, m_LockTxLO3);
	DDX_Control(pDX, IDC_STATIC_LOCK_RX_LO1, m_LockRxLO1);
	DDX_Text(pDX, IDC_EDIT4, m_strRxLO1);
	DDX_Check(pDX, IDC_CHECK20, m_bTxLED);
	DDX_Check(pDX, IDC_CHECK26, m_bRxLED);
}


BEGIN_MESSAGE_MAP(CRFR1CDDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON38, &CRFR1CDDlg::OnBnClickedDebugTxLO1)
	ON_BN_CLICKED(IDC_BUTTON37, &CRFR1CDDlg::OnBnClickedDebugTxLO2RxLO2)
	ON_BN_CLICKED(IDC_BUTTON119, &CRFR1CDDlg::OnBnClickedDebugTxLO3)
	ON_BN_CLICKED(IDC_BUTTON39, &CRFR1CDDlg::OnBnClickedDebugRxLO1)
	ON_BN_CLICKED(IDC_BUTTON1, &CRFR1CDDlg::OnBnClickedSetTxLO1)
	ON_BN_CLICKED(IDC_CHECK25, &CRFR1CDDlg::OnBnClickedSetRFPower)
	ON_BN_CLICKED(IDC_CHECK3, &CRFR1CDDlg::OnBnClickedSetTxMod)
	ON_BN_CLICKED(IDC_BUTTON90, &CRFR1CDDlg::OnBnClickedSetTxAtt0)
	ON_BN_CLICKED(IDC_BUTTON9, &CRFR1CDDlg::OnBnClickedSetTxFilterSw)
	ON_BN_CLICKED(IDC_BUTTON4, &CRFR1CDDlg::OnBnClickedSetTxAtt1)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFR1CDDlg::OnBnClickedSetTxAtt2)
	ON_BN_CLICKED(IDC_BUTTON22, &CRFR1CDDlg::OnBnClickedSetTxAtt3)
	ON_BN_CLICKED(IDC_BUTTON6, &CRFR1CDDlg::OnBnClickedSetTxIOSw1)
	ON_BN_CLICKED(IDC_BUTTON45, &CRFR1CDDlg::OnBnClickedSetTxIOSw2)
	ON_BN_CLICKED(IDC_BUTTON120, &CRFR1CDDlg::OnBnClickedSetLoopSw)
	ON_BN_CLICKED(IDC_BUTTON56, &CRFR1CDDlg::OnBnClickedSetIOMode)
	ON_BN_CLICKED(IDC_BUTTON23, &CRFR1CDDlg::OnBnClickedSetRxIOSw1)
	ON_BN_CLICKED(IDC_BUTTON24, &CRFR1CDDlg::OnBnClickedSetRxIOSw2)
	ON_BN_CLICKED(IDC_BUTTON25, &CRFR1CDDlg::OnBnClickedSetRxIOSw3)
	ON_BN_CLICKED(IDC_BUTTON26, &CRFR1CDDlg::OnBnClickedSetRxLNAAttSw)
	ON_BN_CLICKED(IDC_BUTTON31, &CRFR1CDDlg::OnBnClickedSetRxAtt019Sw)
	ON_BN_CLICKED(IDC_BUTTON27, &CRFR1CDDlg::OnBnClickedSetRxAtt1)
	ON_BN_CLICKED(IDC_BUTTON28, &CRFR1CDDlg::OnBnClickedSetRxAtt2)
	ON_BN_CLICKED(IDC_BUTTON32, &CRFR1CDDlg::OnBnClickedRxIFFilterSw)
	ON_BN_CLICKED(IDC_BUTTON30, &CRFR1CDDlg::OnBnClickedSetRxAtt3)
	ON_BN_CLICKED(IDC_BUTTON40, &CRFR1CDDlg::OnBnClickedGetTemp0)
	ON_BN_CLICKED(IDC_BUTTON41, &CRFR1CDDlg::OnBnClickedGetTemp1)
	ON_BN_CLICKED(IDC_BUTTON121, &CRFR1CDDlg::OnBnClickedGetTemp2)
	ON_BN_CLICKED(IDC_BUTTON122, &CRFR1CDDlg::OnBnClickedGetTemp3)
	ON_BN_CLICKED(IDC_BUTTON123, &CRFR1CDDlg::OnBnClickedGetTemp4)
	ON_BN_CLICKED(IDC_BUTTON124, &CRFR1CDDlg::OnBnClickedGetTemp5)
	ON_BN_CLICKED(IDC_BUTTON125, &CRFR1CDDlg::OnBnClickedGetTemp6)
	ON_BN_CLICKED(IDC_BUTTON126, &CRFR1CDDlg::OnBnClickedGetTemp7)
	ON_BN_CLICKED(IDC_BUTTON128, &CRFR1CDDlg::OnBnClickedGetDetRxComb)
	ON_BN_CLICKED(IDC_BUTTON129, &CRFR1CDDlg::OnBnClickedGetDetRxIF1)
	ON_BN_CLICKED(IDC_BUTTON130, &CRFR1CDDlg::OnBnClickedGetDetRxIF2)
	ON_BN_CLICKED(IDC_BUTTON127, &CRFR1CDDlg::OnBnClickedGetDetTxIF2)
	ON_BN_CLICKED(IDC_BUTTON21, &CRFR1CDDlg::OnBnClickedSetTxLO2RxLO2)
	ON_BN_CLICKED(IDC_BUTTON118, &CRFR1CDDlg::OnBnClickedSetTxLO3)
	ON_BN_CLICKED(IDC_BUTTON29, &CRFR1CDDlg::OnBnClickedSetRxLO1)
	ON_BN_CLICKED(IDC_BUTTON8, &CRFR1CDDlg::OnBnClickedDebugMCP23S17)
	ON_BN_CLICKED(IDC_BUTTON61, &CRFR1CDDlg::OnBnClickedDebugEEPROM)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK20, &CRFR1CDDlg::OnBnClickedSetTxLED)
	ON_BN_CLICKED(IDC_CHECK26, &CRFR1CDDlg::OnBnClickedSetRxLED)
	ON_BN_CLICKED(IDC_BUTTON33, &CRFR1CDDlg::OnBnClickedGetAllTemp)
	ON_BN_CLICKED(IDC_BUTTON101, &CRFR1CDDlg::OnBnClickedGetAllDet)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXFREQ, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT0, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT1, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT2, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT3, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXDET, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXTEMP, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXFREQ, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT019, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT1, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT2, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT3, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXDET, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXTEMP, &CRFR1CDDlg::SyncWithRFDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_IOMODE, &CRFR1CDDlg::SyncWithRFDlg)
	ON_BN_CLICKED(IDC_BUTTON66, &CRFR1CDDlg::OnBnClickedSetTxFreq)
	ON_BN_CLICKED(IDC_BUTTON67, &CRFR1CDDlg::OnBnClickedSetRxFreq)
END_MESSAGE_MAP()


BOOL CRFR1CDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Pow.SetWindowText("Pow On");
	m_TxMOD.SetWindowText("Mod En");
	m_TxAtt0.SetCurSel(20);
	m_TxAtt1.SetCurSel(0);
	m_TxAtt2.SetCurSel(0);
	m_TxAtt3.SetCurSel(0);
	m_TxFilter1Sw.SetCurSel(0);
	m_TxFilter2Sw.SetCurSel(0);
	m_TxIOSw1.SetCurSel(2);
	m_TxIOSw2.SetCurSel(1);
	m_LoopSw.SetCurSel(1);
	m_IOMode.SetCurSel(1);
	m_RxIOSw1.SetCurSel(0);
	m_RxIOSw2.SetCurSel(0);
	m_RxIOSw3.SetCurSel(3);
	m_RxLNAAttSw.SetCurSel(0);
	m_RxAtt019Sw.SetCurSel(0);
	m_RxAtt1.SetCurSel(20);
	m_RxAtt2.SetCurSel(0);
	m_RxAtt3.SetCurSel(0);
	m_RxIFFilterSw.SetCurSel(1);

	m_pEEPROMDlg = new CAT24C256BDlg;
	m_pEEPROMDlg->Create(CAT24C256BDlg::IDD,GetDesktopWindow(),dynamic_cast<ISP1401 *>(m_pSP1401));
	m_pEEPROMDlg->ShowWindow(SW_HIDE);

	m_pMCP23S17Dlg = new CMCP23S17Dlg;
	m_pMCP23S17Dlg->Create(CMCP23S17Dlg::IDD,GetDesktopWindow(),m_pSP1401);
	m_pMCP23S17Dlg->ShowWindow(SW_HIDE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_COMBO60,FixL_xStretch,9,0,1);
	SetControlInfo(IDC_BUTTON45,FixL_xStretch,9,0,1);
	SetControlInfo(IDC_STATIC_17,ChgL_Fixx,9,1,1);
	SetControlInfo(IDC_STATIC_18,ChgL_Fixx,9,1,1);
	SetControlInfo(IDC_COMBO12,ChgL_xStretch,9,1,2);
	SetControlInfo(IDC_BUTTON6,ChgL_xStretch,9,1,2);
	SetControlInfo(IDC_STATIC_9,ChgL_Fixx,9,2,2);
	SetControlInfo(IDC_STATIC_16,ChgL_Fixx,9,2,2);
	SetControlInfo(IDC_COMBO4,ChgL_xStretch,9,2,3);
	SetControlInfo(IDC_BUTTON22,ChgL_xStretch,9,2,3);
	SetControlInfo(IDC_STATIC_7,ChgL_Fixx,9,3,3);
	SetControlInfo(IDC_STATIC_14,ChgL_Fixx,9,3,3);
	SetControlInfo(IDC_COMBO3,ChgL_xStretch,9,3,4);
	SetControlInfo(IDC_BUTTON3,ChgL_xStretch,9,3,4);
	SetControlInfo(IDC_STATIC_20,ChgL_Fixx,9,4,4);
	SetControlInfo(IDC_STATIC_21,ChgL_Fixx,9,4,4);
	SetControlInfo(IDC_COMBO2,ChgL_xStretch,9,4,5);
	SetControlInfo(IDC_BUTTON4,ChgL_xStretch,9,4,5);
	SetControlInfo(IDC_STATIC_29,ChgL_Fixx,9,5,5);
	SetControlInfo(IDC_STATIC_28,ChgL_Fixx,9,5,5);
	SetControlInfo(IDC_COMBO8,ChgL_xStretch,9,5,6);
	SetControlInfo(IDC_COMBO35,ChgL_xStretch,9,5,6);
	SetControlInfo(IDC_BUTTON9,ChgL_xStretch,9,5,6);
	SetControlInfo(IDC_STATIC_11,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_23,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_COMBO59,ChgL_xStretch,9,6,7);
	SetControlInfo(IDC_BUTTON90,ChgL_xStretch,9,6,7);
	SetControlInfo(IDC_STATIC_3,ChgL_Fixx,9,7,7);
	SetControlInfo(IDC_CHECK3,ChgL_xStretch,9,7,8);
	SetControlInfo(IDC_CHECK25,ChgL_xStretch,9,8,9);
	SetControlInfo(IDC_STATIC_1,ChgL_Fixx,9,9,9);
	SetControlInfo(IDC_STATIC_2,ChgL_Fixx,9,9,9);
	SetControlInfo(IDC_EDIT116,ChgL_xStretch,9,2,3);
	SetControlInfo(IDC_BUTTON125,ChgL_xStretch,9,2,3);
	SetControlInfo(IDC_EDIT117,ChgL_xStretch,9,4,5);
	SetControlInfo(IDC_BUTTON126,ChgL_xStretch,9,4,5);
	SetControlInfo(IDC_STATIC_42,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_43,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_48,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_49,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_50,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_51,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_52,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_27,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_EDIT66,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_BUTTON118,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_BUTTON119,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_LOCK_TX_LO3,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_40,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_41,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_EDIT118,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_BUTTON127,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_EDIT60,ChgL_xStretch,9,6,7);
	SetControlInfo(IDC_BUTTON123,ChgL_xStretch,9,6,7);
	SetControlInfo(IDC_STATIC_12,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_EDIT2,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_BUTTON21,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_BUTTON37,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_LOCK_TX_LO2,ChgL_Fixx,9,6,6);
	SetControlInfo(IDC_STATIC_38,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_STATIC_37,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_EDIT115,ChgL_xStretch,9,8,9);
	SetControlInfo(IDC_BUTTON124,ChgL_xStretch,9,8,9);
	SetControlInfo(IDC_STATIC_13,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_EDIT1,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_BUTTON38,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_STATIC_LOCK_TX_LO1,ChgL_Fixx,9,8,8);
	SetControlInfo(IDC_COMBO64,FixL_xStretch,9,0,1);
	SetControlInfo(IDC_BUTTON120,FixL_xStretch,9,0,1);
	SetControlInfo(IDC_STATIC_22,ChgT_Fixx,9,0,0,3,1,2);
	SetControlInfo(IDC_STATIC_114,ChgT_Fixx,9,0,0,3,1,2);
	SetControlInfo(IDC_STATIC_45,ChgT_Fixx,9,0,0,3,1,2);
	SetControlInfo(IDC_COMBO38,ChgT_xStretch,9,0,1,3,1,2);
	SetControlInfo(IDC_BUTTON56,ChgT_xStretch,9,0,1,3,1,2);
	SetControlInfo(IDC_CHECK20,ChgLT_Fixx,9,1,1,3,1,2);
	SetControlInfo(IDC_CHECK26,ChgLT_Fixx,9,1,1,3,1,2);
	SetControlInfo(IDC_EDIT41,ChgLT_xStretch,9,1,2,3,1,2);
	SetControlInfo(IDC_EDIT77,ChgLT_xStretch,9,1,2,3,1,2);
	SetControlInfo(IDC_BUTTON66,ChgLT_Fixx,9,2,2,3,1,2);
	SetControlInfo(IDC_BUTTON67,ChgLT_Fixx,9,2,2,3,1,2);
	SetControlInfo(IDC_BUTTON61,ChgLT_xStretch,9,2,3,3,1,2);
	SetControlInfo(IDC_BUTTON8,ChgLT_xStretch,9,3,4,3,1,2);
	SetControlInfo(IDC_BUTTON33,ChgLT_xStretch,9,4,5,3,1,2);
	SetControlInfo(IDC_BUTTON101,ChgLT_xStretch,9,4,5,3,1,2);
	SetControlInfo(IDC_STATIC_101,ChgT_Fixx,9,0,0,3,2,3);
	SetControlInfo(IDC_STATIC_201,ChgT_Fixx,9,0,0,3,2,3);
	SetControlInfo(IDC_COMBO13,ChgT_xStretch,9,0,1,3,2,3);
	SetControlInfo(IDC_BUTTON23,ChgT_xStretch,9,0,1,3,2,3);
	SetControlInfo(IDC_STATIC_102,ChgLT_Fixx,9,1,1,3,2,3);
	SetControlInfo(IDC_STATIC_200,ChgLT_Fixx,9,1,1,3,2,3);
	SetControlInfo(IDC_COMBO14,ChgLT_xStretch,9,1,2,3,2,3);
	SetControlInfo(IDC_BUTTON24,ChgLT_xStretch,9,1,2,3,2,3);
	SetControlInfo(IDC_STATIC_103,ChgLT_Fixx,9,2,2,3,2,3);
	SetControlInfo(IDC_STATIC_116,ChgLT_Fixx,9,2,2,3,2,3);
	SetControlInfo(IDC_COMBO15,ChgLT_xStretch,9,2,3,3,2,3);
	SetControlInfo(IDC_BUTTON25,ChgLT_xStretch,9,2,3,3,2,3);
	SetControlInfo(IDC_STATIC_104,ChgLT_Fixx,9,3,3,3,2,3);
	SetControlInfo(IDC_STATIC_117,ChgLT_Fixx,9,3,3,3,2,3);
	SetControlInfo(IDC_COMBO16,ChgLT_xStretch,9,3,4,3,2,3);
	SetControlInfo(IDC_BUTTON26,ChgLT_xStretch,9,3,4,3,2,3);
	SetControlInfo(IDC_STATIC_109,ChgLT_Fixx,9,4,4,3,2,3);
	SetControlInfo(IDC_STATIC_124,ChgLT_Fixx,9,4,4,3,2,3);
	SetControlInfo(IDC_COMBO32,ChgLT_xStretch,9,4,5,3,2,3);
	SetControlInfo(IDC_BUTTON31,ChgLT_xStretch,9,4,5,3,2,3);
	SetControlInfo(IDC_STATIC_105,ChgLT_Fixx,9,5,5,3,2,3);
	SetControlInfo(IDC_STATIC_118,ChgLT_Fixx,9,5,5,3,2,3);
	SetControlInfo(IDC_COMBO11,ChgLT_xStretch,9,5,6,3,2,3);
	SetControlInfo(IDC_BUTTON27,ChgLT_xStretch,9,5,6,3,2,3);
	SetControlInfo(IDC_STATIC_106,ChgLT_Fixx,9,6,6,3,2,3);
	SetControlInfo(IDC_STATIC_119,ChgLT_Fixx,9,6,6,3,2,3);
	SetControlInfo(IDC_COMBO17,ChgLT_xStretch,9,6,7,3,2,3);
	SetControlInfo(IDC_BUTTON28,ChgLT_xStretch,9,6,7,3,2,3);
	SetControlInfo(IDC_STATIC_112,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_STATIC_203,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_STATIC_123,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_COMBO19,ChgLT_xStretch,9,7,8,3,2,3);
	SetControlInfo(IDC_BUTTON32,ChgLT_xStretch,9,7,8,3,2,3);
	SetControlInfo(IDC_STATIC_110,ChgLT_Fixx,9,8,8,3,2,3);
	SetControlInfo(IDC_STATIC_121,ChgLT_Fixx,9,8,8,3,2,3);
	SetControlInfo(IDC_COMBO18,ChgLT_xStretch,9,8,9,3,2,3);
	SetControlInfo(IDC_BUTTON30,ChgLT_xStretch,9,8,9,3,2,3);
	SetControlInfo(IDC_STATIC_111,ChgLT_Fixx,9,9,9,3,2,3);
	SetControlInfo(IDC_BUTTON128,ChgLT_xStretch,9,1,2,3,2,3);
	SetControlInfo(IDC_EDIT119,ChgLT_xStretch,9,1,2,3,2,3);
	SetControlInfo(IDC_BUTTON41,ChgLT_xStretch,9,4,5,3,2,3);
	SetControlInfo(IDC_EDIT114,ChgLT_xStretch,9,4,5,3,2,3);
	SetControlInfo(IDC_BUTTON122,ChgLT_xStretch,9,5,6,3,2,3);
	SetControlInfo(IDC_EDIT31,ChgLT_xStretch,9,5,6,3,2,3);
	SetControlInfo(IDC_BUTTON129,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_EDIT120,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_BUTTON121,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_EDIT113,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_STATIC_115,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_STATIC_125,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_BUTTON40,ChgLT_xStretch,9,7,8,3,2,3);
	SetControlInfo(IDC_EDIT12,ChgLT_xStretch,9,7,8,3,2,3);
	SetControlInfo(IDC_STATIC_126,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_STATIC_127,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_BUTTON130,ChgLT_xStretch,9,8,9,3,2,3);
	SetControlInfo(IDC_EDIT121,ChgLT_xStretch,9,8,9,3,2,3);
	SetControlInfo(IDC_STATIC_120,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_EDIT4,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_BUTTON29,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_BUTTON39,ChgLT_Fixx,9,7,7,3,2,3);
	SetControlInfo(IDC_STATIC_LOCK_RX_LO1,ChgLT_Fixx,9,7,7,3,2,3);
	return TRUE;
}

void CRFR1CDDlg::OnBnClickedDebugTxLO1()
{
	g_pHMC1197Dlg->m_pSP1401 = m_pSP1401;
	g_pHMC1197Dlg->ShowWindow(SW_NORMAL);
	g_pHMC1197Dlg->SetFocus();
}

void CRFR1CDDlg::OnBnClickedDebugTxLO2RxLO2()
{
	g_pADF5355Dlg->m_iLOIdx = 1;
	g_pADF5355Dlg->m_pSP1401R1C = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1A = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1CDDlg::OnBnClickedDebugTxLO3()
{
	g_pADF5355Dlg->m_iLOIdx = 2;
	g_pADF5355Dlg->m_pSP1401R1C = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1A = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1CDDlg::OnBnClickedDebugRxLO1()
{
	g_pADF5355Dlg->m_iLOIdx = 3;
	g_pADF5355Dlg->m_pSP1401R1C = m_pSP1401;
	g_pADF5355Dlg->m_pSP1401R1A = NULL;
	g_pADF5355Dlg->ShowWindow(SW_NORMAL);
	g_pADF5355Dlg->SetFocus();
}

void CRFR1CDDlg::OnBnClickedSetTxLO1()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	uint64_t uiFreqLO3 = 0;
	CSP1401R1C::TxFilter Filter = (CSP1401R1C::TxFilter)(m_TxFilter1Sw.GetCurSel());

	str2freq(m_strTxLO1,uiFreqLO1);
	str2freq(m_strTxLO2RxLO2,uiFreqLO2);
	str2freq(m_strTxLO3,uiFreqLO3);
	m_pSP1401->TxLO2Freq(uiFreqLO1,uiFreqLO2,uiFreqLO3,uiFreq);
	freq2str(uiFreq,m_strTxFreq);
	STACHKV(m_pSP1401->SetHMC1197(uiFreqLO1));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1C::Tx_LO_1,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO3.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_uiLO3 = uiFreqLO3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetRFPower()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetPowEn(m_bPowerOnOff));
	m_Pow.SetWindowText(m_bPowerOnOff ? "Pow On" : "Pow Off");
}

void CRFR1CDDlg::OnBnClickedSetTxMod()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetTxModulatorEn(m_bTxMod));
	m_TxMOD.SetWindowText(m_bTxMod ? "Mod En" : "Mod Dis");
}

void CRFR1CDDlg::OnBnClickedSetTxAtt0()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt0.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt0(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt0 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT0,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetTxFilterSw()
{
	UpdateData(TRUE);
	CSP1401R1C::TxFilter Filter1 = (0 == m_TxFilter1Sw.GetCurSel() ? CSP1401R1C::LPF : CSP1401R1C::BPF);
	CSP1401R1C::TxFilter Filter2 = (0 == m_TxFilter2Sw.GetCurSel() ? CSP1401R1C::LPF : CSP1401R1C::BPF);
	STACHKV(m_pSP1401->SetTxFilterSw(Filter1,Filter2));
}

void CRFR1CDDlg::OnBnClickedSetTxAtt1()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt1.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt1(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt1 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT1,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetTxAtt2()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt2.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt2(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt2 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT2,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetTxAtt3()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt3.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt3(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt3 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT3,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetTxIOSw1()
{
	UpdateData(TRUE);
	CSP1401R1C::TxIOSw1 Mode = (CSP1401R1C::TxIOSw1)m_TxIOSw1.GetCurSel();
	STACHKV(m_pSP1401->SetTxIOSw1(Mode));
}

void CRFR1CDDlg::OnBnClickedSetTxIOSw2()
{
	UpdateData(TRUE);
	CSP1401R1C::TxIOSw2 Mode = (CSP1401R1C::TxIOSw2)m_TxIOSw2.GetCurSel();
	STACHKV(m_pSP1401->SetTxIOSw2(Mode));
}

void CRFR1CDDlg::OnBnClickedSetLoopSw()
{
	UpdateData(TRUE);
	CSP1401R1C::LoopSw Mode = (CSP1401R1C::LoopSw)m_LoopSw.GetCurSel();
	STACHKV(m_pSP1401->SetLoopSw(Mode));
}

void CRFR1CDDlg::OnBnClickedSetIOMode()
{
	UpdateData(TRUE);
	IOMode Mode = (IOMode)m_IOMode.GetCurSel();
	STACHKV(m_pSP1401->SetIOMode(Mode));

	CSP1401R1C::TxIOSw1 TxSw1 = CSP1401R1C::Tx_On_1;
	CSP1401R1C::TxIOSw2 TxSw2 = CSP1401R1C::Tx_On_2;
	CSP1401R1C::RxIOSw1 RxSw1 = CSP1401R1C::Rx_IO_On_1;
	CSP1401R1C::RxIOSw2 RxSw2 = CSP1401R1C::Rx_IO_On_2;
	CSP1401R1C::RxIOSw3 RxSw3 = CSP1401R1C::Rx_IO_On_3;
	CSP1401R1C::LoopSw LpSw = CSP1401R1C::Loop_Off; 
	bool bTxLED = true,bRxLED = true;
	m_pSP1401->IOMode2IOSw(Mode,TxSw1,TxSw2,RxSw1,RxSw2,RxSw3,LpSw,bTxLED,bRxLED);
	m_TxIOSw1.SetCurSel((int)TxSw1);
	m_TxIOSw2.SetCurSel((int)TxSw2);
	m_RxIOSw1.SetCurSel((int)RxSw1);
	m_RxIOSw2.SetCurSel((int)RxSw2);
	m_RxIOSw3.SetCurSel((int)RxSw3);
	m_LoopSw.SetCurSel((int)LpSw);
	m_bTxLED = bTxLED;
	m_bRxLED = bRxLED;
	UpdateData(FALSE);

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_Mode = Mode;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_IOMODE,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedSetRxIOSw1()
{
	UpdateData(TRUE);
	CSP1401R1C::RxIOSw1 Mode = (CSP1401R1C::RxIOSw1)m_RxIOSw1.GetCurSel();
	STACHKV(m_pSP1401->SetRxIOSw1(Mode));
}

void CRFR1CDDlg::OnBnClickedSetRxIOSw2()
{
	UpdateData(TRUE);
	CSP1401R1C::RxIOSw2 Mode = (CSP1401R1C::RxIOSw2)m_RxIOSw2.GetCurSel();
	STACHKV(m_pSP1401->SetRxIOSw2(Mode));
}

void CRFR1CDDlg::OnBnClickedSetRxIOSw3()
{
	UpdateData(TRUE);
	CSP1401R1C::RxIOSw3 Mode = (CSP1401R1C::RxIOSw3)m_RxIOSw3.GetCurSel();
	STACHKV(m_pSP1401->SetRxIOSw3(Mode));
}

void CRFR1CDDlg::OnBnClickedSetRxLNAAttSw()
{
	UpdateData(TRUE);
	SP1401::RxLNAAtt LNAAtt = (SP1401::RxLNAAtt)m_RxLNAAttSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxLNAAttSw(LNAAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_LNAAtt = LNAAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedSetRxAtt019Sw()
{
	UpdateData(TRUE);
	SP1401::RxAtt019 Att = (SP1401::RxAtt019)m_RxAtt019Sw.GetCurSel();
	STACHKV(m_pSP1401->SetRxAtt019Sw(Att));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_Att019 = Att;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT019,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedSetRxAtt1()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt1.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt1(dAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dAtt1 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT1,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedSetRxAtt2()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt2.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt2(dAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dAtt2 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT2,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedRxIFFilterSw()
{
	UpdateData(TRUE);
	CSP1401R1C::RxIFFilter Filter = (CSP1401R1C::RxIFFilter)m_RxIFFilterSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxIFFilterSw(Filter));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_IFFilter = Filter;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedSetRxAtt3()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt3.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt3(dAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dAtt3 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT3,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp0()
{
	STACHKV(m_pSP1401->GetTemp0RxLO2(&m_dTemp0));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp1()
{
	STACHKV(m_pSP1401->GetTemp1RxLNA(&m_dTemp1));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp2()
{
	STACHKV(m_pSP1401->GetTemp2RxLO1(&m_dTemp2));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp3()
{
	STACHKV(m_pSP1401->GetTemp3RxPA1(&m_dTemp3));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp4()
{
	STACHKV(m_pSP1401->GetTemp4TxLO2(&m_dTemp4));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp5()
{
	STACHKV(m_pSP1401->GetTemp5TxLO1(&m_dTemp5));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp6()
{
	STACHKV(m_pSP1401->GetTemp6TxPA4(&m_dTemp6));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedGetTemp7()
{
	STACHKV(m_pSP1401->GetTemp7TxPA3(&m_dTemp7));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedGetDetRxComb()
{
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680RxComb(uiDet));
	m_uiDetRxComb = (UINT)uiDet;
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_uiDetComb = m_uiDetRxComb;
	RxState.m_uiDetIF1 = m_uiDetRxIF1;
	RxState.m_uiDetIF2 = m_uiDetRxIF2;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetDetRxIF1()
{
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680RxIF1(uiDet));
	m_uiDetRxIF1 = (UINT)uiDet;
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_uiDetComb = m_uiDetRxComb;
	RxState.m_uiDetIF1 = m_uiDetRxIF1;
	RxState.m_uiDetIF2 = m_uiDetRxIF2;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetDetRxIF2()
{
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680RxIF2(uiDet));
	m_uiDetRxIF2 = (UINT)uiDet;
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_uiDetComb = m_uiDetRxComb;
	RxState.m_uiDetIF1 = m_uiDetRxIF1;
	RxState.m_uiDetIF2 = m_uiDetRxIF2;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedGetDetTxIF2()
{
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680TxIF2(uiDet));
	m_uiDetTxIF2 = (UINT)uiDet;
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_uiDetIF2 = m_uiDetTxIF2;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXDET,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetTxLO2RxLO2()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	uint64_t uiFreqLO3 = 0;
	CSP1401R1C::TxFilter Filter = (CSP1401R1C::TxFilter)(m_TxFilter1Sw.GetCurSel());

	str2freq(m_strTxLO1,uiFreqLO1);
	str2freq(m_strTxLO2RxLO2,uiFreqLO2);
	str2freq(m_strTxLO3,uiFreqLO3);
	m_pSP1401->TxLO2Freq(uiFreqLO1,uiFreqLO2,uiFreqLO3,uiFreq);
	freq2str(uiFreq,m_strTxFreq);
	STACHKV(m_pSP1401->SetADF5355(CSP1401R1C::Tx_LO_2,uiFreqLO2));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1C::Tx_LO_2,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO2RxLO2.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_uiLO3 = uiFreqLO3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetTxLO3()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	uint64_t uiFreqLO3 = 0;
	CSP1401R1C::TxFilter Filter = (CSP1401R1C::TxFilter)(m_TxFilter1Sw.GetCurSel());

	str2freq(m_strTxLO1,uiFreqLO1);
	str2freq(m_strTxLO2RxLO2,uiFreqLO2);
	str2freq(m_strTxLO3,uiFreqLO3);
	m_pSP1401->TxLO2Freq(uiFreqLO1,uiFreqLO2,uiFreqLO3,uiFreq);
	freq2str(uiFreq,m_strTxFreq);
	STACHKV(m_pSP1401->SetADF5355(CSP1401R1C::Tx_LO_3,uiFreqLO3));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1C::Tx_LO_3,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockTxLO3.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_uiLO3 = uiFreqLO3;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetRxLO1()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;

	str2freq(m_strRxLO1,uiFreqLO1);
	str2freq(m_strTxLO2RxLO2,uiFreqLO2);
	m_pSP1401->RxLO2Freq(uiFreqLO1,uiFreqLO2,uiFreq);
	freq2str(uiFreq,m_strRxFreq);
	STACHKV(m_pSP1401->SetADF5355(CSP1401R1C::Rx_LO_1,uiFreqLO1));
	UpdateData(FALSE);

	CBitmap Bitmap;
	bool bLock = false;
	STACHKV(m_pSP1401->DetLO(CSP1401R1C::Rx_LO_1,bLock));
	Bitmap.LoadBitmap(bLock ? IDB_BITMAP_OK : IDB_BITMAP_ERROR);
	m_LockRxLO1.SetBitmap((HBITMAP)Bitmap.Detach());

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_uiLO1 = uiFreqLO1;
	RxState.m_uiLO2 = uiFreqLO2;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}

void CRFR1CDDlg::OnBnClickedDebugMCP23S17()
{	
	m_pMCP23S17Dlg->m_pSP1401 = m_pSP1401;
	m_pMCP23S17Dlg->ShowWindow(SW_NORMAL);
	m_pMCP23S17Dlg->SetFocus();
}

void CRFR1CDDlg::OnBnClickedDebugEEPROM()
{
	m_pEEPROMDlg->m_pSP1401 = dynamic_cast<ISP1401 *>(m_pSP1401);
	m_pEEPROMDlg->ShowWindow(TRUE);
	m_pEEPROMDlg->SetFocus();
}

void CRFR1CDDlg::OnBnClickedSetTxLED()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetLED(m_bTxLED,m_bRxLED));
}

void CRFR1CDDlg::OnBnClickedSetRxLED()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetLED(m_bTxLED,m_bRxLED));
}

void CRFR1CDDlg::OnBnClickedGetAllTemp()
{
	OnBnClickedGetTemp0();
	OnBnClickedGetTemp1();
	OnBnClickedGetTemp2();
	OnBnClickedGetTemp3();
	OnBnClickedGetTemp4();
	OnBnClickedGetTemp5();
	OnBnClickedGetTemp6();
	OnBnClickedGetTemp7();
}

void CRFR1CDDlg::OnBnClickedGetAllDet()
{
	OnBnClickedGetDetRxComb();
	OnBnClickedGetDetRxIF1();
	OnBnClickedGetDetRxIF2();
	OnBnClickedGetDetTxIF2();
}

LRESULT CRFR1CDDlg::SyncWithRFDlg(WPARAM wParam,LPARAM lParam)
{
	UINT uiMsg = GetCurrentMessage()->message;
	CSP1401R1C::TxChainStateAll TxState = *(CSP1401R1C::TxChainStateAll *)lParam;
	CSP1401R1C::RxChainStateAll RxState = *(CSP1401R1C::RxChainStateAll *)lParam;

	if (WM_CSE_SYNCRFANDRFD_DLG_TXFREQ == uiMsg) {
		uint64_t uiFreq = 0;
		m_pSP1401->TxLO2Freq(TxState.m_uiLO1,TxState.m_uiLO2,TxState.m_uiLO3,uiFreq);
		freq2str(uiFreq,m_strTxFreq);
		freq2str(TxState.m_uiLO1,m_strTxLO1);
		freq2str(TxState.m_uiLO2,m_strTxLO2RxLO2);
		freq2str(TxState.m_uiLO3,m_strTxLO3);
		m_TxFilter1Sw.SetCurSel((int)TxState.m_Filter);
		m_TxFilter2Sw.SetCurSel((int)TxState.m_Filter);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT0 == uiMsg) {
		m_TxAtt0.SetCurSel((int)(TxState.m_dAtt0 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT1 == uiMsg) {
		m_TxAtt1.SetCurSel((int)(TxState.m_dAtt1 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT2 == uiMsg) {
		m_TxAtt2.SetCurSel((int)(TxState.m_dAtt2 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXATT3 == uiMsg) {
		m_TxAtt3.SetCurSel((int)(TxState.m_dAtt3 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXDET == uiMsg) {
		m_uiDetTxIF2 = TxState.m_uiDetIF2;
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_TXTEMP == uiMsg) {
		m_dTemp4 = TxState.m_dTemp4;
		m_dTemp5 = TxState.m_dTemp5;
		m_dTemp6 = TxState.m_dTemp6;
		m_dTemp7 = TxState.m_dTemp7;
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXFREQ == uiMsg) {
		uint64_t uiFreq = 0;
		m_pSP1401->RxLO2Freq(RxState.m_uiLO1,RxState.m_uiLO2,uiFreq);
		freq2str(uiFreq,m_strRxFreq);
		freq2str(RxState.m_uiLO1,m_strRxLO1);
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT == uiMsg) {
		m_RxLNAAttSw.SetCurSel((int)RxState.m_LNAAtt);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXATT019 == uiMsg) {
		m_RxAtt019Sw.SetCurSel((int)(RxState.m_Att019));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXATT1 == uiMsg) {
		m_RxAtt1.SetCurSel((int)(RxState.m_dAtt1 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXATT2 == uiMsg) {
		m_RxAtt2.SetCurSel((int)(RxState.m_dAtt2 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXATT3 == uiMsg) {
		m_RxAtt3.SetCurSel((int)(RxState.m_dAtt3 / 0.5));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER == uiMsg) {
		m_RxIFFilterSw.SetCurSel((int)(RxState.m_IFFilter));
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXDET == uiMsg) {
		m_uiDetRxComb = RxState.m_uiDetComb;
		m_uiDetRxIF1 = RxState.m_uiDetIF1;
		m_uiDetRxIF2 = RxState.m_uiDetIF2;
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXTEMP == uiMsg) {
		m_dTemp0 = RxState.m_dTemp0;
		m_dTemp1 = RxState.m_dTemp1;
		m_dTemp2 = RxState.m_dTemp2;
		m_dTemp3 = RxState.m_dTemp3;
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_IOMODE == uiMsg) {
		CSP1401R1C::TxIOSw1 TxSw1 = CSP1401R1C::Tx_On_1;
		CSP1401R1C::TxIOSw2 TxSw2 = CSP1401R1C::Tx_On_2;
		CSP1401R1C::RxIOSw1 RxSw1 = CSP1401R1C::Rx_IO_On_1;
		CSP1401R1C::RxIOSw2 RxSw2 = CSP1401R1C::Rx_On_2;
		CSP1401R1C::RxIOSw3 RxSw3 = CSP1401R1C::Rx_On_3;
		CSP1401R1C::LoopSw	LpSw  = CSP1401R1C::Loop_Off;
		bool bTxLED = true,bRxLED = true;

		m_IOMode.SetCurSel((int)(RxState.m_Mode));
		m_pSP1401->IOMode2IOSw(RxState.m_Mode,TxSw1,TxSw2,RxSw1,RxSw2,RxSw3,LpSw,bTxLED,bRxLED);
		m_TxIOSw1.SetCurSel((int)TxSw1);
		m_TxIOSw2.SetCurSel((int)TxSw2);
		m_RxIOSw1.SetCurSel((int)RxSw1);
		m_RxIOSw2.SetCurSel((int)RxSw2);
		m_RxIOSw3.SetCurSel((int)RxSw3);
		m_LoopSw.SetCurSel((int)LpSw);
		m_bTxLED = bTxLED;
		m_bRxLED = bRxLED;
		UpdateData(FALSE);
		return 0;
	}
	return 0;
}

void CRFR1CDDlg::OnBnClickedSetTxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;
	uint64_t uiFreqLO3 = 0;
	CSP1401R1C::TxFilter Filter = CSP1401R1C::LPF;

	str2freq(m_strTxFreq,uiFreq);
	m_pSP1401->TxFreq2LO(uiFreq,uiFreqLO1,uiFreqLO2,uiFreqLO3,Filter);
	STACHKV(m_pSP1401->SetTxFreq(uiFreq));

	freq2str(uiFreqLO1,m_strTxLO1);
	freq2str(uiFreqLO2,m_strTxLO2RxLO2);
	freq2str(uiFreqLO3,m_strTxLO3);
	m_TxFilter1Sw.SetCurSel((int)Filter);
	m_TxFilter2Sw.SetCurSel((int)Filter);
	UpdateData(FALSE);

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_uiLO1 = uiFreqLO1;
	TxState.m_uiLO2 = uiFreqLO2;
	TxState.m_uiLO3 = uiFreqLO3;
	TxState.m_Filter = Filter;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1CDDlg::OnBnClickedSetRxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq    = 0;
	uint64_t uiFreqLO1 = 0;
	uint64_t uiFreqLO2 = 0;

	str2freq(m_strRxFreq,uiFreq);
	m_pSP1401->RxFreq2LO(uiFreq,uiFreqLO1,uiFreqLO2);
	STACHKV(m_pSP1401->SetRxFreq(uiFreq));

	freq2str(uiFreqLO1,m_strRxLO1);
	freq2str(uiFreqLO2,m_strTxLO2RxLO2);
	UpdateData(FALSE);

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_uiLO1 = uiFreqLO1;
	RxState.m_uiLO2 = uiFreqLO2;
	::SendMessage(ALL_DLG->m_pRFR1CDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}
