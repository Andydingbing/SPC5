#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "BBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CBBDlg, CStretchDlg)
IMPLEMENT_CSE_STRETCH_DLG(CBBDlg)

CBBDlg::CBBDlg(CWnd* pParent /*=NULL*/)
	: CStretchDlg(CBBDlg::IDD, pParent)
	, m_strFreqDDS1(_T("20M"))
	, m_dDDSGain(0)
	, m_strFreqDDS2(_T("0"))
	, m_dPhaseRotateI(0.0)
	, m_dPhaseRotateQ(0.0)
	, m_strAmplitudeBalanceI(_T("8192"))
	, m_strAmplitudeBalanceQ(_T("8192"))
	, m_strDCOffsetI(_T("0"))
	, m_strDCOffsetQ(_T("0"))
	, m_strFreqDDC(_T("-92.64M"))
	, m_iADManual(0)
	, m_iRxPowerCompensate(0)
	, m_strFreqDUSDDS(_T("0"))
	, m_strDAClock(_T("491.53"))
	, m_strADClock(_T("368.65"))
	, m_iADFpga(0)
	, m_strTxFilterCoefReal(_T(""))
	, m_strTxFilterCoefImag(_T(""))
	, m_strRxFilterCoefReal(_T(""))
	, m_strRxFilterCoefImag(_T(""))
	, m_dPowerManual(0)
	, m_bRxFilter(FALSE)
{

}

CBBDlg::~CBBDlg()
{
}

void CBBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFreqDDS1);
	DDX_Text(pDX, IDC_EDIT12, m_dDDSGain);
	DDX_Text(pDX, IDC_EDIT2, m_strFreqDDS2);
	DDX_Control(pDX, IDC_COMBO2, m_DDSSource);
	DDX_Text(pDX, IDC_EDIT5, m_dPhaseRotateI);
	DDX_Control(pDX, IDC_EDIT5, m_PhaseRotateI);
	DDX_Control(pDX, IDC_EDIT6, m_PhaseRotateQ);
	DDX_Text(pDX, IDC_EDIT6, m_dPhaseRotateQ);
	DDX_Control(pDX, IDC_EDIT15, m_AmplitudeBalanceI);
	DDX_Text(pDX, IDC_EDIT15, m_strAmplitudeBalanceI);
	DDX_Control(pDX, IDC_EDIT79, m_AmplitudeBalanceQ);
	DDX_Text(pDX, IDC_EDIT79, m_strAmplitudeBalanceQ);
	DDX_Control(pDX, IDC_EDIT16, m_DCOffsetI);
	DDX_Text(pDX, IDC_EDIT16, m_strDCOffsetI);
	DDX_Control(pDX, IDC_EDIT80, m_DCOffsetQ);
	DDX_Text(pDX, IDC_EDIT80, m_strDCOffsetQ);
	DDX_Text(pDX, IDC_EDIT66, m_strFreqDDC);
	DDX_Text(pDX, IDC_EDIT82, m_iADManual);
	DDX_Text(pDX, IDC_EDIT83, m_iRxPowerCompensate);
	DDX_Text(pDX, IDC_EDIT17, m_strFreqDUSDDS);
	DDX_Text(pDX, IDC_EDIT22, m_strDAClock);
	DDX_Control(pDX, IDC_COMBO17, m_DASwitch);
	DDX_Text(pDX, IDC_EDIT28, m_strADClock);
	DDX_Control(pDX, IDC_COMBO31, m_ADSwitch);
	DDX_Text(pDX, IDC_EDIT88, m_iADFpga);
	DDX_Text(pDX, IDC_EDIT19, m_strTxFilterCoefReal);
	DDX_Text(pDX, IDC_EDIT29, m_strTxFilterCoefImag);
	DDX_Text(pDX, IDC_EDIT113, m_strRxFilterCoefReal);
	DDX_Text(pDX, IDC_EDIT35, m_strRxFilterCoefImag);
	DDX_Text(pDX, IDC_EDIT81, m_dPowerManual);
	DDX_Radio(pDX, IDC_RADIO1, m_bRxFilter);
	DDX_Control(pDX, IDC_COMBO19, m_RxFilter80);
	DDX_Control(pDX, IDC_COMBO63, m_RxFilter160);
	DDX_Control(pDX, IDC_EDIT17, m_DUC);
	DDX_Control(pDX, IDC_EDIT1, m_DDS1);
	DDX_Control(pDX, IDC_EDIT2, m_DDS2);
}


