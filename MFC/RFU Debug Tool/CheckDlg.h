#pragma once
#include "resource.h"

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CString strMsg,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCheckDlg();

// �Ի�������
	enum { IDD = IDD_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMsg;
	virtual BOOL OnInitDialog();
};

void CseMessageBox(CString );
void CseMessageBox(char *format,...);
INT ThreadCheckBox(char *format,...);
INT ThreadCancelBox(char *format,...);