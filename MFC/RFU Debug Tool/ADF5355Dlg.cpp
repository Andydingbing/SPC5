#include "stdafx.h"
#include "RFU_Debug_Tool.h"
#include "ADF5355Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CADF5355Dlg, CDialog)

CADF5355Dlg::CADF5355Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CADF5355Dlg::IDD, pParent)
	, m_pSP1401R1A(NULL)
	, m_pSP1401R1C(NULL)
	, m_strVCOFreq(_T("8G"))
	, m_strRefFreq(_T("100M"))
	, m_bRefDoubler(FALSE)
	, m_bRefDivider(FALSE)
	, m_uiRCounter(1)
	, m_bAutoCal(TRUE)
	, m_uiPhaseValue(1)
	, m_bPhaseAdjust(FALSE)
	, m_bPhaseResync(FALSE)
	, m_bCounterReset(TRUE)
	, m_bCPThreeState(FALSE)
	, m_bPowerDown(FALSE)
	, m_bDoubleBuff(FALSE)
	, m_bRDIV2(FALSE)
	, m_bRefDoublerSame(FALSE)
	, m_bRFOutA(TRUE)
	, m_uiCPBleedCurrent(5)
	, m_bNegativeBleed(TRUE)
	, m_bGatedBleed(FALSE)
	, m_bLOLMode(FALSE)
	, m_uiTimeout(200)
	, m_uiVCOBandDivision(40)
	, m_bADCEnable(TRUE)
	, m_bADCConversion(TRUE)
	, m_uiADCClockDivider(150)
	, m_uiResyncClock(20000)
	, m_strINT(_T(""))
	, m_strFRAC1(_T(""))
	, m_strFRAC2(_T(""))
	, m_strMOD1(_T(""))
	, m_strMOD2(_T(""))
	, m_strReg0_R(_T(""))
	, m_strReg1_R(_T(""))
	, m_strReg2_R(_T(""))
	, m_strReg3_R(_T(""))
	, m_strReg4_R(_T(""))
	, m_strReg5_R(_T(""))
	, m_strReg6_R(_T(""))
	, m_strReg7_R(_T(""))
	, m_strReg8_R(_T(""))
	, m_strReg9_R(_T(""))
	, m_strReg10_R(_T(""))
	, m_strReg11_R(_T(""))
	, m_strReg12_R(_T(""))
	, m_strReg0_W(_T(""))
	, m_strReg1_W(_T(""))
	, m_strReg2_W(_T(""))
	, m_strReg3_W(_T(""))
	, m_strReg4_W(_T(""))
	, m_strReg5_W(_T(""))
	, m_strReg6_W(_T(""))
	, m_strReg7_W(_T(""))
	, m_strReg8_W(_T(""))
	, m_strReg9_W(_T(""))
	, m_strReg10_W(_T(""))
	, m_strReg11_W(_T(""))
	, m_strReg12_W(_T(""))
	, m_bUseHexDirectly(FALSE)
{

}

CADF5355Dlg::~CADF5355Dlg()
{
}

