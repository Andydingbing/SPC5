#pragma once
#include "AttachThreadDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CRFR1AGuardDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CRFR1AGuardDlg)

public:
	CRFR1AGuardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFR1AGuardDlg();

// 对话框数据
	enum { IDD = IDD_RF_R1A_GUARD };
public:
	vector<CSP1401R1A *> m_pSP1401;
	CSP3501 *m_pSP3501;
	CChartCtrl *m_pChart;
	vector<CChartLineSerie *> m_pTempLineSerie; 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_ATTACHTHREAD_DLG()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_Mode;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void ResetChart();
	afx_msg void OnBnClickedStartRecord();
	afx_msg LRESULT ShowData(WPARAM wParam,LPARAM lParam);
	afx_msg void OnCbnSelchangeBlowerAjustMode();
	afx_msg LRESULT SetToIdle(WPARAM wParam,LPARAM lParam);
};
