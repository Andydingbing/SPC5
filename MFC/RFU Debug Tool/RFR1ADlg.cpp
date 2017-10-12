#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1ADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRFR1ADlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CRFR1ADlg)

CRFR1ADlg::CRFR1ADlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CRFR1ADlg::IDD, pParent)
	, m_strSN(_T(""))
	, m_strTxFreq(_T("3.8G"))
	, m_strTxDet(_T("0"))
	, m_strTxTemp(_T("0"))
	, m_strRxFreq(_T("2.4G"))
	, m_strRxDet(_T("0"))
	, m_strRxTemp(_T("0"))
	, m_strTxFreqStep(_T("10M"))
	, m_strRxFreqStep(_T("10M"))
	, m_strSNRfVer(_T("SP1401R1B"))
	, m_uiSNOrdinal(00000000)
	, m_dDGain(0.0)
{
}

CRFR1ADlg::~CRFR1ADlg()
{
}

void CRFR1ADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO7, m_TxAtt1);
	DDX_Control(pDX, IDC_COMBO8, m_TxAtt2);
	DDX_Control(pDX, IDC_COMBO10, m_TxAtt3);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO5, m_RxAtt1);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO6, m_RxAtt2);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO8, m_RxIFFilterSw);
	DDX_Control(pDX, IDC_COMBO9, m_TxPAAttSw);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO, m_RxLNAAttSw);
	DDX_Text(pDX, IDC_EDIT26, m_strTxFreq);
	DDX_Text(pDX, IDC_EDIT21, m_strTxDet);
	DDX_Text(pDX, IDC_EDIT76, m_strSN);
	DDX_Text(pDX, IDC_EDIT27, m_strTxTemp);
	DDX_Text(pDX, IDC_EDIT73, m_strRxFreq);
	DDX_Text(pDX, IDC_EDIT23, m_strRxDet);
	DDX_Text(pDX, IDC_EDIT28, m_strRxTemp);
	DDX_Text(pDX, IDC_EDIT67, m_strTxFreqStep);
	DDX_Text(pDX, IDC_EDIT69, m_strRxFreqStep);
	DDX_Control(pDX, IDC_COMBO25, m_IOMode);
	DDX_Text(pDX, IDC_EDIT1, m_strSNRfVer);
	DDX_Text(pDX, IDC_EDIT2, m_uiSNOrdinal);
	DDX_Text(pDX, IDC_EDIT3, m_dDGain);
}


BEGIN_MESSAGE_MAP(CRFR1ADlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON10, &CRFR1ADlg::OnBnClickedSetTxAtt1)
	ON_BN_CLICKED(IDC_BUTTON11, &CRFR1ADlg::OnBnClickedSetTxAtt2)
	ON_BN_CLICKED(IDC_BUTTON13, &CRFR1ADlg::OnBnClickedSetTxAtt3)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON5, &CRFR1ADlg::OnBnClickedSetRxAtt1)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON6, &CRFR1ADlg::OnBnClickedSetRxAtt2)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON8, &CRFR1ADlg::OnBnClickedSetRxIFFilterSw)
	ON_BN_CLICKED(IDC_BUTTON12, &CRFR1ADlg::OnBnClickedSetTxPAAttSw)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON, &CRFR1ADlg::OnBnClickedSetRxLNAAttSw)
	ON_BN_CLICKED(IDC_BUTTON17, &CRFR1ADlg::OnBnClickedSetTxFreq)
	ON_BN_CLICKED(IDC_BUTTON15, &CRFR1ADlg::OnBnClickedGetTxDet)
	ON_BN_CLICKED(IDC_BUTTON58, &CRFR1ADlg::OnBnClickedSetRxFreq)
	ON_BN_CLICKED(IDC_BUTTON18, &CRFR1ADlg::OnBnClickedGetRxDet)
	ON_BN_CLICKED(IDC_BUTTON19, &CRFR1ADlg::OnBnClickedGetTxTemp)
	ON_BN_CLICKED(IDC_BUTTON20, &CRFR1ADlg::OnBnClickedGetRxTemp)
	ON_BN_CLICKED(IDC_BUTTON64, &CRFR1ADlg::OnBnClickedWriteSN)
	ON_BN_CLICKED(IDC_BUTTON65, &CRFR1ADlg::OnBnClickedReadSN)
	ON_BN_CLICKED(IDC_BUTTON34, &CRFR1ADlg::OnBnClickedSetTxAll)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON9, &CRFR1ADlg::OnBnClickedSetRxAll)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CRFR1ADlg::OnDeltaposSpinChangeTxFreq)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CRFR1ADlg::OnDeltaposSpinChangeRxFreq)
	ON_BN_CLICKED(IDC_BUTTON30, &CRFR1ADlg::OnBnClickedSetIOMode)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXFREQ, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT1, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT2, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXPAATT, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT3, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXDET, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXTEMP, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXFREQ, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT1, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT2, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXDET, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXTEMP, &CRFR1ADlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_IOMODE, &CRFR1ADlg::SyncWithRFDDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFR1ADlg::OnBnClickedSyncTxGain)
