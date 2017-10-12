#pragma once
#include "CalBaseDlg.h"
#include "CalR1CRxAttTabCtrl.h"

class CCalR1CRxAttDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CRxAttDlg)

public:
	CCalR1CRxAttDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CRxAttDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1C_RX_ATT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	CString m_strFreq;
	virtual BOOL OnInitDialog();
	CProtectEyeListCtrl m_ListAttOP;
	CProtectEyeListCtrl m_ListAttIO;
	CComboBox m_CalIOMode;
	CComboBox m_IOMode;
	CCalR1CRxAttTabCtrl m_TabCtrl;
	void ResetShowCtrl();
	afx_msg void OnLvnGetdispinfoList1RxCalData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList2RxCalData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnItemchangedListRxAttR1COP(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListRxAttR1CIO(NMHDR *pNMHDR, LRESULT *pResult);
};
