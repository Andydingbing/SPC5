#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "RFR1CDlg.h"
#include "ChartDateTimeAxis.h"
#include "RFR1CGuardThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRFR1CDlg, CStretchDlg)
IMPLEMENT_CSE_ATTACHTHREAD_DLG(CRFR1CDlg)

CRFR1CDlg::CRFR1CDlg(CWnd* pParent /*=NULL*/)
	: CAttachThreadDlg(CRFR1CDlg::IDD, pParent)
	, m_strTxFreq(_T("3.8G"))
	, m_strTxFreqStep(_T("10M"))
	, m_dDGain(0.0)
	, m_strRxFreq(_T("2.4G"))
	, m_strRxFreqStep(_T("10M"))
	, m_strSNRfVer(_T("SP1401R1C"))
	, m_uiSNOrdinal(00000000)
	, m_strSN(_T(""))
	, m_dTemp0(0.0)
	, m_dTemp1(0.0)
	, m_dTemp2(0.0)
	, m_dTemp3(0.0)
	, m_dTemp4(0.0)
	, m_dTemp5(0.0)
	, m_dTemp6(0.0)
	, m_dTemp7(0.0)
	, m_uiDetTxIF2(0)
	, m_uiDetRxComb(0)
	, m_uiDetRxIF1(0)
	, m_uiDetRxIF2(0)
{
	m_pChart = NULL;
}

CRFR1CDlg::~CRFR1CDlg()
{
	if (m_pChart) {
		delete m_pChart;
		m_pChart = NULL;
	}
}

void CRFR1CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT26, m_strTxFreq);
	DDX_Text(pDX, IDC_EDIT67, m_strTxFreqStep);
	DDX_Control(pDX, IDC_COMBO7, m_TxAtt0);
	DDX_Control(pDX, IDC_COMBO8, m_TxAtt1);
	DDX_Control(pDX, IDC_COMBO9, m_TxAtt2);
	DDX_Control(pDX, IDC_COMBO10, m_TxAtt3);
	DDX_Control(pDX, IDC_COMBO25, m_IOMode);
	DDX_Text(pDX, IDC_EDIT3, m_dDGain);
	DDX_Text(pDX, IDC_EDIT73, m_strRxFreq);
	DDX_Text(pDX, IDC_EDIT69, m_strRxFreqStep);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO, m_RxLNAAttSw);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO5, m_RxAtt019Sw);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO6, m_RxAtt1);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO7, m_RxAtt2);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO8, m_RxIFFilterSw);
	DDX_Control(pDX, IDC_RF_RX_LNAATTSW_COMBO9, m_RxAtt3);
	DDX_Text(pDX, IDC_EDIT1, m_strSNRfVer);
	DDX_Text(pDX, IDC_EDIT2, m_uiSNOrdinal);
	DDX_Text(pDX, IDC_EDIT76, m_strSN);
	DDX_Text(pDX, IDC_EDIT4, m_dTemp0);
	DDX_Text(pDX, IDC_EDIT25, m_dTemp1);
	DDX_Text(pDX, IDC_EDIT122, m_dTemp2);
	DDX_Text(pDX, IDC_EDIT123, m_dTemp3);
	DDX_Text(pDX, IDC_EDIT124, m_dTemp4);
	DDX_Text(pDX, IDC_EDIT125, m_dTemp5);
	DDX_Text(pDX, IDC_EDIT126, m_dTemp6);
	DDX_Text(pDX, IDC_EDIT127, m_dTemp7);
	DDX_Text(pDX, IDC_EDIT21, m_uiDetTxIF2);
	DDX_Text(pDX, IDC_EDIT23, m_uiDetRxComb);
	DDX_Text(pDX, IDC_EDIT50, m_uiDetRxIF1);
	DDX_Text(pDX, IDC_EDIT52, m_uiDetRxIF2);
	DDX_Control(pDX, IDC_COMBO1, m_BlowerAjustMode);
}


