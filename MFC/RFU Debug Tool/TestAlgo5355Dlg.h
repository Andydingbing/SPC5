#pragma once
#include "AttachThreadDlg.h"
#include "TestAlgo5355Thread.h"
#include "XGroupBox.h"
#include "ProtectEyeListCtrl.h"

class CTestAlgo5355Dlg : public CAttachThreadDlg
{
	DECLARE_DYNAMIC(CTestAlgo5355Dlg)

public:
	CTestAlgo5355Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestAlgo5355Dlg();

// 对话框数据
	enum { IDD = IDD_TEST_ALGO_5355 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_ATTACHTHREAD_DLG()
public:
	vector<CTestAlgo5355Thread::Result> m_Data;
	vector<CTestAlgo5355Thread::Result> m_ErrData;
	CProtectEyeListCtrl m_List;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	afx_msg void OnBnClickedStart();
	afx_msg LRESULT ShowData(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnGetdispinfoList(NMHDR *pNMHDR, LRESULT *pResult);
	void ResetData();
	void ResetList();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedShowErrData();
};