BEGIN_MESSAGE_MAP(CBBDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CBBDlg::OnBnClickedSetDDS1Freq)
	ON_BN_CLICKED(IDC_BUTTON21, &CBBDlg::OnBnClickedSetDDSGain)
	ON_BN_CLICKED(IDC_BUTTON16, &CBBDlg::OnBnClickedSetDDS2Freq)
	ON_EN_CHANGE(IDC_EDIT5, &CBBDlg::OnEnChangeEditPhaseRotateI)
	ON_EN_CHANGE(IDC_EDIT6, &CBBDlg::OnEnChangeEditPhaseRotateQ)
	ON_EN_CHANGE(IDC_EDIT15, &CBBDlg::OnEnChangeEditAmplitudeBalanceI)
	ON_EN_CHANGE(IDC_EDIT79, &CBBDlg::OnEnChangeEditAmplitudeBalanceQ)
	ON_EN_CHANGE(IDC_EDIT16, &CBBDlg::OnEnChangeEditDCOffsetI)
	ON_EN_CHANGE(IDC_EDIT80, &CBBDlg::OnEnChangeEditDCOffsetQ)
	ON_BN_CLICKED(IDC_BUTTON3, &CBBDlg::OnBnClickedSetDDCFreq)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON58, &CBBDlg::OnBnClickedGetADManual)
	ON_BN_CLICKED(IDC_BUTTON59, &CBBDlg::OnBnClickedSetRxPowerCompensate)
	ON_BN_CLICKED(IDC_BUTTON23, &CBBDlg::OnBnClickedSetDUCDDS)
	ON_BN_CLICKED(IDC_BUTTON25, &CBBDlg::OnBnClickedSetDAClock)
	ON_BN_CLICKED(IDC_BUTTON27, &CBBDlg::OnBnClickedSetDASwitch)
	ON_BN_CLICKED(IDC_BUTTON8, &CBBDlg::OnBnClickedSetDAReset)
	ON_BN_CLICKED(IDC_BUTTON29, &CBBDlg::OnBnClickedSetADClock)
	ON_BN_CLICKED(IDC_BUTTON34, &CBBDlg::OnBnClickedSetADSwitch)
	ON_BN_CLICKED(IDC_BUTTON80, &CBBDlg::OnBnClickedGetADFpga)
	ON_BN_CLICKED(IDC_BUTTON31, &CBBDlg::OnBnClickedSetDDSSrc)
	ON_BN_CLICKED(IDC_BUTTON32, &CBBDlg::OnBnClickedSetTxFilter)
	ON_BN_CLICKED(IDC_BUTTON33, &CBBDlg::OnBnClickedSetRxFilter)
	ON_BN_CLICKED(IDC_BUTTON9, &CBBDlg::OnBnClickedGetPowerManual)
	ON_BN_CLICKED(IDC_RADIO1, &CBBDlg::OnBnClickedSelRxFilter80M)
	ON_BN_CLICKED(IDC_RADIO2, &CBBDlg::OnBnClickedSelRxFilter160M)
	ON_EN_CHANGE(IDC_EDIT17, &CBBDlg::OnEnChangeDUCDDS)
	ON_EN_CHANGE(IDC_EDIT1, &CBBDlg::OnEnChangeDDS1)
	ON_EN_CHANGE(IDC_EDIT2, &CBBDlg::OnEnChangeDDS2)
	ON_BN_CLICKED(IDC_CHECK3, &CBBDlg::OnBnClickedSetTxFilterSw)
END_MESSAGE_MAP()


void CBBDlg::OnBnClickedSetDDS1Freq()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	str2freq(m_strFreqDDS1,uiFreq);
	STACHKV(m_pSP2401->SetDDS1(double(uiFreq)));
}

