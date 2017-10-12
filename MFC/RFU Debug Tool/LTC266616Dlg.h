#pragma once

class CLTC266616Dlg : public CDialog
{
	DECLARE_DYNAMIC(CLTC266616Dlg)
public:
	CSP1401R1A *m_pSP1401;
public:
	CLTC266616Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLTC266616Dlg();

// 对话框数据
	enum { IDD = IDD_LTC2666_16 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd, CSP1401R1A *m_pSP1401);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strDAC0;
	CString m_strDAC1;
	CString m_strDAC2;
	CString m_strDAC3;
	CString m_strDAC4;
	CString m_strDAC5;
	CString m_strDAC6;
	CString m_strDAC7;
	double m_dV0;
	double m_dV1;
	double m_dV2;
	double m_dV3;
	double m_dV4;
	double m_dV5;
	double m_dV6;
	double m_dV7;
	afx_msg void OnBnClickedSetDAC0();
	afx_msg void OnBnClickedSetDAC1();
	afx_msg void OnBnClickedSetDAC2();
	afx_msg void OnBnClickedSetDAC3();
	afx_msg void OnBnClickedSetDAC4();
	afx_msg void OnBnClickedSetDAC5();
	afx_msg void OnBnClickedSetDAC6();
	afx_msg void OnBnClickedSetDAC7();
	BOOL m_bUseV0;
	BOOL m_bUseV1;
	BOOL m_bUseV2;
	BOOL m_bUseV3;
	BOOL m_bUseV4;
	BOOL m_bUseV5;
	BOOL m_bUseV6;
	BOOL m_bUseV7;
	BOOL m_bUseV;
	afx_msg void OnBnClickedUseAllV();
	afx_msg void OnBnClickedSetDAC();
	void SetWindowTextX();
	BOOL ShowWindow(int nCmdShow);
protected:
	virtual void OnCancel();
};
