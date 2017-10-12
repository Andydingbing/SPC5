#pragma once

class CLogConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogConfigDlg)

public:
	CLogConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogConfigDlg();

// �Ի�������
	enum { IDD = IDD_LOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnCancel();
public:
	BOOL m_bAll;
	BOOL m_bPrompt;
	BOOL m_bTrace;
	BOOL m_bMsgList;
	BOOL m_bMsgListToFile;
	BOOL m_bRegList;
	BOOL m_bRegListToFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSetAll();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
};
