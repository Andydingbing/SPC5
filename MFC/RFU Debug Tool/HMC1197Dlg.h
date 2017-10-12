#pragma once
#include "XGroupBox.h"
#include "afxwin.h"

class CHMC1197Dlg : public CDialog
{
	DECLARE_DYNAMIC(CHMC1197Dlg)
public:
	CHMC1197Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHMC1197Dlg();
// 对话框数据
	enum { IDD = IDD_HMC1197 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	enum FreqMode {
		_INT_Mode = 0,
		_FRAC_Mode,
		_EXACT_Mode
	};
public:
	CSP1401R1C *m_pSP1401;
	CXGroupBox m_XGroupBoxReg0x00;
	CXGroupBox m_XGroupBoxReg0x01;
	CXGroupBox m_XGroupBoxReg0x02;
	CXGroupBox m_XGroupBoxReg0x03;
	CXGroupBox m_XGroupBoxReg0x04;
	CXGroupBox m_XGroupBoxReg0x05;
	CXGroupBox m_XGroupBoxReg0x06;
	CXGroupBox m_XGroupBoxReg0x07;
	CXGroupBox m_XGroupBoxReg0x08;
	CXGroupBox m_XGroupBoxReg0x09;
	CXGroupBox m_XGroupBoxReg0x0A;
	CXGroupBox m_XGroupBoxReg0x0B;
	CXGroupBox m_XGroupBoxReg0x0C;
	CXGroupBox m_XGroupBoxReg0x0F;
	CXGroupBox m_XGroupBoxReg0x10;
	CXGroupBox m_XGroupBoxReg0x11;
	CXGroupBox m_XGroupBoxReg0x12;
	CXGroupBox m_XGroupBoxReg0x13;
	CXGroupBox m_XGroupBoxReg0x14;
	CXGroupBox m_XGroupBoxReg0x15;
	CXGroupBox m_XGroupBoxReg0x16;
	CXGroupBox m_XGroupBoxReg0x17;
	CXGroupBox m_XGroupBoxReg0x18;
	CXGroupBox m_XGroupBoxReg0x19;
	CXGroupBox m_XGroupBoxReg0x1A;
	CXGroupBox m_XGroupBoxReg0x21;
	CXGroupBox m_XGroupBoxQuickSet;
	CString m_strReg0x00Read;
	short m_sSoftReset;
	CComboBox m_ChipEnablePin;
	virtual BOOL OnInitDialog();
	BOOL m_bSPIChipEnable;
	BOOL m_bKeepBiasOn;
	BOOL m_bKeepPFDOn;
	BOOL m_bKeepCPOn;
	BOOL m_bKeepRefBufferOn;
	BOOL m_bKeepVCOOn;
	BOOL m_bKeepGPODriverOn;
	CString m_strReg0x01Read;
	int m_iRDiv;
	int m_iINT;
	int m_iFRAC;
	int m_iReg0x05Write;
	CComboBox m_DSMType;
	BOOL m_bSyncSPIMode;
	BOOL m_bExactFreqMode;
	BOOL m_bBypassDSM;
	BOOL m_bAutoseedDSM;
	BOOL m_bDSMEnable;
	CString m_strReg0x06Read;
	CComboBox m_LDCounts;
	BOOL m_bLDEnable;
	short m_sLDTraining;
	BOOL m_bCSPEnable;
	CString m_strReg0x07Read;
	BOOL m_bGPOLDOSDOEnable;
	BOOL m_bVCOPsBiasEnable;
	BOOL m_bHighRef;
	CComboBox m_SDOOutputLevel;
	CString m_strReg0x08Read;
	CComboBox m_CPDownGain;
	CComboBox m_CPUpGain;
	CComboBox m_CPOffset;
	BOOL m_bOffsetUpEnable;
	BOOL m_bOffsetDownEnable;
	BOOL m_bHikCPMode;
	CString m_strReg0x09Read;
	CComboBox m_VtuneResolution;
	BOOL m_bAutoCalDisable;
	CComboBox m_FSMVSPIClock;
	BOOL m_bAutoRelockOnce;
	CString m_strReg0x0ARead;
	CComboBox m_PDPhaseSelect;
	BOOL m_bPDUpOutput;
	BOOL m_bPDDownOutput;
	BOOL m_bForceCPUp;
	BOOL m_bForceCPDown;
	BOOL m_bForceCPMidRail;
	CString m_strReg0x0BRead;
	int m_iChannelsPerFPD;
	CComboBox m_GPOSel;
	BOOL m_bGPOTestData;
	BOOL m_bPreventAutomuxSDO;
	BOOL m_bDisablePFET;
	BOOL m_bDisableNFET;
	CString m_strReg0x0FRead;
	int m_iVCOTuneCurve;
	BOOL m_bVCOTuningBusy;
	CString m_strReg0x10Read;
	int m_iSARErrorMagnitude;
	BOOL m_bSARError;
	BOOL m_bGPOOut;
	BOOL m_bLockDetectOut;
	CString m_strReg0x13Read;
	CComboBox m_AuxSPIMode;
	short m_sAuxGPOValues;
	CComboBox m_AuxGPOOutputLevel;
	BOOL m_bPhaseSync;
	CComboBox m_AuxSPIGPOOutput;
	CComboBox m_AuxSPIOutputs;
	CString m_strReg0x14Read;
	BOOL m_bVCOManualCal;
	int m_iCapacitorSwitchSet;
	CComboBox m_VCOBand;
	BOOL m_bManualVCOTune;
	BOOL m_bAutoScaleCPCurrent;
	CString m_strReg0x15Read;
	CComboBox m_RFDividerRatio;
	CComboBox m_LO1OutputGain;
	CComboBox m_LO2OutputGain;
	CComboBox m_DividerOutputGain;
	CString m_strReg0x16Read;
	BOOL m_bVCOSubsysMasterEnable;
	BOOL m_bVCOEnable;
	BOOL m_bExternalVCOBufferEnable;
	BOOL m_bPLLBufferEnable;
	BOOL m_bLO1OutputBufferEnable;
	BOOL m_bLO2OutputBufferEnable;
	BOOL m_bExternalVCOInputEnable;
	BOOL m_bPreLockMute;
	CComboBox m_LO1OutputMode;
	CComboBox m_LO2OutputMode;
	CComboBox m_CPOutputSelect;
	CString m_strReg0x17Read;
	CComboBox m_ExternalVCOBias;
	int m_iReg0x19Write;
	int m_iReg0x1AWrite;
	CComboBox m_HarmonicLPFBand;
	void Para2Reg();
	uint32_t Para2Reg00();
	uint32_t Para2Reg01();
	uint32_t Para2Reg02();
	uint32_t Para2Reg03();
	uint32_t Para2Reg04();
	uint32_t Para2Reg05();
	uint32_t Para2Reg06();
	uint32_t Para2Reg07();
	uint32_t Para2Reg08();
	uint32_t Para2Reg09();
	uint32_t Para2Reg0a();
	uint32_t Para2Reg0b();
	uint32_t Para2Reg0c();
	uint32_t Para2Reg0f();
	uint32_t Para2Reg14();
	uint32_t Para2Reg15();
	uint32_t Para2Reg16();
	uint32_t Para2Reg17();
	uint32_t Para2Reg18();
	uint32_t Para2Reg19();
	uint32_t Para2Reg1a();
	uint32_t Para2Reg21();
	afx_msg void OnBnClickedWriteReg0x00();
	afx_msg void OnBnClickedWriteReg0x01();
	afx_msg void OnBnClickedWriteReg0x02();
	afx_msg void OnBnClickedWriteReg0x03();
	afx_msg void OnBnClickedWriteReg0x04();
	afx_msg void OnBnClickedWriteReg0x05();
	afx_msg void OnBnClickedWriteReg0x06();
	afx_msg void OnBnClickedWriteReg0x07();
	afx_msg void OnBnClickedWriteReg0x08();
	afx_msg void OnBnClickedWriteReg0x09();
	afx_msg void OnBnClickedWriteReg0x0a();
	afx_msg void OnBnClickedWriteReg0x0b();
	afx_msg void OnBnClickedWriteReg0x0c();
	afx_msg void OnBnClickedWriteReg0x0f();
	afx_msg void OnBnClickedWriteReg0x14();
	afx_msg void OnBnClickedWriteReg0x15();
	afx_msg void OnBnClickedWriteReg0x16();
	afx_msg void OnBnClickedWriteReg0x17();
	afx_msg void OnBnClickedWriteReg0x18();
	afx_msg void OnBnClickedWriteReg0x19();
	afx_msg void OnBnClickedWriteReg0x1a();
	afx_msg void OnBnClickedWriteReg0x21();
	afx_msg void OnBnClickedReadReg0x00();
	afx_msg void OnBnClickedReadReg0x01();
	afx_msg void OnBnClickedReadReg0x02();
	afx_msg void OnBnClickedReadReg0x03();
	afx_msg void OnBnClickedReadReg0x04();
	afx_msg void OnBnClickedReadReg0x05();
	afx_msg void OnBnClickedReadReg0x06();
	afx_msg void OnBnClickedReadReg0x07();
	afx_msg void OnBnClickedReadReg0x08();
	afx_msg void OnBnClickedReadReg0x09();
	afx_msg void OnBnClickedReadReg0x0a();
	afx_msg void OnBnClickedReadReg0x0b();
	afx_msg void OnBnClickedReadReg0x0c();
	afx_msg void OnBnClickedReadReg0x0f();
	afx_msg void OnBnClickedReadReg0x10();
	afx_msg void OnBnClickedReadReg0x11();
	afx_msg void OnBnClickedReadReg0x12();
	afx_msg void OnBnClickedReadReg0x13();
	afx_msg void OnBnClickedReadReg0x14();
	afx_msg void OnBnClickedReadReg0x15();
	afx_msg void OnBnClickedReadReg0x16();
	afx_msg void OnBnClickedReadReg0x17();
	afx_msg void OnBnClickedReadReg0x18();
	afx_msg void OnBnClickedReadReg0x19();
	afx_msg void OnBnClickedReadReg0x1a();
	CComboBox m_Mode;
	CString m_strFreqRF;
	CString m_strFreqVCO;
	afx_msg void OnBnClickedApply();
	double m_dFreqError;
	afx_msg void OnCbnSelchangeMode();
	afx_msg void OnEnChangeFreqRF();
	afx_msg void OnBnClickedWriteAll();
	afx_msg void OnBnClickedReadAll();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL ShowWindow(int nCmdShow);
	void SetWindowTextX();
	CString m_strReg0x02Read;
	CString m_strReg0x03Read;
	CString m_strReg0x04Read;
	CString m_strReg0x05Read;
	CString m_strReg0x0CRead;
	CString m_strReg0x11Read;
	CString m_strReg0x12Read;
	CString m_strReg0x18Read;
	CString m_strReg0x19Read;
	CString m_strReg0x1ARead;
	CString m_strReg0x00Write;
	CString m_strReg0x01Write;
	CString m_strReg0x02Write;
	CString m_strReg0x03Write;
	CString m_strReg0x04Write;
	CString m_strReg0x05Write;
	CString m_strReg0x06Write;
	CString m_strReg0x07Write;
	CString m_strReg0x08Write;
	CString m_strReg0x09Write;
	CString m_strReg0x0aWrite;
	CString m_strReg0x0bWrite;
	CString m_strReg0x0cWrite;
	CString m_strReg0x0dWrite;
	CString m_strReg0x0eWrite;
	CString m_strReg0x0fWrite;
	CString m_strReg0x10Write;
	CString m_strReg0x11Write;
	CString m_strReg0x12Write;
	CString m_strReg0x13Write;
	CString m_strReg0x14Write;
	CString m_strReg0x15Write;
	CString m_strReg0x16Write;
	CString m_strReg0x17Write;
	CString m_strReg0x18Write;
	CString m_strReg0x19Write;
	CString m_strReg0x1aWrite;
	CString m_strReg0x1bWrite;
	CString m_strReg0x1cWrite;
	CString m_strReg0x1dWrite;
	CString m_strReg0x1eWrite;
	CString m_strReg0x1fWrite;
	afx_msg void OnBnClickedWReg0x00();
	afx_msg void OnBnClickedWReg0x01();
	afx_msg void OnBnClickedWReg0x02();
	afx_msg void OnBnClickedWReg0x03();
	afx_msg void OnBnClickedWReg0x04();
	afx_msg void OnBnClickedWReg0x05();
	afx_msg void OnBnClickedWReg0x06();
	afx_msg void OnBnClickedWReg0x07();
	afx_msg void OnBnClickedWReg0x08();
	afx_msg void OnBnClickedWReg0x09();
	afx_msg void OnBnClickedWReg0x0a();
	afx_msg void OnBnClickedWReg0x0b();
	afx_msg void OnBnClickedWReg0x0c();
	afx_msg void OnBnClickedWReg0x0d();
	afx_msg void OnBnClickedWReg0x0e();
	afx_msg void OnBnClickedWReg0x0f();
	afx_msg void OnBnClickedWReg0x10();
	afx_msg void OnBnClickedWReg0x11();
	afx_msg void OnBnClickedWReg0x12();
	afx_msg void OnBnClickedWReg0x13();
	afx_msg void OnBnClickedWReg0x14();
	afx_msg void OnBnClickedWReg0x15();
	afx_msg void OnBnClickedWReg0x16();
	afx_msg void OnBnClickedWReg0x17();
	afx_msg void OnBnClickedWReg0x18();
	afx_msg void OnBnClickedWReg0x19();
	afx_msg void OnBnClickedWReg0x1a();
	afx_msg void OnBnClickedWReg0x1b();
	afx_msg void OnBnClickedWReg0x1c();
	afx_msg void OnBnClickedWReg0x1d();
	afx_msg void OnBnClickedWReg0x1e();
	afx_msg void OnBnClickedWReg0x1f();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck34();
	afx_msg void OnBnClickedCheck35();
	afx_msg void OnBnClickedCheck36();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnChangeEdit90();
	afx_msg void OnEnChangeEdit91();
	afx_msg void OnEnChangeEdit92();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnBnClickedCheck37();
	afx_msg void OnBnClickedCheck38();
	afx_msg void OnBnClickedCheck39();
	afx_msg void OnBnClickedCheck40();
	afx_msg void OnBnClickedCheck41();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnBnClickedCheck42();
	afx_msg void OnEnChangeEdit93();
	afx_msg void OnBnClickedCheck43();
	afx_msg void OnBnClickedCheck44();
	afx_msg void OnBnClickedCheck45();
	afx_msg void OnBnClickedCheck46();
	afx_msg void OnCbnSelchangeCombo31();
	afx_msg void OnCbnSelchangeCombo32();
	afx_msg void OnCbnSelchangeCombo39();
	afx_msg void OnCbnSelchangeCombo40();
	afx_msg void OnBnClickedCheck47();
	afx_msg void OnBnClickedCheck48();
	afx_msg void OnBnClickedCheck49();
	afx_msg void OnCbnSelchangeCombo41();
	afx_msg void OnBnClickedCheck50();
	afx_msg void OnCbnSelchangeCombo42();
	afx_msg void OnBnClickedCheck51();
	afx_msg void OnCbnSelchangeCombo43();
	afx_msg void OnBnClickedCheck52();
	afx_msg void OnBnClickedCheck53();
	afx_msg void OnBnClickedCheck54();
	afx_msg void OnBnClickedCheck55();
	afx_msg void OnBnClickedCheck56();
	afx_msg void OnEnChangeEdit94();
	afx_msg void OnCbnSelchangeCombo44();
	afx_msg void OnBnClickedCheck57();
	afx_msg void OnBnClickedCheck58();
	afx_msg void OnBnClickedCheck59();
	afx_msg void OnBnClickedCheck60();
	afx_msg void OnCbnSelchangeCombo45();
	afx_msg void OnEnChangeEdit96();
	afx_msg void OnCbnSelchangeCombo46();
	afx_msg void OnBnClickedCheck65();
	afx_msg void OnCbnSelchangeCombo47();
	afx_msg void OnCbnSelchangeCombo48();
	afx_msg void OnBnClickedCheck66();
	afx_msg void OnEnChangeEdit97();
	afx_msg void OnCbnSelchangeCombo49();
	afx_msg void OnBnClickedCheck67();
	afx_msg void OnBnClickedCheck68();
	afx_msg void OnCbnSelchangeCombo50();
	afx_msg void OnCbnSelchangeCombo51();
	afx_msg void OnCbnSelchangeCombo52();
	afx_msg void OnCbnSelchangeCombo53();
	afx_msg void OnBnClickedCheck69();
	afx_msg void OnBnClickedCheck70();
	afx_msg void OnBnClickedCheck71();
	afx_msg void OnBnClickedCheck72();
	afx_msg void OnBnClickedCheck73();
	afx_msg void OnBnClickedCheck74();
	afx_msg void OnBnClickedCheck75();
	afx_msg void OnBnClickedCheck76();
	afx_msg void OnCbnSelchangeCombo54();
	afx_msg void OnCbnSelchangeCombo55();
	afx_msg void OnCbnSelchangeCombo56();
	afx_msg void OnCbnSelchangeCombo57();
	afx_msg void OnEnChangeEdit112();
	afx_msg void OnEnChangeEdit98();
	afx_msg void OnBnClickedWReg();
};