void CADF5355Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strVCOFreq);
	DDX_Text(pDX, IDC_EDIT11, m_strRefFreq);
	DDX_Check(pDX, IDC_CHECK1, m_bRefDoubler);
	DDX_Check(pDX, IDC_CHECK2, m_bRefDivider);
	DDX_Text(pDX, IDC_EDIT2, m_uiRCounter);
	DDX_Control(pDX, IDC_COMBO1, m_Prescaler);
	DDX_Check(pDX, IDC_CHECK5, m_bAutoCal);
	DDX_Text(pDX, IDC_EDIT3, m_uiPhaseValue);
	DDX_Check(pDX, IDC_CHECK6, m_bPhaseAdjust);
	DDX_Check(pDX, IDC_CHECK7, m_bPhaseResync);
	DDX_Control(pDX, IDC_COMBO20, m_SDLoadReset);
	DDX_Check(pDX, IDC_CHECK8, m_bCounterReset);
	DDX_Check(pDX, IDC_CHECK9, m_bCPThreeState);
	DDX_Check(pDX, IDC_CHECK10, m_bPowerDown);
	DDX_Control(pDX, IDC_COMBO21, m_PDPolarity);
	DDX_Control(pDX, IDC_COMBO22, m_MuxLogic);
	DDX_Control(pDX, IDC_COMBO23, m_RefMode);
	DDX_Control(pDX, IDC_COMBO24, m_CurrentSetting);
	DDX_Check(pDX, IDC_CHECK11, m_bDoubleBuff);
	DDX_Check(pDX, IDC_CHECK12, m_bRDIV2);
	DDX_Check(pDX, IDC_CHECK13, m_bRefDoublerSame);
	DDX_Control(pDX, IDC_COMBO25, m_MuxOut);
	DDX_Control(pDX, IDC_COMBO26, m_RFOutputPower);
	DDX_Check(pDX, IDC_CHECK14, m_bRFOutA);
	DDX_Control(pDX, IDC_COMBO27, m_RFOutB);
	DDX_Control(pDX, IDC_COMBO28, m_MTLD);
	DDX_Text(pDX, IDC_EDIT27, m_uiCPBleedCurrent);
	DDX_Control(pDX, IDC_COMBO29, m_RFDividerSel);
	DDX_Control(pDX, IDC_COMBO30, m_FeedbackSel);
	DDX_Check(pDX, IDC_CHECK15, m_bNegativeBleed);
	DDX_Check(pDX, IDC_CHECK16, m_bGatedBleed);
	DDX_Control(pDX, IDC_COMBO31, m_LDMode);
	DDX_Control(pDX, IDC_COMBO32, m_FracN_LDPrecision);
	DDX_Check(pDX, IDC_CHECK17, m_bLOLMode);
	DDX_Control(pDX, IDC_COMBO33, m_LDCycleCount);
	DDX_Control(pDX, IDC_COMBO34, m_LeSync);
	DDX_Control(pDX, IDC_COMBO35, m_SynthesizerLockTimeout);
	DDX_Control(pDX, IDC_COMBO36, m_AutomaticLevelTimeout);
	DDX_Text(pDX, IDC_EDIT33, m_uiTimeout);
	DDX_Text(pDX, IDC_EDIT34, m_uiVCOBandDivision);
	DDX_Check(pDX, IDC_CHECK18, m_bADCEnable);
	DDX_Check(pDX, IDC_CHECK19, m_bADCConversion);
	DDX_Text(pDX, IDC_EDIT35, m_uiADCClockDivider);
	DDX_Text(pDX, IDC_EDIT36, m_uiResyncClock);
	DDX_Text(pDX, IDC_EDIT30, m_strINT);
	DDX_Text(pDX, IDC_EDIT31, m_strFRAC1);
	DDX_Text(pDX, IDC_EDIT37, m_strFRAC2);
	DDX_Text(pDX, IDC_EDIT38, m_strMOD1);
	DDX_Text(pDX, IDC_EDIT39, m_strMOD2);
	DDX_Text(pDX, IDC_EDIT40, m_strReg0_R);
	DDX_Text(pDX, IDC_EDIT41, m_strReg1_R);
	DDX_Text(pDX, IDC_EDIT42, m_strReg2_R);
	DDX_Text(pDX, IDC_EDIT43, m_strReg3_R);
	DDX_Text(pDX, IDC_EDIT44, m_strReg4_R);
	DDX_Text(pDX, IDC_EDIT45, m_strReg5_R);
	DDX_Text(pDX, IDC_EDIT46, m_strReg6_R);
	DDX_Text(pDX, IDC_EDIT47, m_strReg7_R);
	DDX_Text(pDX, IDC_EDIT48, m_strReg8_R);
	DDX_Text(pDX, IDC_EDIT49, m_strReg9_R);
	DDX_Text(pDX, IDC_EDIT50, m_strReg10_R);
	DDX_Text(pDX, IDC_EDIT51, m_strReg11_R);
	DDX_Text(pDX, IDC_EDIT52, m_strReg12_R);
	DDX_Control(pDX, IDC_COMBO37, m_ApplyMode);
	DDX_Text(pDX, IDC_EDIT53, m_strReg0_W);
	DDX_Text(pDX, IDC_EDIT54, m_strReg1_W);
	DDX_Text(pDX, IDC_EDIT55, m_strReg2_W);
	DDX_Text(pDX, IDC_EDIT56, m_strReg3_W);
	DDX_Text(pDX, IDC_EDIT57, m_strReg4_W);
	DDX_Text(pDX, IDC_EDIT58, m_strReg5_W);
	DDX_Text(pDX, IDC_EDIT59, m_strReg6_W);
	DDX_Text(pDX, IDC_EDIT60, m_strReg7_W);
	DDX_Text(pDX, IDC_EDIT61, m_strReg8_W);
	DDX_Text(pDX, IDC_EDIT62, m_strReg9_W);
	DDX_Text(pDX, IDC_EDIT63, m_strReg10_W);
	DDX_Text(pDX, IDC_EDIT64, m_strReg11_W);
	DDX_Text(pDX, IDC_EDIT65, m_strReg12_W);
	DDX_Check(pDX, IDC_CHECK20, m_bUseHexDirectly);
}


