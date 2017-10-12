#pragma once
#include "resource.h"

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CString strMsg,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckDlg();

// 对话框数据
	enum { IDD = IDD_CHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMsg;
	virtual BOOL OnInitDialog();
};

void CseMessageBox(CString );
void CseMessageBox(char *format,...);
INT ThreadCheckBox(char *format,...);
INT ThreadCancelBox(char *format,...);