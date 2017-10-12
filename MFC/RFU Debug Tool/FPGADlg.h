#pragma once
#include "Stretchdlg.h"

class CFPGADlg : public CStretchDlg
{
	DECLARE_DYNCREATE(CFPGADlg)
public:
	viPCIDev *m_K7_0;
	viPCIDev *m_K7_1;
	viPCIDev *m_S6;
	CSP2401R1A *m_pSP2401_0;
	CSP2401R1A *m_pSP2401_2;
public:
	CFPGADlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFPGADlg();

// 对话框数据
	enum { IDD = IDD_FPGA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_CSE_STRETCH_DLG()
public:
	CString m_strK7_0_Offset;
	CString m_strK7_1_Offset;
	CString m_strK7_0_Data;
	CString m_strK7_1_Data;
	CString m_strK7_0_Status;
	CString m_strK7_1_Status;
	CString m_strS6_BAR0_Offset;
	CString m_strS6_BAR0_Data;
	CString m_strS6_BAR1_Offset;
	CString m_strS6_BAR1_Data;
	CString m_strS6Status;
	afx_msg void OnBnClickedWriteK7_0();
	afx_msg void OnBnClickedReadK7_0();
	afx_msg void OnBnClickedWriteK7_1();
	afx_msg void OnBnClickedReadK7_1();
	afx_msg void OnBnClickedWriteS6_BAR0();
	afx_msg void OnBnClickedReadS6_BAR0();
	afx_msg void OnBnClickedWriteS6_BAR1();
	afx_msg void OnBnClickedReadS6_BAR1();
	afx_msg void OnBnClickedSelBitK7_0();
	afx_msg void OnBnClickedSelBitK7_1();
	CEdit m_BitFileK7_0;
	CEdit m_BitFileK7_1;
	afx_msg void OnBnClickedLoadBitK7_0();
	afx_msg void OnBnClickedLoadBitK7_1();
	CString m_strFPGABitStatus;
	afx_msg void OnBnClickedSelRegFile();
	CEdit m_RegFile;
	CString m_strRegFileData;
	afx_msg void OnBnClickedExecuteWriteFromFile();
	CComboBox m_K7Sel;
};