BEGIN_MESSAGE_MAP(CRFR1CDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON17, &CRFR1CDlg::OnBnClickedSetTxFreq)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CRFR1CDlg::OnDeltaposSpinChangeTxFreq)
	ON_BN_CLICKED(IDC_BUTTON10, &CRFR1CDlg::OnBnClickedSetTxAtt0)
	ON_BN_CLICKED(IDC_BUTTON11, &CRFR1CDlg::OnBnClickedSetTxAtt1)
	ON_BN_CLICKED(IDC_BUTTON12, &CRFR1CDlg::OnBnClickedSetTxAtt2)
	ON_BN_CLICKED(IDC_BUTTON13, &CRFR1CDlg::OnBnClickedSetTxAtt3)
	ON_BN_CLICKED(IDC_BUTTON30, &CRFR1CDlg::OnBnClickedSetIOMode)
	ON_BN_CLICKED(IDC_BUTTON1, &CRFR1CDlg::OnBnClickedSyncTxGain)
	ON_BN_CLICKED(IDC_BUTTON58, &CRFR1CDlg::OnBnClickedSetRxFreq)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON, &CRFR1CDlg::OnBnClickedSetRxLNAAttSw)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON5, &CRFR1CDlg::OnBnClickedSetRxAtt019Sw)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON6, &CRFR1CDlg::OnBnClickedSetRxAtt1)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON7, &CRFR1CDlg::OnBnClickedSetRxAtt2)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON8, &CRFR1CDlg::OnBnClickedSetRxIFFilterSw)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON10, &CRFR1CDlg::OnBnClickedSetRxAtt3)
	ON_BN_CLICKED(IDC_BUTTON64, &CRFR1CDlg::OnBnClickedWriteSN)
	ON_BN_CLICKED(IDC_BUTTON65, &CRFR1CDlg::OnBnClickedReadSN)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFR1CDlg::OnBnClickedGetTemp0)
	ON_BN_CLICKED(IDC_BUTTON28, &CRFR1CDlg::OnBnClickedGetTemp1)
	ON_BN_CLICKED(IDC_BUTTON37, &CRFR1CDlg::OnBnClickedGetTemp2)
	ON_BN_CLICKED(IDC_BUTTON131, &CRFR1CDlg::OnBnClickedGetTemp3)
	ON_BN_CLICKED(IDC_BUTTON132, &CRFR1CDlg::OnBnClickedGetTemp4)
	ON_BN_CLICKED(IDC_BUTTON133, &CRFR1CDlg::OnBnClickedGetTemp5)
	ON_BN_CLICKED(IDC_BUTTON134, &CRFR1CDlg::OnBnClickedGetTemp6)
	ON_BN_CLICKED(IDC_BUTTON135, &CRFR1CDlg::OnBnClickedGetTemp7)
	ON_BN_CLICKED(IDC_BUTTON15, &CRFR1CDlg::OnBnClickedDetTxIF2)
	ON_BN_CLICKED(IDC_BUTTON18, &CRFR1CDlg::OnBnClickedDetRxComb)
	ON_BN_CLICKED(IDC_BUTTON136, &CRFR1CDlg::OnBnClickedDetRxIF1)
	ON_BN_CLICKED(IDC_BUTTON137, &CRFR1CDlg::OnBnClickedDetRxIF2)
	ON_BN_CLICKED(IDC_BUTTON34, &CRFR1CDlg::OnBnClickedSetTxAll)
	ON_BN_CLICKED(IDC_RF_RX_LNAATTSW_BUTTON9, &CRFR1CDlg::OnBnClickedSetRxAll)
	ON_BN_CLICKED(IDC_BUTTON42, &CRFR1CDlg::OnBnClickedGetTempAll)
	ON_BN_CLICKED(IDC_BUTTON43, &CRFR1CDlg::OnBnClickedDetAll)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXFREQ, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT0, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT1, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT2, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXATT3, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXDET, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_TXTEMP, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXFREQ, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT019, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT1, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT2, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXATT3, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXDET, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_RXTEMP, &CRFR1CDlg::SyncWithRFDDlg)
	ON_MESSAGE(WM_CSE_SYNCRFANDRFD_DLG_IOMODE, &CRFR1CDlg::SyncWithRFDDlg)
	ON_WM_SIZE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CRFR1CDlg::OnDeltaposSpinChangeRxFreq)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CRFR1CDlg::OnCbnSelchangeBlowerAjustMode)
	ON_BN_CLICKED(IDC_CHECK42, &CRFR1CDlg::OnBnClickedStartRecord)
	ON_MESSAGE(WM_CSE_SINGLE_THREAD_DONE, &CRFR1CDlg::SetToIdle)
	ON_MESSAGE(WM_CSE_RF_R1C_TEMP_GUARD_SHOW, &CRFR1CDlg::ShowData)