BEGIN_MESSAGE_MAP(CADF5355Dlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON33, &CADF5355Dlg::OnBnClickedApply)
	ON_BN_CLICKED(IDC_CHECK1, &CADF5355Dlg::OnBnClickedChangeRefDoubler)
	ON_BN_CLICKED(IDC_BUTTON40, &CADF5355Dlg::OnBnClickedSetReg0)
	ON_BN_CLICKED(IDC_BUTTON41, &CADF5355Dlg::OnBnClickedSetReg1)
	ON_BN_CLICKED(IDC_BUTTON42, &CADF5355Dlg::OnBnClickedSetReg2)
	ON_BN_CLICKED(IDC_BUTTON43, &CADF5355Dlg::OnBnClickedSetReg3)
	ON_BN_CLICKED(IDC_BUTTON44, &CADF5355Dlg::OnBnClickedSetReg4)
	ON_BN_CLICKED(IDC_BUTTON45, &CADF5355Dlg::OnBnClickedSetReg5)
	ON_BN_CLICKED(IDC_BUTTON46, &CADF5355Dlg::OnBnClickedSetReg6)
	ON_BN_CLICKED(IDC_BUTTON47, &CADF5355Dlg::OnBnClickedSetReg7)
	ON_BN_CLICKED(IDC_BUTTON48, &CADF5355Dlg::OnBnClickedSetReg8)
	ON_BN_CLICKED(IDC_BUTTON49, &CADF5355Dlg::OnBnClickedSetReg9)
	ON_BN_CLICKED(IDC_BUTTON50, &CADF5355Dlg::OnBnClickedSetReg10)
	ON_BN_CLICKED(IDC_BUTTON51, &CADF5355Dlg::OnBnClickedSetReg11)
	ON_BN_CLICKED(IDC_BUTTON52, &CADF5355Dlg::OnBnClickedSetReg12)
	ON_BN_CLICKED(IDC_CHECK13, &CADF5355Dlg::OnBnClickedChangeRefDoublerSame)
	ON_EN_CHANGE(IDC_EDIT1, &CADF5355Dlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT11, &CADF5355Dlg::OnEnChangeEdit11)
	ON_BN_CLICKED(IDC_CHECK2, &CADF5355Dlg::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_EDIT2, &CADF5355Dlg::OnEnChangeEdit2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CADF5355Dlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK5, &CADF5355Dlg::OnBnClickedCheck5)
	ON_EN_CHANGE(IDC_EDIT3, &CADF5355Dlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_CHECK6, &CADF5355Dlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CADF5355Dlg::OnBnClickedCheck7)
	ON_CBN_SELCHANGE(IDC_COMBO20, &CADF5355Dlg::OnCbnSelchangeCombo20)
	ON_BN_CLICKED(IDC_CHECK8, &CADF5355Dlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CADF5355Dlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CADF5355Dlg::OnBnClickedCheck10)
	ON_CBN_SELCHANGE(IDC_COMBO21, &CADF5355Dlg::OnCbnSelchangeCombo21)
	ON_CBN_SELCHANGE(IDC_COMBO22, &CADF5355Dlg::OnCbnSelchangeCombo22)
	ON_CBN_SELCHANGE(IDC_COMBO23, &CADF5355Dlg::OnCbnSelchangeCombo23)
	ON_CBN_SELCHANGE(IDC_COMBO24, &CADF5355Dlg::OnCbnSelchangeCombo24)
	ON_BN_CLICKED(IDC_CHECK11, &CADF5355Dlg::OnBnClickedCheck11)
	ON_BN_CLICKED(IDC_CHECK12, &CADF5355Dlg::OnBnClickedCheck12)
	ON_CBN_SELCHANGE(IDC_COMBO25, &CADF5355Dlg::OnCbnSelchangeCombo25)
	ON_CBN_SELCHANGE(IDC_COMBO26, &CADF5355Dlg::OnCbnSelchangeCombo26)
	ON_BN_CLICKED(IDC_CHECK14, &CADF5355Dlg::OnBnClickedCheck14)
	ON_CBN_SELCHANGE(IDC_COMBO27, &CADF5355Dlg::OnCbnSelchangeCombo27)
	ON_CBN_SELCHANGE(IDC_COMBO28, &CADF5355Dlg::OnCbnSelchangeCombo28)
	ON_EN_CHANGE(IDC_EDIT27, &CADF5355Dlg::OnEnChangeEdit27)
	ON_CBN_SELCHANGE(IDC_COMBO29, &CADF5355Dlg::OnCbnSelchangeCombo29)
	ON_CBN_SELCHANGE(IDC_COMBO30, &CADF5355Dlg::OnCbnSelchangeCombo30)
	ON_BN_CLICKED(IDC_CHECK15, &CADF5355Dlg::OnBnClickedCheck15)
	ON_BN_CLICKED(IDC_CHECK16, &CADF5355Dlg::OnBnClickedCheck16)
	ON_CBN_SELCHANGE(IDC_COMBO31, &CADF5355Dlg::OnCbnSelchangeCombo31)
	ON_CBN_SELCHANGE(IDC_COMBO32, &CADF5355Dlg::OnCbnSelchangeCombo32)
	ON_BN_CLICKED(IDC_CHECK17, &CADF5355Dlg::OnBnClickedCheck17)
	ON_CBN_SELCHANGE(IDC_COMBO33, &CADF5355Dlg::OnCbnSelchangeCombo33)
	ON_CBN_SELCHANGE(IDC_COMBO34, &CADF5355Dlg::OnCbnSelchangeCombo34)
	ON_CBN_SELCHANGE(IDC_COMBO35, &CADF5355Dlg::OnCbnSelchangeCombo35)
	ON_CBN_SELCHANGE(IDC_COMBO36, &CADF5355Dlg::OnCbnSelchangeCombo36)
	ON_EN_CHANGE(IDC_EDIT33, &CADF5355Dlg::OnEnChangeEdit33)
	ON_EN_CHANGE(IDC_EDIT34, &CADF5355Dlg::OnEnChangeEdit34)
	ON_BN_CLICKED(IDC_CHECK18, &CADF5355Dlg::OnBnClickedCheck18)
	ON_BN_CLICKED(IDC_CHECK19, &CADF5355Dlg::OnBnClickedCheck19)
	ON_EN_CHANGE(IDC_EDIT35, &CADF5355Dlg::OnEnChangeEdit35)
	ON_EN_CHANGE(IDC_EDIT36, &CADF5355Dlg::OnEnChangeEdit36)
	ON_BN_CLICKED(IDC_BUTTON61, &CADF5355Dlg::OnBnClickedRegR_RegW_0)
	ON_BN_CLICKED(IDC_BUTTON68, &CADF5355Dlg::OnBnClickedRegR_RegW_1)
	ON_BN_CLICKED(IDC_BUTTON69, &CADF5355Dlg::OnBnClickedRegR_RegW_2)
	ON_BN_CLICKED(IDC_BUTTON70, &CADF5355Dlg::OnBnClickedRegR_RegW_3)
	ON_BN_CLICKED(IDC_BUTTON71, &CADF5355Dlg::OnBnClickedRegR_RegW_4)
	ON_BN_CLICKED(IDC_BUTTON72, &CADF5355Dlg::OnBnClickedRegR_RegW_5)
	ON_BN_CLICKED(IDC_BUTTON73, &CADF5355Dlg::OnBnClickedRegR_RegW_6)
	ON_BN_CLICKED(IDC_BUTTON74, &CADF5355Dlg::OnBnClickedRegR_RegW_7)
	ON_BN_CLICKED(IDC_BUTTON75, &CADF5355Dlg::OnBnClickedRegR_RegW_8)
	ON_BN_CLICKED(IDC_BUTTON76, &CADF5355Dlg::OnBnClickedRegR_RegW_9)
	ON_BN_CLICKED(IDC_BUTTON77, &CADF5355Dlg::OnBnClickedRegR_RegW_10)
	ON_BN_CLICKED(IDC_BUTTON78, &CADF5355Dlg::OnBnClickedRegR_RegW_11)
	ON_BN_CLICKED(IDC_BUTTON79, &CADF5355Dlg::OnBnClickedRegR_RegW_12)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CADF5355Dlg::UpdateREG_R(uint32_t reg[])
{
	REG_R(0).Format("%#010x",reg[0]);
	REG_R(1).Format("%#010x",reg[1]);
	REG_R(2).Format("%#010x",reg[2]);
	REG_R(3).Format("%#010x",reg[3]);
	REG_R(4).Format("%#010x",reg[4]);
	REG_R(5).Format("%#010x",reg[5]);
	REG_R(6).Format("%#010x",reg[6]);
	REG_R(7).Format("%#010x",reg[7]);
	REG_R(8).Format("%#010x",reg[8]);
	REG_R(9).Format("%#010x",reg[9]);
	REG_R(10).Format("%#010x",reg[10]);
	REG_R(11).Format("%#010x",reg[11]);
	REG_R(12).Format("%#010x",reg[12]);
}