void CBBDlg::OnBnClickedSetDDSGain()
{
	UpdateData(TRUE);
	STACHKV(m_pSP2401->SetTxPowerComp(m_dDDSGain));
}

void CBBDlg::OnBnClickedSetDDS2Freq()
{
	UpdateData(TRUE);
	uint64_t uiFreq = 0;
	str2freq(m_strFreqDDS2,uiFreq);
	STACHKV(m_pSP2401->SetDDS2(double(uiFreq)));
}

BOOL CBBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_DASwitch.SetCurSel(0);
	m_ADSwitch.SetCurSel(0);
	m_DDSSource.SetCurSel(3);

//	default order 144 2i3d fir filter coef
//	m_strRxFilterCoefReal = "-7,6,13,22,32,39,39,31,12,-14,-42,-66,-78,-71,-42,5,61,112,144,142,102,26,-71,-168,-236,-253,-205,-92,64,229,359,414,366,212,-22,-286,-512,-635,-606,-411,-77,325,698,922,960,730,271,-326,-922,-1358,-1493,-1249,-631,252,1207,1992,2371,2171,1343,-13,-1643,-3176,-4185,-4274,-3167,-776,2759,7076,11638,15818,19011,20739,20739,19011,15818,11638,7076,2759,-776,-3167,-4274,-4185,-3176,-1643,-13,1343,2171,2371,1992,1207,252,-631,-1249,-1493,-1358,-922,-326,271,730,960,922,698,325,-77,-411,-606,-635,-512,-286,-22,212,366,414,359,229,64,-92,-205,-253,-236,-168,-71,26,102,142,144,112,61,5,-42,-71,-78,-66,-42,-14,12,31,39,39,32,22,13,6,-7";
//	m_strRxFilterCoefImag = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";

	m_strRxFilterCoefReal = "55,-74,-117,-42,61,25,-84,-55,80,73,-86,-104,80,133,-73,-171,53,208,-26,-249,-16,285,69,-320,-139,345,223,-362,-326,362,446,-345,-589,300,755,-222,-956,95,1205,106,-1539,-440,2044,1061,-3021,-2633,6360,16383,16383,6360,-2633,-3021,1061,2044,-440,-1539,106,1205,95,-956,-222,755,300,-589,-345,446,362,-326,-362,223,345,-139,-320,69,285,-16,-249,-26,208,53,-171,-73,133,80,-104,-86,73,80,-55,-84,25,61,-42,-117,-74,55";
	m_strRxFilterCoefImag = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
	m_RxFilter80.SetCurSel(1);
	m_RxFilter160.SetCurSel(0);
	m_RxFilter160.EnableWindow(FALSE);
	UpdateData(FALSE);

	this->GetWindowRect(&m_crWnd);
	SetControlInfo(IDC_STATIC_UP,FixL_xyStretch,4,0,2,1,0,1);
	SetControlInfo(IDC_STATIC_DOWN,ChgL_xyStretch,4,2,4,1,0,1);
	SetControlInfo(IDC_EDIT22,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON25,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_COMBO17,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON27,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_BUTTON8,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_COMBO2,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON31,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT1,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON1,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT2,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON16,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT12,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON21,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT17,FixL_xStretch,4,0,2);
	SetControlInfo(IDC_BUTTON23,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT5,FixL_xStretch,4,0,1);
	SetControlInfo(IDC_STATIC_TX_Q,ChgL_Fixx,4,1,1);
	SetControlInfo(IDC_EDIT6,ChgL_xStretch,4,1,2);
	SetControlInfo(IDC_EDIT15,FixL_xStretch,4,0,1);
	SetControlInfo(IDC_EDIT79,ChgL_xStretch,4,1,2);
	SetControlInfo(IDC_EDIT16,FixL_xStretch,4,0,1);
	SetControlInfo(IDC_EDIT80,ChgL_xStretch,4,1,2);
	SetControlInfo(IDC_EDIT19,FixL_xyStretch,4,0,2,2,0,1);
	SetControlInfo(IDC_STATIC_TX_FILTER_COEF_IMAG,ChgT_Fixx,4,0,0,2,1,2);
	SetControlInfo(IDC_EDIT29,ChgLT_xyStretch,4,0,2,2,1,2);
	SetControlInfo(IDC_CHECK3,ChgLT_Fixx,4,2,2,2,0,0);
	SetControlInfo(IDC_BUTTON32,ChgL_xyStretch,4,2,2,2,0,2);
	SetControlInfo(IDC_STATIC_AD_CLOCK,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT28,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON29,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_AD_SWITCH,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_COMBO31,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON34,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_DDC,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT66,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON3,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_RXPOWER,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT81,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON9,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_ADMANUAL,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT82,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON58,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_ADFPGA,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT88,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON80,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_RXPOWER_COMPENSATE,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT83,ChgL_xStretch,4,2,4);
	SetControlInfo(IDC_BUTTON59,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_STATIC_RX_FILTER_COEF_REAL,ChgL_Fixx,4,2,2);
	SetControlInfo(IDC_EDIT113,ChgL_xyStretch,4,2,4,2,0,1);
	SetControlInfo(IDC_STATIC_RX_FILTER_COEF_IMAG,ChgLT_Fixx,4,2,2,2,1,2);
	SetControlInfo(IDC_EDIT35,ChgLT_xyStretch,4,2,4,2,1,2);
	SetControlInfo(IDC_BUTTON33,ChgL_xyStretch,4,4,4,2,0,2);
	SetControlInfo(IDC_RADIO1,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_RADIO2,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_COMBO19,ChgL_Fixx,4,4,4);
	SetControlInfo(IDC_COMBO63,ChgL_Fixx,4,4,4);
	return TRUE;
}

