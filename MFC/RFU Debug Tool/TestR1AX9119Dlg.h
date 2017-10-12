#pragma once
#include "TestBaseDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CTestR1AX9119Dlg : public CTestR1ABaseDlg
{
	DECLARE_DYNAMIC(CTestR1AX9119Dlg)

public:
	CTestR1AX9119Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1AX9119Dlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1A_X9119 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1A_DLG()
public:
	CChartCtrl *m_pChart;
	CChartLineSerie *m_pLineSerie;
	afx_msg void OnBnClickedStartTest();
	virtual BOOL OnInitDialog();
	CString m_strFreq;
};
