#pragma once
#include "Stretchdlg.h"

class CRFArbDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CRFArbDlg)

public:
	CRFArbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRFArbDlg();

// 对话框数据
	enum { IDD = IDD_RF_ARB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	ISP1401 *m_pSP1401;
	CSP3301 *m_pSP3301;
	int m_iFreqOffset;
	BOOL m_bReTrigger;
	BOOL m_bAutoStart;
	UINT m_uiCurSegment;
	UINT m_uiCurSample;
	UINT m_uiCurCycle;
	UINT m_uiMultiSegNext;
	int m_iCycles;
	int m_iTriggerDelay;
	int m_iAdditionalSamples;
	double m_dCurTimer;
	CString m_strPath;
	CString m_strInfo;
	CComboBox m_RepMode;
	CComboBox m_MultiSegTrigSrc;
	CComboBox m_MultiSegRepMode;
	CComboBox m_TriggerSrc;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSelectArbFilePath();
	afx_msg void OnBnClickedSetArb();
	afx_msg void OnBnClickedArbStart();
	afx_msg void OnBnClickedArbStop();
	afx_msg void OnBnClickedArbLoad();
	afx_msg void OnBnClickedGetArbStatus();
	afx_msg void OnBnClickedSetNextSeg();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupIQChart();
	afx_msg void OnBnClickedMunualTrigger();
};