void CBBDlg::OnEnChangeEditPhaseRotateI()
{
	UpdateData(TRUE);
	STACHKV(m_pSP2401->SetTxPhaseRotateI(m_dPhaseRotateI));
	OnBnClickedGetADFpga();
	(CPhaseRotateScrollEdit *)(GetDlgItem(IDC_EDIT5))->SetFocus();
}

void CBBDlg::OnEnChangeEditPhaseRotateQ()
{
	return;
}

void CBBDlg::OnEnChangeEditAmplitudeBalanceI()
{
	UpdateData(TRUE);
	uint16_t uiI = (uint16_t)(_ttoi(m_strAmplitudeBalanceI));
	uint16_t uiQ = (uint16_t)(_ttoi(m_strAmplitudeBalanceQ));
	STACHKV(m_pSP2401->SetTxAmplitudeBalance(uiI,uiQ));
	OnBnClickedGetADFpga();
	(CAmplitudeBalanceScrollEdit *)(GetDlgItem(IDC_EDIT15))->SetFocus();
}

void CBBDlg::OnEnChangeEditAmplitudeBalanceQ()
{
	UpdateData(TRUE);
	uint16_t uiI = (uint16_t)(_ttoi(m_strAmplitudeBalanceI));
	uint16_t uiQ = (uint16_t)(_ttoi(m_strAmplitudeBalanceQ));
	STACHKV(m_pSP2401->SetTxAmplitudeBalance(uiI,uiQ));
	OnBnClickedGetADFpga();
	(CAmplitudeBalanceScrollEdit *)(GetDlgItem(IDC_EDIT79))->SetFocus();
}

void CBBDlg::OnEnChangeEditDCOffsetI()
{
	UpdateData(TRUE);
	uint16_t uiI = (uint16_t)(_ttoi(m_strDCOffsetI));
	uint16_t uiQ = (uint16_t)(_ttoi(m_strDCOffsetQ));
	STACHKV(m_pSP2401->SetTxDCOffset(uiI,uiQ));
	OnBnClickedGetADFpga();
	(CDCOffsetScrollEdit *)(GetDlgItem(IDC_EDIT16))->SetFocus();
}

void CBBDlg::OnEnChangeEditDCOffsetQ()
{
	UpdateData(TRUE);
	uint16_t uiI = (uint16_t)(_ttoi(m_strDCOffsetI));
	uint16_t uiQ = (uint16_t)(_ttoi(m_strDCOffsetQ));
	STACHKV(m_pSP2401->SetTxDCOffset(uiI,uiQ));
	OnBnClickedGetADFpga();
	(CDCOffsetScrollEdit *)(GetDlgItem(IDC_EDIT80))->SetFocus();
}

