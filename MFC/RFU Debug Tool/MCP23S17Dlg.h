#pragma once

class CMCP23S17Dlg : public CDialog
{
	DECLARE_DYNAMIC(CMCP23S17Dlg)

public:
	CMCP23S17Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMCP23S17Dlg();

// 对话框数据
	enum { IDD = IDD_MCP23S17 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CSP1401R1C *m_pSP1401;
	BOOL m_bGPA0W;
	BOOL m_bGPA1W;
	BOOL m_bGPA2W;
	BOOL m_bGPA3W;
	BOOL m_bGPA4W;
	BOOL m_bGPA5W;
	BOOL m_bGPA6W;
	BOOL m_bGPA7W;
	BOOL m_bGPB0W;
	BOOL m_bGPB1W;
	BOOL m_bGPB2W;
	BOOL m_bGPB3W;
	BOOL m_bGPB4W;
	BOOL m_bGPB5W;
	BOOL m_bGPB6W;
	BOOL m_bGPB7W;
	afx_msg void OnBnClickedSetGPIO();
	afx_msg void OnBnClickedGetGPIO();
	BOOL m_bGPA0R;
	BOOL m_bGPA1R;
	BOOL m_bGPA2R;
	BOOL m_bGPA3R;
	BOOL m_bGPA4R;
	BOOL m_bGPA5R;
	BOOL m_bGPA6R;
	BOOL m_bGPA7R;
	BOOL m_bGPB0R;
	BOOL m_bGPB1R;
	BOOL m_bGPB2R;
	BOOL m_bGPB3R;
	BOOL m_bGPB4R;
	BOOL m_bGPB5R;
	BOOL m_bGPB6R;
	BOOL m_bGPB7R;
	afx_msg void OnBnClickedEnAllGPA();
	afx_msg void OnBnClickedEnAllGPB();
	BOOL m_bGPAW;
	BOOL m_bGPBW;
	CString m_strAddrW;
	CString m_strDataW;
	CString m_strAddrR;
	CString m_strDataR;
	afx_msg void OnBnClickedSPIWrite();
	afx_msg void OnBnClickedSPIRead();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd, CSP1401R1C *pSP1401);
	void SetWindowTextX();
	BOOL ShowWindow(int nCmdShow);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
