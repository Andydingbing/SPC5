#pragma once
#include "StretchDlg.h"
#include "HMC1197Dlg.h"
#include "ADF5355dlg.h"
#include "AT24C256BDlg.h"
#include "MCP23S17Dlg.h"

class CRFR1CDDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CRFR1CDDlg)

public:
	CRFR1CDDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFR1CDDlg();

// 对话框数据
	enum { IDD = IDD_RF_R1C_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CSP1401R1C *m_pSP1401;
	CAT24C256BDlg *m_pEEPROMDlg;
	CMCP23S17Dlg  *m_pMCP23S17Dlg;
	BOOL m_bPowerOnOff;
	BOOL m_bTxMod;
	BOOL m_bTxLED;
	BOOL m_bRxLED;
	UINT m_uiDetRxComb;
	UINT m_uiDetRxIF1;
	UINT m_uiDetRxIF2;
	UINT m_uiDetTxIF2;
	double m_dTemp0;
	double m_dTemp1;
	double m_dTemp2;
	double m_dTemp3;
	double m_dTemp4;
	double m_dTemp5;
	double m_dTemp6;
	double m_dTemp7;
	CButton m_Pow;
	CButton m_TxMOD;
	CComboBox m_TxAtt0;
	CComboBox m_TxFilter1Sw;
	CComboBox m_TxFilter2Sw;
	CComboBox m_TxAtt1;
	CComboBox m_TxAtt2;
	CComboBox m_TxAtt3;
	CComboBox m_TxIOSw1;
	CComboBox m_TxIOSw2;
	CComboBox m_LoopSw;
	CComboBox m_IOMode;
	CComboBox m_RxIOSw1;
	CComboBox m_RxIOSw2;
	CComboBox m_RxIOSw3;
	CComboBox m_RxLNAAttSw;
	CComboBox m_RxAtt019Sw;
	CComboBox m_RxAtt1;
	CComboBox m_RxAtt2;
	CComboBox m_RxIFFilterSw;
	CComboBox m_RxAtt3;
	CString m_strTxLO1;
	CString m_strTxLO2RxLO2;
	CString m_strTxLO3;
	CString m_strTxFreq;
	CString m_strRxFreq;
	CStatic m_LockTxLO1;
	CStatic m_LockTxLO2RxLO2;
	CStatic m_LockTxLO3;
	CStatic m_LockRxLO1;
	CString m_strRxLO1;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDebugTxLO1();
	afx_msg void OnBnClickedDebugTxLO2RxLO2();
	afx_msg void OnBnClickedDebugTxLO3();
	afx_msg void OnBnClickedDebugRxLO1();
	afx_msg void OnBnClickedSetTxLO1();
	afx_msg void OnBnClickedSetRFPower();
	afx_msg void OnBnClickedSetTxMod();
	afx_msg void OnBnClickedSetTxAtt0();
	afx_msg void OnBnClickedSetTxFilterSw();
	afx_msg void OnBnClickedSetTxAtt1();
	afx_msg void OnBnClickedSetTxAtt2();
	afx_msg void OnBnClickedSetTxAtt3();
	afx_msg void OnBnClickedSetTxIOSw1();
	afx_msg void OnBnClickedSetTxIOSw2();
	afx_msg void OnBnClickedSetLoopSw();
	afx_msg void OnBnClickedSetIOMode();
	afx_msg void OnBnClickedSetRxIOSw1();
	afx_msg void OnBnClickedSetRxIOSw2();
	afx_msg void OnBnClickedSetRxIOSw3();
	afx_msg void OnBnClickedSetRxLNAAttSw();
	afx_msg void OnBnClickedSetRxAtt019Sw();
	afx_msg void OnBnClickedSetRxAtt1();
	afx_msg void OnBnClickedSetRxAtt2();
	afx_msg void OnBnClickedRxIFFilterSw();
	afx_msg void OnBnClickedSetRxAtt3();
	afx_msg void OnBnClickedGetTemp0();
	afx_msg void OnBnClickedGetTemp1();
	afx_msg void OnBnClickedGetTemp2();
	afx_msg void OnBnClickedGetTemp3();
	afx_msg void OnBnClickedGetTemp4();
	afx_msg void OnBnClickedGetTemp5();
	afx_msg void OnBnClickedGetTemp6();
	afx_msg void OnBnClickedGetTemp7();
	afx_msg void OnBnClickedGetDetRxComb();
	afx_msg void OnBnClickedGetDetRxIF1();
	afx_msg void OnBnClickedGetDetRxIF2();
	afx_msg void OnBnClickedGetDetTxIF2();
	afx_msg void OnBnClickedSetTxLO2RxLO2();
	afx_msg void OnBnClickedSetTxLO3();
	afx_msg void OnBnClickedSetRxLO1();
	afx_msg void OnBnClickedDebugMCP23S17();
	afx_msg void OnBnClickedDebugEEPROM();
	afx_msg void OnBnClickedSetTxLED();
	afx_msg void OnBnClickedSetRxLED();
	afx_msg void OnBnClickedGetAllTemp();
	afx_msg void OnBnClickedGetAllDet();
	afx_msg LRESULT SyncWithRFDlg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedSetTxFreq();
	afx_msg void OnBnClickedSetRxFreq();
};