void CBBDlg::OnBnClickedSetDDCFreq()
{
	UpdateData(TRUE);
	double dFreqDDC = 0.0;
	str2freq(m_strFreqDDC,dFreqDDC);
	STACHKV(m_pSP2401->SetDDC(dFreqDDC));
}

void CBBDlg::OnBnClickedGetADManual()
{
	UpdateData(TRUE);
	int64_t iAD = 0;
	STACHKV(m_pSP1401->GetADS5474Manual(iAD));
	m_iADManual = LONG64(iAD);
	m_dPowerManual = round(ad2dBc(_0dBFS,m_iADManual),3);
	m_iRxPowerCompensate = (int)(_0dBFS - iAD);
	UpdateData(FALSE);
}

void CBBDlg::OnBnClickedSetRxPowerCompensate()
{
	UpdateData(TRUE);
	STACHKV(m_pSP2401->SetRxPowerComp(int32_t(m_iRxPowerCompensate)));
}

void CBBDlg::OnBnClickedSetDUCDDS()
{
	UpdateData(TRUE);
	double dFreq = 0.0;
	str2freq(m_strFreqDUSDDS,dFreq);
	STACHKV(m_pSP2401->SetDUCDDS(dFreq));
}

void CBBDlg::OnBnClickedSetDAClock()
{
	UpdateData(TRUE);
	double dFreqMHz = 0.0;
	str2freq(m_strDAClock,dFreqMHz);
	STACHKV(m_pSP2401->SetDAFreqMHz(dFreqMHz));
}

void CBBDlg::OnBnClickedSetDASwitch()
{
	UpdateData(TRUE);
	CSP2401R1A::ADAPort Port = (0 == m_DASwitch.GetCurSel() ? CSP2401R1A::FromRf : CSP2401R1A::FromBb);
	STACHKV(m_pSP2401->SetDASw(Port));
}

void CBBDlg::OnBnClickedSetDAReset()
{
	STACHKV(m_pSP2401->DAReset());
	OnBnClickedSetDAClock();
}

void CBBDlg::OnBnClickedSetADClock()
{
	UpdateData(TRUE);
	double dFreqMHz = 0.0;
	str2freq(m_strADClock,dFreqMHz);
	STACHKV(m_pSP2401->SetADFreqMHz(dFreqMHz));
}

void CBBDlg::OnBnClickedSetADSwitch()
{
	UpdateData(TRUE);
	CSP2401R1A::ADAPort Port = (0 == m_ADSwitch.GetCurSel() ? CSP2401R1A::FromRf : CSP2401R1A::FromBb);
	STACHKV(m_pSP2401->SetADSw(Port));
}

void CBBDlg::OnBnClickedGetADFpga()
{
	STACHKV(m_pSP1401->GetADS5474(m_iADFpga));
	UpdateData(FALSE);
}

void CBBDlg::OnBnClickedSetDDSSrc()
{
	UpdateData(TRUE);
	CSP2401R1A::DASrc Source = (CSP2401R1A::DASrc)(m_DDSSource.GetCurSel());
	STACHKV(m_pSP2401->SetDDSSrc(Source));
}

void CBBDlg::OnBnClickedSetTxFilter()
{
	UpdateData(TRUE);
	int iIdxStar = 0,iIdxStop = 0;
	CString strCoef;
	double dCoefReal[TX_FILTER_ORDER_2I],dCoefImag[TX_FILTER_ORDER_2I];
	int i = 0;
	while ((iIdxStop = m_strTxFilterCoefReal.Find(',',iIdxStar)) != -1) {
		strCoef = m_strTxFilterCoefReal.Mid(iIdxStar,iIdxStop - iIdxStar);
		dCoefReal[i] = atof(strCoef);
		iIdxStar = iIdxStop + 1;
		i ++;
		if (TX_FILTER_ORDER_2I - 1 == i)
			break;
	}
	strCoef = m_strTxFilterCoefReal.Mid(iIdxStar);
	dCoefReal[i] = atof(strCoef);
	
	iIdxStar = iIdxStop = 0;
	i = 0;
	while ((iIdxStop = m_strTxFilterCoefImag.Find(',',iIdxStar)) != -1) {
		strCoef = m_strTxFilterCoefImag.Mid(iIdxStar,iIdxStop - iIdxStar);
		dCoefImag[i] = atof(strCoef);
		iIdxStar = iIdxStop + 1;
		i ++;
		if (TX_FILTER_ORDER_2I - 1 == i)
			break;
	}
	strCoef = m_strTxFilterCoefImag.Mid(iIdxStar);
	dCoefImag[i] = atof(strCoef);
	
	bool bByPass = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK3)))->GetCheck());
	m_pSP2401->SetTxFilterSw(!bByPass);
	m_pSP2401->SetTxFilter(dCoefReal,dCoefImag);
}

