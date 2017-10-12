#pragma once
#include "CalBaseDlg.h"
#include "CalR1CRxFilterTabCtrl.h"

class CCalR1CRxFilterDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CRxFilterDlg)

public:
	CCalR1CRxFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CRxFilterDlg();

	// 对话框数据
	enum { IDD = IDD_CAL_R1C_RX_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	CCalR1CRxFilterTabCtrl m_TabCtrl;
	virtual BOOL OnInitDialog();
	void ResetShowCtrl();
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnGetdispinfoListRxR1cFilter80(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListRxR1cFilter160(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_BW;
	BOOL m_bFast;
};
