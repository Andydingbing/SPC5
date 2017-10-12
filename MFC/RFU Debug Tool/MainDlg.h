#pragma once
#include "AttachThreadDlg.h"
#include "MainThread.h"
#include "MainTabCtrl.h"
#include "MainTreeCtrl.h"

class CMainDlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CMainDlg)
public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	INT16 m_iTabIdxRF;
	INT16 m_iTabIdxDMA;
	INT16 m_iTabIdxRFU;

	CMainTreeCtrl *m_pTreeCtrl;	
	CMainTabCtrl *m_pTabCtrl;
	CDialog *m_pShowingDlg;
	
	void ShowDesiredDlg();
	void ModifyTabCtrl();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT Boot(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT BootDone(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT SP3301Change(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTcnSelchangeCardSelTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkFunctionTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickFunctionTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
