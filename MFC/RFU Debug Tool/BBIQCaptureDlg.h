#pragma once
#include "StretchDlg.h"
#include "afxwin.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CBBIQCaptureDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CBBIQCaptureDlg)

public:
	CBBIQCaptureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBBIQCaptureDlg();

// 对话框数据
	enum { IDD = IDD_BB_IQ_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	ISP1401 *m_pSP1401;
	CSP2401R1A *m_pSP2401;
	CChartCtrl *m_Chart;
	CChartLineSerie *m_pILineSerie;
	CChartLineSerie *m_pQLineSerie;
	CChartLineSerie *m_pDFTLineSerie;
	virtual BOOL OnInitDialog();
	CComboBox m_TriggerSrc;
	CComboBox m_TriggerEdge;
	UINT m_uiTriggerOffset;
	UINT m_uiCapOffset;
	UINT m_uiTriggerTimeout;
	UINT m_uiCapSamples;
	UINT m_uiTriggerGap;
	double m_dThreshold;
	CEdit m_FilePath;
	afx_msg void OnBnClickedStartCapture();
	afx_msg void OnBnClickedAbortCapture();
	afx_msg void OnBnClickedShowDataTimeDomain();
	BOOL m_bShowI;
	BOOL m_bShowQ;
	afx_msg void OnBnClickedSetDataPath();
	afx_msg void OnBnClickedOpenWaveVisionDlg();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedShowDataDFT();
	bool m_bShowingDFT;
	afx_msg void OnBnClickedShowDataI();
	afx_msg void OnBnClickedShowDataQ();
	afx_msg void OnBnClickedStartPowerMeasure();
	CString m_strStateMachine;
	double m_dPower;
	double m_dPeakPower;
	afx_msg LRESULT ShowDataContinuousIQCap(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedSetMaxHold();
	afx_msg void OnBnClickedSetMinHold();
	afx_msg void OnEnChangeSamples();
	afx_msg void OnCbnSelchangeTriggerSource();
	afx_msg void OnCbnSelchangeTriggerEdge();
	afx_msg void OnEnChangeTriggerOffset();
	afx_msg void OnEnChangeCaptureOffset();
	afx_msg void OnEnChangeTimeout();
	afx_msg void OnEnChangeTriggerGap();
	afx_msg void OnEnChangeThreshold();
};
