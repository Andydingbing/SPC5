#pragma once
#include "TestBaseDlg.h"
#include "ProtectEyeListCtrl.h"
#include "ChartCtrl.h"
#include "ChartPointsSerie.h"

class CTestR1CTTBDlg : public CTestR1CBaseDlg
{
	DECLARE_DYNAMIC(CTestR1CTTBDlg)

public:
	CTestR1CTTBDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1CTTBDlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1C_TTB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1C_DLG()
public:
	CChartCtrl *m_pChart;
	vector<CChartPointsSerie *> m_PointsSerie;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	int m_iSpeedStep;
	int m_iKeepTime;
	afx_msg void OnBnClickedStart();
	void ResetChart();
	virtual BOOL OnInitDialog();
	CProtectEyeListCtrl m_List;
};
