#pragma once
#include "StretchDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CArbIQChartDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CArbIQChartDlg)

public:
	CArbIQChartDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArbIQChartDlg();

// 对话框数据
	enum { IDD = IDD_ARB_IQ_CHART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CChartCtrl *m_pChart;
	CChartLineSerie *m_pILineSerie;
	CChartLineSerie *m_pQLineSerie;
	CChartLineSerie *m_pDFTLineSerie;
	BOOL m_bI;
	BOOL m_bQ;
	BOOL m_bShowingDFT;
	CEdit m_FilePath;
	CComboBox m_Seg;
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	void DrawSeg(int32_t iSeg);
	afx_msg LRESULT OnDrawIQWav(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCheckI();
	afx_msg void OnBnClickedCheckQ();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedSelArbFile();
	afx_msg void OnBnClickedShowTD();
	afx_msg void OnBnClickedShowDFT();
};