END_MESSAGE_MAP()


void CRFR1ADlg::OnBnClickedSetTxAtt1()
{
	UpdateData(TRUE);
	int32_t iAtt = m_TxAtt1.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetTxAtt1(iAtt));

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt1 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT1,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedSetTxAtt2()
{
	UpdateData(TRUE);
	int32_t iAtt = m_TxAtt2.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetTxAtt2(iAtt));

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt2 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT2,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedSetTxAtt3()
{
	UpdateData(TRUE);
	int32_t iAtt = m_TxAtt3.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetTxAtt3(iAtt));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt3 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT3,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedSetRxAtt1()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt1.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt1(dAtt));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_dAtt1 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT1,0,(LPARAM)&RxState);
}

void CRFR1ADlg::OnBnClickedSetRxAtt2()
{
	UpdateData(TRUE);
	int32_t iAtt = m_RxAtt2.GetCurSel() * 10;
	STACHKV(m_pSP1401->SetRxAtt2(iAtt));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_iAtt2 = iAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT2,0,(LPARAM)&RxState);
}

void CRFR1ADlg::OnBnClickedSetRxIFFilterSw()
{
	UpdateData(TRUE);
	CSP1401R1A::RxIFFilter Filter = (CSP1401R1A::RxIFFilter)m_RxIFFilterSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxIFFilterSw(Filter));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_IFFilter = Filter;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER,0,(LPARAM)&RxState);
}

void CRFR1ADlg::OnBnClickedSetTxPAAttSw()
{
	UpdateData(TRUE);
	SP1401::TxPAAtt PAAtt = (SP1401::TxPAAtt)m_TxPAAttSw.GetCurSel();
	STACHKV(m_pSP1401->SetTxPAAttSw(PAAtt));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_PAAtt = PAAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXPAATT,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedSetRxLNAAttSw()
{
	UpdateData(TRUE);
	SP1401::RxLNAAtt LNAAtt = (SP1401::RxLNAAtt)m_RxLNAAttSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxLNAAttSw(LNAAtt));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_LNAAtt = LNAAtt;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT,0,(LPARAM)&RxState);
}

BOOL CRFR1ADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TxPAAttSw.SetCurSel(1);
	m_TxAtt1.SetCurSel(1);
	m_TxAtt2.SetCurSel(2);
	m_TxAtt3.SetCurSel(2);
	m_RxAtt1.SetCurSel(40);
	m_RxAtt2.SetCurSel(2);
	m_RxLNAAttSw.SetCurSel(0);
	m_RxIFFilterSw.SetCurSel(0);
	m_IOMode.SetCurSel(1);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT26,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_SPIN1,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_BUTTON17,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT67,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_COMBO7,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON10,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_COMBO8,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON11,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_COMBO9,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON12,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_COMBO10,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON13,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT21,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON15,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT27,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON19,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_COMBO25,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_BUTTON30,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_BUTTON34,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_TX,FixL_xStretch,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXFREQ,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXLNAATT,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXATT1,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXATT2,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXIFFILTER,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXDET,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RXTEMP,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT73,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_SPIN2,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON58,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_EDIT69,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO5,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON5,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO6,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON6,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO8,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON8,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_EDIT23,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_BUTTON18,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_EDIT28,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_BUTTON20,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON9,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_STATIC_RF_RX,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT3,FixL_xStretch,2,0,1);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RFVER,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_STATIC_RF_RFSN,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT1,ChgL_Fixx,2,1,1);
	SetControlInfo(IDC_EDIT2,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_EDIT76,ChgL_xStretch,2,1,2);
	SetControlInfo(IDC_BUTTON64,ChgL_Fixx,2,2,2);
	SetControlInfo(IDC_BUTTON65,ChgL_Fixx,2,2,2);
	return TRUE;
}