void CADF5355Dlg::UpdateREG_W(uint32_t reg[])
{
	REG_W(0).Format("%#010x",reg[0]);
	REG_W(1).Format("%#010x",reg[1]);
	REG_W(2).Format("%#010x",reg[2]);
	REG_W(3).Format("%#010x",reg[3]);
	REG_W(4).Format("%#010x",reg[4]);
	REG_W(5).Format("%#010x",reg[5]);
	REG_W(6).Format("%#010x",reg[6]);
	REG_W(7).Format("%#010x",reg[7]);
	REG_W(8).Format("%#010x",reg[8]);
	REG_W(9).Format("%#010x",reg[9]);
	REG_W(10).Format("%#010x",reg[10]);
	REG_W(11).Format("%#010x",reg[11]);
	REG_W(12).Format("%#010x",reg[12]);
}

void CADF5355Dlg::OnBnClickedApply()
{
	UpdateData(TRUE);
	DECLARE_REG_AND_BIT_CTRL_MEM();

	if (m_bUseHexDirectly) {
		sscanf(REG_W(0),"%x",&uiReg[0]);
		sscanf(REG_W(1),"%x",&uiReg[1]);
		sscanf(REG_W(2),"%x",&uiReg[2]);
		sscanf(REG_W(3),"%x",&uiReg[3]);
		sscanf(REG_W(4),"%x",&uiReg[4]);
		sscanf(REG_W(5),"%x",&uiReg[5]);
		sscanf(REG_W(6),"%x",&uiReg[6]);
		sscanf(REG_W(7),"%x",&uiReg[7]);
		sscanf(REG_W(8),"%x",&uiReg[8]);
		sscanf(REG_W(9),"%x",&uiReg[9]);
		sscanf(REG_W(10),"%x",&uiReg[10]);
		sscanf(REG_W(11),"%x",&uiReg[11]);
		sscanf(REG_W(12),"%x",&uiReg[12]);
	}
	else {
		Parameter2Reg;
	}

	UpdateREG_R(uiReg);
	UpdateREG_W(uiReg);
	UpdateData(FALSE);
	
	if (m_pSP1401R1A) {
		DECLARE_R1A_SETLOREG_FUNCTION_MAP();
		if (m_ApplyMode.GetCurSel() == 0) {
			for (int i = 0;i < 13;i ++)
				(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[12 - i]);
		}
		else {
			for (int i = 0;i < 13;i ++)
				(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[12 - i]);
			uiReg[4] = uiReg[4] | (1 << 4);
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[4]);
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[2]);
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[1]);
			uiReg[0] = uiReg[0] & (~(1 << 21));
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[0]);
			uiReg[4] = uiReg[4] & (~(1 << 4));
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[4]);
			Sleep(10);
			uiReg[0] = uiReg[0] | (1 << 21);
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(uiReg[0]);
		}
	}
	if (m_pSP1401R1C) {
		DECLARE_R1C_SETLOREG_FUNCTION_MAP();
		if (m_ApplyMode.GetCurSel() == 0) {
			for (int i = 0;i < 13;i ++)
				(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[12 - i]);
		}
		else {
			for (int i = 0;i < 13;i ++)
				(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[12 - i]);
			uiReg[4] = uiReg[4] | (1 << 4);
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[4]);
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[2]);
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[1]);
			uiReg[0] = uiReg[0] & (~(1 << 21));
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[0]);
			uiReg[4] = uiReg[4] & (~(1 << 4));
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[4]);
			Sleep(10);
			uiReg[0] = uiReg[0] | (1 << 21);
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(uiReg[0]);
		}
	}
	SetFocus();
}

