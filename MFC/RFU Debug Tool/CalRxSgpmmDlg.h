#pragma once
#include "CalBaseDlg.h"
#include "ProtectEyeListCtrl.h"

class CCalRxSgpmmDlg : public CCalR1ABaseDlg
{
	DECLARE_DYNAMIC(CCalRxSgpmmDlg)

public:
	CCalRxSgpmmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalRxSgpmmDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1A_RX_SGPMM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1A_DLG()
public:
	vector<CSGPMMFile::FreqSeg> m_FreqSeg;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	CString m_strPowerStar;
	CString m_strPowerStop;
	CString m_strPowerStep;
	CString m_strPath;
	CProtectEyeListCtrl m_List;
	void ResetShowCtrl();
	virtual BOOL OnInitDialog();
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLvnGetdispinfoListSgPmMap(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSelCalFile();
};
