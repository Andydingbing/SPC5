#pragma once
#include "TestBaseDlg.h"
#include "ChartCtrl.h"

class CTestR1CBTRDlg : public CTestR1CBaseDlg
{
	DECLARE_DYNAMIC(CTestR1CBTRDlg)

public:
	CTestR1CBTRDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1CBTRDlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1C_BTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1C_DLG()
public:
	CChartCtrl *m_pTempChart;
	CChartCtrl *m_pLinerChart;
	double m_dEnvTemp;
	int m_iSpeedStep;
	int m_iKeepTime;
	afx_msg void OnBnClickedSetPath();
	void ResetChart();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStart();
	afx_msg LRESULT Done(WPARAM wParam,LPARAM lParam);
	void ResetTempLineSerie();
	afx_msg void OnBnClickedShowRF0();
	afx_msg void OnBnClickedShowRF1();
	afx_msg void OnBnClickedShowRF2();
	afx_msg void OnBnClickedShowRF3();
	afx_msg void OnBnClickedShowFitData();
	void PolyFit();
	afx_msg void OnBnClickedShowOriginLinerData();
	afx_msg void OnEnChangeEnvTemp();
	afx_msg void OnEnChangeSpeedStep();
	afx_msg void OnEnChangeKeepTime();
};
