#pragma once
#include "TestBaseDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CTestR1AX9119Dlg : public CTestR1ABaseDlg
{
	DECLARE_DYNAMIC(CTestR1AX9119Dlg)

public:
	CTestR1AX9119Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestR1AX9119Dlg();

// �Ի�������
	enum { IDD = IDD_TEST_R1A_X9119 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1A_DLG()
public:
	CChartCtrl *m_pChart;
	CChartLineSerie *m_pLineSerie;
	afx_msg void OnBnClickedStartTest();
	virtual BOOL OnInitDialog();
	CString m_strFreq;
};