void CRFR1ADlg::OnBnClickedSetTxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	str2freq(m_strTxFreq,uiFreq);
	STACHKV(m_pSP1401->SetTxFreq(uiFreq));

	CSP1401R1A::TxChainStateAll TxState;
	m_pSP1401->TxFreq2LO(uiFreq,TxState.m_uiLO1,TxState.m_uiLO2,TxState.m_Band);
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedGetTxDet()
{
	UpdateData(TRUE);
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->SetDetSw(CSP1401R1A::Tx));
	STACHKV(m_pSP1401->GetAD7680(uiDet));
	m_strTxDet.Format("%d",uiDet);
	UpdateData(FALSE);

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_uiDet = uiDet;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXDET,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedSetRxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq;
	str2freq(m_strRxFreq,uiFreq);
	STACHKV(m_pSP1401->SetRxFreq(uiFreq));

	CSP1401R1A::RxChainStateAll RxState;
	m_pSP1401->RxFreq2LO(uiFreq,RxState.m_uiLO1,RxState.m_uiLO2);
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}

void CRFR1ADlg::OnBnClickedGetRxDet()
{
	UpdateData(TRUE);
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->SetDetSw(CSP1401R1A::Rx));
	STACHKV(m_pSP1401->GetAD7680(uiDet));
	m_strRxDet.Format("%d",uiDet);
	UpdateData(FALSE);

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_uiDet = uiDet;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1ADlg::OnBnClickedGetTxTemp()
{
	UpdateData(TRUE);
	double dTemp = 0.0;
	STACHKV(m_pSP1401->GetTxTemp(dTemp));
	m_strTxTemp.Format("%.2f¡æ",dTemp);
	UpdateData(FALSE);

	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_dTemp = dTemp;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1ADlg::OnBnClickedGetRxTemp()
{
	UpdateData(TRUE);
	double dTemp = 0.0;
	STACHKV(m_pSP1401->GetRxTemp(dTemp));
	m_strRxTemp.Format("%.2f¡æ",dTemp);
	UpdateData(FALSE);

	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_dTemp = dTemp;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1ADlg::OnBnClickedWriteSN()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetSN((LPSTR)(LPCTSTR)m_strSNRfVer,(int32_t)m_uiSNOrdinal));
}

void CRFR1ADlg::OnBnClickedReadSN()
{
	UpdateData(TRUE);
	char szSN[32];
	memset(szSN,0,sizeof(szSN));
	STACHKV(m_pSP1401->GetSN(szSN));
	m_strSN.Format("%s",szSN);
	UpdateData(FALSE);
}

void CRFR1ADlg::OnBnClickedSetTxAll()
{
	OnBnClickedSetTxFreq();
	OnBnClickedSetTxAtt1();
	OnBnClickedSetTxAtt2();
	OnBnClickedSetTxPAAttSw();
	OnBnClickedSetTxAtt3();
	OnBnClickedSetIOMode();
	OnBnClickedGetTxDet();
	OnBnClickedGetTxTemp();
}

void CRFR1ADlg::OnBnClickedSetRxAll()
{
	OnBnClickedSetRxFreq();
	OnBnClickedSetRxLNAAttSw();
	OnBnClickedSetRxAtt1();
	OnBnClickedSetRxAtt2();
	OnBnClickedSetRxIFFilterSw();
	OnBnClickedGetRxDet();
	OnBnClickedGetRxTemp();
}

