#pragma once
#include "Stretchdlg.h"
#include "DUCScrollEdit.h"
#include "PhaseRotateScrollEdit.h"
#include "AmplitudeBalanceScrollEdit.h"
#include "DCOffsetScrollEdit.h"

class CBBDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CBBDlg)

public:
	CBBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBBDlg();
public:
// 对话框数据
	enum { IDD = IDD_BB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CSP1401R1A *m_pSP1401;
	CSP2401R1A *m_pSP2401;
	CString m_strFreqDDS1;
	double m_dDDSGain;
	afx_msg void OnBnClickedSetDDS1Freq();
	afx_msg void OnBnClickedSetDDSGain();
	CString m_strFreqDDS2;
	CComboBox m_DDSSource;
	afx_msg void OnBnClickedSetDDS2Freq();
	virtual BOOL OnInitDialog();
	double m_dPhaseRotateI;
	CPhaseRotateScrollEdit m_PhaseRotateI;
	double m_dPhaseRotateQ;
	CPhaseRotateScrollEdit m_PhaseRotateQ;
	afx_msg void OnEnChangeEditPhaseRotateI();
	afx_msg void OnEnChangeEditPhaseRotateQ();
	CAmplitudeBalanceScrollEdit m_AmplitudeBalanceI;
	CString m_strAmplitudeBalanceI;
	CAmplitudeBalanceScrollEdit m_AmplitudeBalanceQ;
	CString m_strAmplitudeBalanceQ;
	afx_msg void OnEnChangeEditAmplitudeBalanceI();
	afx_msg void OnEnChangeEditAmplitudeBalanceQ();
	CDCOffsetScrollEdit m_DCOffsetI;
	CString m_strDCOffsetI;
	CDCOffsetScrollEdit m_DCOffsetQ;
	CString m_strDCOffsetQ;
	afx_msg void OnEnChangeEditDCOffsetI();
	afx_msg void OnEnChangeEditDCOffsetQ();
	CString m_strFreqDDC;
	afx_msg void OnBnClickedSetDDCFreq();
	afx_msg void OnBnClickedGetADManual();
	LONG64 m_iADManual;
	int m_iRxPowerCompensate;
	afx_msg void OnBnClickedSetRxPowerCompensate();
	CString m_strFreqDUSDDS;
	afx_msg void OnBnClickedSetDUCDDS();
	CString m_strDAClock;
	CComboBox m_DASwitch;
	CString m_strADClock;
	CComboBox m_ADSwitch;
	afx_msg void OnBnClickedSetDAClock();
	afx_msg void OnBnClickedSetDASwitch();
	afx_msg void OnBnClickedSetDAReset();
	afx_msg void OnBnClickedSetADClock();
	afx_msg void OnBnClickedSetADSwitch();
	LONG64 m_iADFpga;
	afx_msg void OnBnClickedGetADFpga();
	afx_msg void OnBnClickedSetDDSSrc();
	CString m_strTxFilterCoefReal;
	CString m_strTxFilterCoefImag;
	CString m_strRxFilterCoefReal;
	CString m_strRxFilterCoefImag;
	afx_msg void OnBnClickedSetTxFilter();
	afx_msg void OnBnClickedSetRxFilter();
	afx_msg void OnBnClickedGetPowerManual();
	double m_dPowerManual;
	BOOL m_bRxFilter;
	afx_msg void OnBnClickedSelRxFilter80M();
	CComboBox m_RxFilter80;
	CComboBox m_RxFilter160;
	afx_msg void OnBnClickedSelRxFilter160M();
	CDUCScrollEdit m_DUC;
	afx_msg void OnEnChangeDUCDDS();
	afx_msg void OnEnChangeDDS1();
	CDUCScrollEdit m_DDS1;
	CDUCScrollEdit m_DDS2;
	afx_msg void OnEnChangeDDS2();
	afx_msg void OnBnClickedSetTxFilterSw();
};
