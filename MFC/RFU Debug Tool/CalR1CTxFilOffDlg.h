#pragma once
#include "CalBaseDlg.h"
#include "CalR1CTxFilOffTabCtrl.h"

class CCalR1CTxFilOffDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CTxFilOffDlg)

public:
	CCalR1CTxFilOffDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CTxFilOffDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1C_TX_FILOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	CCalR1CTxFilOffTabCtrl m_TabCtrl;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	CComboBox m_CalIOMode;
	virtual BOOL OnInitDialog();
	void ResetShowCtrl();
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnGetdispinfoListTxR1cFilOffOp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTxR1cFilOffIo(NMHDR *pNMHDR, LRESULT *pResult);
};