END_MESSAGE_MAP()


void CRFR1CDlg::OnBnClickedSetTxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	str2freq(m_strTxFreq,uiFreq);
	STACHKV(m_pSP1401->SetTxFreq(uiFreq));

	CSP1401R1C::TxChainStateAll TxState;
	m_pSP1401->TxFreq2LO(uiFreq,TxState.m_uiLO1,TxState.m_uiLO2,TxState.m_uiLO3,TxState.m_Filter);
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXFREQ,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnDeltaposSpinChangeTxFreq(NMHDR *pNMHDR, LRESULT *pResult)
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

void CRFR1CDlg::OnBnClickedSetTxAtt0()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt0.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt0(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt0 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT0,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedSetTxAtt1()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt1.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt1(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt1 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT1,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedSetTxAtt2()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt2.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt2(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt2 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT2,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedSetTxAtt3()
{
	UpdateData(TRUE);
	double dAtt = m_TxAtt3.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetTxAtt3(dAtt));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt3 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT3,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedSetIOMode()
{
	UpdateData(TRUE);
	IOMode Mode = (IOMode)m_IOMode.GetCurSel();
	STACHKV(m_pSP1401->SetIOMode(Mode));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_Mode = Mode;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_IOMODE,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSyncTxGain()
{
	UpdateData(TRUE);
	double dAtt0 = m_TxAtt0.GetCurSel() * 0.5;
	double dAtt1 = m_TxAtt1.GetCurSel() * 0.5;
	double dAtt2 = m_TxAtt2.GetCurSel() * 0.5;
	double dAtt3 = m_TxAtt3.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SyncSetTxGain(dAtt0,dAtt1,dAtt2,dAtt3,m_dDGain));

	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dAtt0 = dAtt0;
	TxState.m_dAtt1 = dAtt1;
	TxState.m_dAtt2 = dAtt2;
	TxState.m_dAtt3 = dAtt3;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT0,0,(LPARAM)&TxState);
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT1,0,(LPARAM)&TxState);
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT2,0,(LPARAM)&TxState);
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXATT3,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedSetRxFreq()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	str2freq(m_strRxFreq,uiFreq);
	STACHKV(m_pSP1401->SetRxFreq(uiFreq));

	CSP1401R1C::RxChainStateAll RxState;
	m_pSP1401->RxFreq2LO(uiFreq,RxState.m_uiLO1,RxState.m_uiLO2);
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXFREQ,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetRxLNAAttSw()
{
	UpdateData(TRUE);
	SP1401::RxLNAAtt LNAAtt = (SP1401::RxLNAAtt)m_RxLNAAttSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxLNAAttSw(LNAAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_LNAAtt = LNAAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetRxAtt019Sw()
{
	UpdateData(TRUE);
	SP1401::RxAtt019 Att = (SP1401::RxAtt019)m_RxAtt019Sw.GetCurSel();
	STACHKV(m_pSP1401->SetRxAtt019Sw(Att));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_Att019 = Att;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT019,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetRxAtt1()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt1.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt1(dAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dAtt1 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT1,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetRxAtt2()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt2.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt2(dAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dAtt2 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT2,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetRxIFFilterSw()
{
	UpdateData(TRUE);
	CSP1401R1C::RxIFFilter Filter = (CSP1401R1C::RxIFFilter)m_RxIFFilterSw.GetCurSel();
	STACHKV(m_pSP1401->SetRxIFFilterSw(Filter));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_IFFilter = Filter;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXIFFILTER,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetRxAtt3()
{
	UpdateData(TRUE);
	double dAtt = m_RxAtt3.GetCurSel() * 0.5;
	STACHKV(m_pSP1401->SetRxAtt3(dAtt));

	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dAtt3 = dAtt;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXATT3,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedWriteSN()
{
	UpdateData(TRUE);
	STACHKV(m_pSP1401->SetSN((LPSTR)(LPCTSTR)m_strSNRfVer,(int32_t)m_uiSNOrdinal));
}

void CRFR1CDlg::OnBnClickedReadSN()
{
	UpdateData(TRUE);
	char szSN[32];
	memset(szSN,0,sizeof(szSN));
	STACHKV(m_pSP1401->GetSN(szSN));
	m_strSN.Format("%s",szSN);
	UpdateData(FALSE);
}

void CRFR1CDlg::OnBnClickedGetTemp0()
{
	STACHKV(m_pSP1401->GetTemp0RxLO2(&m_dTemp0));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedGetTemp1()
{
	STACHKV(m_pSP1401->GetTemp1RxLNA(&m_dTemp1));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedGetTemp2()
{
	STACHKV(m_pSP1401->GetTemp2RxLO1(&m_dTemp2));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedGetTemp3()
{
	STACHKV(m_pSP1401->GetTemp3RxPA1(&m_dTemp3));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::RxChainStateAll RxState;
	RxState.m_dTemp0 = m_dTemp0;
	RxState.m_dTemp1 = m_dTemp1;
	RxState.m_dTemp2 = m_dTemp2;
	RxState.m_dTemp3 = m_dTemp3;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXTEMP,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedGetTemp4()
{
	STACHKV(m_pSP1401->GetTemp4TxLO2(&m_dTemp4));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedGetTemp5()
{
	STACHKV(m_pSP1401->GetTemp5TxLO1(&m_dTemp5));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedGetTemp6()
{
	STACHKV(m_pSP1401->GetTemp6TxPA4(&m_dTemp6));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedGetTemp7()
{
	STACHKV(m_pSP1401->GetTemp7TxPA3(&m_dTemp7));
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_dTemp4 = m_dTemp4;
	TxState.m_dTemp5 = m_dTemp5;
	TxState.m_dTemp6 = m_dTemp6;
	TxState.m_dTemp7 = m_dTemp7;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXTEMP,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedDetTxIF2()
{
	uint32_t uiDet = 0;
	STACHKV(m_pSP1401->GetAD7680TxIF2(uiDet));
	m_uiDetTxIF2 = (UINT)uiDet;
	UpdateData(FALSE);

	UpdateData(TRUE);
	CSP1401R1C::TxChainStateAll TxState;
	TxState.m_uiDetIF2 = m_uiDetTxIF2;
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_TXDET,0,(LPARAM)&TxState);
}

void CRFR1CDlg::OnBnClickedDetRxComb()
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
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedDetRxIF1()
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
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedDetRxIF2()
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
	::SendMessage(ALL_DLG->m_pRFR1CDDlg[m_pSP1401->GetRfIdx()].GetSafeHwnd(),WM_CSE_SYNCRFANDRFD_DLG_RXDET,0,(LPARAM)&RxState);
}

void CRFR1CDlg::OnBnClickedSetTxAll()
{
	OnBnClickedSetTxFreq();
	OnBnClickedSetTxAtt0();
	OnBnClickedSetTxAtt1();
	OnBnClickedSetTxAtt2();
	OnBnClickedSetTxAtt3();
	OnBnClickedSetIOMode();
	OnBnClickedGetTemp4();
	OnBnClickedGetTemp5();
	OnBnClickedGetTemp6();
	OnBnClickedGetTemp7();
	OnBnClickedDetTxIF2();
}

void CRFR1CDlg::OnBnClickedSetRxAll()
{
	OnBnClickedSetRxFreq();
	OnBnClickedSetRxLNAAttSw();
	OnBnClickedSetRxAtt019Sw();
	OnBnClickedSetRxAtt1();
	OnBnClickedSetRxAtt2();
	OnBnClickedSetRxIFFilterSw();
	OnBnClickedSetRxAtt3();
	OnBnClickedGetTemp0();
	OnBnClickedGetTemp1();
	OnBnClickedGetTemp2();
	OnBnClickedGetTemp3();
	OnBnClickedDetRxComb();
	OnBnClickedDetRxIF1();
	OnBnClickedDetRxIF2();
}

void CRFR1CDlg::OnBnClickedGetTempAll()
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

void CRFR1CDlg::OnBnClickedDetAll()
{
	OnBnClickedDetTxIF2();
	OnBnClickedDetRxComb();
	OnBnClickedDetRxIF1();
	OnBnClickedDetRxIF2();
}

BOOL CRFR1CDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TxAtt0.SetCurSel(20);
	m_TxAtt1.SetCurSel(0);
	m_TxAtt2.SetCurSel(0);
	m_TxAtt3.SetCurSel(0);
	m_IOMode.SetCurSel(1);
	m_RxLNAAttSw.SetCurSel(0);
	m_RxAtt019Sw.SetCurSel(0);
	m_RxAtt1.SetCurSel(20);
	m_RxAtt2.SetCurSel(0);
	m_RxAtt3.SetCurSel(0);
	m_RxIFFilterSw.SetCurSel(1);
	m_BlowerAjustMode.SetCurSel(0);

	m_pChart = new CChartCtrl;
	m_pChart->Create(this,CRect(360,230,810,370),IDC_RF_R1C_TEMP_CHART);
	m_pChart->GetLegend()->UndockLegend(50,20);
	m_pChart->GetLegend()->SetVisible(true);
	m_pChart->GetTitle()->AddString(_T("Temperature(Not Recording)"));
	m_pChart->SetZoomRectColor(BLACK);

	CChartDateTimeAxis *pDataTimeAxis = m_pChart->CreateDateTimeAxis(CChartCtrl::BottomAxis);
	pDataTimeAxis->SetDiscrete(false);
	pDataTimeAxis->SetTickIncrement(true,CChartDateTimeAxis::tiSecond,5);
	pDataTimeAxis->SetAutomatic(true);
	pDataTimeAxis->SetTickLabelFormat(false,_T("%H:%M:%S"));
	CChartStandardAxis *pTempAxis = m_pChart->CreateStandardAxis(CChartCtrl::LeftAxis);
	pTempAxis->SetAutomatic(false);
	pTempAxis->SetMinMax(40.0,80.0);
	pTempAxis->GetGrid()->SetVisible(false);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_EDIT26,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_SPIN1,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_EDIT67,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_COMBO7,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_COMBO8,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_COMBO9,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_COMBO10,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_COMBO25,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_EDIT3,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_BUTTON17,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON34,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON10,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON11,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON12,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON13,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON30,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_TX,FixL_xStretch,11,0,2);
	SetControlInfo(IDC_STATIC_RF_RXFREQ,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_RXLNAATT,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_RXATT1,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_RXATT2,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_RXVGAIN,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_RXIFFILTER,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_STATIC_RF_RXVGAIN2,ChgL_Fixx,11,2,2);
	SetControlInfo(IDC_EDIT73,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_SPIN2,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_EDIT69,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO5,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO6,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO7,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO8,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_COMBO9,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_BUTTON58,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON9,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON5,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON6,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON7,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON8,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_RF_RX_LNAATTSW_BUTTON10,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_RF_RX,ChgL_xStretch,11,2,4);
	SetControlInfo(IDC_STATIC_RF_RFVER,ChgL_Fixx,11,8,8);
	SetControlInfo(IDC_STATIC_RF_RFSN,ChgL_Fixx,11,8,8);
	SetControlInfo(IDC_EDIT1,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_EDIT2,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_EDIT76,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_BUTTON64,ChgL_xStretch,11,10,11);
	SetControlInfo(IDC_BUTTON65,ChgL_xStretch,11,10,11);
	SetControlInfo(IDC_STATIC_27,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_28,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_29,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_30,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_31,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_32,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_33,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_STATIC_34,ChgL_Fixx,11,4,4);
	SetControlInfo(IDC_EDIT4,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT25,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT122,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT123,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT124,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT125,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT126,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_EDIT127,ChgL_xStretch,11,4,6);
	SetControlInfo(IDC_BUTTON2,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON28,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON37,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON131,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON132,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON133,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON134,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON135,ChgL_xStretch,11,6,7);
	SetControlInfo(IDC_BUTTON42,ChgL_xStretch,11,7,8);
	SetControlInfo(IDC_STATIC_TEMP,ChgL_xStretch,11,4,8);
	SetControlInfo(IDC_STATIC_RF_TXDET,ChgL_Fixx,11,8,8);
	SetControlInfo(IDC_STATIC_RF_RXDET,ChgL_Fixx,11,8,8);
	SetControlInfo(IDC_STATIC_RF_RXDET2,ChgL_Fixx,11,8,8);
	SetControlInfo(IDC_STATIC_RF_RXDET3,ChgL_Fixx,11,8,8);
	SetControlInfo(IDC_EDIT21,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_EDIT23,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_EDIT50,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_EDIT52,ChgL_xStretch,11,8,10);
	SetControlInfo(IDC_BUTTON15,ChgL_xStretch,11,10,11);
	SetControlInfo(IDC_BUTTON18,ChgL_xStretch,11,10,11);
	SetControlInfo(IDC_BUTTON136,ChgL_xStretch,11,10,11);
	SetControlInfo(IDC_BUTTON137,ChgL_xStretch,11,10,11);
	SetControlInfo(IDC_BUTTON43,ChgL_xStretch,11,8,11);
	SetControlInfo(IDC_STATIC_DET,ChgL_xStretch,11,8,11);
	SetControlInfo(IDC_RF_R1C_TEMP_CHART,FixL_xyStretch,1,0,1,1,0,1);

	int nID[11] = { IDC_SLIDER_BLOWER_1,
		IDC_SLIDER_BLOWER_2,
		IDC_SLIDER_BLOWER_3,
		IDC_SLIDER_BLOWER_4,
		IDC_SLIDER_BLOWER_5,
		IDC_SLIDER_BLOWER_6,
		IDC_SLIDER_BLOWER_7,
		IDC_SLIDER_BLOWER_8,
		IDC_SLIDER_BLOWER_9,
		IDC_SLIDER_BLOWER_10,
		IDC_SLIDER_BLOWER_11
	};
	for (int i = 0;i < ARRAY_SIZE(nID);i ++) {
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetRange(0,255);
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetLineSize(1);
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetTicFreq(1);
		((CSliderCtrl *)(GetDlgItem(nID[i])))->SetPos(255);
		AddThreadIdleCtrl(nID[i]);
	}
	SetControlInfo(IDC_SLIDER_BLOWER_1,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_2,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_3,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_4,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_5,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_6,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_7,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_8,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_9,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_10,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_SLIDER_BLOWER_11,FixL_yStretch,1,1,1,1,0,1);
	SetControlInfo(IDC_STATIC_BLOWER_1_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_2_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_3_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_4_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_5_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_6_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_7_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_8_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_9_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_10_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	SetControlInfo(IDC_STATIC_BLOWER_11_SPEED,ChgT_Fixx,1,1,1,1,1,1);
	return TRUE;
}

LRESULT CRFR1CDlg::SyncWithRFDDlg(WPARAM wParam,LPARAM lParam)
{
	UINT uiMsg = GetCurrentMessage()->message;
	CSP1401R1C::TxChainStateAll TxState = *(CSP1401R1C::TxChainStateAll *)lParam;
	CSP1401R1C::RxChainStateAll RxState = *(CSP1401R1C::RxChainStateAll *)lParam;

	if (WM_CSE_SYNCRFANDRFD_DLG_TXFREQ == uiMsg) {
		uint64_t uiFreq = 0;
		m_pSP1401->TxLO2Freq(TxState.m_uiLO1,TxState.m_uiLO2,TxState.m_uiLO3,uiFreq);
		freq2str(uiFreq,m_strTxFreq);
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
		UpdateData(FALSE);
		return 0;
	}
	if (WM_CSE_SYNCRFANDRFD_DLG_RXLNAATT == uiMsg) {
		m_RxLNAAttSw.SetCurSel((int)(RxState.m_LNAAtt));
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
		m_RxIFFilterSw.SetCurSel(int(RxState.m_IFFilter));
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
		m_IOMode.SetCurSel((int)(RxState.m_Mode));
		UpdateData(FALSE);
		return 0;
	}
	return 0;
}

void CRFR1CDlg::OnDeltaposSpinChangeRxFreq(NMHDR *pNMHDR, LRESULT *pResult)
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

void CRFR1CDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurCtrlID = pScrollBar->GetDlgCtrlID();
	if (nCurCtrlID < IDC_SLIDER_BLOWER_1 || nCurCtrlID > IDC_SLIDER_BLOWER_11)
		return;

	int nCurCtrlPos = ((CSliderCtrl *)pScrollBar)->GetPos();
	int nBlowerID[11] = { IDC_SLIDER_BLOWER_1,
		IDC_SLIDER_BLOWER_2,
		IDC_SLIDER_BLOWER_3,
		IDC_SLIDER_BLOWER_4,
		IDC_SLIDER_BLOWER_5,
		IDC_SLIDER_BLOWER_6,
		IDC_SLIDER_BLOWER_7,
		IDC_SLIDER_BLOWER_8,
		IDC_SLIDER_BLOWER_9,
		IDC_SLIDER_BLOWER_10,
		IDC_SLIDER_BLOWER_11
	};
	int nSpeedID[11] = {IDC_STATIC_BLOWER_1_SPEED,
		IDC_STATIC_BLOWER_2_SPEED,
		IDC_STATIC_BLOWER_3_SPEED,
		IDC_STATIC_BLOWER_4_SPEED,
		IDC_STATIC_BLOWER_5_SPEED,
		IDC_STATIC_BLOWER_6_SPEED,
		IDC_STATIC_BLOWER_7_SPEED,
		IDC_STATIC_BLOWER_8_SPEED,
		IDC_STATIC_BLOWER_9_SPEED,
		IDC_STATIC_BLOWER_10_SPEED,
		IDC_STATIC_BLOWER_11_SPEED
	};

	if (0 == m_BlowerAjustMode.GetCurSel()) {
		for (int i = 0;i < ARRAY_SIZE(nBlowerID);i ++) {
			m_pSP3501->SetBlower(i,255 - nCurCtrlPos);
			if (nBlowerID[i] != pScrollBar->GetDlgCtrlID())
				((CSliderCtrl *)(GetDlgItem(nBlowerID[i])))->SetPos(nCurCtrlPos);
		}
	}
	if (1 == m_BlowerAjustMode.GetCurSel()) {
		for (int i = 0;i < ARRAY_SIZE(nBlowerID);i ++) {
			if (nBlowerID[i] == pScrollBar->GetDlgCtrlID()) {
				m_pSP3501->SetBlower(i,255 - nCurCtrlPos);
				break;
			}
		}
	}

	for (int i = 0;i < ARRAY_SIZE(nBlowerID);i ++) {
		int iPos = ((CSliderCtrl *)(GetDlgItem(nBlowerID[i])))->GetPos();
		CString strSpeed = "";
		strSpeed.Format("%d",255 - iPos);
		if (::IsWindow(ALL_DLG->m_pRFR1AGuardDlg[0]))
			((CSliderCtrl *)(ALL_DLG->m_pRFR1AGuardDlg[0].GetDlgItem(nBlowerID[i])))->SetPos(iPos);
		for (int32_t j = 0;j < MAX_RF;j ++) {
			if (::IsWindow(ALL_DLG->m_pRFR1CDlg[j])) {
				((CSliderCtrl *)(ALL_DLG->m_pRFR1CDlg[j].GetDlgItem(nBlowerID[i])))->SetPos(iPos);
				((CStatic *)(ALL_DLG->m_pRFR1CDlg[j].GetDlgItem(nSpeedID[i])))->SetWindowText(strSpeed);
			}
		}
	}
	CAttachThreadDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRFR1CDlg::OnCbnSelchangeBlowerAjustMode()
{
	int iMode = m_BlowerAjustMode.GetCurSel();
	if (::IsWindow(ALL_DLG->m_pRFR1AGuardDlg[0]))
		ALL_DLG->m_pRFR1AGuardDlg[0].m_Mode.SetCurSel(iMode);
	for (int32_t i = 0;i < MAX_RF;i ++) {
		if (::IsWindow(ALL_DLG->m_pRFR1CDlg[i]))
			ALL_DLG->m_pRFR1CDlg[i].m_BlowerAjustMode.SetCurSel(iMode);
	}
}

void CRFR1CDlg::ResetChart()
{
	for (int32_t i = 0;i < m_pTempLineSerie.size();i ++)
		m_pTempLineSerie[i]->ClearSerie();
	m_pTempLineSerie.clear();
	m_pChart->RemoveAllSeries();

	COLORREF clrTemp[8] = {BLACK,RED,GREEN,BLUE,BROWN,NAVY,PURPLE,GREY};
	TChartString strName[8] = {"Rx_LO2","Rx_LNA","Rx_LO1","Rx_PA1","Tx_LO2","Tx_LO1","Tx_PA4","Tx_PA3"};

	for (int32_t i = 0;i < 8;i ++) {
		CChartLineSerie *pLineSerie = m_pChart->CreateLineSerie();
		m_pTempLineSerie.push_back(pLineSerie);
		m_pTempLineSerie[i]->SetSeriesOrdering(poNoOrdering);
		m_pTempLineSerie[i]->SetColor(clrTemp[i]);
		m_pTempLineSerie[i]->SetName(strName[i]);
		m_pTempLineSerie[i]->SetVisible(true);
	}
}

void CRFR1CDlg::OnBnClickedStartRecord()
{
	int iCheck = 0;
	if (BST_CHECKED == (iCheck = ((CButton *)(GetDlgItem(IDC_CHECK42)))->GetCheck())) {
		TEST_THREAD_EXISTED_CHKV();
		for (int32_t i = 0;i < MAX_RF;i ++) {
			if (::IsWindow(ALL_DLG->m_pRFR1CDlg[i])) {
				ALL_DLG->m_pRFR1CDlg[i].ResetChart();
				((CButton *)(ALL_DLG->m_pRFR1CDlg[i].GetDlgItem(IDC_CHECK42)))->SetCheck(iCheck);
				((CButton *)(ALL_DLG->m_pRFR1CDlg[i].GetDlgItem(IDC_CHECK42)))->SetWindowText(_T("Stop Record"));
				ALL_DLG->m_pRFR1CDlg[i].m_pChart->GetTitle()->RemoveAll();
				ALL_DLG->m_pRFR1CDlg[i].m_pChart->GetTitle()->AddString(_T("Temperature(Recording)(X:Time Y:Temp)"));
			}
		}
		m_pThread = new CRFR1CGuardThread(this);
		m_pThread->CreateThread();
	}
	else {
		for (int32_t i = 0;i < MAX_RF;i ++) {
			if (::IsWindow(ALL_DLG->m_pRFR1CDlg[i])) {
				((CButton *)(ALL_DLG->m_pRFR1CDlg[i].GetDlgItem(IDC_CHECK42)))->SetCheck(iCheck);
				((CButton *)(ALL_DLG->m_pRFR1CDlg[i].GetDlgItem(IDC_CHECK42)))->SetWindowText(_T("Start Record"));
				ALL_DLG->m_pRFR1CDlg[i].m_pChart->GetTitle()->RemoveAll();
				ALL_DLG->m_pRFR1CDlg[i].m_pChart->GetTitle()->AddString(_T("Temperature(Not Recording)"));
				ALL_DLG->m_pRFR1CDlg[i].m_pChart->GetTitle()->SetColor(BLACK);
			}
		}
		CCSEWinThread::m_bRunning = FALSE;
	}
}

LRESULT CRFR1CDlg::SetToIdle(WPARAM wParam,LPARAM lParam)
{
	CCSEWinThread::m_bRunning = FALSE;
	return 0;
}

LRESULT CRFR1CDlg::ShowData(WPARAM wParam,LPARAM lParam)
{
	CRFR1CGuardThread::Result Data = *(CRFR1CGuardThread::Result *)lParam;

	m_pChart->GetTitle()->SetColor(iseven(Data.m_iIdx) ? BLACK : GREEN);
	for (int32_t i = 0;i < 8;i ++) {
		TChartString strName = m_pTempLineSerie.at(i)->GetName();
		strName.resize(6);
		TChartStringStream strStr;
		if (Data.m_dTemp[i] > 0.0 && Data.m_dTemp[i] < 100.0) {
			strStr << strName << "   " << Data.m_dTemp[i];
			m_pTempLineSerie.at(i)->SetName(strStr.str());
			m_pTempLineSerie.at(i)->AddPoint(Data.m_Time,Data.m_dTemp[i]);
		}
		else {
			m_pTempLineSerie.at(i)->AddPoint(Data.m_Time,Data.m_dTemp[i]);
		}
	}
	return 0;
}