#pragma once
#include "Resource.h"
#include "AttachThreadDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CRFR1CDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CRFR1CDlg)

public:
	CRFR1CDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFR1CDlg();

// 对话框数据
	enum { IDD = IDD_RF_R1C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_ATTACHTHREAD_DLG()
public:
	CChartCtrl *m_pChart;
	vector<CChartLineSerie *> m_pTempLineSerie;
	CSP1401R1C *m_pSP1401;
	CSP3501 *m_pSP3501;
	UINT m_uiDetTxIF2;
	UINT m_uiDetRxComb;
	UINT m_uiDetRxIF1;
	UINT m_uiDetRxIF2;
	UINT m_uiSNOrdinal;
	double m_dDGain;
	double m_dTemp0;
	double m_dTemp1;
	double m_dTemp2;
	double m_dTemp3;
	double m_dTemp4;
	double m_dTemp5;
	double m_dTemp6;
	double m_dTemp7;
	CString m_strTxFreq;
	CString m_strTxFreqStep;
	CString m_strRxFreq;
	CString m_strRxFreqStep;
	CString m_strSNRfVer;
	CString m_strSN;
	CComboBox m_TxAtt0;
	CComboBox m_TxAtt1;
	CComboBox m_TxAtt2;
	CComboBox m_TxAtt3;
	CComboBox m_IOMode;
	CComboBox m_RxLNAAttSw;
	CComboBox m_RxAtt019Sw;
	CComboBox m_RxAtt1;
	CComboBox m_RxAtt2;
	CComboBox m_RxIFFilterSw;
	CComboBox m_RxAtt3;
	CComboBox m_BlowerAjustMode;
	void ResetChart();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSetTxFreq();
	afx_msg void OnDeltaposSpinChangeTxFreq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSetTxAtt0();
	afx_msg void OnBnClickedSetTxAtt1();
	afx_msg void OnBnClickedSetTxAtt2();
	afx_msg void OnBnClickedSetTxAtt3();
	afx_msg void OnBnClickedSetIOMode();
	afx_msg void OnBnClickedSyncTxGain();
	afx_msg void OnBnClickedSetRxFreq();
	afx_msg void OnBnClickedSetRxLNAAttSw();
	afx_msg void OnBnClickedSetRxAtt019Sw();
	afx_msg void OnBnClickedSetRxAtt1();
	afx_msg void OnBnClickedSetRxAtt2();
	afx_msg void OnBnClickedSetRxIFFilterSw();
	afx_msg void OnBnClickedSetRxAtt3();
	afx_msg void OnBnClickedWriteSN();
	afx_msg void OnBnClickedReadSN();
	afx_msg void OnBnClickedGetTemp0();
	afx_msg void OnBnClickedGetTemp1();
	afx_msg void OnBnClickedGetTemp2();
	afx_msg void OnBnClickedGetTemp3();
	afx_msg void OnBnClickedGetTemp4();
	afx_msg void OnBnClickedGetTemp5();
	afx_msg void OnBnClickedGetTemp6();
	afx_msg void OnBnClickedGetTemp7();
	afx_msg void OnBnClickedDetTxIF2();
	afx_msg void OnBnClickedDetRxComb();
	afx_msg void OnBnClickedDetRxIF1();
	afx_msg void OnBnClickedDetRxIF2();
	afx_msg void OnBnClickedSetTxAll();
	afx_msg void OnBnClickedSetRxAll();
	afx_msg void OnBnClickedGetTempAll();
	afx_msg void OnBnClickedDetAll();
	afx_msg void OnBnClickedStartRecord();
	afx_msg void OnDeltaposSpinChangeRxFreq(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeBlowerAjustMode();
	afx_msg LRESULT SyncWithRFDDlg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT SetToIdle(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT ShowData(WPARAM wParam,LPARAM lParam);
};
