#pragma once

class CAT24C256BDlg : public CDialog
{
	DECLARE_DYNAMIC(CAT24C256BDlg)

public:
	CAT24C256BDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAT24C256BDlg();

// 对话框数据
	enum { IDD = IDD_AT24C256B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ISP1401 *m_pSP1401;
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd, ISP1401 *pSP1401);
	CString m_strAddrW;
	CString m_strAddrR;
	UINT m_uiLengthW;
	UINT m_uiLengthR;
	CString m_strDataW;
	CString m_strDataR;
	afx_msg void OnBnClickedWrite();
	afx_msg void OnBnClickedRead();
	void SetWindowTextX();
	BOOL ShowWindow(int nCmdShow);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
};
