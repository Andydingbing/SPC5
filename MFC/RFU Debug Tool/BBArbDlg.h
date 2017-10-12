#pragma once
#include "Stretchdlg.h"

class CBBArbDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CBBArbDlg)

public:
	CBBArbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBBArbDlg();

// 对话框数据
	enum { IDD = IDD_BB_ARB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	ISP1401 *m_pSP1401;
	CEdit m_FilePath;
	afx_msg void OnBnClickedSelectArbFilePath();
	CComboBox m_TriggerSrc;
	virtual BOOL OnInitDialog();
	int m_iFreqOffset;
	BOOL m_bReTrigger;
	int m_iCycles;
	int m_iTriggerDelay;
	BOOL m_bAutoStart;
	int m_iAdditionalSamples;
	CComboBox m_RepMode;
	CComboBox m_MultiSegTrigSrc;
	CComboBox m_MultiSegRepMode;
	CEdit m_FileInfo;
	afx_msg void OnBnClickedSetArb();
	afx_msg void OnBnClickedArbStart();
	afx_msg void OnBnClickedArbStop();
	afx_msg void OnBnClickedArbLoad();
	UINT m_uiCurSegment;
	UINT m_uiCurSample;
	UINT m_uiCurCycle;
	double m_dCurTimer;
	afx_msg void OnBnClickedGetArbStatus();
	afx_msg void OnBnClickedSetNextSeg();
	UINT m_uiMultiSegNext;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupIQChart();
	afx_msg void OnBnClickedMunualTrigger();
};
