#pragma once
#include "AttachThreadDlg.h"
#include "Led.h"

class CCalR1CDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CCalR1CDlg)

public:
	CCalR1CDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1C };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_ATTACHTHREAD_DLG()
public:
	CSP1401R1C *m_pSP1401;
	CLed m_Led[10];
	void ResetShowCtrl();
	virtual BOOL OnInitDialog();
	afx_msg virtual void OnBnClickedStartCal();
	afx_msg LRESULT CalChildThreadCancel(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Done(WPARAM wParam,LPARAM lParam);
};
