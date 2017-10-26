#pragma once
#include "resource.h"
#include "StretchDlg.h"
#include "Led.h"
#include "AmpOffScrollEdit.h"

class CTxDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CTxDlg)

public:
	CTxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTxDlg();

// 对话框数据
	enum { IDD = IDD_TX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_WICO_STRETCH_DLG()

public:
 	CLed m_Led[8];
	CEdit m_Path[8];
	CAmpOffScrollEdit m_AmpOff[8];
	double m_dAmpOffset[8];
	double m_dPhase[8];
	double m_dCF;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSelArb0();
	afx_msg void OnBnClickedSelArb1();
	afx_msg void OnBnClickedSelArb2();
	afx_msg void OnBnClickedSelArb3();
	afx_msg void OnBnClickedSelArb4();
	afx_msg void OnBnClickedSelArb5();
	afx_msg void OnBnClickedSelArb6();
	afx_msg void OnBnClickedSelArb7();
	afx_msg void OnBnClickedUseRF0();
	afx_msg void OnBnClickedUseRF2();
	afx_msg void OnBnClickedUseRF4();
	afx_msg void OnBnClickedUseRF6();
	afx_msg void OnEnChangeArb0();
	afx_msg void OnEnChangeArb2();
	afx_msg void OnEnChangeArb4();
	afx_msg void OnEnChangeArb6();
};
