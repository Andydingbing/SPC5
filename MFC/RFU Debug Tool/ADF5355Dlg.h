#pragma once

#define Parameter2Reg012				\
	uint64_t uiVCOFreq = 0;				\
	str2freq(m_strVCOFreq,uiVCOFreq);	\
	uint64_t uiRefFreq = 0;				\
	uint32_t _INT = 0;					\
	uint32_t _FRAC1 = 0;				\
	uint32_t _FRAC2 = 0;				\
	uint32_t _MOD1 = 0;					\
	uint32_t _MOD2 = 0;					\
										\
	str2freq(m_strRefFreq,uiRefFreq);	\
	adf5355_freq_formula(uiVCOFreq,uint32_t(uiRefFreq),m_bRefDoubler,m_bRefDivider,m_uiRCounter,ADF5355_FREQ_SPACE,_INT,_FRAC1,_FRAC2,_MOD1,_MOD2);\
	m_strINT.Format("%d",_INT);			\
	m_strFRAC1.Format("%d",_FRAC1);		\
	m_strFRAC2.Format("%d",_FRAC2);		\
	m_strMOD1.Format("%d",_MOD1);		\
	m_strMOD2.Format("%d",_MOD2);		\
	UpdateData(FALSE);					\
										\
	uint32_t uiPrescaler = m_Prescaler.GetCurSel();			\
	uint32_t uiAutocal   = m_bAutoCal ? 1 : 0;				\
	uiReg[0] = adf5355para2reg0(_INT,uiPrescaler,uiAutocal);\
	uiReg[1] = adf5355para2reg1(_FRAC1);					\
	uiReg[2] = adf5355para2reg2(_MOD2,_FRAC2);

#define Parameter2Reg3										\
	uint32_t uiPhaseAdjust = (m_bPhaseAdjust ? 1 : 0);		\
	uint32_t uiPhaseResync = (m_bPhaseResync ? 1 : 0);		\
	uint32_t uiSDLoadReset = (m_SDLoadReset.GetCurSel());	\
	uiReg[3] = adf5355para2reg3(m_uiPhaseValue,uiPhaseAdjust,uiPhaseResync,uiSDLoadReset);

#define Parameter2Reg4											\
	uint32_t uiCounterReset  = (m_bCounterReset ? 1 : 0);		\
	uint32_t uiCPThreeState  = (m_bCPThreeState ? 1 : 0);		\
	uint32_t uiPowerDown     = (m_bPowerDown ? 1 : 0);			\
	uint32_t uiPDPolarity    = (m_PDPolarity.GetCurSel());		\
	uint32_t uiMuxLogic      = (m_MuxLogic.GetCurSel());		\
	uint32_t uiRefMode		 = (m_RefMode.GetCurSel());			\
	uint32_t uiCurrentSet    = (m_CurrentSetting.GetCurSel());	\
	uint32_t uiDoubleBuff    = (m_bDoubleBuff ? 1 : 0);			\
	uint32_t uiRCounter		 = (m_uiRCounter);					\
	uint32_t uiRDIV2		 = (m_bRDIV2 ? 1 : 0);				\
	uint32_t uiRefDoubler	 = (m_bRefDoublerSame ? 1 : 0);		\
	uint32_t uiMuxOut		 = (m_MuxOut.GetCurSel());			\
	uiReg[4] = adf5355para2reg4(uiCounterReset,uiCPThreeState,uiPowerDown,uiPDPolarity,uiMuxLogic,uiRefMode,uiCurrentSet,uiDoubleBuff,uiRCounter,uiRDIV2,uiRefDoubler,uiMuxOut);

#define Parameter2Reg5	\
	uiReg[5] = adf5355para2reg5();

#define Parameter2Reg6											\
	uint32_t uiRFOutputPower = (m_RFOutputPower.GetCurSel());	\
	uint32_t uiRFOutA		 = (m_bRFOutA ? 1 : 0);				\
	uint32_t uiRFOutB    	 = (m_RFOutB.GetCurSel());			\
	uint32_t uiMTLD		     = (m_MTLD.GetCurSel());			\
	uint32_t uiChargePumpBC  = (m_uiCPBleedCurrent);			\
	uint32_t uiRFDividerSel  = (m_RFDividerSel.GetCurSel());	\
	uint32_t uiFeedbackSel   = (m_FeedbackSel.GetCurSel());		\
	uint32_t uiNegativeBleed = (m_bNegativeBleed ? 1 : 0);		\
	uint32_t uiGatedBleed	 = (m_bGatedBleed ? 1 : 0);			\
	uiReg[6] = adf5355para2reg6(uiRFOutputPower,uiRFOutA,uiRFOutB,uiMTLD,uiChargePumpBC,uiRFDividerSel,uiFeedbackSel,uiNegativeBleed,uiGatedBleed);

