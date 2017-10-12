#pragma once
#include "TestBaseDlg.h"
#include "ProtectEyeListCtrl.h"

//TS : Temperature Stability / 温度稳定性
class CTestR1ATSDlg : public CTestR1ABaseDlg
{
	DECLARE_DYNAMIC(CTestR1ATSDlg)

public:
	typedef struct TxResult {
		vector<double> m_Power;
		vector<double> m_Temp;
	public:
		TxResult();
	}TxResult;

	typedef struct RxResult {
		vector<double> m_Power;
		vector<double> m_Temp;
	public:
		RxResult();
	}RxResult;

public:
	CTestR1ATSDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1ATSDlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1A_TS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1A_DLG()
public:
	vector<TxResult> m_TxData;
	vector<RxResult> m_RxData;
	CProtectEyeListCtrl m_ListTx;
	CProtectEyeListCtrl m_ListRx;
	CString m_strTxFreqStar;
	CString m_strTxFreqStop;
	CString m_strTxFreqStep;
	CString m_strRxFreqStar;
	CString m_strRxFreqStop;
	CString m_strRxFreqStep;
	BOOL m_bTestTx;
	BOOL m_bTestRx;
	BOOL m_bRxPM;
	double m_dTxPower;
	double m_dRxPower;
	double m_dSpaceingInterval;
	CString m_strTxPath;
	CString m_strRxPath;
	void ResetShowCtrl();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedStartTest();
	afx_msg void OnBnClickedSetTxTestFile();
	afx_msg void OnBnClickedSetRxTestFile();
	afx_msg void OnLvnGetdispinfoListTestTxTempStability(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoListTestRxTempStability(NMHDR *pNMHDR, LRESULT *pResult);
};
