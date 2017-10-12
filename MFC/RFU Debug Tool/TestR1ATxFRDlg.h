#pragma once
#include "TestBaseDlg.h"
#include "TestR1ATxFRThread.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CTestR1ATxFRDlg : public CTestR1ABaseDlg
{
	DECLARE_DYNAMIC(CTestR1ATxFRDlg)

public:
	CTestR1ATxFRDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1ATxFRDlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1A_TX_FR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1A_DLG()
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
	void ResetShowCtrl(CTestR1ATxFRThread::TestRFFRInfo *pInfo);
	void ResetShowCtrl(CTestR1ATxFRThread::TestBBFRInfo *pInfo);
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
};
