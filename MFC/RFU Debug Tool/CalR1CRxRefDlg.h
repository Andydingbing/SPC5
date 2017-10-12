#pragma once
#include "CalBaseDlg.h"
#include "ProtectEyeListCtrl.h"

class CCalR1CRxRefDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CRxRefDlg)
public:
	CCalR1CRxRefDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CRxRefDlg();

	// 对话框数据
	enum { IDD = IDD_CAL_R1C_RX_REF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	vector<RxRefOPTableR1C::DataF> m_DataOP;
	vector<RxRefIOTableR1C::DataF> m_DataIO;
	CChartCtrl *m_pChart;
	vector<CChartLineSerie *> m_OPLineSerie[3];
	vector<CChartLineSerie *> m_IOLineSerie[3];
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	virtual BOOL OnInitDialog();
	double m_dRef;
	double m_dPower;
	CString m_strLNAAtt;
	float m_fAtt1;
	float m_fAtt2;
	float m_fAtt3;
	CComboBox m_IOMode;
	CProtectEyeListCtrl m_ListOP;
	CProtectEyeListCtrl m_ListIO;
	afx_msg void OnLvnGetdispinfoList1RxCalData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoList2RxCalData(NMHDR *pNMHDR, LRESULT *pResult);
	void ResetShowCtrl();
	CComboBox m_CalIOMode;
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
};
