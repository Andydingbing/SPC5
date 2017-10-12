#pragma once
#include "CalBaseDlg.h"
#include "ProtectEyeListCtrl.h"

class CCalR1ATxSideBandDlg : public CCalR1ABaseDlg
{
	DECLARE_DYNAMIC(CCalR1ATxSideBandDlg)
public:
	CCalR1ATxSideBandDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1ATxSideBandDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1A_TX_SIDEBAND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1A_DLG()
public:
	vector<TxSidebandTableR1A::DataF> m_Data;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	virtual BOOL OnInitDialog();
	double m_dTh;
	UINT m_uAmI;
	UINT m_uAmQ;
	void ResetShowCtrl();
	CProtectEyeListCtrl m_List;
	afx_msg void OnLvnGetdispinfoListTxR1aSideband(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	BOOL m_bUseSA;
};
