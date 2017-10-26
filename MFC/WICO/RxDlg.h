#pragma once
#include "resource.h"
#include "StretchDlg.h"
#include "Led.h"
#include "AmpOffScrollEdit.h"

class CRxDlg : public CStretchDlg
{
	DECLARE_DYNAMIC(CRxDlg)

public:
	CRxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRxDlg();

// 对话框数据
	enum { IDD = IDD_RX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_WICO_STRETCH_DLG()

public:
	CLed m_Led[8];
	CEdit m_Path[8];
	CAmpOffScrollEdit m_RefOff[8];
	double m_dRefOffset[8];
	double m_dPhase[8];
	double m_dCF;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSelIQ0();
	afx_msg void OnBnClickedSelIQ1();
	afx_msg void OnBnClickedSelIQ2();
	afx_msg void OnBnClickedSelIQ3();
	afx_msg void OnBnClickedSelIQ4();
	afx_msg void OnBnClickedSelIQ5();
	afx_msg void OnBnClickedSelIQ6();
	afx_msg void OnBnClickedSelIQ7();
};