void CADF5355Dlg::OnBnClickedChangeRefDoubler()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	UpdateData(TRUE);
	Parameter2Reg;
	m_bRefDoublerSame = m_bRefDoubler;
	if (m_bRefDoubler == TRUE)
		m_bRefDivider = !m_bRefDoubler;
	UpdateData(FALSE);
}

BOOL CADF5355Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ApplyMode.SetCurSel(1);
	m_Prescaler.SetCurSel(0);
	m_SDLoadReset.SetCurSel(0);
	m_PDPolarity.SetCurSel(1);
	m_MuxLogic.SetCurSel(1);
	m_RefMode.SetCurSel(1);
	m_CurrentSetting.SetCurSel(0);
	m_MuxOut.SetCurSel(6);
	m_RFOutputPower.SetCurSel(3);
	m_RFOutB.SetCurSel(1);
	m_MTLD.SetCurSel(0);
	m_RFDividerSel.SetCurSel(0);
	m_FeedbackSel.SetCurSel(1);
	m_LDMode.SetCurSel(0);
	m_FracN_LDPrecision.SetCurSel(3);
	m_LDCycleCount.SetCurSel(0);
	m_LeSync.SetCurSel(1);
	m_SynthesizerLockTimeout.SetCurSel(30);
	m_AutomaticLevelTimeout.SetCurSel(30);
	SetWindowTextX();

	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
	return TRUE;
}

