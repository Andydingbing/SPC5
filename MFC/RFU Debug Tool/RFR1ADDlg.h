#pragma once
#include "StretchDlg.h"
#include "ADF5355Dlg.h"
#include "LTC266616Dlg.h"
#include "AT24C256BDlg.h"

#define TIMER_LO_LOCK_DET 1

class CRFR1ADDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CRFR1ADDlg)
public:
	CRFR1ADDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFR1ADDlg();

// 对话框数据
	enum { IDD = IDD_RF_R1A_ADVANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CSP1401R1A *m_pSP1401;
	CLTC266616Dlg *m_pLTC2666Dlg;
	CAT24C256BDlg *m_pEEPROMDlg;
	virtual BOOL OnInitDialog();
	CString m_strTxLO1;
	CString m_strTxLO2;
	CString m_strRxLO1;
	CString m_strRxLO2;
	CString m_strTxDet;
	CString m_strRxDet;
	CString m_strTxTemp;
	CString m_strRxTemp;
	CString m_strTxFreq;
	CString m_strRxFreq;
	BOOL m_bTxADL5375;
	BOOL m_bPowerOnOff;
	CButton m_TxMOD;
	CButton m_Pow;
	BOOL m_bTxLED;
	BOOL m_bRxLED;
	CComboBox m_TxBandSw;
	CComboBox m_TxPAAttSw;
	CComboBox m_RxLNAAttSw;
	CComboBox m_RxIFFilterSw;
	CComboBox m_TxIOSw;
	CComboBox m_RxIOSw1;
	CComboBox m_RxIOSw2;
	CComboBox m_RxIOSw;
	CComboBox m_IOMode;	CComboBox m_TxAtt1;
	CComboBox m_TxAtt2;
	CComboBox m_TxAtt3;
	CComboBox m_RxAtt1;
	CComboBox m_RxAtt2;
	CStatic m_LockTxLO2;
	CStatic m_LockTxLO1;
	CStatic m_LockRxLO1;
	CStatic m_LockRxLO2;
	afx_msg void OnBnClickedSetTxLO1();
	afx_msg void OnBnClickedSetTxLO2();
	afx_msg void OnBnClickedSetRxLO1();
	afx_msg void OnBnClickedSetRxLO2();
	afx_msg void OnBnClickedDebugTxLO1();
	afx_msg void OnBnClickedDebugTxLO2();
	afx_msg void OnBnClickedDebugRxLO1();
	afx_msg void OnBnClickedDebugRxLO2();
	afx_msg void OnBnClickedSetLTC2666();
	afx_msg void OnBnClickedGetTxDet();
	afx_msg void OnBnClickedGetRxDet();
	afx_msg void OnBnClickedSetTxADL5375();
	afx_msg void OnBnClickedSetTxBandSw();
	afx_msg void OnBnClickedSetTxPAAttSw();
	afx_msg void OnBnClickedSetRFPower();
	afx_msg void OnBnClickedSetRxLNAAttSw();
	afx_msg void OnBnClickedSetRxIFFilterSw();
	afx_msg void OnBnClickedSetTxIOSw();
	afx_msg void OnBnClickedSetRxIOSw1();
	afx_msg void OnBnClickedSetRxIOSw2();
	afx_msg void OnBnClickedSetRxIOSw();
	afx_msg void OnBnClickedSetIOMode();
	afx_msg void OnBnClickedSetTxAtt1();
	afx_msg void OnBnClickedSetTxAtt2();
	afx_msg void OnBnClickedSetTxAtt3();
	afx_msg void OnBnClickedSetRxAtt1();
	afx_msg void OnBnClickedSetRxAtt2();
	afx_msg void OnBnClickedGetTxTemp();
	afx_msg void OnBnClickedGetRxTemp();
	afx_msg void OnBnClickedDebugEEPROM();
	afx_msg void OnBnClickedSetTxLED();
	afx_msg void OnBnClickedSetRxLED();
	afx_msg void OnBnClickedSetTxFreq();
	afx_msg void OnBnClickedSetRxFreq();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT SyncWithRFDlg(WPARAM wParam,LPARAM lParam);
};
