#pragma once
#include "CalBaseDlg.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CCalR1ATxAttDlg : public CCalR1ABaseDlg
{
	DECLARE_DYNAMIC(CCalR1ATxAttDlg)

public:
	CCalR1ATxAttDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1ATxAttDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1A_TX_ATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1A_DLG()
public:
	CChartCtrl *m_pChart;
	typedef vector<CChartLineSerie *> LineSerieVector;
	LineSerieVector m_OPLineSerie[9];
	LineSerieVector m_IOLineSerie[9];
	virtual BOOL OnInitDialog();
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	afx_msg void OnBnClickedShowAtt10();
	afx_msg void OnBnClickedShowAtt20();
	afx_msg void OnBnClickedShowAtt30();
	afx_msg void OnBnClickedShowAtt40();
	afx_msg void OnBnClickedShowAtt50();
	afx_msg void OnBnClickedShowAtt60();
	afx_msg void OnBnClickedShowAtt70();
	afx_msg void OnBnClickedShowAtt80();
	afx_msg void OnBnClickedShowAtt90();
	BOOL m_bShowAtt10;
	BOOL m_bShowAtt20;
	BOOL m_bShowAtt30;
	BOOL m_bShowAtt40;
	BOOL m_bShowAtt50;
	BOOL m_bShowAtt60;
	BOOL m_bShowAtt70;
	BOOL m_bShowAtt80;
	BOOL m_bShowAtt90;
	BOOL m_bShowAtt1;
	BOOL m_bShowAtt2;
	BOOL m_bShowAtt3;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ResetShowCtrl();
	CComboBox m_CalIOMode;
	afx_msg void OnCbnSelchangeIOMode();
	CComboBox m_IOMode;
	CComboBox m_Att;
	double m_dAttOffset;
};