void CBBDlg::OnBnClickedSetRxFilter()
{
	UpdateData(TRUE);
	int iIdxStar = 0,iIdxStop = 0;
	CString strCoef;
	int i = 0;
	double dCoefReal[RX_FILTER_ORDER],dCoefImag[RX_FILTER_ORDER];

	while ((iIdxStop = m_strRxFilterCoefReal.Find(',',iIdxStar)) != -1) {
		strCoef = m_strRxFilterCoefReal.Mid(iIdxStar,iIdxStop - iIdxStar);
		dCoefReal[i] = atof(strCoef);
		iIdxStar = iIdxStop + 1;
		i ++;
		if (RX_FILTER_ORDER - 1 == i)
			break;
	}
	strCoef = m_strRxFilterCoefReal.Mid(iIdxStar);
	dCoefReal[i] = atof(strCoef);

	iIdxStar = iIdxStop = 0;
	i = 0;
	while ((iIdxStop = m_strRxFilterCoefImag.Find(',',iIdxStar)) != -1) {
		strCoef = m_strRxFilterCoefImag.Mid(iIdxStar,iIdxStop - iIdxStar);
		dCoefImag[i] = atof(strCoef);
		iIdxStar = iIdxStop + 1;
		i ++;
		if (RX_FILTER_ORDER - 1 == i)
			break;
	}
	strCoef = m_strRxFilterCoefImag.Mid(iIdxStar);
	dCoefImag[i] = atof(strCoef);

	m_pSP2401->SetRxFilter(dCoefReal,dCoefImag);
	if ((0 == m_bRxFilter) && (1 == m_RxFilter80.GetCurSel()))
		m_pSP2401->SetRxFilterSw(CSP2401R1A::_3D);
	else
		m_pSP2401->SetRxFilterSw(CSP2401R1A::_2I3D);
}

void CBBDlg::OnBnClickedGetPowerManual()
{
	OnBnClickedGetADManual();
}

void CBBDlg::OnBnClickedSelRxFilter80M()
{
	m_RxFilter80.EnableWindow(TRUE);
	m_RxFilter160.EnableWindow(FALSE);
}

void CBBDlg::OnBnClickedSelRxFilter160M()
{
	m_RxFilter80.EnableWindow(FALSE);
	m_RxFilter160.EnableWindow(TRUE);
}

void CBBDlg::OnEnChangeDUCDDS()
{
	UpdateData(TRUE);
	double dFreq = 0.0;
	str2freq(m_strFreqDUSDDS,dFreq);
	STACHKV(m_pSP2401->SetDUCDDS(dFreq));
}

void CBBDlg::OnEnChangeDDS1()
{
	UpdateData(TRUE);
	double dFreq = 0.0;
	str2freq(m_strFreqDDS1,dFreq);
	STACHKV(m_pSP2401->SetDDS1(dFreq));
}

void CBBDlg::OnEnChangeDDS2()
{
	UpdateData(TRUE);
	double dFreq = 0.0;
	str2freq(m_strFreqDDS2,dFreq);
	STACHKV(m_pSP2401->SetDDS2(dFreq));
}

void CBBDlg::OnBnClickedSetTxFilterSw()
{
	bool bByPass = (BST_CHECKED == ((CButton *)(GetDlgItem(IDC_CHECK3)))->GetCheck());
	STACHKV(m_pSP2401->SetTxFilterSw(!bByPass));
}
