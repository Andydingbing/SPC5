#pragma once
#include "TestBaseDlg.h"
#include "TestR1CRxFRThread.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CTestR1CRxFRDlg : public CTestR1CBaseDlg
{
	DECLARE_DYNAMIC(CTestR1CRxFRDlg)

public:
	CTestR1CRxFRDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1CRxFRDlg();

	// 对话框数据
	enum { IDD = IDD_TEST_R1C_RX_FR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1C_DLG()
public:
	CChartCtrl *m_pChart;
	vector<CChartLineSerie *> m_pFRLineSerie;
	CString m_strRFFreqStar;
	CString m_strRFFreqStop;
	CString m_strRFFreqStep;
	afx_msg void OnBnClickedStartTestRF();
	afx_msg void OnBnClickedStartTestIF();
	virtual BOOL OnInitDialog();
	CString m_strRFFreq;
	CString m_strIFFreqStep;
	void ResetShowCtrl(CTestR1CRxFRThread::TestRFFRInfo *pInfo);
	void ResetShowCtrl(CTestR1CRxFRThread::TestIFFRInfo *pInfo);
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
};