void CADF5355Dlg::SetWindowTextX()
{
	if (!::IsWindow(m_hWnd))
		return;

	CString strWndText;
	uint32_t uiRfuIdx = 0,uiRfIdx = 0;
	char szLOName[8];
	memset(szLOName,0,sizeof(szLOName));
	if (m_pSP1401R1A) {
		switch (m_iLOIdx) {
			case 0 : {strcpy(szLOName,"Tx_LO1");break;}
			case 1 : {strcpy(szLOName,"Tx_LO2");break;}
			case 2 : {strcpy(szLOName,"Rx_LO1");break;}
			case 3 : {strcpy(szLOName,"Rx_LO2");break;}
		}
		uiRfIdx = m_pSP1401R1A->GetRfIdx();
		uiRfuIdx = m_pSP1401R1A->GetRfuIdx();
		strWndText.Format("R1A_ADF5355_RFU%d_RF%d_%s",uiRfuIdx,uiRfIdx,szLOName);
	}
	if (m_pSP1401R1C) {
		switch (m_iLOIdx) {
			case 0 : {break;}
			case 1 : {strcpy(szLOName,"Tx_LO2");break;}
			case 2 : {strcpy(szLOName,"Tx_LO3");break;}
			case 3 : {strcpy(szLOName,"Rx_LO1");break;}
		}
		uiRfIdx = m_pSP1401R1C->GetRfIdx();
		uiRfuIdx = m_pSP1401R1C->GetRfuIdx();
		strWndText.Format("R1C_ADF5355_RFU%d_RF%d_%s",uiRfuIdx,uiRfIdx,szLOName);
	}
	return SetWindowText(strWndText);
}

BOOL CADF5355Dlg::ShowWindow(int nCmdShow)
{
	SetWindowTextX();
	return CDialog::ShowWindow(nCmdShow);
}

