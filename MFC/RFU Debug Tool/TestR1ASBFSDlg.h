#pragma once
#include "TestBaseDlg.h"

//SBFS : Single Board Function Stability / 单板功能稳定性
class CTestR1ASBFSDlg : public CTestR1ABaseDlg
{
	DECLARE_DYNAMIC(CTestR1ASBFSDlg)

public:
	CTestR1ASBFSDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestR1ASBFSDlg();

// 对话框数据
	enum { IDD = IDD_TEST_R1A_SBFS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_TEST_R1A_DLG()
public:
	CSP3301 *m_pSP3301;
	viPCIDev *m_S6;
	CEdit m_BitFile_K7_0;
	CEdit m_BitFile_K7_1;
	int m_iRepeatTimes;
	afx_msg void OnBnClickedSelBitK7_0();
	afx_msg void OnBnClickedSelBitK7_1();
	afx_msg void OnBnClickedStartTestDynDown();
	afx_msg void OnBnClickedStartTestSN();
	afx_msg void OnBnClickedStartTestLO();
	BOOL m_bDynDownK7_0;
	BOOL m_bDynDownK7_1;
	CString m_strSN;
	virtual BOOL OnInitDialog();
	CString m_strTxLO1FreqStar;
	CString m_strTxLO1FreqStop;
	CString m_strTxLO1FreqStep;
	CString m_strTxLO2FreqStar;
	CString m_strTxLO2FreqStop;
	CString m_strTxLO2FreqStep;
	CString m_strRxLO1FreqStar;
	CString m_strRxLO1FreqStop;
	CString m_strRxLO1FreqStep;
	CString m_strRxLO2FreqStar;
	CString m_strRxLO2FreqStop;
	CString m_strRxLO2FreqStep;
	BOOL m_bTestTxLO1;
	BOOL m_bTestTxLO2;
	BOOL m_bTestRxLO1;
	BOOL m_bTestRxLO2;
	CStatic m_TxLO1Lock;
	CStatic m_TxLO2Lock;
	CStatic m_RxLO1Lock;
	CStatic m_RxLO2Lock;
	int m_iCurrentCount;
	ULONGLONG m_uiTxLO1Cur;
	ULONGLONG m_uiTxLO2Cur;
	ULONGLONG m_uiRxLO1Cur;
	ULONGLONG m_uiRxLO2Cur;
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedCheckBitK70();
	afx_msg void OnBnClickedCheckBitK71();
};
