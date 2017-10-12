#pragma once
#include "CalBaseDlg.h"
#include "CalR1CTxAttTabCtrl.h"

class CCalR1CTxAttDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CTxAttDlg)

public:
	CCalR1CTxAttDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CTxAttDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1C_TX_ATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	CComboBox m_CalIOMode;
	CString m_strFreq;
	CComboBox m_IOMode;
	virtual BOOL OnInitDialog();
	CProtectEyeListCtrl m_ListAttOP;
	CProtectEyeListCtrl m_ListAttIO;
	CCalR1CTxAttTabCtrl m_TabCtrl;
	void ResetShowCtrl();
	afx_msg void OnLvnGetdispinfoListTxR1cAttOp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTxR1cAttIo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnItemchangedListTxAttR1COP(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListTxAttR1CIO(NMHDR *pNMHDR, LRESULT *pResult);
};