void CADF5355Dlg::OnBnClickedSetReg0()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(0),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg1()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(1),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg2()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(2),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg3()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(3),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg4()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(4),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg5()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(5),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg6()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(6),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg7()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(7),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg8()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(8),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg9()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(9),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg10()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(10),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg11()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(11),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedSetReg12()
{
	UpdateData(TRUE);
	if (m_bUseHexDirectly) {
		uint32_t reg = 0;
		sscanf(REG_W(12),"%x",&reg);
		if (m_pSP1401R1A) {
			DECLARE_R1A_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1A->*Set_R1A_LO_Reg[m_iLOIdx])(reg);
		}
		if (m_pSP1401R1C) {
			DECLARE_R1C_SETLOREG_FUNCTION_MAP();
			(m_pSP1401R1C->*Set_R1C_LO_Reg[m_iLOIdx])(reg);
		}
		SetFocus();
	}
}

void CADF5355Dlg::OnBnClickedChangeRefDoublerSame()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	UpdateData(TRUE);
	Parameter2Reg;
	m_bRefDoubler = m_bRefDoublerSame;
	if (m_bRefDoublerSame == TRUE)
		m_bRefDivider = !m_bRefDoublerSame;
	UpdateData(FALSE);
}

BOOL CADF5355Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void CADF5355Dlg::OnEnChangeEdit1()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit11()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck2()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	UpdateData(TRUE);
	if (m_bRefDivider == TRUE) {
		m_bRefDoubler = !m_bRefDivider;
		m_bRefDoublerSame = !m_bRefDivider;
	}
	UpdateData(FALSE);
	Parameter2Reg;
	UpdateData(FALSE);
}

void CADF5355Dlg::OnEnChangeEdit2()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo1()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck5()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit3()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck6()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck7()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo20()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck8()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck9()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck10()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo21()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo22()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo23()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo24()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck11()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck12()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo25()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo26()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck14()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo27()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo28()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit27()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo29()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo30()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck15()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck16()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo31()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo32()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck17()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo33()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo34()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo35()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnCbnSelchangeCombo36()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit33()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit34()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck18()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedCheck19()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit35()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnEnChangeEdit36()
{
	DECLARE_REG_AND_BIT_CTRL_MEM();
	Parameter2Reg;
}

void CADF5355Dlg::OnBnClickedRegR_RegW_0()
{
	UpdateData(TRUE);
	REG_W(0) = REG_R(0);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_1()
{
	UpdateData(TRUE);
	REG_W(1) = REG_R(1);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_2()
{
	UpdateData(TRUE);
	REG_W(2) = REG_R(2);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_3()
{
	UpdateData(TRUE);
	REG_W(3) = REG_R(3);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_4()
{
	UpdateData(TRUE);
	REG_W(4) = REG_R(4);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_5()
{
	UpdateData(TRUE);
	REG_W(5) = REG_R(5);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_6()
{
	UpdateData(TRUE);
	REG_W(6) = REG_R(6);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_7()
{
	UpdateData(TRUE);
	REG_W(7) = REG_R(7);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_8()
{
	UpdateData(TRUE);
	REG_W(8) = REG_R(8);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_9()
{
	UpdateData(TRUE);
	REG_W(9) = REG_R(9);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_10()
{
	UpdateData(TRUE);
	REG_W(10) = REG_R(10);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_11()
{
	UpdateData(TRUE);
	REG_W(11) = REG_R(11);
	UpdateData(FALSE);
}

void CADF5355Dlg::OnBnClickedRegR_RegW_12()
{
	UpdateData(TRUE);
	REG_W(12) = REG_R(12);
	UpdateData(FALSE);
}

HBRUSH CADF5355Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_1000:
		{
			pDC->SetTextColor(RGB(255,0,0));
			return (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
			break;
		}
	case IDC_STATIC_1001:
	case IDC_STATIC_1002:
		{
			pDC->SetTextColor(RGB(0,0,255));
			return (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
			break;
		}
	case IDC_STATIC_1003:
		{
			pDC->SetTextColor(RGB(128,0,0));
			return (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
			break;
		}
	default:
		return hbr;
	}
	return hbr;
}
