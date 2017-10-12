#pragma once
#include "CalBaseDlg.h"
#include "ProtectEyeListCtrl.h"
#include "ChartCtrl.h"
#include "ChartLineSerie.h"

class CCalR1CTxPowerDlg : public CCalR1CBaseDlg
{
	DECLARE_DYNAMIC(CCalR1CTxPowerDlg)

public:
	CCalR1CTxPowerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalR1CTxPowerDlg();

// 对话框数据
	enum { IDD = IDD_CAL_R1C_TX_POWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_CAL_R1C_DLG()
public:
	vector<TxPowerOPTableR1C::DataF> m_DataOP;
	vector<TxPowerIOTableR1C::DataF> m_DataIO;
	CProtectEyeListCtrl m_ListOP;
	CProtectEyeListCtrl m_ListIO;
	CChartCtrl *m_pChart;
	vector<CChartLineSerie *> m_OPLineSerie;
	vector<CChartLineSerie *> m_IOLineSerie;
	CString m_strFreqStar;
	CString m_strFreqStop;
	CString m_strFreqStep;
	double m_dDGain;
	double m_dAtt0;
	double m_dAtt1;
	double m_dAtt2;
	double m_dAtt3;
	double m_dPower;
	CComboBox m_IOMode;
	CComboBox m_CalIOMode;
	afx_msg virtual LRESULT Done(WPARAM wParam,LPARAM lParam);
	void ResetShowCtrl();
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnGetdispinfoListTxR1cPowerOp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTxR1cPowerIo(NMHDR *pNMHDR, LRESULT *pResult);
};
