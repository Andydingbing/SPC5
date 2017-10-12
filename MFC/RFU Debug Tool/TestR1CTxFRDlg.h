#pragma once
#include "TestBaseDlg.h"
#include "TestR1CTxFRThread.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CTestR1CTxFRDlg : public CTestR1CBaseDlg
{
	DECLARE_DYNAMIC(CTestR1CTxFRDlg)

public:
	CTestR1CTxFRDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1CTxFRDlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1C_TX_FR };

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
	afx_msg void OnBnClickedStartTestBB();
	virtual BOOL OnInitDialog();
	CString m_strRFFreq;
	CString m_strBBFreqStep;
	void ResetShowCtrl(CTestR1CTxFRThread::TestRFFRInfo *pInfo);
	void ResetShowCtrl(CTestR1CTxFRThread::TestBBFRInfo *pInfo);
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
};
