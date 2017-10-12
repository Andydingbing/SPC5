#pragma once
#include "StretchDlg.h"

class CRFR1ADlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CRFR1ADlg)
public:
	CRFR1ADlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFR1ADlg();

// 对话框数据
	enum { IDD = IDD_RF_R1A };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CSP1401R1A *m_pSP1401;
	CString m_strTxFreq;
	CString m_strTxFreqStep;
	CString m_strTxDet;
	CString m_strTxTemp;
	CString m_strRxFreq;
	CString m_strRxFreqStep;
	CString m_strRxDet;
	CString m_strRxTemp;
	CString m_strSN;
	CString m_strSNRfVer;
	UINT m_uiSNOrdinal;
	double m_dDGain;
	CComboBox m_TxAtt1;
	CComboBox m_TxAtt2;
	CComboBox m_TxAtt3;
	CComboBox m_TxPAAttSw;
	CComboBox m_RxAtt1;
	CComboBox m_RxAtt2;
	CComboBox m_RxIFFilterSw;
	CComboBox m_RxLNAAttSw;
	CComboBox m_IOMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSetTxAtt1();
	afx_msg void OnBnClickedSetTxAtt2();
	afx_msg void OnBnClickedSetTxAtt3();
	afx_msg void OnBnClickedSetRxAtt1();
	afx_msg void OnBnClickedSetRxAtt2();
	afx_msg void OnBnClickedSetRxIFFilterSw();
	afx_msg void OnBnClickedSetTxPAAttSw();
	afx_msg void OnBnClickedSetRxLNAAttSw();
	afx_msg void OnBnClickedSetTxFreq();
	afx_msg void OnBnClickedGetTxDet();
	afx_msg void OnBnClickedSetRxFreq();
	afx_msg void OnBnClickedGetRxDet();
	afx_msg void OnBnClickedGetTxTemp();
	afx_msg void OnBnClickedGetRxTemp();
	afx_msg void OnBnClickedWriteSN();
	afx_msg void OnBnClickedReadSN();
	afx_msg void OnBnClickedSetTxAll();
	afx_msg void OnBnClickedSetRxAll();
	afx_msg void OnDeltaposSpinChangeTxFreq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinChangeRxFreq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSetIOMode();
	afx_msg void OnBnClickedSyncTxGain();
	LRESULT SyncWithRFDDlg(WPARAM wParam,LPARAM lParam);
};