#define Parameter2Reg7													\
	uint32_t uiLDMode			= (m_LDMode.GetCurSel());				\
	uint32_t uiFracN_LDPrecison = (m_FracN_LDPrecision.GetCurSel());	\
	uint32_t uiLOLMode		    = (m_bLOLMode ? 1 : 0);					\
	uint32_t uiLDCycleCount	    = (m_LDCycleCount.GetCurSel());			\
	uint32_t uiLeSync			= (m_LeSync.GetCurSel());				\
	uiReg[7] = adf5355para2reg7(uiLDMode,uiFracN_LDPrecison,uiLOLMode,uiLDCycleCount,uiLeSync);

#define Parameter2Reg8	\
	uiReg[8] = adf5355para2reg8();

#define Parameter2Reg9												\
	uint32_t uiSynthLockTO = (m_SynthesizerLockTimeout.GetCurSel());\
	uint32_t uiAutoLevelTO = (m_AutomaticLevelTimeout.GetCurSel());	\
	uint32_t uiTimeout	   = (m_uiTimeout);							\
	uint32_t uiVCOBandDiv  = (m_uiVCOBandDivision);					\
	uiReg[9] = adf5355para2reg9(uiSynthLockTO,uiAutoLevelTO,uiTimeout,uiVCOBandDiv);

#define Parameter2Reg10										\
	uint32_t uiADCEnable	 = (m_bADCEnable ? 1 : 0);		\
	uint32_t uiADCConversion = (m_bADCConversion ? 1 : 0);	\
	uint32_t uiADCClockDiv   = (m_uiADCClockDivider);		\
	uiReg[10] = adf5355para2reg10(uiADCEnable,uiADCConversion,uiADCClockDiv);

#define Parameter2Reg11	\
	uiReg[11] = adf5355para2reg11();

#define Parameter2Reg12							\
	uint32_t uiResyncClock = (m_uiResyncClock);	\
	uiReg[12] = adf5355para2reg12(uiResyncClock);

#define Parameter2Reg	\
	UpdateData(TRUE);	\
	Parameter2Reg012;	\
	Parameter2Reg3;		\
	Parameter2Reg4;		\
	Parameter2Reg5;		\
	Parameter2Reg6;		\
	Parameter2Reg7;		\
	Parameter2Reg8;		\
	Parameter2Reg9;		\
	Parameter2Reg10;	\
	Parameter2Reg11;	\
	Parameter2Reg12;	\
	UpdateREG_R(uiReg);	\
	UpdateData(FALSE)

#define DECLARE_REG_AND_BIT_CTRL_MEM()	\
	uint32_t uiReg[13];					\
	uint32_t uiCtrlBit[13];				\
	for (int32_t i = 0;i < 13;i ++) {	\
		uiReg[i] = 0;					\
		uiCtrlBit[i] = 0;				\
	}

