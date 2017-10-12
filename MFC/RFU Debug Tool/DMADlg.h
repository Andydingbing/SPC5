#pragma once
#include "ChartCtrl.h"
#include "ChartLineSerie.h"
#include "AttachThreadDlg.h"

class CDMADlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CDMADlg)

public:
	CDMADlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDMADlg();

// 对话框数据
	enum { IDD = IDD_DMA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_ATTACHTHREAD_DLG()
public:
	ISP1401 *m_pSP1401;
	CChartCtrl *m_pChart;
	CChartLineSerie *m_pLineSerie;
	UINT m_uiRTlpSize;
	UINT m_uiRSamples;
	UINT m_uiWTlpSize;
	UINT m_uiWSamples;
	BOOL m_bTestR;
	BOOL m_bTestW;
	UINT m_uiTestSamples;
	UINT m_uiTestTimes;
	UINT m_uiTestCurTimes;
	CEdit m_WPath;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRStart();
	afx_msg void OnBnClickedWStart();
	afx_msg void OnBnClickedSetWPath();
	afx_msg void OnBnClickedOpenDataFile();
	afx_msg void OnBnClickedShowWV();
	afx_msg void OnBnClickedStartWRTest();
};