void CRFR1ADlg::OnDeltaposSpinChangeTxFreq(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	uint64_t uiFreq = 0;
	uint64_t uiFreqStep = 0;
	char szNewFreq[16] = {0};
	str2freq(m_strTxFreq,uiFreq);
	str2freq(m_strTxFreqStep,uiFreqStep);

	if (1 == pNMUpDown->iDelta) {		//clicked down
		if ((uiFreq - uiFreqStep) >= RF_TX_FREQ_STAR)
			uiFreq -= uiFreqStep;
	}
	else if (-1 == pNMUpDown->iDelta) {	//clicked up
		if ((uiFreq + uiFreqStep) <= RF_TX_FREQ_STOP)
			uiFreq += uiFreqStep;
	}
	freq2str(uiFreq,szNewFreq);
	m_strTxFreq.Format("%s",szNewFreq);
	UpdateData(FALSE);
	OnBnClickedSetTxFreq();

	*pResult = 0;
}

void CRFR1ADlg::OnDeltaposSpinChangeRxFreq(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	uint64_t uiFreq = 0;
	uint64_t uiFreqStep = 0;
	char szNewFreq[16] = {0};
	str2freq(m_strRxFreq,uiFreq);
	str2freq(m_strRxFreqStep,uiFreqStep);

	if (1 == pNMUpDown->iDelta) {		//clicked down
		if ((uiFreq - uiFreqStep) >= RF_RX_FREQ_STAR)
			uiFreq -= uiFreqStep;
	}
	else if (-1 == pNMUpDown->iDelta) {	//clicked up
		if ((uiFreq + uiFreqStep) <= RF_RX_FREQ_STOP)
			uiFreq += uiFreqStep;
	}
	freq2str(uiFreq,szNewFreq);
	m_strRxFreq.Format("%s",szNewFreq);
	UpdateData(FALSE);
	OnBnClickedSetRxFreq();

	*pResult = 0;
}

void CRFR1ADlg::OnBnClickedSetIOMode()
{
	UpdateData(TRUE);
	IOMode Mode = (IOMode)m_IOMode.GetCurSel();
	STACHKV(m_pSP1401->SetIOMode(Mode));
	
	CSP1401R1A::RxChainStateAll RxState;
	RxState.m_Mode = Mode;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_IOMODE,0,(LPARAM)&RxState);
}

LRESULT CRFR1ADlg::SyncWithRFDDlg(WPARAM wParam,LPARAM lParam)
{
	UINT uiMsg = GetCurrentMessage()->message;
	CSP1401R1A::TxChainStateAll TxState = *(CSP1401R1A::TxChainStateAll *)lParam;
	CSP1401R1A::RxChainStateAll RxState = *(CSP1401R1A::RxChainStateAll *)lParam;

	if (WM_CSE_SYNCRFANDRFD_DLG_TXFREQ == uiMsg) {
		uint64_t uiFreq = 0;
		m_pSP1401->TxLO2Freq(TxState.m_uiLO1,TxState.m_uiLO2,TxState.m_Band,uiFreq);
		freq2str(uiFreq,m_strTxFreq);
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
		m_IOMode.SetCurSel((int)RxState.m_Mode);
		UpdateData(FALSE);
		return 0;
	}
	return 0;
}

void CRFR1ADlg::OnBnClickedSyncTxGain()
{
	UpdateData(TRUE);
	int32_t iAtt1 = m_TxAtt1.GetCurSel() * 10;
	int32_t iAtt2 = m_TxAtt2.GetCurSel() * 10;
	int32_t iAtt3 = m_TxAtt3.GetCurSel() * 10;
	STACHKV(m_pSP1401->SyncSetTxGain(iAtt1,iAtt2,iAtt3,m_dDGain));
	
	CSP1401R1A::TxChainStateAll TxState;
	TxState.m_iAtt1 = iAtt1;
	TxState.m_iAtt2 = iAtt2;
	TxState.m_iAtt3 = iAtt3;
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT1,0,(LPARAM)&TxState);
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT2,0,(LPARAM)&TxState);
	::SendMessage(ALL_DLG->m_pRFR1ADDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT3,0,(LPARAM)&TxState);
}