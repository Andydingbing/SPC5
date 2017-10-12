#pragma once
#include "afxwin.h"
#include "X9119ScrollEdit.h"

class CX9119Dlg : public CDialog
{
	DECLARE_DYNAMIC(CX9119Dlg)
public:
	CSP3501 *m_pSP3501;
public:
	CX9119Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CX9119Dlg();

// �Ի�������
	enum { IDD = IDD_X9119 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strX9119Value;
	CX9119ScrollEdit m_X9119Value;
	CString m_strX9119Voltage;
	afx_msg void OnEnChangeEditX9119Value();
	virtual BOOL OnInitDialog();
};