class CADF5355Dlg : public CDialog
{
	DECLARE_DYNAMIC(CADF5355Dlg)

public:
	CADF5355Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CADF5355Dlg();

// 对话框数据
	enum { IDD = IDD_ADF5355 };
public:
	CSP1401R1A *m_pSP1401R1A;
	CSP1401R1C *m_pSP1401R1C;
	int32_t m_iLOIdx;									//R1A : Tx LO1 = 0;Tx LO2 = 1;Rx LO1 = 2;Rx LO2 = 3;
														//R1C : Tx LO1 = 0;Tx LO2 = 1;Tx LO3 = 2;Rx LO1 = 3;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void UpdateREG_R(uint32_t reg[]);
	void UpdateREG_W(uint32_t reg[]);
public:
	CString m_strVCOFreq;
	CString m_strRefFreq;
	BOOL m_bRefDoubler;
	BOOL m_bRefDivider;
	UINT m_uiRCounter;
	CComboBox m_Prescaler;
	BOOL m_bAutoCal;
	UINT m_uiPhaseValue;
	BOOL m_bPhaseAdjust;
	BOOL m_bPhaseResync;
	CComboBox m_SDLoadReset;
	BOOL m_bCounterReset;
	BOOL m_bCPThreeState;
	BOOL m_bPowerDown;
	CComboBox m_PDPolarity;
	CComboBox m_MuxLogic;
	CComboBox m_RefMode;
	CComboBox m_CurrentSetting;
	BOOL m_bDoubleBuff;
	BOOL m_bRDIV2;
	BOOL m_bRefDoublerSame;
	CComboBox m_MuxOut;
	CComboBox m_RFOutputPower;
	BOOL m_bRFOutA;
	CComboBox m_RFOutB;
	CComboBox m_MTLD;
	UINT m_uiCPBleedCurrent;
	CComboBox m_RFDividerSel;
	CComboBox m_FeedbackSel;
	BOOL m_bNegativeBleed;
	BOOL m_bGatedBleed;
	CComboBox m_LDMode;
	CComboBox m_FracN_LDPrecision;
	BOOL m_bLOLMode;
	CComboBox m_LDCycleCount;
	CComboBox m_LeSync;
	CComboBox m_SynthesizerLockTimeout;
	CComboBox m_AutomaticLevelTimeout;
	UINT m_uiTimeout;
	UINT m_uiVCOBandDivision;
	BOOL m_bADCEnable;
	BOOL m_bADCConversion;
	UINT m_uiADCClockDivider;
	UINT m_uiResyncClock;
	CString m_strINT;
	CString m_strFRAC1;
	CString m_strFRAC2;
	CString m_strMOD1;
	CString m_strMOD2;

#define REG_R(index) m_strReg##index##_R
	CString m_strReg0_R;
	CString m_strReg1_R;
	CString m_strReg2_R;
	CString m_strReg3_R;
	CString m_strReg4_R;
	CString m_strReg5_R;
	CString m_strReg6_R;
	CString m_strReg7_R;
	CString m_strReg8_R;
	CString m_strReg9_R;
	CString m_strReg10_R;
	CString m_strReg11_R;
	CString m_strReg12_R;
	CComboBox m_ApplyMode;
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedChangeRefDoubler();
	virtual BOOL OnInitDialog();

#define REG_W(index) m_strReg##index##_W
	CString m_strReg0_W;
	CString m_strReg1_W;
	CString m_strReg2_W;
	CString m_strReg3_W;
	CString m_strReg4_W;
	CString m_strReg5_W;
	CString m_strReg6_W;
	CString m_strReg7_W;
	CString m_strReg8_W;
	CString m_strReg9_W;
	CString m_strReg10_W;
	CString m_strReg11_W;
	CString m_strReg12_W;
	BOOL m_bUseHexDirectly;
	afx_msg void OnBnClickedSetReg0();
	afx_msg void OnBnClickedSetReg1();
	afx_msg void OnBnClickedSetReg2();
	afx_msg void OnBnClickedSetReg3();
	afx_msg void OnBnClickedSetReg4();
	afx_msg void OnBnClickedSetReg5();
	afx_msg void OnBnClickedSetReg6();
	afx_msg void OnBnClickedSetReg7();
	afx_msg void OnBnClickedSetReg8();
	afx_msg void OnBnClickedSetReg9();
	afx_msg void OnBnClickedSetReg10();
	afx_msg void OnBnClickedSetReg11();
	afx_msg void OnBnClickedSetReg12();
	afx_msg void OnBnClickedChangeRefDoublerSame();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnCbnSelchangeCombo20();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnCbnSelchangeCombo21();
	afx_msg void OnCbnSelchangeCombo22();
	afx_msg void OnCbnSelchangeCombo23();
	afx_msg void OnCbnSelchangeCombo24();
	afx_msg void OnBnClickedCheck11();
	afx_msg void OnBnClickedCheck12();
	afx_msg void OnCbnSelchangeCombo25();
	afx_msg void OnCbnSelchangeCombo26();
	afx_msg void OnBnClickedCheck14();
	afx_msg void OnCbnSelchangeCombo27();
	afx_msg void OnCbnSelchangeCombo28();
	afx_msg void OnEnChangeEdit27();
	afx_msg void OnCbnSelchangeCombo29();
	afx_msg void OnCbnSelchangeCombo30();
	afx_msg void OnBnClickedCheck15();
	afx_msg void OnBnClickedCheck16();
	afx_msg void OnCbnSelchangeCombo31();
	afx_msg void OnCbnSelchangeCombo32();
	afx_msg void OnBnClickedCheck17();
	afx_msg void OnCbnSelchangeCombo33();
	afx_msg void OnCbnSelchangeCombo34();
	afx_msg void OnCbnSelchangeCombo35();
	afx_msg void OnCbnSelchangeCombo36();
	afx_msg void OnEnChangeEdit33();
	afx_msg void OnEnChangeEdit34();
	afx_msg void OnBnClickedCheck18();
	afx_msg void OnBnClickedCheck19();
	afx_msg void OnEnChangeEdit35();
	afx_msg void OnEnChangeEdit36();
	afx_msg void OnBnClickedRegR_RegW_0();
	afx_msg void OnBnClickedRegR_RegW_1();
	afx_msg void OnBnClickedRegR_RegW_2();
	afx_msg void OnBnClickedRegR_RegW_3();
	afx_msg void OnBnClickedRegR_RegW_4();
	afx_msg void OnBnClickedRegR_RegW_5();
	afx_msg void OnBnClickedRegR_RegW_6();
	afx_msg void OnBnClickedRegR_RegW_7();
	afx_msg void OnBnClickedRegR_RegW_8();
	afx_msg void OnBnClickedRegR_RegW_9();
	afx_msg void OnBnClickedRegR_RegW_10();
	afx_msg void OnBnClickedRegR_RegW_11();
	afx_msg void OnBnClickedRegR_RegW_12();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL ShowWindow(int nCmdShow);
	void SetWindowTextX();
};
