#pragma once
#include "StretchDlg.h"
#include "ChartCtrl.h"
#include "EditableListCtrl.h"
#include "afxwin.h"

class CTestFittingInterpDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CTestFittingInterpDlg)

public:
	CTestFittingInterpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestFittingInterpDlg();

// 对话框数据
	enum { IDD = IDD_TEST_FITTING_INTERP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CChartCtrl *m_pChart;
	CChartLineSerie *m_pLineSerie;
	CChartPointsSerie *m_pPointSerie;
	CComboBox m_Type;
	virtual BOOL OnInitDialog();
	CStatic m_ConfigText;
	CEdit m_ConfigEdit;
	CEditableListCtrl m_List;
	void UpdateListToChart();
	int GetListColumn();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedDecreasePt();
	afx_msg void OnBnClickedAddPt();
	CStatic m_Fx;
};
