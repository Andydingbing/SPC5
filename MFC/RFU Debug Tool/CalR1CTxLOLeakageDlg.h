#pragma once
#include "CalBaseDlg.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CTxLOLeakageDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CTxLOLeakageDlg)
public:
	CCalR1CTxLOLeakageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CTxLOLeakageDlg();

	// 对话框数据
	enum { IDD = IDD_CAL_R1C_TX_LOLEAKAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	CSP3501 *m_pSP3501;
	vector<TxLOLeakageTableR1C::DataF> m_Data;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	virtual BOOL OnInitDialog();
	int m_iDCOffsetI;
	int m_iDCOffsetQ;
	BOOL m_bCalX9119;
	CProtectEyeListCtrl m_List;
	void ResetShowCtrl();
	afx_msg void OnLvnGetdispinfoListTxR1cLoleakage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	BOOL m_bUseSA;
	afx_msg void OnBnClickedCalRef();
};
